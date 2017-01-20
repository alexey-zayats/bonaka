// keeper.cpp

#include <QSqlError>
#include <QString>
#include <QByteArray>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QVector>
#include <QDebug>

#include "keeper.h"
#include "entity.h"

// TODO:
// 1. check default schema on connection
// 2. fetch user on connection

namespace Entity
{
    Keeper * Keeper::p_instance = 0;
    KeeperDestroyer Keeper::destroyer;

    KeeperDestroyer::~KeeperDestroyer() {
        delete p_instance;
    }
    void KeeperDestroyer::initialize( Keeper* p ) {
        p_instance = p;
    }

    Keeper Keeper::getInstance() {
        if(!p_instance)     {
            p_instance = new Keeper();
            destroyer.initialize( p_instance);
        }
        return p_instance;
    }

    Keeper::Keeper() : QObject()
    {
        setObjectName( QLatin1String("Entity::Keeper") );
    }

    Keeper::Keeper( const Keeper& ) {
    }

    Keeper& operator=( Keeper& ) {
    }

    Keeper::~Keeper()
    {
        qDeleteAll(sqlErrors);
        close();
    }

    int Keeper::open( const QHash<QString,QString> &opt )
    {
        connectOpt = opt;
        alias = connectOpt[ QLatin1String("alias") ];
        db = QSqlDatabase::addDatabase( QLatin1String("QPSQL"), alias );
        db.setDatabaseName ( connectOpt[ QLatin1String("dbname") ] );
        db.setUserName ( connectOpt[ QLatin1String("user") ] );
        db.setPassword ( connectOpt[ QLatin1String("password") ] );
        db.setHostName ( connectOpt[ QLatin1String("host") ] );
        db.setPort ( connectOpt[ QLatin1String("port") ].toInt() );
        db.setConnectOptions ( connectOpt[ QLatin1String("options") ] );
        if ( !db.open() )
        {
            QStringList query;
            query.append( QLatin1String("Connection error") );
            for ( QHash<QString, QString>::const_iterator it = opt.constBegin(); it != opt.constEnd(); ++it )
                if ( it.key().compare( QLatin1String("password") ) )
                    query.append( it.key() + QLatin1Char('=') + it.value() );

            sqlErrors.append(
                new SqlError(db.lastError().text(), query.join(QLatin1String("\n")))
            );

            return 0;
        }

        roleName = connectOpt[ QLatin1String("user") ];
        rolePass = connectOpt[ QLatin1String("password") ];

        initSchema();

        return 1;
    }

    void Keeper::initSchema() {

        QSqlQuery query( db );

        QString sql = QLatin1String("SELECT id, name, title, amodel, trail FROM class");
        query.exec(sql);

        if ( !query.exec() ) {
            sqlErrors.append(new SqlError(query.lastError().text(), query.executedQuery()));
            return;
        }

        QSqlQuery attrQuery(db);
        sql = QLatin1String(
            "SELECT cast( a.name::varchar as text) as _name, " +
            "a.default_value, a.is_array, a.is_unique, a.is_searchable, " +
            "a.referenced, a.id, a.type_name as type_name, " +
            "t.pgtype as type_pgtype " +
            "FROM attr AS a, class AS c, type AS t " +
            "WHERE c.trail @> ? " +
            "AND a.class_name = c.name " +
            "AND t.name = a.type_name");
        attrQuery.prepare(sql);

        Object *obj = 0;
        while (query.next()) {
            QString className = query.value("name").toString();
            switch (className) {
                case QLatin1String("obj"):
                    obj = new Object();
                    break;
                case QLatin1String("tree"):
                    obj = new Tree();
                    break;
                case QLatin1String("link"):
                    obj = new Link();
                    break;
                case QLatin1String("file"):
                    obj = new File();
                    break;
                case QLatin1String("role"):
                    obj = new Role();
                    break;
                case QLatin1String("user"):
                    obj = new User();
                    break;
            }
            obj->setProperty()
            attrQuery.addBindValue(query.value("trail").toString());
            if ( !attrQuery.exec() ) {
                return;
            }

            schema.insert();
        }

    }

    SqlError *Keeper::lastError() {
         return sqlErrors.last();
    }

    int Keeper::reOpen() {
        return open( connectOpt );
    }

    void Keeper::close() {
        db.close();
        QSqlDatabase::removeDatabase( alias );
    }

    TQueryParam Keeper::prepareParam( int flags, TQueryParam &values ) {
        TQueryParam tmp;
        tmp.append( flags );
        tmp += values;
        return tmp;
    }

    TQueryParam Keeper::prepareParam( int flags, QVariant value )
    {
        TQueryParam tmp;
        tmp.append( flags );
        tmp.append( value );
        return tmp;
    }

    QSqlQuery Keeper::parseQueryMap( const QString &table,
                                     const QStringList &fileds,
                                     const QStringList &wList, const TQueryMap &opts )
    {
        if ( !db.isOpen() ) reOpen();

        QString sql;
        TQueryMap opt = opts;
        QStringList names = wList;

        // clearing buffers
        binds.clear();
        wheres.clear();
        orders.clear();
        groups.clear();
        offset.clear();
        limit.clear();

        if ( opt.contains( QLatin1String("order") ) )
        {
            orders = QLatin1String(" ORDER BY ") + opt[ QLatin1String("order") ].toStringList().join( QLatin1String(", ") );
        }

        if ( opt.contains( QLatin1String("group") ) )
        {
            QStringList params = opt[ QLatin1String("group") ].toStringList();
            groups = QLatin1String(" GROUP BY ") + params.join( QLatin1String(", ") );
        }

        for ( QStringList::Iterator it = names.begin(); it != names.end(); ++it ) {
            if ( opt.contains( *it ) ) parseQueryParam( *it, opt[ *it ] );
        }

        if ( opt.contains( QLatin1String("limit") ) ) {
            limit = QLatin1String(" LIMIT %1");
            limit = limit.arg( opt[ QLatin1String("limit") ].toInt() );
        }
        if ( opt.contains( QLatin1String("offset") ) ) {
            offset = QLatin1String(" OFFSET %1");
            offset = offset.arg( opt[ QLatin1String("offset") ].toInt() );
        }

        // perform sql computing
        sql = QLatin1String("SELECT %1 FROM %2");
        sql = sql.arg( fileds.join(QLatin1String(", ")) ).arg( table );

        if ( wheres.count() ) {
            sql.append( QLatin1String(" WHERE ") );
            sql.append( wheres.join( QLatin1String(" ") ) );
        }

        sql.append( groups );
        sql.append( orders );
        sql.append( limit );
        sql.append( offset );

        QSqlQuery query( db );
        query.prepare( sql );

        qDebug() << sql << binds;

        for( QList<QVariant>::Iterator it = binds.begin(); it != binds.end(); ++it )
            query.addBindValue( (*it) );

        return query;
    }

    void Keeper::parseQueryParam( const QString &name, const QVariant &params )
    {
        QStringList values;
        TQueryParam list = params.toList();
        TQueryParam::Iterator it = list.begin();
        int flag = (*it++).toInt();

        if ( flag & And ) wheres.append( QLatin1String("AND") ), flag ^= And;
        else if ( flag & Or ) wheres.append( QLatin1String("OR") ), flag ^= Or;

        QString f;
        switch ( flag ) {
        case Is:
            f = QLatin1String("( %1 IS ? )");
            wheres.append(f.arg(name));
            binds.append(*it);
            break;
        case Is | Field:
            f = QLatin1String("( %1 IS %2 )");
            wheres.append(f.arg(name, (*it).toString()));
            break;
        case Equal:
            f = QLatin1String("( %1 = ? )");
            wheres.append(f.arg(name));
            binds.append( *it );
            break;
        case ( Equal | Field ):
            f = QLatin1String("( %1 = %2 )");
            wheres.append(f.arg(name, (*it).toString()));
            break;
        case (Not | Equal):
            f = QLatin1String("( %1 <> ? )");
            wheres.append(f.arg(name));
            binds.append(*it);
            break;
        case ( Not | Equal | Field ):
            f = QLatin1String("( %1 <> %2 )");
            wheres.append(f.arg(name, (*it).toString()));
            break;
        case Greater:
            f = QLatin1String("( %1 > ? )");
            wheres.append(f.arg(name));
            binds.append(*it);
            break;
        case ( Greater | Field ):
            f = QLatin1String("( %1 > %2 )");
            wheres.append(f.arg(name, (*it).toString()));
            break;
        case Less:
            f = QLatin1String("( %1 < ? )");
            wheres.append(f.arg(name));
            binds.append(*it);
            break;
        case ( Less | Field ):
            f = QLatin1String("( %1 < %2 )");
            wheres.append(f.arg(name, (*it).toString()));
            break;
        case ( Equal | Greater ):
            f = QLatin1String("( %1 >= ? )");
            wheres.append(f.arg(name));
            binds.append(*it);
            break;
        case ( Equal | Less ):
            f = QLatin1String("( %1 <= ? )");
            wheres.append(f.arg(name));
            binds.append(*it);
            break;
        case Interval:
            f = QLatin1String("( ( ? < %1 ) AND ( %2 < ? ) )");
            wheres.append(f.arg(name, name));
            binds.append(*it++);
            binds.append(*it);
            break;
        case Segment:
            f = QLatin1String("( ( ? <= %1 ) AND ( %2 <= ? ) )");
            wheres.append(f.arg(name, name));
            binds.append(*it++);
            binds.append(*it);
            break;
        case InSet:
            for( ; it != list.end(); ++it )	{
                values.append( (*it).toString() );
            }
            f = QLatin1String(" ( %1 IN (%2) )" );
            wheres.append(f.arg(name, values.join(QLatin1Char(','))));
            break;
        case ( Not | InSet ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            f = QLatin1String("( %1 NOT IN (%2) )");
            wheres.append(f.arg( name, values.join(QLatin1Char(',')) ) );
            break;
        case ( Like | Case ):
            f = QLatin1String("( %1 ilike ? )");
            wheres.append(f.arg(name));
            binds.append(*it);
            break;
        case ( Equal | Case ):
            f = QLatin1String("( %1 = ? )");
            wheres.append(f.arg(name));
            binds.append(*it);
            break;
        case Like:
            f = QLatin1String("( %1 like ? )");
            wheres.append(f.arg(name));
            binds.append( *it );
            break;
        case ( Equal | IntArray ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            f = QLatin1String("( %1 = '{%2}' )");
            wheres.append(f.arg(name, values.join(QLatin1Char(','))));
            break;
            // INTERSECTION
        case ( Intersect | IntArray ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            f = QLatin1String("( %1 && '{%2}' )");
            wheres.append(f.arg(name, values.join(QLatin1Char(','))));
            break;
        case ( Intersect | LTree ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            f = QLatin1String("( %1 @> '%2' )");
            wheres.append(f.arg(name, values.join(QLatin1Char('.'))));
            break;
            // CONTAINS
        case ( Contains | IntArray ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            f = QLatin1String("( %1 @ '{%2}' )");
            wheres.append(f.arg(name, values.join(QLatin1Char(','))));
            break;
        case ( Contains | LTree ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            f = QLatin1String("( %1 <@ '%2' )");
            wheres.append(f.arg(name, values.join(QLatin1Char('.'))));
            break;
            // WITHINS
        case ( Within | IntArray ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            f = QLatin1String("( %1 ~ '{%2}' )");
            wheres.append(f.arg(name, values.join(QLatin1Char(','))));
            break;
        }
    }

    Object *objectByType( const QByteArray &type ) {

    }

    QString Keeper::tableByType( const QByteArray &type )
    {
        Object *obj = this->objectByType(type);
        QString table = obj->getTable();
        delete obj;
        return table;
    }

    QStringList Keeper::filedsByType( const QByteArray &type )
    {
        Object *obj = this->objectByType(type);
        QStringList list = obj->nameSchema();
        delete obj;
        return list;
    }

    void Keeper::clearObjectList( QList<Object*> list )
    {
        while (!list.isEmpty())
            delete list.takeFirst();
    }

    // --------------------------------------------------------------------------------------------------
    // DATA RETRIVING FUNCTIONS

    QList<Object*> Keeper::getObjectList( const QByteArray &type, const TQueryMap &opts )
    {
        Object *obj = 0;
        QList<Object*> list;
        QString table = tableByType( type );
        QStringList fileds = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, fileds, opts );
        if ( !query.exec() ) throw SqlError( query.lastError().text(), query.executedQuery() );

        while ( query.next() )
        {
            QCoreApplication::processEvents ();
            obj = objectByType(type);
            if ( obj )
            {
                obj->restore( query );
                list.append( obj );
            }
        }

        return list;
    }

    int Keeper::countObjects( const QByteArray &type, const TQueryMap &opts )
    {
        QString table = tableByType( type );
        QStringList fileds = ( QStringList() << QLatin1String("count(id)") );
        QStringList wList = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, wList, opts );
        if ( !query.exec() ) {
            throw SqlError( query.lastError().text(), query.executedQuery() );
        }

        query.next();
        return query.value(0).toInt();
    }

    QList<QVariant> Keeper::getFieldList( const QByteArray &type, const QString &filed, const TQueryMap &opts )
    {
        QList<QVariant> list;
        QString table = tableByType( type );
        QStringList fileds( filed );
        QStringList wList = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, wList, opts );
        if ( !query.exec() ) throw SqlError( query.lastError().text(), query.executedQuery() );

        while ( query.next() ) {
            QCoreApplication::processEvents ();
            list.append( query.value(0) );
        }

        return list;
    }

    QList<QVariant> Keeper::getListOfFields( const QByteArray &type, const QStringList &fileds, const TQueryMap &opts )
    {
        QList<QVariant> list;
        QString table = tableByType( type );
        QStringList wList = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, wList, opts );
        if ( !query.exec() ) throw SqlError( query.lastError().text(), query.executedQuery() );

        while ( query.next() ) {
            QStringList row;
            for( int i = 0; i < fileds.size(); i++ ) {
                QCoreApplication::processEvents ();
                row.append( query.value( i ).toString() );
            }
            list.append( row );
        }
        return list;
    }

    QHash<int,QVariant> Keeper::getFieldMap( const QByteArray &type, const QString &filed, const TQueryMap &opts )
    {
        QHash<int,QVariant> map;
        QString table = tableByType( type );
        QStringList wList = filedsByType( type );
        QStringList fileds = ( QStringList() << QLatin1String("id") << filed );

        QSqlQuery query = parseQueryMap( table, fileds, wList, opts );
        if ( !query.exec() )
            return map;

        while ( query.next() ) {
            QCoreApplication::processEvents ();
            map.insert( query.value(0).toInt(), query.value(1) );
        }

        return map;
    }

    QHash<QString,Object*> Keeper::getObjectMap( const QByteArray &type, const QString &filed, const TQueryMap &opts )
    {
        QHash<QString,Object*> map;
        QString table = tableByType( type );
        QStringList fileds = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, fileds, opts );
        if ( !query.exec() )
            return map;

        Object *obj = 0;

        int i = 0;
        bool by_order = filed.compare(QLatin1String("order")) == 0;

        while ( query.next() )
        {
            QCoreApplication::processEvents ();
            obj = objectByType(type);
            if ( obj )
            {
                obj->restore( query );
                if ( by_order ) {
                    map.insert( QString::number(++i), obj );
                } else {
                    map.insert( obj->get(filed), obj );
                }
            }
        }

        return map;
    }

}

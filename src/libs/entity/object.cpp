// object.cpp

#include "object.h"
#include "keeper.h"

#include <QFile>
#include <QTextStream>

#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextCodec>
#include <QDebug>

namespace Entity
{

    Object::Object() : QObject()
	{
        init();
	}

	Object::Object( const Object &obj ) : QObject()
	{
		this->schema = obj.schema;
		this->formItems = obj.formItems;
		this->viewItems = obj.viewItems;
		this->searchItems = obj.searchItems;
		this->data = obj.data;
		this->table = obj.table;
		this->type = obj.type;
		this->id = obj.id;
		this->searchable = obj.searchable;
	}

	Object &Object::operator=( const Object &obj )
	{
		this->schema = obj.schema;
		this->formItems = obj.formItems;
		this->viewItems = obj.viewItems;
		this->searchItems = obj.searchItems;
		this->data = obj.data;
		this->table = obj.table;
		this->type = obj.type;
		this->id = obj.id;
		this->searchable = obj.searchable;
		return *this;
	}

	Object::~Object()
	{
		qDeleteAll(schema);
	}

	Attr *Object::attribute( const QString &name )
	{
		Attr *a = 0;

		for( int i = 0; i < schema.size(); i++ ) {
			a =  schema.at(i);
			if ( a->getName().compare(name) == 0 )
				return a;
			else
				a = 0;
		}
		return a;
	}

	void Object::init()
	{
        table = QLatin1String("obj");

        QSqlDatabase db = Keeper::getInstance()->database();
        QSqlQuery query(db);

        sql = QLatin1String(
            "SELECT cast( a.name::varchar as text) as _name, " +
            "a.default_value, a.is_array, a.is_unique, a.is_searchable, " +
            "a.referenced, a.id, a.type_name as type_name, " +
            "t.pgtype as type_pgtype " +
            "FROM attr AS a, class AS c, type AS t " +
            "WHERE c.trail @> ? " +
            "AND a.class_name = c.name " +
            "AND t.name = a.type_name");
        query.prepare(sql);
        query.addBindValue(table);

        if(!query.exec()) {

        }

        while(query.next()) {
            schema.append( new Attr( QLatin1String("id"),      tr("ID"),               Attr::Int,      true,  true );
        }


        QString sql = QLatin1String("");
        QSqlQuery query = db.exec(sql);
		//                  name               name         Title                type            ro	    visible
        schema.append( new Attr( QLatin1String("id"),      tr("ID"),               Attr::Int,      true,  true ) );
        schema.append( new Attr( QLatin1String("guid"),    tr("GUID"),             Attr::String,   true,  true ) );
        schema.append( new Attr( QLatin1String("relname"), tr("RelName"),          Attr::String,   false, true ) );
        schema.append( new Attr( QLatin1String("name"),    tr("Name"),             Attr::String,   false, true ) );
        schema.append( new Attr( QLatin1String("created"), tr("Create date/time"), Attr::DateTime, true,  true ) );
        schema.append( new Attr( QLatin1String("updated"), tr("Update date/time"), Attr::DateTime, true,  true ) );

		id = 0;
		type = OObject;
		mtype = aList;
        table = QLatin1String("obj");

        Object::setObjectName( QLatin1String("Melampig::Object") );

//        viewItems.append( "id" );
//        viewItems.append( "name" );
//        viewItems.append( "created" );
	}

	QStringList Object::nameSchema() const
	{
		QStringList names;
		for( int i = 0; i < schema.size(); i++ ) {
			names.append(  schema.at(i)->getName() );
		}

		return names;
	}

	QString Object::get( const QString &key )
	{
		QString out;

		if ( attribute(key) != 0 && data.contains(key) )
			out = data[key];

		return out;
	}

	void Object::set( const QString &key, const QString &value )
	{
		if ( attribute(key) == 0 ) {
			return;
		}
		if ( data.contains( key ) ) data.remove( key );
		data.insert( key, value );
        if ( key.compare( QLatin1String("id") ) == 0 ) id = value.toInt();
	}

	void Object::store()
	{
		if ( table.isEmpty() )
			return;

		QStringList field;
		QStringList ph;
		QList<QString> binds;

		QString val;


		Attr *a = 0;
		QHashIterator< QString, QString > i(data);
		while (i.hasNext() )
		{
			i.next();

			// we reject not schemed pairs key=>value
			a = attribute(i.key() );
			if ( a == 0 )
				continue;

			if ( id && a->isReadOnly() )
				continue;

            //if ( !a->isVisible() )
            //	continue;

			val = i.value();

			if ( id  )
			{
				// for update
                if ( i.key().compare( QLatin1String("id") ) )
				{
                    QString f;
					if ( a->getRef() != ONull && a->isArray() == false && val.toInt() == 0  )
					{
                        f = QLatin1String( "%1 = NULL" );
                        field.append( f.arg( i.key() ) );
					} else if ( a->getType() == Attr::Svg || a->getType() == Attr::Image ) {
                        f = QLatin1String( "%1 = decode(?, 'base64')" );
                        field.append( f.arg( i.key() ) );
						binds.append( val );
					} else {
                        f = QLatin1Literal( "%1 = ?" );
                        field.append( f.arg( i.key() ) );
						binds.append( val );
					}
				}
			}
			else // for insert
			{
				field.append( i.key() );

				if ( a->getRef() != ONull && val.toInt() == 0  ) {
                    ph.append( QLatin1String("NULL") );
				} else if ( a->getType() == Attr::Svg || a->getType() == Attr::Image ) {
                    ph.append( QLatin1String("decode(?, 'base64')") );
					binds.append( val );
				} else {
                    ph.append( QLatin1String("?") );
					binds.append( val );
				}
			}
		}

        QString sql;
        QString j = QLatin1String(", ");
        if ( id ) {
            sql =  QLatin1String( "UPDATE %1 SET %2 WHERE id = ?" );
            sql = sql.arg( table, field.join(j) );
        } else {
            sql =  QLatin1String( "INSERT INTO %1 (%2) VALUES (%3)" );
            sql = sql.arg( table, field.join(j), ph.join(j) );
        }


		QSqlQuery query( keeper->database() );
		query.prepare( sql );

		for( int i = 0; i < binds.size(); i++ )
			query.addBindValue( binds.at(i) );

		if ( id ) query.addBindValue( id );

		if ( !query.exec() ) {
			throw SqlError( query.lastError().text(), query.executedQuery() );
		}

        qDebug() << query.executedQuery() << binds;

		if ( !id ) {
            query.exec( QLatin1String("SELECT currval('obj_id_seq')") );
			query.next();
            set( QLatin1String("id"), query.value(0).toString() );
		}

//		restore( get("id").toInt() );
	}

	void Object::remove()
	{
		if ( table.isEmpty() || id < 0 )
			return;

        QString sql = QLatin1String( "DELETE FROM %1 WHERE id = ?" );
        sql = sql.arg( table );

		QSqlQuery query( keeper->database() );
		query.prepare( sql );
		query.addBindValue( id );

        qDebug() << query.executedQuery() << id;

		if ( !query.exec() )
			throw SqlError( query.lastError().text(), query.executedQuery() );
	}

	void Object::restore( const TQueryMap &opts )
	{
		QStringList flist = nameSchema();
		QSqlQuery query = keeper->parseQueryMap( table, flist, flist, opts );

		if ( !query.exec() )
			throw SqlError( query.lastError().text(), query.executedQuery() );

		if ( !query.isActive() )
			return;

		if ( query.next() )
			restore( query );
	}

	void Object::restore( int idx )
	{
		TQueryMap opts;
        opts.insert( QLatin1String("id"), keeper->prepareParam( Equal, idx ) );
		restore( opts );
	}

	void Object::restore( QSqlQuery &query )
	{
		Attr *a = 0;
		QStringList field = nameSchema();
		for( int i = 0; i < field.size(); i++ ) {
			a = attribute(field.at(i));
			switch ( a->getType() )
			{
				case Attr::Svg:
				case Attr::Image: {
                    set( field.at(i), QLatin1String(query.value(i).toByteArray().toBase64()) );
					break;
			   }
			   default:
					set( field.at(i), query.value(i).toString() );
					break;
			}
		}
	}

	QStringList Object::toList( const QString &str )
	{
		QString a = str;
        a.remove( QLatin1Char('{') );
        a.remove( QLatin1Char('}') );
        return a.isEmpty() ? QStringList() : a.split( QLatin1Char(',') );
	}

	QString Object::toArray( const QStringList &list )
	{
        return QLatin1Char('{') + list.join( QLatin1Char(',') ) + QLatin1Char('}');
	}
}


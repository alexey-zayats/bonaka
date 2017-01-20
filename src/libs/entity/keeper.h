// keeper.h

#ifndef ENTITY_KEEPER_H
#define ENTITY_KEEPER_H

#define VERSION 0.0.1

#include <QObject>
#include <QHash>
#include <QList>
#include <QSqlDatabase>
#include <QStringList>

#include "errors.h"
#include "entity_global.h"
#include "defines.h"

namespace Entity
{
    class Keeper;

    class KeeperDestroyer
    {
      private:
        Keeper* p_instance;
      public:
        ~KeeperDestroyer();
        void initialize( Keeper* p );
    };

	class ENTITY_EXPORT Keeper : QObject
	{
		Q_OBJECT

		public:
            ~Keeper();

			// open connection to pgsql backend based on options map
            int open( const QHash<QString,QString> &opt );
            int reOpen();

			// close connection
			void close();

			// return object for sql queries
			QSqlDatabase database() const { return db; }
			QString getAlias() const { return alias; }
			QHash<QString,QString> getConnectOpt() const { return connectOpt; }

			// prepare params for query execution
			TQueryParam prepareParam( int flags, TQueryParam &values );
			// prepare params for query execution
			TQueryParam prepareParam( int flags, QVariant value );

			// parse query map and return QSqlQuery
			QSqlQuery parseQueryMap( const QString &view, const QStringList &nameSchema, const QStringList &wList, const TQueryMap &opts );

            QList<Object*> getObjectList( const QByteArray &type, const TQueryMap &opts );
            QList<QVariant> getFieldList( const QByteArray &type, const QString &filed, const TQueryMap &opts );
            QList<QVariant> getListOfFields( const QByteArray &type, const QStringList &fileds, const TQueryMap &opts );
            QHash<int,QVariant> getFieldMap( const QByteArray &type, const QString &filed, const TQueryMap &opts );
            QHash<QString,Object*> getObjectMap( const QByteArray &type, const QString &filed, const TQueryMap &opts );
            int countObjects( const QByteArray &type, const TQueryMap &opts );

			void clearObjectList( QList<Object*> list );
            Object *objectByType( const QByteArray &type );

            SqlError *lastError();

            static Keeper* getInstance();

        protected:
            Keeper() { }
            Keeper( const Keeper& );
            Keeper& operator=( Keeper& );
            ~Keeper() { }

            friend class KeeperDestroyer;

		private:
            Keeper();
            Keeper(const Keeper&);
            Keeper& operator=(Keeper&);

            static Keeper* p_instance;
            static KeeperDestroyer destroyer;

			// parse one query item
			void parseQueryParam( const QString &name, const QVariant &params );

            QString tableByType( const QByteArray &type );
            QStringList filedsByType( const QByteArray &type );

			QList<QVariant> binds;
			QStringList wheres;
			QString groups,
					orders,
					offset,
					limit;

			QString alias;
			QString roleName, rolePass;
			QSqlDatabase db;
			QHash<QString,QString> connectOpt;
            QVector<SqlError*> sqlErrors;
            QHash<QByteArray,Object*> schema;
	};
}

#endif // MELAMPIG_KEEPER_H

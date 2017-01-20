#ifndef ENTITY_CONNECTION_H
#define ENTITY_CONNECTION_H

#include <QStringList>
#include <QString>
#include <QHash>
#include <QObject>

#include "entity_global.h"

namespace Entity
{
	class Keeper;

	enum SslMode
	{
		SSLDisable = 0,
		SSLAllow = 1,
		SSLPrefer = 2,
		SSLRequire = 3
	};

    class ENTITY_EXPORT Connection : public QObject
	{
		Q_OBJECT
		public:
            Connection( QObject *parent = 0 );
            Connection( const QString &host, const QString &name, QObject *parent = 0 );
            ~Connection();

			void open();
			void close();
			void store();
			void remove();
			bool isOpen();

			QString getAlias() const { return alias; }
			QString getName() const { return dbname; }
			QString getHost() const { return host; }
            QString getUsername() const { return username; }
            QString getPassword() const { return password; }

			int getPort() const { return port; }
			int getSSLMode() const { return sslMode; }
			int getTimeout() const { return timeout; }

			Keeper *getKeeper() const { return keeper; }
            static void init();

		public slots:
			void setAlias( const QString &str ) { alias = str; }
			void setName( const QString &str ) { dbname = str; }
			void setHost( const QString &str ) { host = str; }
			void setUserName( const QString &str ) { username = str; }
			void setPassword( const QString &str ) { password = str; }
			void setPort( int i ) { port = i; }
			void setSSLMode( int mode ) { sslMode = mode; }
			void setTimeout( int tout ) { timeout = tout; }

		private:
			QString alias;
			QString dbname;
			QString host;
			int port;
			QString username;
			QString password;
			int sslMode;
			int timeout;
			Keeper *keeper;
	};
}

#endif // ENTITY_CONNECTION_H




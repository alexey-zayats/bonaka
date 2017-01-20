#include <QSettings>
#include <QtGlobal>
#include <QMessageBox>
#include <QDebug>

#include "errors.h"
#include "connection.h"
#include "keeper.h"

namespace Entity
{
    void Connection::init() {

    }

    Connection::Connection ( QObject *parent ) : QObject( parent )
	{
		keeper = 0;
        alias = QLatin1String("");
        host = QLatin1String("localhost");
        dbname = QLatin1String("");
        username = QLatin1String("");
        password = QLatin1String("");
		sslMode = SSLPrefer;
		port = 5432;
		timeout = 15;
	}

    Connection::Connection( const QString &h, const QString &a, QObject *parent )
	: QObject( parent )
	{
		keeper = 0;
		QSettings s;
        s.beginGroup(QLatin1String("connections"));

		QStringList hosts = s.childGroups();

		if ( hosts.contains( h ) )
		{
			s.beginGroup ( h );
			QStringList aliases = s.childGroups();
			if ( aliases.contains( a ) )
			{
				s.beginGroup( a );
                host = s.value(QLatin1String("host")).toString();
                alias = s.value(QLatin1String("alias")).toString();
                dbname = s.value(QLatin1String("dbname")).toString();
                port = s.value(QLatin1String("port")).toInt();
                sslMode = s.value(QLatin1String("ssl")).toInt();
                timeout = s.value(QLatin1String("timeout")).toInt();
                username = s.value(QLatin1String("username")).toString();
                password = s.value(QLatin1String("password")).toString();
                s.endGroup(); // end alias
			}

			s.endGroup(); // end host
		}

		s.endGroup();
	}

    Connection::~Connection()
	{
		close();
	}

    void Connection::close()
	{
		if ( keeper )
		{
			keeper->close();
			delete keeper;
			keeper = 0;
		}
	}

    bool Connection::isOpen()
	{
		if ( !keeper ) return false;
		return keeper->database().isOpen();
	}

    void Connection::open()
	{
		QHash<QString,QString> opt;
        opt.insert(QLatin1String("alias"), alias);
        opt.insert(QLatin1String("dbname"), dbname);
        opt.insert(QLatin1String("user"), username);
        opt.insert(QLatin1String("host"), host);
        opt.insert(QLatin1String("port"), QString::number(port));
        opt.insert(QLatin1String("password"), password);

		QStringList list;

        QString f = QLatin1String("connect_timeout=%1");
        list.append(f.arg(timeout));

		switch( sslMode )
		{
			case SSLDisable:
                list.append(QLatin1String("sslmode=disable"));
				break;
			case SSLAllow:
                list.append(QLatin1String("sslmode=allow"));
				break;
			case SSLPrefer:
                list.append(QLatin1String("sslmode=prefer"));
				break;
			case SSLRequire:
                list.append(QLatin1String("sslmode=require"));
				break;
		}

        if ( list.size() > 0 ) {
            opt.insert(QLatin1String("options"), list.join(QLatin1Char(';')));
        }

        keeper = new Keeper();
        if ( keeper->open( opt ) == 0) {
            qDebug() << keeper->lastError()->getText();
		}
	}

    void Connection::store()
	{
		QSettings s;

        s.beginGroup(QLatin1String("connections"));
		s.beginGroup( host );
		s.beginGroup( alias );

        s.setValue(QLatin1String("host"), host );
        s.setValue(QLatin1String("port"), port );
        s.setValue(QLatin1String("alias"), alias );
        s.setValue(QLatin1String("dbname"), dbname );
        s.setValue(QLatin1String("ssl"), sslMode );
        s.setValue(QLatin1String("timeout"), timeout );
        s.setValue(QLatin1String("username"), username );
        s.setValue(QLatin1String("password"), password );

		s.endGroup();	// alias
		s.endGroup();	// host
        s.endGroup();	// connections
	}

    void Connection::remove()
	{
		close();
		QSettings s;

        s.beginGroup(QLatin1String("connections"));

		s.beginGroup( host );
		s.remove( alias );

		if ( s.childGroups().size() == 0 )
		{
			s.endGroup();
			s.remove( host );
		}
	}
}



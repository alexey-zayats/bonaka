#include "util.h"

#include <QString>
#include <QStringList>

namespace Entity
{
	QStringList Util::toList( const QString &str )
	{
		QString a = str;
        a.remove( QLatin1Char('{') );
        a.remove( QLatin1Char('}') );
        return a.isEmpty() ? QStringList() : a.split( QLatin1Char(',') );
	}

	QString Util::toArray( const QStringList &list )
	{
        return QLatin1Char('{') + list.join( QLatin1Char(',') ) + QLatin1Char('}');
	}

	int Util::factorial(int num)
	{
		int result = 1;
		for( int i = 1; i <= num; i++ ) {
			result *= i;
		}
		return result;
	}
}

// util.h

#ifndef ENTITY_UTIL_H
#define ENTITY_UTIL_H

class QString;
class QStringList;

#include "entity_global.h"

namespace Entity
{
	class ENTITY_EXPORT Util
	{
		public:
			//Util();

			static QStringList toList( const QString &str );
			static QString toArray( const QStringList &list );
			static int factorial(int);
	};
}

#endif // ENTITY_UTIL_H

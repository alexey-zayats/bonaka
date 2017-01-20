// link.h

#ifndef ENTITY_LINK_H
#define ENTITY_LINK_H

#include "object.h"

namespace Entity
{
	class ENTITY_EXPORT Link : public Object
	{
		Q_OBJECT

	public:
		Link( Keeper *keeper );
		Link( int id, Keeper *keeper );
		Link( const TQueryMap &opts, Keeper *keepers );

	protected:
		void init();
	};
}

#endif // ENTITY_LINK_H

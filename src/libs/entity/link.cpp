// link.cpp

#include "link.h"

namespace Entity
{
	Link::Link(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Link::Link( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Link::Link ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Link::init()
	{
        table = QLatin1String("link");

        Object::setObjectName( QLatin1String("Entity::Link") );
		type = OLink;
		mtype = aTable;

		//		name		name			Title			type		ro	visible
        schema.append( new Attr( QLatin1String("src"),       tr("Source"),      Attr::ObjectRef, false, true ) );
        schema.append( new Attr( QLatin1String("dst"),       tr("Destination"), Attr::ObjectRef, false, true ) );
	}

}

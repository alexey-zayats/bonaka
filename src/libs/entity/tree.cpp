// tree.cpp

#include "tree.h"
#include "keeper.h"

namespace Entity
{
    Tree::Tree(Keeper *keeper) : Object(keeper)
    {
        init();
    }

    Tree::Tree( int id, Keeper *keeper) : Object(keeper)
    {
        init();
        restore( id );
    }

    Tree::Tree ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
    {
        init();
        restore( opts );
    }

    Tree::~Tree()
    {
        qDeleteAll( childs );
    }

    void Tree::init()
    {
        _parent = 0;
        type = OTree;

        childsPopulated = false;
        childsCount = 0;

        table = QLatin1String("tree");
        mtype = aTree;

        Object::setObjectName( QLatin1String("Melampig::Tree") );

        //		name		name	Title			type		ro	visible
        schema.append( new Attr( QLatin1String("trail"), 	tr("Path"),	    Attr::LTree,     true,  true ) );
        schema.append( new Attr( QLatin1String("level"), 	tr("Level"),	Attr::Int, 	     true,  true ) );
        schema.append( new Attr( QLatin1String("pid"),     tr("Pid"),      Attr::ObjectRef, false,  true, OTree ) );
    }

    void Tree::setParent(Tree *parent)
    {
        if( _parent != 0 ) {
            set(QLatin1String("pid"), parent->get(QLatin1String("id")));
            store();
        }
        _parent = parent;
    }

    void Tree::populateChilds()
    {
        if ( childsPopulated ) return;

        childsPopulated = true;

        // new child
        if ( id == 0 && get( QLatin1String("pid") ).toInt() ) return;

        TQueryMap opts;
        if ( id == 0 ) {
            opts.insert( QLatin1String("pid"), keeper->prepareParam( Is | Field, QLatin1String("NULL") )  );
        } else {
            opts.insert( QLatin1String("pid"), keeper->prepareParam( Equal, id )  );
        }
        opts.insert( QLatin1String("order"), QStringList() << QLatin1String("title") );

        Attr *a = 0;
        QString f;
        QStringList fileds = nameSchema();
        for( int i = 0; i < fileds.size(); i++ )
        {
            a = attribute(fileds.at(i));
            if ( a->getType() == Attr::Svg ||
                 a->getType() == Attr::Image ) {
                f = QLatin1String("encode(%1, 'base64') as %1");
                fileds.replace(i, f.arg(a->getName()));
            }
        }

        f = QLatin1String("(select count(*) from %1 as a where a.trail ~ (%1.trail::text || '.*{1}')::lquery )");
        fileds << f.arg(getTable());

        QList<Attr*> attr = attrSchema();

        QList<QVariant> rows = keeper->getListOfFields(type, fileds, opts);
        for(int j = 0; j < rows.size(); j++ ) {
            QStringList row = rows.at(j).toStringList();
            Tree *node = (Tree*)keeper->objectByType(type);
            node->setParent(this);
            for(int i = 0; i < row.size(); i++) {
                if ( (i+1) == row.size() ) {
                    node->setChildCount(row.at(i).toInt());
                } else {
                    node->set(attr.at(i)->getName(), row.at(i));
                }
            }
            childs.append(node);
        }

        setChildCount ( childs.size() );
    }

    int Tree::childCount()
    {
        return childsCount;
    }

    int Tree::childIndex(Tree *child)
    {
        if ( childsCount == 0 && !childsPopulated ) populateChilds();

        if ( childsCount == 0 ) return 0;

        return childs.indexOf(child);
    }

    Tree *Tree::child(int pos)
    {
        if ( !childsPopulated ) populateChilds();

        if ( childsCount == 0 )  return 0;
        if ( pos >= childsCount || pos < 0) return 0;

        return (Tree*)childs.at(pos);
    }

    void Tree::addChild( Tree *tree, int position )
    {
        if ( childsCount == 0 && !childsPopulated ) populateChilds();

        if ( position < 0 )
            childs.append( tree );
        else
            childs.insert(position, tree);

        setChildCount(childs.size());
    }

    void Tree::removeChild( int position )
    {
        if (position < 0 || position > childs.size())
            return;

        childs.removeAt(position);

        setChildCount(childs.size());
    }

    Tree *Tree::parent()
    {
        return _parent;
    }

    bool Tree::insertChild(int position)
    {
         if (position < 0 || position > childs.size())
             return false;

         Tree *item = (Tree*)keeper->objectByType(type);
         item->setParent(this);
         item->set(QLatin1String("pid"), get( QLatin1String("id")));
         childs.insert(position, item);

         setChildCount(childs.size());

         return true;
     }

    bool Tree::deleteChild(int position)
     {
         if (position < 0 || position > childs.size())
             return false;

         Tree *item = (Tree*)childs.takeAt(position);
         item->remove();
         delete item;

         setChildCount(childs.size());

         return true;
     }

    Tree *Tree::search(int sid)
    {
        Tree *node;
        foreach (Object *child, childs)
        {
            if ( child->get(QLatin1String("id")).toInt() == sid ) {
                return (Tree*)child;
            }
            else {
                node = ((Tree*)child)->search(sid);
                if ( 0 != node ) return node;
            }
        }
        return 0;
    }
}

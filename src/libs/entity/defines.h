#ifndef ENTITY_DEFINES_H
#define ENTITY_DEFINES_H

#include <QList>
#include <QHash>
#include <QVariant>

namespace Entity
{

    typedef QList<QVariant> TQueryParam;
    typedef QHash<QVariant, QVariant> TQueryMap;

    enum Expression
    {
        Equal = 1,             // just equal: id = 10
        Not = 2,               // just not equal: id != 10
        Greater = 4,           // used with Equal: id >= 10
        Less = 8,              // used with Equal: id <= 10
        Interval = 16,         // search in range: ( 1 < id ) and ( id < 10 )
        Segment = 32,          // search in range with end-points: ( 1 <= id ) and ( id <= 10 )
        InSet = 64,            // find in set: id IN ( 1,2,3 )
        Like = 128,            // pattern matching
        Case = 256,            // case insensitive tree as ILIKE or lower()

        Field = 512,

        // used with user defined types
        Intersect = 1024,
        Contains = 2048,
        Within = 4096,

        And = 8192,
        Or = 16384,
        Is = 32768,

        // user defined types
        // Treat upper bits for bottom types
        // Starts with: 2^16

        // Queries for data organized as a tree-like structures
        // Intersect: @>
        // Contains: <@
        LTree = 65536,

        // one-dimensional array of int4's
        // Intersect: &&
        // Contains: @
        // Within: ~
        IntArray = 131072,

        // Queries for data organized as a hash-like structures
        // Intersect: @>
        // Contains: <@
        HStore = 262144
	};

}

#endif // ENTITY_DEFINES_H

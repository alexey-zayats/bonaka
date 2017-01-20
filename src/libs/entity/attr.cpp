// attr.cpp

#include "attr.h"

namespace Entity
{
	Attr::Attr()
	{}

    Attr::Attr( const QString &str_name,
                const QString &str_title,
                QByteArray view_type,
                bool is_ro,
                bool is_visible,
                QByteArray ref_type,
                bool isArray )
        : name ( str_name ),
          title( str_title ),
          viewType( view_type ),
          ro( is_ro ),
          visible( is_visible ),
          anArray(isArray),
          referenceType(ref_type)
    {
    }

	Attr& Attr::operator=( const Attr &a )
	{
        setName(a.getName());
        setTitle(a.getTitle());
        setViewType(a.getViewType());
        setReadOnly(a.isReadOnly());
        setVisible(a.isVisible());
        setReferenceType(a.getReferenceType());
		setArray( a.isArray() );
		return *this;
	}

    Attr::~Attr()
    {
    }

}

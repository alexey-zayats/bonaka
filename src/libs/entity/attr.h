// attr.h

#ifndef ENTITY_ATTR_H
#define ENTITY_ATTR_H

#include <QVariant>
#include "entity_global.h"
#include "keeper.h"

namespace Entity
{
    class ENTITY_EXPORT Attr
	{
		public:

			enum ViewType
			{

				Bool = 1, // +
				Int = 2, // +
				Double = 3,

				String = 4, // +
				Text = 5,
				Html = 6,

				Date = 7, // +
				Time = 8, // +
				DateTime = 9, // +

				Email = 10, // +/-
				Password = 11, // +

				File = 12,
				Image = 13,
				Svg = 14,

				ObjectRef = 15,

				Checkbox = 16,
				Radio = 17,

				LTree = 18,
				HStore = 19,
				IntArray = 20

			};

			Attr();
            Attr( const QString &name, const QString &title, QByteArray viewType,
                  bool ro, bool visible, QByteArray refType, bool isArray = false );
			virtual ~Attr();

			Attr& operator=( const Attr &a );

			QString getName() const { return name; }
			QString getTitle() const { return title; }
            QByteArray getViewType() const { return viewType; }
            QByteArray getReferenceType() const { return referenceType;}

			bool isReadOnly() const { return ro; }
			bool isVisible() const { return visible; }
			bool isArray() const { return anArray; }

			void setName( const QString &s ) { name = s; }
            void setViewType( const QByteArray tt ) { viewType = tt; }
			void setReadOnly( bool is ) { ro = is; }
			void setVisible( bool is ) { visible = is; }
			void setTitle( const QString &s ) { title = s; }

            void setReferenceType(const QByteArray o) { referenceType = o;}
			void setArray(const bool s) { anArray = s; }

		private:
			QString name;
			QString title;

            QByteArray viewType;

			bool ro, visible, anArray;
            QByteArray referenceType;
	};
}

#endif // ENTITY_ATTR_H

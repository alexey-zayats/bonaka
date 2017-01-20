// object.h

#ifndef ENTITY_OBJECT_H
#define ENTITY_OBJECT_H

#include <QObject>
#include <QList>
#include <QHash>

#include "entity_global.h"
#include "attr.h"

namespace Entity
{
	class ENTITY_EXPORT Object : public QObject
	{
		Q_OBJECT

		public:
            Object();

			Object(const Object &obj);
			Object &operator=( const Object &obj );

			~Object();

			QList<Attr*> attrSchema() const { return schema; }

			QStringList nameSchema() const;
			Attr *attribute( const QString &name );

			QString getTable() const { return table; }
			QStringList getViewItem() const { return viewItems; }
			QStringList getSearchItems() const { return searchItems; }
			QStringList getFormItems() const { return formItems; }

			void store();
			void remove();

			QString get( const QString &key );
			void set( const QString &key, const QString &value );
			bool contains( const QString &key ) { return data.contains(key); }

			void restore( int id );
			void restore( QSqlQuery &query );
			void restore( const TQueryMap &opts );

            QByteArray getModelType() { return mtype; }
            QByteArray getType() { return type; }
			bool isSearchAble() { return searchable; }

			static QStringList toList( const QString &str );
			static QString toArray( const QStringList &list );

		protected:
			void init();

			QList<Attr*> schema;

			QStringList formItems;
			QStringList viewItems;
			QStringList searchItems;

			QHash< QString, QString > data;

			QString table;

            QByteArray mtype;
            QByteArray type;

			int id;
			bool searchable;
	};
}

#endif // MELAMPIG_OBJECT_H

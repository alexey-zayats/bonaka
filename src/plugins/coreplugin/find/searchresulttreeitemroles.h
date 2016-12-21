#ifndef SEARCHRESULTTREEITEMROLES_H
#define SEARCHRESULTTREEITEMROLES_H

#include <QAbstractItemView>

namespace Core {
namespace Internal {
namespace ItemDataRoles {

enum Roles
{
    ResultItemRole = Qt::UserRole,
    ResultLineRole,
    ResultLineNumberRole,
    ResultIconRole,
    ResultHighlightBackgroundColor,
    ResultHighlightForegroundColor,
    SearchTermStartRole,
    SearchTermLengthRole,
    IsGeneratedRole
};

} // namespace Internal
} // namespace Core
} // namespace ItemDataRoles

#endif // SEARCHRESULTTREEITEMROLES_H

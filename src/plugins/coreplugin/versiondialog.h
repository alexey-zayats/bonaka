#ifndef VERSIONDIALOG_H
#define VERSIONDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QEvent;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class VersionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VersionDialog(QWidget *parent);

    bool event(QEvent *event);

};

} // namespace Internal
} // namespace Core

#endif // VERSIONDIALOG_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QDir;
class QFile;
class QWidget;
class QLineEdit;
class QToolButton;
class QGridLayout;
QT_END_NAMESPACE

#include "widgets_global.h"

namespace Widgets
{
class WIDGETS_EXPORT FileChooser : public QWidget
{
    Q_OBJECT
public:
    FileChooser(QWidget *parent = 0);
    FileChooser(QWidget *parent,
                const QString &caption,
                const QString &directory,
                const QString &filter);

    const QString file();

    void setCaption ( const QString &caption );
    void setNameFilter ( const QString &nameFilter );
    void setDirectory ( const QString &directory );

public slots:
    void chooseFileClicked();

signals:
    void fileChoosed(const QString &file);

private:
    void init();

    QToolButton *m_chooseFile;
    QLineEdit *m_lineEdit;
    QGridLayout *m_layout;

    QString m_choosedFile;

    QString m_caption;
    QString m_nameFilter;
    QString m_directory;
};

}

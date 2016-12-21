#ifndef FINDPLACEHOLDER_H
#define FINDPLACEHOLDER_H

#include "core_global.h"

#include <QPointer>
#include <QWidget>

namespace Core {
namespace Internal { class FindToolBar; }

class CORE_EXPORT FindToolBarPlaceHolder : public QWidget
{
    Q_OBJECT
public:
    explicit FindToolBarPlaceHolder(QWidget *owner, QWidget *parent = 0);
    ~FindToolBarPlaceHolder();
    QWidget *owner() const;
    bool isUsedByWidget(QWidget *widget);

    void setWidget(Internal::FindToolBar *widget);

    static FindToolBarPlaceHolder *getCurrent();
    static void setCurrent(FindToolBarPlaceHolder *placeHolder);

    void setLightColored(bool lightColored);
    bool isLightColored() const;

private:
    QWidget *m_owner;
    QPointer<Internal::FindToolBar> m_subWidget;
    bool m_lightColored;
    static FindToolBarPlaceHolder *m_current;
};

} // namespace Core

#endif // FINDPLACEHOLDER_H

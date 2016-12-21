
#ifndef NAVIGATIONTREEVIEW_H
#define NAVIGATIONTREEVIEW_H

#include "utils_global.h"

#include "itemviews.h"

namespace Utils {

class UTILS_EXPORT NavigationTreeView : public TreeView
{
	Q_OBJECT
public:
	explicit NavigationTreeView(QWidget *parent = 0);

protected:
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);
	void resizeEvent(QResizeEvent *event);
};

} // Utils

#endif // NAVIGATIONTREEVIEW_H

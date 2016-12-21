
#ifndef MANHATTANSTYLE_H
#define MANHATTANSTYLE_H

#include "core_global.h"

#include <QProxyStyle>

class ManhattanStylePrivate;

class CORE_EXPORT ManhattanStyle : public QProxyStyle
{
	Q_OBJECT

public:
	explicit ManhattanStyle(const QString &baseStyleName);

	~ManhattanStyle();

	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;
	void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = 0) const;

	QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const;
	QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const;
	QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget) const;

	SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option, const QPoint &pos, const QWidget *widget = 0) const;
	QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt, const QWidget *widget = 0) const;
	QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *option = 0, const QWidget *widget = 0) const;
	int styleHint(StyleHint hint, const QStyleOption *option = 0, const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const;
	QRect itemRect(QPainter *p, const QRect &r, int flags, bool enabled, const QPixmap *pixmap, const QString &text, int len = -1) const;
	QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const;

	int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const;

	QPalette standardPalette() const;

	void polish(QWidget *widget);
	void polish(QPalette &pal);
	void polish(QApplication *app);

	void unpolish(QWidget *widget);
	void unpolish(QApplication *app);

private:
	void drawButtonSeparator(QPainter *painter, const QRect &rect, bool reverse) const;

	ManhattanStylePrivate *d;
};

#endif // MANHATTANSTYLE_H

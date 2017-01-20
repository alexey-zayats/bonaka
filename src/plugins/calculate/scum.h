#ifndef SCUM_H
#define SCUM_H

#include <QString>

namespace Calculate {

class Scum
{
public:
    Scum(QString kind,
         qreal down,
         qreal up);

    QString kind() { return m_kind; }
    qreal down() { return m_down; }
    qreal up() { return m_up; }
    qreal median() { return m_median; }

private:
    QString m_kind;
    qreal m_down;
    qreal m_up;
    qreal m_median;
};

}

#endif // SCUM_H

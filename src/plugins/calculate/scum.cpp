#include "scum.h"

namespace Calculate {

Scum::Scum (QString kind,
      qreal down,
      qreal up)
{
    m_kind = kind;
    m_down = down;
    m_up = up;
    m_median = (m_down + m_up)/2;
}

}

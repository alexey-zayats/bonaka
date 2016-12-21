
#ifndef OVERRIDECURSOR_H
#define OVERRIDECURSOR_H

#include "utils_global.h"

#include <QCursor>

namespace Utils {

class UTILS_EXPORT OverrideCursor
{
public:
    OverrideCursor(const QCursor &cursor);
    ~OverrideCursor();
    void set();
    void reset();
private:
    bool m_set;
    QCursor m_cursor;
};

}

#endif // OVERRIDECURSOR_H

#ifndef EMAILVALIDATOR_H
#define EMAILVALIDATOR_H

#include <QValidator>
#include "utils_global.h"

QT_BEGIN_NAMESPACE
class QRegExp;
QT_END_NAMESPACE

namespace Utils {

class UTILS_EXPORT EmailValidator : public QValidator
{
    Q_OBJECT
public:
    explicit EmailValidator(QObject *parent = 0);
    State validate(QString &text, int &pos) const;
    void fixup(QString &text) const;

private:
    const QRegExp m_validMailRegExp;
    const QRegExp m_intermediateMailRegExp;
};

}

#endif // EMAILVALIDATOR_H

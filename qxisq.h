#ifndef QXISQ_H
#define QXISQ_H

#include <QObject>
#include "xisq.h"

class QXisq : public QObject,public Xisq
{
    Q_OBJECT
    Q_PROPERTY(double a READ getA WRITE setA)
public:
    explicit QXisq(QObject *parent = nullptr);

signals:

public slots:
    bool slotRead(QString s,int t);
    QString result();
    void slotClear();
};

#endif // QXISQ_H

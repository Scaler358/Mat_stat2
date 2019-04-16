#include "qxisq.h"

QXisq::QXisq(QObject *parent) : QObject(parent),Xisq () {}

bool QXisq::slotRead(QString s,int t){
    switch (t) {
        case 1:return this->addFile(s.toStdString(),Type::Binom);
        case 2:return this->addFile(s.toStdString(),Type::Puas);
        case 3:return this->addFile(s.toStdString(),Type::Norm);
        case 4:return this->addFile(s.toStdString(),Type::Rivnom);
        case 5:return this->addFile(s.toStdString(),Type::Eksponen);
    }
    return this->addFile(s.toStdString(),Default);
}

QString QXisq::result(){
    QString res="";
    res+="Спочатку зчитуємо файли: \n"+QString::fromStdString(this->tableToString());
    res+="\nДалі визначаємо параметр";
    std::pair<double,double> t;
    double p1=0,p2=0;
    switch (this->getType()) {
    case Default:break;
    case Binom:res+=" p = "+QString::number(p1=this->calcParam());break;
    case Puas:res+=" l = "+QString::number(p1=this->calcParam());break;
    case Rivnom: t=this->calcParamRivnom();res+="и a = "+QString::number(p1=t.first)+" та b = "+QString::number(p2=t.second);break;
    case Norm: t=this->calcParamNorm();res+="и l = "+QString::number(p1=t.first)+" та б = "+QString::number(p2=t.second);break;
    case Eksponen:res+=" l = "+QString::number(p1=this->calcParam());break;
    }
    res+=".\n Потім отримуємо такі ймовірності :\n"+QString::fromStdString(this->piToString(p1,p2));
    double Xi;
    if(getType()!=Norm&&getType()!=Rivnom){
        Xi=this->calcXiSq(this->calcParam());
    }
    else{
        if(this->getType()==Norm){
            t=this->calcParamNorm();
        }
        else {
            t=this->calcParamRivnom();
        }
        Xi=this->calcXiSq(t.first,t.second);
    }
    res+="\nНа основі цього визначаємо, що Хі = "+QString::number(Xi)+".\n";
    res+="Для d.f. = "+QString::number(this->get_d_f_())+" та а = "+QString::number(this->getA())+" рахуємо Хі кр.\n";
    double Xikr=this->getXiSqKr();
    res+="Xi кр. = "+QString::number(Xikr)+".\n";
    res+="Через те, що емпіричне значення "+QString::number(Xi)+(Xi<Xikr?" менше ":" більше ")+"критичного "+QString::number(Xikr)+" гіпотезу "+(Xi<Xikr?"приймаємо.\n":"відхиляємо.\n");
    return res;
}

void QXisq::slotClear(){
    this->clear();
}

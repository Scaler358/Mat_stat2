#ifndef XISQ_H
#define XISQ_H

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <list>
#include <climits>
#include "alg/src/specialfunctions.h"



enum Type{
    Default,
    Binom,
    Puas,
    Norm,
    Rivnom,
    Eksponen
};

class Xisq
{
 private:
    Type type;
    std::map<double,int> table;
    std::map<std::pair<double,double>,int> tableN;
    double a;
    int d_f_;

    std::pair<double,int> stop(std::string s);
    std::pair<std::pair<double,double>,int> stopp(std::string s);
    double ser();
    double stand();
    double dysp();
    int fact(int n);
    double paramBinom();
    double paramPuas();
    std::pair<double,double> paramRivnom();
    double paramEksponen();
    std::pair<double,double> paramNorm();
    std::list<std::pair<double,int>> getPi(double param,double param_second=0);
    double xisq(double param,double param_second=0);

public:
    Xisq();
    Xisq operator+=(std::string file);
    bool addFile(std::string file,Type type);
    void setA(double new_a);
    double getA();
    Type getType();
    int get_d_f_();
    int size();
    double N();
    double calcParam();
    std::pair<double,double> calcParamRivnom();
    std::pair<double,double> calcParamNorm();
    double calcXiSq(double param,double param_second=0);
    double getXiSqKr();
    std::string tableToString();
    std::string piToString(double param,double param_second=0);
    void clear();
};

#endif // XISQ_H

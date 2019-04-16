#include "xisq.h"
#include <QDebug>

Xisq::Xisq():type(Default),table(),tableN(),a(0.05),d_f_(-1){}

Xisq Xisq::operator+=(std::string file){
    if(this->type!=Default){
        this->addFile(file,this->type);
    }
    return *this;
}

bool Xisq::addFile(std::string file, Type type){
    std::ifstream file_stream;
    try {
        file_stream.open(file);
    } catch (...) {
        return false;
    }
    if((this->type==type||this->type==Default)&&type!=Default){
        while(!file_stream.eof()){
            std::string temp;
            std::getline(file_stream,temp,'|');
            if(type==Binom||type==Puas)
            {
                std::pair<double,int> p=this->stop(temp);
                this->table[p.first]=p.second;
            }
            else {
                std::pair<std::pair<double,double>,int> p=this->stopp(temp);
                this->tableN[p.first]=p.second;
                this->table[(p.first.first+p.first.second)/2]=p.second;
            }
        }
    }
    if(this->table.size()){
        this->type=type;
        return true;
    }
    file_stream.close();
    return false;
}

void Xisq::setA(double new_a){
    if(new_a<1&&new_a>0)
        this->a=new_a;
}

double Xisq::getA(){
    return this->a;
}

Type Xisq::getType(){
    return this->type;
}

int Xisq::get_d_f_(){
    return this->d_f_;
}

int Xisq::size(){
    int sum_=0;
    for(auto i=this->table.begin();i!=this->table.end();i++){
        sum_+=i->second;
    }
    return sum_;
}

double Xisq::N(){
    if(this->type==Default){
        return INT_MIN;
    }
    double max=INT_MIN;
    for(auto i=this->table.begin();i!=this->table.end();i++){
        if(max<i->first)
            max=i->first;
    }
    return max;
}

double Xisq::calcParam()
{
    switch (this->type) {
    case Default: return INT_MIN;
    case Binom: return this->paramBinom();
    case Puas: return this->paramPuas();
    case Norm: return INT_MIN;
    case Rivnom: return INT_MIN;
    case Eksponen: return this->paramEksponen();
    }
    return 0.0;
}

std::pair<double,double> Xisq::calcParamRivnom(){
    return this->paramRivnom();
}

std::pair<double,double> Xisq::calcParamNorm(){
    return this->paramNorm();
}

double Xisq::calcXiSq(double main,double param_second){
    if(this->type==Default){
        return INT_MIN;
    }
    else if(this->type==Norm||this->type==Rivnom)
        return this->xisq(main,param_second);
    return this->xisq(main);
}

std::pair<double,int> Xisq::stop(std::string s){
    std::string temp1="";
    std::string temp2="";
    bool turn=0;
    for(auto i=s.begin();i!=s.end();i++){
        if(isdigit(*i)){
            if(!turn){
                temp1+=*i;
            }
            else {
                temp2+=*i;
            }
            continue;
        }
        if(*i=='.'||*i=='-'){
            if(!turn)
                temp1+=*i;
        }
        else if(*i==':')
            turn=1;
        else {
            continue;
        }
    }
    return std::pair<double,int>(std::stod(temp1),std::stoi(temp2));
}

std::pair<std::pair<double,double>,int> Xisq::stopp(std::string s){
    std::string temp1="";
    std::string temp2="";
    std::string temp3="";
    bool lim=0;
    bool turn=0;
    for(auto i=s.begin();i!=s.end();i++){
        if(isdigit(*i)){
            if(!turn){
                if(!lim)
                    temp1+=*i;
                else {
                    temp2+=*i;
                }
            }
            else {
                temp3+=*i;
            }
            continue;
        }
        if(*i=='.'||*i=='-'){
            if(!turn){
                if(!lim)
                    temp1+=*i;
                else {
                    temp2+=*i;
                }
            }
        }
        else if(*i==':')
            turn=1;
        else if(*i==';')
            lim=1;
        else {
            continue;
        }
    }
    return std::pair<std::pair<double,double>,int>(std::pair<double,double>(std::stod(temp1),std::stod(temp2)),std::stoi(temp3));
}

double Xisq::ser(){
    if(this->type==Default){
        return INT_MIN;
    }
    double res_sum=0.0;
    for(auto i=this->table.begin();i!=this->table.end();i++){
        res_sum+=i->first*i->second;
    }
    return res_sum/this->size();
}
double Xisq::stand(){
    if(this->type==Default){
        return INT_MIN;
    }
    double res=0;
    double ser_=this->ser();
    for(auto i=this->table.begin();i!=this->table.end();i++){
        res+=pow(i->first-ser_,2)*i->second;
    }
    return sqrt(res/(this->size()-1));
}

double Xisq::dysp(){
    if(this->type==Default){
        return INT_MIN;
    }
    double res=0;
    double ser_=this->ser();
    for(auto i=this->table.begin();i!=this->table.end();i++){
        res+=pow(i->first-ser_,2)*i->second;
    }
    return res/this->size();
}

int Xisq::fact(int n){
    if(n<0)
        return 0;
    else if (n==0) {
        return 1;
    }
    else {
        return n*fact(n-1);
    }
}
double Xisq::paramBinom(){
    if(this->type==Default){
        return INT_MIN;
    }
    return this->ser()/this->N();
}
double Xisq::paramPuas(){
    if(this->type==Default){
        return INT_MIN;
    }
    return this->ser();
}
std::pair<double,double> Xisq::paramRivnom(){
    if(this->type==Default){
        return std::pair<double,double>(INT_MIN,INT_MIN);
    }
    double ser_=this->ser();
    double st=this->stand();
    return std::pair<double,double>(ser_-(sqrt(3)*st),ser_+(sqrt(3)*st));
}

std::pair<double,double> Xisq::paramNorm(){
    if(this->type==Default){
        return std::pair<double,double>(INT_MIN,INT_MIN);
    }
    return std::pair<double,double>(this->ser(),sqrt(this->dysp()));
}

double Xisq::paramEksponen(){
    if(this->type==Default){
        return INT_MIN;
    }
    return (1./this->ser());
}

std::list<std::pair<double,int>> Xisq::getPi(double param,double param_second){
    if(this->type==Default||param<INT_MIN+1){
        throw std::exception();
    }
    std::list<std::pair<double,int>> pi;
    double bufferP=0;
    int bufferI=0;
    int k=0;
    int N=static_cast<int>(this->N());
    double p=0;
    if(this->type==Binom||this->type==Puas){
        for(auto i=this->table.begin();i!=this->table.end();i++,k++){
            if(this->type==Binom){
                double c=fact(N)/(fact(k)*fact(N-k));
                p=c*pow(param,k)*pow(1-param,N-k);
            }
            if(this->type==Puas){
                p=(exp(-param)*pow(param,k))/fact(k);
            }
            p+=bufferP;
            if(i->second+bufferI<5||p*this->size()<10){
                bufferP+=p;
                bufferI+=i->second;
            }
            else {
                pi.push_back(std::pair<double,int>(p,i->second+bufferI));
                bufferP=0;
                bufferI=0;
            }
        }
    }
    else{
        for(auto i=this->tableN.begin();i!=this->tableN.end();i++){
            if(this->type==Norm){
                double t1=i==this->tableN.begin()?t1=INT_MIN:t1=i->first.first;
                double t2=i==--this->tableN.end()?t2=INT_MAX:t2=i->first.second;
                t1=(t1-param)/param_second;
                t2=(t2-param)/param_second;
                p=alglib::normaldistribution(t2)-alglib::normaldistribution(t1);
            }
            else if(this->type==Rivnom){
                double t1=i==this->tableN.begin()?t1=param:t1=i->first.first;
                double t2=i==--this->tableN.end()?t2=param_second:t2=i->first.second;
                p=(t2-t1)/(param_second-param);
            }
            else if(this->type==Eksponen){
                double t1=i==this->tableN.begin()?t1=0:t1=i->first.first;
                double t2=i==--this->tableN.end()?t2=INT_MAX:t2=i->first.second;
                p=std::exp(-param*t1)-std::exp(-param*t2);
            }
            p+=bufferP;
            if(i->second+bufferI<5||p*this->size()<10){
                bufferP+=p;
                bufferI+=i->second;
            }
            else {
                pi.push_back(std::pair<double,int>(p,i->second+bufferI));
                bufferP=0;
                bufferI=0;
            }
        }
    }
    if(bufferP>0.0){
        auto t=pi.back();
        pi.back()=std::pair<double,int>(t.first+bufferP,t.second+bufferI);
    }
    double sum=0;
    for(auto i=pi.begin();i!=--pi.end();i++){
        sum+=i->first;
    }
    pi.back().first=1-sum;
    return pi;
}

double Xisq::xisq(double param,double param_second){
    if(this->type==Default||param<INT_MIN+1){
        return INT_MIN;
    }
    std::list<std::pair<double,int>> pi;
    if(this->type==Norm||this->type==Rivnom)
        pi=this->getPi(param,param_second);
    else {
        pi=this->getPi(param);
    }
    if(this->type==Rivnom||this->type==Norm)
        this->d_f_=pi.size()-3;
    else {
        this->d_f_=pi.size()-2;
    }
    double res=0;
    int s=this->size();
    for(auto i=pi.begin();i!=pi.end();i++){
        double t=i->first*s;
        double xi=pow(i->second-t,2)/t;
        res+=xi;
    }
    return res;
}
double Xisq::getXiSqKr(){
    if(this->d_f_>0)
        return alglib::invchisquaredistribution(this->d_f_,this->a);
    else {
        return INT_MAX;
    }
}

std::string Xisq::tableToString(){
    if(this->type==Default)
        return "";
    else if(this->type==Binom||this->type==Puas){
        std::string res="|";
        for(auto i:table){
            res+=" "+std::to_string(static_cast<int>(i.first))+" : "+std::to_string(i.second)+" |";
        }
        return res;
    }
    else {
        std::string res="|";
        for(auto i=tableN.begin();i!=tableN.end();i++){
            res+=" ["+std::to_string(static_cast<int>(i->first.first))+" ; "+std::to_string(static_cast<int>(i->first.second))+(i==--tableN.end()?" ]":" )")+ " : "+std::to_string(i->second)+" |";
        }
        return res;
    }
}

std::string Xisq::piToString(double param,double param_second){
    if(this->type==Default)
        return "";
    else {
        auto t=this->getPi(param,param_second);
        std::string res="|";
        for(auto i=t.begin();i!=t.end();i++){
            res+=" "+std::to_string(i->first)+" : "+std::to_string(i->second)+" |";
        }
        return res;
    }
}

void Xisq::clear(){
    this->table.clear();
    this->tableN.clear();
    this->type=Default;
}

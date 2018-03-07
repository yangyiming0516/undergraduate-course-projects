#include <fstream>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace std;
#define del 0.05
int unisum,bisum;
map<string,int> unidic;
map<pair<string,string>,int> bidic;
map<int,double> uniGT,biGT;
map<int,double> d;

//good-turing

double score(string &st){
    string s1,s2;
    istringstream sen2(st);
    sen2>>s1;
    double t1,t2;
    double w=0;
     while (!sen2.eof()){
        sen2>>s2;
        //cout <<s1 <<' '<<s2<<endl;
        if (unidic.find(s1)==unidic.end()){
            t1=uniGT[0];
        }
        else t1=uniGT[unidic[s1]];
        if (bidic.find(pair<string,string>(s1,s2))==bidic.end()){
            t2=biGT[0];
        }
        else t2=biGT[bidic[pair<string,string>(s1,s2)]];
        w+=log(t2/t1);
        //cout <<t1<<' '<<t2<<endl;
        //cout <<log(t2/t1)<<endl;
        s1=s2;
    }
    return w;
}


//add-del
/*
double score(string &st){
    string s1,s2;
    double numuni,numbi;
    istringstream sen(st);
    double w=0;
    sen>>s1;
    while (!sen.eof()){
        sen>>s2;
        if (unidic.find(s1)==unidic.end()) numuni=0; else numuni=unidic[s1];
        if (bidic.find(pair<string,string>(s1,s2))==bidic.end()) numbi=0; else numbi=bidic[pair<string,string>(s1,s2)];
        //»ØÍË
        if (numbi>0)
        w+=log(((numbi+del)/(bisum+del*unidic.size()*unidic.size()))/((numuni+del)/(unisum+del*unidic.size())));
        else{
        if (unidic.find(s2)==unidic.end()) numuni=0; else numuni=unidic[s2];
        w+=log(0.0001*(numuni+del)/(unisum+del*unidic.size()));
        }
        s1=s2;
    }

    return w;
}
*/

int main(){
    ifstream data;
    data.open("unigram-count.txt");
    string s;
    int n;
    while (1){
        data>>s>>n;
        if (data.eof()) break;
        unidic[s]=n;
        unisum+=n;
        uniGT[n]+=1;
    }
    data.close();
    data.open("bigram-count.txt");
    string s2;
    while (1){
        data>>s>>s2>>n;
        if (data.eof()) break;
        bidic[pair<string,string>(s,s2)]=n;
        bisum+=n;
        biGT[n]+=1;
    }
    data.close();
    map<int,double>::iterator it;
    double tmp=uniGT[1];
    for (it=uniGT.begin();it!=uniGT.end();it++){
        int r=it->first;
        if (uniGT.find(r+1)==uniGT.end()) break;
        d[r]=uniGT[r+1]*(r+1)/uniGT[r];
    }
    for (;it!=uniGT.end();it++){
        d[it->first]=it->first;
    }
    uniGT[0]=1;
    d[0]=tmp;
    double p=0;
    for (it=uniGT.begin();it!=uniGT.end();it++){
        p+=d[it->first]*it->second/unisum;
    }
    for (it=uniGT.begin();it!=uniGT.end();it++){
        it->second=d[it->first]/unisum/p;
    }

    tmp=biGT[1];
    for (it=biGT.begin();it!=biGT.end();it++){
        int r=it->first;
        if (biGT.find(r+1)==biGT.end()) break;
        d[r]=biGT[r+1]*(r+1)/biGT[r];
    }
    for (;it!=biGT.end();it++){
        d[it->first]=it->first;
    }
    biGT[0]=1;
    d[0]=tmp;
    p=0;
    for (it=biGT.begin();it!=biGT.end();it++){
        p+=d[it->first]*it->second/bisum;
    }
    for (it=biGT.begin();it!=biGT.end();it++){
        it->second=d[it->first]/bisum/p;
    }

    uniGT[0]/=unidic.size();
    biGT[0]/=unidic.size()*unidic.size()-bidic.size();
    cout << "ready" << endl;

    data.open("test-init.txt");
    ofstream ans;
    ans.open("myanswer.txt");

    while (!data.eof()){
        getline(data,s);
        double res1=score(s);
        getline(data,s);
        double res2=score(s);
        getline(data,s);
        if (res1>res2) ans<<1; else ans<<2;
        ans<<endl<<endl;
        //system("Pause");
        //cout <<res1 << ' '<<res2<<endl;
    }

    return 0;
}

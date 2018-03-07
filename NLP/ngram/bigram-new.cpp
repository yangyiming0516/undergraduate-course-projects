#include <fstream>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace std;

int unisum,bisum;
map<string,int> unidic;
map<pair<string,string>,int> bidic;
map<int,double> uniGT,biGT;
map<int,double> d;
map<string,double> Q;
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
        if (bidic.find(pair<string,string>(s1,s2))!=bidic.end()){
            int num=bidic[pair<string,string>(s1,s2)];
            w+=log((d[num]/bisum)/(unidic[s1]/unisum));
        }else{

            int num=unidic[s2];
            double x=num/unisum/biGT[0];
            if (Q.find(s1)!=Q.end()) x*=Q[s1];
            else{

                double y=1;
                map<string,int>::iterator ite;
                for (ite=unidic.begin();ite!=unidic.end();ite++){
                    pair<string,string> pa(s1,ite->first);
                    if (bidic.find(pa)!=bidic.end()){
                        y-=(d[bidic[pa]]/bisum)/(unidic[s1]/unisum);
                    }
                }
                x*=y;
                Q[s1]=y;



            }
            w+=log(x);


        }
        //cout <<t1<<' '<<t2<<endl;
        //cout <<log(t2/t1)<<endl;
        s1=s2;
    }
    return w;
}

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
    biGT[0]=biGT[1]/bisum;
    for (int i=1;i<10;i++){
        d[i]=biGT[i+1]*(i+1)/biGT[i];
    }

    for (it=biGT.find(10);it!=biGT.end();it++){
        d[it->first]=it->first;
    }

    cout << "ready" << endl;

    data.open("test-init.txt");
    ofstream ans;
    ans.open("myanswer.txt");
    int nn=0;
    while (!data.eof()){
        nn++;
        getline(data,s);
        double res1=score(s);
        nn++;
        getline(data,s);
        double res2=score(s);
        getline(data,s);
        nn++;
        if (res1>res2) ans<<1; else ans<<2;
        ans<<endl<<endl;
        //cout <<nn<<endl;
        //system("Pause");
        //cout <<res1 << ' '<<res2<<endl;
    }

    return 0;
}

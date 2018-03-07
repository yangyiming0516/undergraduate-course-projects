#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <io.h>
#include <vector>
#include <cmath>

#define K 30
//在此设置K
using namespace std;

int cluster[19000];
int cnt[50];
int num[20]={790,970,980,980,960,980,970,990,990,990,990,990,980,990,980,990,910,940,770,620};
int kind[19000];
vector<int> dk[20],dc[50];
int P[20][30];
double F[20][30];

int main(){
    ifstream res;
    res.open("v1tfidf30res.txt");
	//对应不同输入
    int n;
    for (int i=0;i<18760;i++){
        cout <<i<<endl;
        res >> n;
        cnt[n]++;
        dc[n].push_back(i);
        cluster[i]=n;

        ostringstream s;
        s<<"data/*-"<<i+1<<".txt";
        string fileName=s.str();
        _finddata_t fileInfo;
        long handle = _findfirst(fileName.c_str(), &fileInfo);
        int p;
        sscanf(fileInfo.name,"%d",&p);
        kind[i]=p-1;
        dk[p-1].push_back(i);
        P[p-1][n]+=1;
    }

    for (int i=0;i<20;i++){
        for (int j=0;j<K;j++)
            cout << P[i][j] <<' ';
        cout <<endl;
    }

    double E=0;
    for (int j=0;j<K;j++){
        double Ej=0;
        for (int i=0;i<20;i++){
            if (P[i][j]!=0)
            Ej-=P[i][j]*log(P[i][j]*1.0/cnt[j])/cnt[j];
        }
        E+=cnt[j]*Ej;
        //cout << cnt[j]*Ej <<endl;
    }
    E/=18760;
    cout <<"E="<<endl;
    cout << E << endl;

    for (int i=0;i<20;i++)
        for (int j=0;j<K;j++)
    {

        double r=1.0*P[i][j]/num[i];
        double p=1.0*P[i][j]/cnt[j];
        F[i][j]=2*r*p/(r+p);
    }
    double f=0;
    for (int i=0;i<20;i++){
        double max=0;
        for (int j=0;j<K;j++) if (F[i][j]>max) max=F[i][j];
        f+=num[i]*1.0/18760*max;
        //cout << num[i]*1.0/18760*max<< endl;
    }
    cout << "F="<<endl;
    cout <<f<<endl;
    return 0;
}

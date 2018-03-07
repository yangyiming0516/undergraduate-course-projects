#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

#define num 1110

map<int,double> data[19000];
int type[19000];
int K;
vector<double> center[50];
vector<int> cluster[50];
int maxn=100000;

double getdata(int r,int s){
    if (data[r].find(s)!=data[r].end()) {return data[r][s];} else return 0;
}

double dis(int x,int y){
    double p=0;
    for (int i=0;i<num;i++){
        p+=getdata(x,i)*center[y][i];
    }
    return p;
}


void KMeans(){
    double change=2;
    maxn=0;
    vector<double> s;
    for (int i=0;i<num;i++) s.push_back(0);
    while (maxn<50&&change>0.01){
        maxn++;
        cout<<"第"<<maxn<<"次迭代"<<endl;
        double max=0;
        for (int i=0;i<18760;i++){
            max=0;
            for (int j=0;j<K;j++)
                {
                    double tmp=dis(i,j);
                    if (tmp>max){
                        max=tmp;
                        type[i]=j;
                    }
                }
        }

        change=0;
        for (int i=0;i<K;i++) cluster[i].clear();
        for (int i=0;i<18760;i++){
            cluster[type[i]].push_back(i);
        }

        //for (int i=0;i<K;i++) cout << cluster[i].size()<<endl;

        double sum=0;
        for (int i=0;i<K;i++){
            for (int j=0;j<num;j++) s[j]=0;
            for (int j=0;j<cluster[i].size();j++){
               for (int w=0;w<num;w++)
               s[w]+=getdata(cluster[i][j],w);
            }

            for (int j=0;j<num;j++){
                s[j]/=cluster[i].size();
                change+=fabs(s[j]-center[i][j]);
                center[i][j]=s[j];
            }
        }
        cout << "change="<<change<<endl;
    }

}

int main(){
    cout <<"输入K值" << endl;
    cin >> K;

    ifstream v;
    v.open("v1tfidf.txt");
    double t;
    int n=1,i=0,p;
    for (int i=1;i<=18760;i++){
        v>>p;
        if (p!=i) cout << "wrong" << endl;
        v>>p;
        while (p!=-1){
            v>>t;
            data[i-1][p-1]=t;
            v>>p;
        }
    }

    srand((unsigned int)time(NULL));
    for (int i=0;i<K;i++){
        int tmp=rand()%18760;
        for (int j=0;j<num;j++)
        center[i].push_back(getdata(tmp,j));
    }

    KMeans();

    ofstream res;
    res.open("v2tfidf20res.txt");
    for (int i=0;i<18760;i++)
        res<<type[i]<<endl;
    return 0;
}

#include <cstdio>
#include <iostream>
#include <io.h>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

vector<string> checkword;
map<string,int> num;
map<string,int> tmp;

int main(){
    ifstream dict;
    dict.open("rank2.txt");
    string word;
    int n;
    while (!dict.eof()){
        dict>>n>>word;
        checkword.push_back(word);
    }
    ifstream idf;
    idf.open("idf.txt");
    while (!idf.eof()){
        idf>>n>>word;
        num[word]=n;
    }
    ofstream out;
    out.open("v2tfidf.txt");
    for (int i=1;i<=18760;i++){
        cout<<"No"<<i<<endl;
        ostringstream s;
        s<<"data/*-"<<i<<".txt";
        string fileName=s.str();
        _finddata_t fileInfo;
        long handle = _findfirst(fileName.c_str(), &fileInfo);

        do
        {
            ostringstream ss;
            ss<<"data/"<<fileInfo.name;
            ifstream input;
            input.open(ss.str());
            tmp.clear();
            string w;
            int q;
            int cnt=0;
            while (!input.eof()){
                input >> w >> q;
                cnt+=q;
                if (tmp.find(w)!=tmp.end()) tmp[w]+=q;else tmp[w]=q;
            }
            vector<string>::iterator it;
            vector<double> T;
            double sum=0;
            for (it=checkword.begin();it!=checkword.end();it++){
                double m=tmp[*it]*1.0/cnt;
                m*=log(18760/(1+num[*it]));
                T.push_back(m);
                sum+=m*m;
            }
            sum=sqrt(sum);
            out << i << endl;
            for (int j=0;j<T.size();j++){
                if (T[j]==0) continue;
                out<<j<<' '<<T[j]/sum<<endl;
            }
            out<<-1;
            out<<endl;
        } while (_findnext(handle, &fileInfo) == 0);
    }

    return 0;
}

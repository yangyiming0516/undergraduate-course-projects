/*
    By YY_More
    compare
*/
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main(){
    ifstream answer;
    answer.open("test.answer.txt");
    ifstream myanswer;
    myanswer.open("res.txt");
    string s1,s2;
    int cR=0,cA=0,cM=0;
    for (int i=0;i<3984;i++){
        getline(answer,s1);
        getline(myanswer,s2);
        istringstream i1(s1);
        istringstream i2(s2);
        map<string,int> D1,D2;
        string word;
        while (1){
            if (i1.eof()) break;
            i1>>word;
            D1[word]++;
            cA++;
        }
        while (1){
            if (i2.eof()) break;
            i2>>word;
            D2[word]++;
            cM++;
        }
        map<string,int>::iterator it;
        for (it=D2.begin();it!=D2.end();it++){
            int tmp=D1[it->first];
            cR+=tmp<it->second?tmp:it->second;
        }
    }
    double pre=1.0*cR/cM;
    double rec=1.0*cR/cA;
    double F=pre*rec*2/(pre+rec);
    cout << "precision:"<<pre<<endl;
    cout<<"recall:"<<rec<<endl;
    cout<<"F-score:"<<F<<endl;

    return 0;
}

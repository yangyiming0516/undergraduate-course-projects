#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

map<string,int> D;

int main(){
    ifstream data;
    data.open("train-init.txt");
    string s;
    while (1){
        data >> s;
        if (data.eof()) break;
        D[s]++;
    }
    data.close();
    map<string,int>::iterator it;
    ofstream cnt;
    cnt.open("unigram-count.txt");
    for (it=D.begin();it!=D.end();it++){
        cnt<<it->first<<' '<<it->second<<endl;
    }
    cnt.close();
    return 0;
}

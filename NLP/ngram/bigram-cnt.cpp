#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

map<pair<string,string>,int> D;

int main(){
    ifstream data;
    data.open("train-init.txt");
    string s;
    string l;
    while (1){
        data >> s;
        if (data.eof()) break;
        if (s!="[s]"){
            D[pair<string,string>(l,s)]++;
        }
        l=s;
    }
    data.close();
    map<pair<string,string>,int>::iterator it;
    ofstream cnt;
    cnt.open("bigram-count.txt");
    for (it=D.begin();it!=D.end();it++){
        cnt<<(it->first).first<<' '<<(it->first).second<<' '<<it->second<<endl;
    }
    cnt.close();
    return 0;
}

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

multimap<int,string> p;

int main(){
    ifstream dict;
    dict.open("dict2.txt");
    ofstream out;
    out.open("rank2.txt");
    string word;
    int n;
    while (!dict.eof()){
        dict>>word>>n;
        p.insert(pair<int,string>(n,word));
    }
    multimap<int,string>::reverse_iterator it;
    for (it=p.rbegin();it!=p.rend();it++)
        out<<it->first<<' '<<it->second<<endl;
    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <cstdlib>
using namespace std;

int main(){
    ifstream data;
    data.open("test.txt");
    ofstream output;
    output.open("test-init.txt");
    string s;
    for (int i=0;i<30000;i++){
        getline(data,s);
        istringstream input(s);
        output << "[s] ";
        while (1){
        input>>s;
        if (input.eof()) break;

        if (atof(s.c_str())!=0) output<<"0 ";
        else{
        for (int i=0;i<s.size();i++){
            if (s[i]>='A'&&s[i]<='Z') s[i]+=32;
            }
            output<<s<<' ';
        }
        }
        output<<"[e]"<<endl;
    }


    return 0;
}

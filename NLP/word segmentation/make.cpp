#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

int main(){
    ifstream input;
    input.open("test.answer.txt");
    ofstream output;
    output.open("mytest.answer.txt");
    string line,word;
    while (1){
        getline(input,line);
        if (input.eof()) break;
        istringstream ss(line);
        while (1){
            ss>>word;
            if (ss.eof()) break;
            output<<word<<' ';
        }
        output<<endl;
    }

    return 0;
}

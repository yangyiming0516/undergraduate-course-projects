#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(){
    ifstream ans;
    ans.open("test_answer.txt");
    ifstream myans;
    myans.open("myanswer.txt");
    int cnt=0;
    int a,b;
    for (int i=0;i<10000;i++){
        ans>>a;
        myans>>b;
        if (a==b) cnt++;

    }
    cout << cnt*1.0/10000<<endl;
    return 0;
}

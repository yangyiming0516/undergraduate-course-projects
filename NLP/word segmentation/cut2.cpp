/*
    By YY_More
    structure perceptron
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

map<string,int> Dict;
vector<vector<int> > Data,Data2;
vector<vector<string> >Data3;
vector<vector<bool> > Status;
map<int,double> W[2][2];
double F[1000][2];
int P[1000][2];

int main(){
    int N;
    cout<<"ÊäÈëÑµÁ·´ÎÊý"<<endl;
    cin>>N;
    ifstream input;
    input.open("mytrain.txt");
    string line;
    string word;
    int n=0;
    Dict[" "]=0;
    while (1){
        if (input.eof()) break;
        getline(input,line);
        istringstream ss(line);
        bool flag=true;
        vector<int> data;
        vector<bool> status;
        while (1){
            ss>>word;
            if (ss.eof()) break;
            if (word=="$"){
                flag=true;
                continue;
            }
            if (Dict.find(word)==Dict.end())
                Dict[word] = ++n;
            data.push_back(Dict[word]);
            status.push_back(flag);
            flag=false;
        }
        Data.push_back(data);
        Status.push_back(status);
    }
    for (int k=1;k<=N;k++){
        for (int st=0;st<k;st++)
        for (int i=st;i<Data.size();i+=k){
            memset(F,0,sizeof(F));
            memset(P,0,sizeof(P));
            vector<int>::iterator it;
            vector<int> f[1000];
            for (int j=0;j<Data[i].size();j++){
                if (j>0) f[j].push_back(Data[i][j-1]+1); else f[j].push_back(1);
                f[j].push_back(Data[i][j]+Dict.size()+1);
                if (j<Data[i].size()-1) f[j].push_back(Data[i][j+1]+2*Dict.size()+1); else f[j].push_back(2*Dict.size()+1);
                if (j>0) f[j].push_back(Data[i][j-1]*Dict.size()+Data[i][j]+3*Dict.size()+1); else f[j].push_back(Data[i][j]+3*Dict.size()+1);
                if (j<Data[i].size()-1)
                    f[j].push_back(Data[i][j]*Dict.size()+Data[i][j+1]+Dict.size()*Dict.size()+3*Dict.size()+1);
                else
                    f[j].push_back(Data[i][j]*Dict.size()+Dict.size()*Dict.size()+3*Dict.size()+1);
                int p,q;
                if (j==0){
                    F[0][1]=0;F[0][0]=0;
                    P[0][1]=0;P[0][0]=0;
                    for (it=f[j].begin();it!=f[j].end();it++){
                        F[0][1]+=W[0][1][*it];
                        F[0][0]+=W[0][0][*it];
                    }
                }
                else{
                    for (q=0;q<=1;q++){
                        double n0=F[j-1][0],n1=F[j-1][1];
                        for (it=f[j].begin();it!=f[j].end();it++){
                            n0+=W[0][q][*it];
                            n1+=W[1][q][*it];
                        }
                        if (n0>n1){F[j][q]=n0;P[j][q]=0;} else {F[j][q]=n1;P[j][q]=1;}
                    }
                }
            }
            int last;
            if (F[Data[i].size()-1][0]>=F[Data[i].size()-1][1]) last=0; else last=1;
            vector<int> seq;
            seq.push_back(last);
            for (int h=Data[i].size()-1;h>0;h--){
                    last=P[h][last];
                    seq.push_back(last);
            }
            /*for (int h=0;h<Data[i].size();h++) cout <<seq[Data[i].size()-1-h];
            cout<<endl;
            for (int h=0;h<Data[i].size();h++) cout << Status[i][h];
            cout << endl<<endl;
            */
            int last2=0;
            last=0;
            for (int h=0;h<Data[i].size();h++){
                if (seq[Data[i].size()-1-h]!=Status[i][h]){
                    for (it=f[h].begin();it!=f[h].end();it++){
                        W[last][Status[i][h]][*it]+=1;
                        W[last2][seq[Data[i].size()-1-h]][*it]-=1;
                    }
                }
                last=Status[i][h];
                last2=seq[Data[i].size()-1-h];
               // cout <<last <<' '<<last2<<endl;
            }
        }
    }

    ifstream test;
    test.open("mytest.txt");
    while (1){
        getline(test,line);
        if (test.eof()) break;
        istringstream ss(line);
        vector<int> data;
        vector<string> dataa;
        while (1){
            ss>>word;
            if (ss.eof()) break;
            if (Dict.find(word)==Dict.end())
                data.push_back(0);
            else
            data.push_back(Dict[word]);
            dataa.push_back(word);
        }
        Data2.push_back(data);
        Data3.push_back(dataa);
    }
    ofstream output;
    output.open("res.txt");
    for (int i=0;i<Data2.size();i++){
            memset(F,0,sizeof(F));
            memset(P,0,sizeof(P));
            vector<int>::iterator it;
            vector<int> f[1000];
            for (int j=0;j<Data2[i].size();j++){
                if (j>0) f[j].push_back(Data2[i][j-1]+1); else f[j].push_back(1);
                f[j].push_back(Data2[i][j]+Dict.size()+1);
                if (j<Data2[i].size()-1) f[j].push_back(Data2[i][j+1]+2*Dict.size()+1); else f[j].push_back(2*Dict.size()+1);
                if (j>0) f[j].push_back(Data2[i][j-1]*Dict.size()+Data2[i][j]+3*Dict.size()+1); else f[j].push_back(Data2[i][j]+3*Dict.size()+1);
                if (j<Data2[i].size()-1)
                    f[j].push_back(Data2[i][j]*Dict.size()+Data2[i][j+1]+Dict.size()*Dict.size()+3*Dict.size()+1);
                else
                    f[j].push_back(Data2[i][j]*Dict.size()+Dict.size()*Dict.size()+3*Dict.size()+1);
                int p,q;
                if (j==0){
                    F[0][1]=0;F[0][0]=0;
                    P[0][1]=0;P[0][0]=0;
                    for (it=f[j].begin();it!=f[j].end();it++){
                        F[0][1]+=W[0][1][*it];
                        F[0][0]+=W[0][0][*it];
                    }
                }
                else{
                    for (q=0;q<=1;q++){
                        double n0=F[j-1][0],n1=F[j-1][1];
                        for (it=f[j].begin();it!=f[j].end();it++){
                            n0+=W[0][q][*it];
                            n1+=W[1][q][*it];
                        }
                        if (n0>n1){F[j][q]=n0;P[j][q]=0;} else {F[j][q]=n1;P[j][q]=1;}
                    }
                }
            }
            int last;
            if (F[Data2[i].size()-1][0]>=F[Data2[i].size()-1][1]) last=0; else last=1;
            vector<int> seq;
            seq.push_back(last);
            for (int h=Data2[i].size()-1;h>0;h--){
                    last=P[h][last];
                    seq.push_back(last);
            }
            for (int h=0;h<Data2[i].size();h++){
                if (seq[Data2[i].size()-1-h]==true) output<<' ';
                output<< Data3[i][h];
            }
            output<<endl;
        }
    return 0;
}

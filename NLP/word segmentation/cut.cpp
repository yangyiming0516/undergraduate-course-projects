/*
    By YY_More
    normal perceptron
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

map<string,int> Dict;
vector<vector<int> > Data,Data2;
vector<vector<string> >Data3;
vector<vector<bool> > Status;
map<int,double> W;


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
            for (int j=0;j<Data[i].size();j++){
                vector<int> f;
                if (j>0) f.push_back(Data[i][j-1]+1); else f.push_back(1);
                f.push_back(Data[i][j]+Dict.size()+1);
                if (j<Data[i].size()-1) f.push_back(Data[i][j+1]+2*Dict.size()+1); else f.push_back(2*Dict.size()+1);
                if (j>0) f.push_back(Data[i][j-1]*Dict.size()+Data[i][j]+3*Dict.size()+1); else f.push_back(Data[i][j]+3*Dict.size()+1);
                if (j<Data[i].size()-1)
                    f.push_back(Data[i][j]*Dict.size()+Data[i][j+1]+Dict.size()*Dict.size()+3*Dict.size()+1);
                else
                    f.push_back(Data[i][j]*Dict.size()+Dict.size()*Dict.size()+3*Dict.size()+1);
                vector<int>::iterator it;
                double ny=0,nn=0;
                for (it=f.begin();it!=f.end();it++){
                    ny+=W[*it];
                    nn+=W[-*it];
                }
                bool ft;
                if (ny>nn) ft=true; else ft=false;
                if (ft!=Status[i][j])
                    if (ft==true){
                        for (it=f.begin();it!=f.end();it++){
                        W[*it]-=1;
                        W[-*it]+=1;
                        }
                    }
                    else{
                        for (it=f.begin();it!=f.end();it++){
                        W[*it]+=1;
                        W[-*it]-=1;
                        }
                    }
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
        for (int j=0;j<Data2[i].size();j++){
                vector<int> f;
                if (j>0) f.push_back(Data2[i][j-1]+1); else f.push_back(1);
                f.push_back(Data2[i][j]+Dict.size()+1);
                if (j<Data2[i].size()-1) f.push_back(Data2[i][j+1]+2*Dict.size()+1); else f.push_back(2*Dict.size()+1);
                if (j>0) f.push_back(Data2[i][j-1]*Dict.size()+Data2[i][j]+3*Dict.size()+1); else f.push_back(Data2[i][j]+3*Dict.size()+1);
                if (j<Data2[i].size()-1)
                    f.push_back(Data2[i][j]*Dict.size()+Data2[i][j+1]+Dict.size()*Dict.size()+3*Dict.size()+1);
                else
                    f.push_back(Data2[i][j]*Dict.size()+Dict.size()*Dict.size()+3*Dict.size()+1);
                vector<int>::iterator it;
                double ny=0,nn=0;
                for (it=f.begin();it!=f.end();it++){
                    ny+=W[*it];
                    nn+=W[-*it];
                }
                if (ny>=nn) output<<' ';
                output<<Data3[i][j];
        }
        output<<endl;
    }
    return 0;
}

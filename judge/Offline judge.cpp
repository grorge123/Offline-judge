#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;
bool checkinp(string inp,int SZ){
    if(inp.size()!=SZ){cout <<"輸出錯誤"<<endl;return false;}
    for(int i=0;i<inp.size();i++){
        if(inp[i]<'0'||inp[i]>'9'){
            cout << "輸出錯誤"<<endl;
            return false;
        }
    }
    return true;
}
string tryhavefile(vector<string> name){
    while(true){
        for(int i=0;i<name.size();i++){
            fstream finp;
            finp.open("./inp/"+name[i]+".cpp",ios::in);
            if(finp){
                return name[i];
            }
        }
    }
    return 0;
}
int compile(string inp,string ans,string pro,int tim){
    fstream finp,fans,fpro;
    fans.open("./ans/"+ans+".cpp",ios::in);
    finp.open("./inp/"+inp+".cpp",ios::in);
    fpro.open("./pro/"+pro+".txt",ios::in);
    if(!finp){
        cout << "輸入檔名錯誤!"<<endl;
        return 0;
    }
    if(!fans){
        cout << "輸入答案檔名錯誤!"<<endl;
        return 5;
    }
    if(!fpro){
        cout <<"題目找不到"<<endl;
        return 5;
    }
    string cmd = "g++ -g -O2 -std=gnu++11 -o ./out/"+inp+".exe"+" ./inp/"+inp+".cpp";
    cout << cmd<<endl;
    if(system(cmd.c_str())){
        return 0;
    }
    cmd="g++ -g -O2 -std=gnu++11 -o ./ansout/" + ans + ".exe" + " ./ans/" + ans + ".cpp";
    if(system(cmd.c_str())){
        cout << "答案編輯錯誤!"<<endl;
        return 5;
    }
    auto startTime = steady_clock::now();
    string cmdS = "ansout\\" + ans + ".exe < " + "pro\\" + pro + ".txt > " + "judge\\" + ans + ".txt";
//    cout << cmdS<<endl;
    system(cmdS.c_str());
    //cout << "correct answer cost " << (steady_clock::now() - startTime).count() << " at test" << i << endl;
    auto timeout =  (steady_clock::now() - startTime) * 15;
    auto timeout2 = seconds(tim);
    cmdS = "out\\" + inp + ".exe < " + "pro\\" + pro + ".txt > " + "judge\\" + inp + ".txt";
    startTime = steady_clock::now();
    int alltime = time(0);
    if(system(cmdS.c_str()))return 4;
    auto currentTime = steady_clock::now();
    alltime = alltime-time(0);
//    cout << "submit answer cost  " << (steady_clock::now() - startTime).count() << endl;
    if(currentTime - startTime > timeout || currentTime - startTime > timeout2 ) {
        return 2;
    }
    return 1;
}
int judge(string inp,string ans, string pro){
    fstream finp,fans;
    fans.open("./judge/"+ans+".txt",ios::in);
    finp.open("./judge/"+inp+".txt",ios::in);
    vector<string> ansarry,inparry;
    string now;
    while(fans>>now){
        ansarry.push_back(now);
    }
    while(finp>>now){
        inparry.push_back(now);
    }
//    for(int i=0;i<ansarry.size();i++){
//        cout << ansarry[i]<<endl;
//    }
//    for(int i=0;i<inparry.size();i++){
//        cout << inparry[i]<<endl;
//    }
    if(inparry.size()!=ansarry.size()){
        cout << inparry.size()<<" "<<ansarry.size()<<endl;
        return 3;
    }
    for(int i=0;i<inparry.size();i++){
        if(inparry[i]!=ansarry[i]){
            cout <<inparry[i]<<" "<<ansarry[i]<<endl;
            return 3;
        }
    }
    return 1;
}
void rm(string inp,string ans){
    string cmd = "del judge\\" + inp + ".txt";
    system(cmd.c_str());
    cmd = "del judge\\" + ans + ".txt";
    system(cmd.c_str());
    cmd = "del out\\" + inp + ".exe";
    system(cmd.c_str());
    cmd = "del ansout\\" + ans + ".exe";
    system(cmd.c_str());
    cmd = "del inp\\" + inp + ".cpp";
    system(cmd.c_str());
}
int main(){
    string inp,ans,pro,out,type;
    int tim;
    vector<string> name;
    while(true){
        do{
            cout << "請選擇模式:(1)單一檔案測試 (2)多筆檔案測試";
            cin >> type;
        }while(!checkinp(type,1));
        if(type=="1"){
            int SZ;
            cout << "請輸入數量,答案檔名,問題資料夾,時間";
            cin >>SZ>>ans>>pro>>tim;
            for(int i=0;i<SZ;i++){
                string tmp;
                cin >> tmp;
                name.push_back(tmp);
            }
        }else{
            cout << "請輸入程式檔名,答案檔名,問題資料夾,時間";
            cin >> inp>>ans>> pro>>tim;
        }
        do{
            if(type=="1")inp=tryhavefile(name);
            int result=compile(inp,ans,pro,tim);
            if(result==1){
                result=judge(inp,ans,pro);
            }
            switch(result){
            case 0:
                out="Oh! CE! Maybe you need to see your code again";
                cout << "Oh! CE! Maybe you need to see your code again"<<endl;
                break;
            case 1:
                out="Wow!That is AC!!!You are so smart!";
                cout << "Wow!That is AC!!!You are so smart!"<<endl;
                break;
            case 2:
                out="TLE! Your program run time is to more";
                cout << "TLE! Your program run time is to more"<<endl;
                break;
            case 3:
                out="WA!!Your program runs result is not same as anser!!!(Be careful!!!Otherwise you will see me again) *\(O_O)/*";
                cout << "WA!!Your program runs result is not same as anser!!!(Be careful!!!Otherwise you will see me again) *\(O_O)/*"<<endl;
                break;
            case 4:
                out="RE!!Maybe some bug in your code but it con be compile";
                cout << "RE!!Maybe some bug in your code but it con be compile"<<endl;
                break;
            case 5:
                out="SE!! System have some error!";
                cout << "SE!! System have some error!"<<endl;
                return 0;
            }
            fstream file;
            file.open("./result/"+inp+".txt",ios::app);
            time_t now = time(0);
            file<<ctime(&now)<<out<<' '<<pro<<'\n';
            file.clear();
            rm(inp,ans);
        }while(type=="1");
    }
    return 0;
}

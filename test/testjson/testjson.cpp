#include "json.hpp"
//using namespace nlohmann;
using json = nlohmann::json;
#include<iostream>
#include<vector>
#include<map>
#include<string>
using namespace std;

//json序列化示例1
string func1(){
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhangsan";
    js["to"] = "li si";
    js["msg"] = "hello, what are you doing now?";

    string sendBuf = js.dump();  //json数据对象 =》 序列化 json字符串
    //cout << js<<endl;
    //cout<< sendBuf.c_str()<<endl;
    return sendBuf;
    
}
//序列化示例2
string func2(){
    //添加数组
    json js;
    js["id"] = {1,2,3,4,5};
    //添加key-value
    js["name"] = "zhang san";
    //添加对象
    //js["msg"]["zhang san"] = "hello world";
    //js["msg"]["li si"] = "hello china";
    //统一添加
    js["msg"] = {{"liu wu","hi world"},{"liu liu","hello china"},{"duan","hahaha"}};
    //cout << js << endl;
    return js.dump();
}

//json示例3：序列化容器
string func3(){
    json js;
    //序列化vector
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);

    js["list"] = vec;
    //序列化map
    map<int,string> m;
    m.insert({1,"华山"});
    m.insert({2,"黄山"});
    m.insert({3,"泰山"});

    js["path"] = m;
    
    //cout<<js<<endl;
    return js.dump();

}


int main(){
    string recvBuf = func3();
    //数据反序列化 json字符串 =》 反序列化 数据对象
    json jsbuf = json::parse(recvBuf);
    // cout<<jsbuf["msg_type"]<<endl;
    // cout<<jsbuf["from"]<<endl;
    // cout<<jsbuf["to"]<<endl;
    // cout<<jsbuf["msg"]<<endl;

    // cout<<jsbuf["id"]<<endl;
    // auto arr = jsbuf["id"];
    // cout<<arr[2]<<endl;
    // auto msgjs = jsbuf["msg"];
    // cout << msgjs["zhang san"] <<endl;
    // cout << msgjs["duan"] <<endl;

    vector<int> vec = jsbuf["list"]; //js对象里数组理性，直接放入vector容器
    for(int &v : vec){
        cout<<v<<" ";
    }
    cout<<endl;
    map<int,string> mymap = jsbuf["path"];
    for(auto &p : mymap){
        cout<<p.first<<" " <<p.second<<endl;
    }
    cout<<endl;
    return 0;
}

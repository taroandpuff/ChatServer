#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std; 

void fun() {  
    json js;
    js["msg_type"] = {1, 2, 3, 4}; 
    js["from"] = "zhang san";
    js["to"] = "li si";
    js["msg"] = "hello, what are you doing now?";
    string sendBuf = js.dump();
    cout << sendBuf.c_str();
}

int main() {
    fun();
    return 0;
}
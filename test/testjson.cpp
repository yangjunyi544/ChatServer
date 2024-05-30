#include "json.hpp"
using json = nlohmann::json;
#include <iostream>
using namespace std;
 
int main(){
    json js;
 // 添加数组
    js["id"] = {1,2,3,4,5}; 
// 添加key-value
    js["name"] = "zhang san"; 
// 上面等同于下面这句一次性添加数组对象
    js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};
    cout << js << endl;
    return 0;
}


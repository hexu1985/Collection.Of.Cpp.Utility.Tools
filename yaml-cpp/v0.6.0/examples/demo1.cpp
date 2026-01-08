#include <fstream>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <assert.h>

int main()
{
    YAML::Node node;  
    assert(node.IsNull());  //初始化的节点是Null类型
    node["key"] = "value";  //当你给它赋值键值对，它转变为Map类型
                            //node.force_insert("key", "value");//这个操作和上面等价，但是它不会检查是否存在"key"键，不推荐使用
    if(node["mascot"])
        std::cout << node["mascot"].as<std::string>() << "\n";//单纯的查询操作不会增加一个key，当然上面的if不会执行

    node["number"] = 255;
    assert(node.IsMap());   //node是一个Map
    node["seq"].push_back("first element");
    node["seq"].push_back("second element");//node的seq下是Sequence类型，有两个参数

    YAML::Node node_2;  
    node_2.push_back("first item");//如果你不给node_2键值对，它是一个sequence类型
    node_2.push_back("second_item");
    node_2.push_back("third_item");
    std::vector<int> v = {1,3,5,7,9};//给node_2插入了一个Sequence
    node_2.push_back(v);
    assert(node_2.IsSequence());//当然，node_2仍然是一个Sequence

    assert(node_2[0].as<std::string>() == "first item");
    //对于Sequence类型，你可以使用它的下标来访问
    //注意这里as<T>是一个模板转换，node_2[0]的type是NodeType::Scalar
    auto it = node_2.begin();
    for(; it != node_2.end(); it++)
        std::cout << *(it) << std::endl;
    //当然，你也可以用迭代器来访问
    //他们的类型分别是NodeType::Scalar，NodeType::Scalar，NodeType::Scalar，NodeType::Sequence
    //取值时记得使用as进行模板转换
    node_2["key"] = "value";
    assert(node_2.IsMap());//一旦node_2接收到键值对，它转变为Map类型
    assert(node_2[0].as<std::string>() == "first item");//此时，Sequence时的下标变为它的key值
    node["node_2"] = node_2;//将node_2作为node的一个子项
    node["pointer_to_first_element"] = node["seq"][0];//你也可以给已有的node设置一个别名，类似于一个指针
    assert(node["pointer_to_first_element"].as<std::string>() == "first element");//你可以通过这个指针访问那个node

    node.remove(node["seq"][0]);//你可以通过指定一个node来删除它
    node.remove("pointer_to_first_element");//你也可以通过指定key来删除它

    std::cout << "\n==========================================\n";
    std::cout << node << std::endl;
}


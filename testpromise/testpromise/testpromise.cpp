// testpromise.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <uvpp/uvpp.hpp>
#include <uvpp/resolver.hpp>
#include "promise.hpp"
#include <memory>


using namespace uvpp;
using namespace promise;


Defer async_connect(std::shared_ptr<uvpp::Tcp> _tcp, std::string& addr, uint16_t port) {
    return newPromise([&](Defer d) {
        _tcp->connect(addr, port, [=](error err) {
            d->resolve(err);
            });
        });
}


Defer async_resolve(std::string& hostname, std::shared_ptr<uvpp::Resolver> resolver) {
    return newPromise([=](Defer d) {
        resolver->resolve(hostname, [=](const error& err, bool s, const std::string& ip){
            d->resolve(ip);
            });
        });
}

int main()
{
    uvpp::loop  _loop;
    std::shared_ptr<uvpp::Tcp> _tcp = std::make_shared<uvpp::Tcp>(_loop);
    std::shared_ptr<uvpp::Resolver> _resolver = std::make_shared<uvpp::Resolver>(_loop);

    uvpp::Timer timer(_loop);
    timer.start([]() {
        
        }, std::chrono::duration<uint64_t, std::milli>(1000), std::chrono::duration<uint64_t, std::milli>(1000));

    std::string hostname = "www.baidu.com";
    async_resolve(hostname, _resolver)
    .then([=](std::string& addr) {
        return async_connect(_tcp, addr, 80);
     })
    .then([=](error err) {
         printf("%d", err);
        });



    
  
    _loop.run();
    _loop.stop();
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

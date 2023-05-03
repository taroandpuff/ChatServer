#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

class ChatServer {
public:
    ChatServer(EventLoop* loop, // 事件循环 epoll
            const InetAddress& listenAddr, // ip+port
            const string& nameArg)  // 服务器名字
            : _server(loop, listenAddr, nameArg), _loop(loop) {
                // 给服务器注册用户注册和创建断开的回调
                _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
                // 给服务器注册用户读写事件回调
                _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
                // 设置服务器线程数量
                _server.setThreadNum(2);

            }
    // 开启事件循环
    void start() {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开   epoll listenfd accept
    void onConnection(const TcpConnectionPtr& conn) {
        if (conn->connected()) {
            cout << conn->peerAddress().toIpPort() << " -> " <<
                conn->localAddress().toIpPort() << " state:online " << endl;
        } else {
            cout << conn->peerAddress().toIpPort() << " -> " <<
                conn->localAddress().toIpPort() << " state:offline " << endl;
                conn->shutdown(); // close
        }
    }

    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr& conn, // 连接
                            Buffer* buffer, // 缓冲区
                            Timestamp time) { // 接受到的时间信息
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << " time: " << time.toString() << endl;
        conn->send(buf);
    }
    TcpServer _server; // #1
    EventLoop* _loop; // #2 epoll 

};

int main() {
    EventLoop loop; // epoll
    InetAddress addr("0.0.0.0", 6000);
    ChatServer server(&loop, addr, "ChatServer");
    server.start(); // 启动服务, listenfd epoll_ctl->epoll
    loop.loop(); // epoll_wait以阻塞方式等待新用户连接,已连接用户的读写事件等
    return 0;
}
/*
muduo网络库给用户提供了两个主要类
TcpServer : 用于编写服务器程序
TcpClient : 用来编写客户端程序

epoll + 线程池
好处：能把网络I/O的代码和业务代码区分开
                用户的连接和断开  用户的可读写事件
*/

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

//基于muduo网络库开发服务器程序
//1. 组合TcpServer对象
//2. 创建EventLoop事件循环对象的指针
//3. 明确Tcpserver构造函数需要什么参数，输出chatserver的构造函数
//4. 在当前服务器类的构造函数中，注册处理连接的回调函数和处理读写事件的回调函数
//5. 设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
class Chat_Server
{
public:
    ChatServer(EventLoop* loop,   //事件循环
            const InetAddress& listenAddr,    //IP+Port
            const string& nameArg)         //服务器的名字
            :_server(loop,listenAddr,nameArg),_loop(loop)
            {
                //给服务器注册用户连接的创建和断开回调
                _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));

                //给服务器注册用户读写事件回调
                _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));

                //设置服务器端的线程数量  1个I/O线程 3个worker线程
                _server.setThreadNum(4);
            }

            //开启事件循环
            void start(){
                _server.start();
            }
private:
    //专门处理用户的连接创建和断开  epoll  listenfd  accept
    void onConnection(const TcpConnectionPtr &conn){
        
        if(conn->connected()){
            cout<< conn->peerAddress().toIpPort() << " -> " <<
            conn->localAddress().toIpPort() << " state : online " << endl;
        }else{
            cout<< conn->peerAddress().toIpPort() << " -> " <<
            conn->localAddress().toIpPort() << " state : offline " << endl;
            conn->shutdown();  //close(fd)
            //_loop->quit();   //整个服务器结束
        }
    }
    //专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn,  //连接
                            Buffer *buffer,     //缓冲区
                            Timestamp time)   //接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() <<endl;
        conn->send(buf);
    }
    TcpServer _server;  //#1 成员变量定义一个对象
    EventLoop *_loop;    //#2 相当于epoll
};

int main()
{
    EventLoop loop;  //epoll
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop,addr,"ChatServer");

    server.start();  //listenfd epoll_ctl=>epoll
    loop.loop();  //调用了epoll_wait 以阻塞方式等待新用户连接，已连接用户的读写事件等

    return 0;
}
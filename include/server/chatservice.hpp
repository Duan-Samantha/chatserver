#pragma once

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "json.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"
using json = nlohmann::json;
using namespace std;
using namespace muduo;
using namespace muduo::net;

//消息ID对应的回调
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

//单例模式设计服务器业务类
/*单例模式使用场景：1.需要频繁进行创建和销毁对象；
2.创建对象耗费时间或资源过多；
3.经常用到的对象、工具类对象、频繁访问数据库或文件的对象*/
class ChatService
{
public:
    //获取单例对象的接口函数
    static ChatService *instance();
    //处理登陆业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    //服务器异常，业务重置方法
    void reset();
    //添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //加入群组
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //群聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);

    void handleRedisSubscribeMessage(int, string);

private:
    ChatService();

    //存储id和其对应业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;

    //存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    //定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;

    //数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    // redis 操作对象
    Redis _redis;
};
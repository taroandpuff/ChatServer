### 基于Muduo的聊天服务器
项目描述：此项目基于 Muduo 开源网络库实现了聊天服务器，支持用户添加好友、一对一聊天群组聊天、
离线消息处理等功能，单机情况下，支持 2万+ 的并发，在此基础上利用 Nginx 负载均衡实现服务器集群，
集群三台服务器时，可支持 7万+ 的并发，同时利用 Redis 的发布订阅功能实现了跨服务器通信
主要工作：
1. 使用 Muduo 网络库作为项目的核心网络模块，提高并发网络I/O服务
2. 使用 Json 序列化和反序列化消息作为私有通信协议
3. 使用 MySQL 关系数据库作为项目数据的存储结构
4. 配置 Nginx 基于 TCP 的负载均衡，实现聊天服务器集群化，提高并发量
5. 使用 Redis 的发布订阅功能实现跨服务器的消息通信

### TODO
- 将网络库换成自己手写的
- 将MySQL连接改写成连接池

### 通信格式
服务器和客户端的通信采用了JSON来完成数据在网络中的标准传输。 对于不同的数据则是采用了不同的格式，具体如下：
```
1.登录
json["msgid"] = LOGIN_MSG;
json["id"]			//用户id
json["pwd"]	//密码

2.登录反馈
json["msgid"] = LOGIN_MSG_ACK;
json["id"]			//登录用户id
json["name"]		//登录用户密码
json["offlinemsg"]	//离线消息
json["friends"]		//好友信息,里面有id、name、state三个字段
json["groups"]		//群组信息,里面有id，groupname，groupdesc，users三个字段
					//users里面则有id，name，state，role四个字段
json["errno"]		//错误字段，错误时被设置成1，用户不在线设置成2
json["errmsg"]		//错误信息

3.注册
json["msgid"] = REG_MSG;
json["name"]		//用户姓名
json["pwd"]	//密码

4.注册反馈
json["msgid"] = REG_MSG_ACK;
json["id"]			//给用户返回他的id号
json["errno"]		//错误信息，失败会被设置为1

5.加好友
json["msgid"] = ADD_FRIEND_MSG;
json["id"]			//当前用户id
json["friendid"]	//要加的好友的id

6.一对一聊天
json["msgid"] = ONE_CHAT_MSG;
json["id"]			//发送者id
json["name"]		//发送者姓名
json["to"]			//接受者id
json["msg"]			//消息内容
json["time"]		//发送时间

7.创建群
json["msgid"] = CREATE_GROUP_MSG;
json["id"]			//群创建者id
json["groupname"]	//群名
json["groupdesc"]	//群描述 creator 创建者 normal 群员

8.加入群
json["msgid"] = ADD_GROUP_MSG;
json["id"]			//用户id
json["groupid"]		//群id

9.群聊
json["msgid"] = GROUP_CHAT_MSG;
json["id"]			//发送者id
json["name"]		//发送者姓名
json["groupid"]		//群id
json["msg"]			//消息内容
json["time"]		//发送时间

10.注销
json["msgid"] = LOGINOUT_MSG;
json["id"]			//要注销的id
```

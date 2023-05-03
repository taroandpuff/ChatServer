#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"
#include <string>
#include <vector>
using namespace std;

// 维护族群信息的接口操作方法
class GroupModel {
public:
    // 创建族群
    bool createGroup(Group &group);
    // 加入组群
    void addGroup(int userid, int groupid, string role);
    // 查询用户所在组群信息 
    vector<Group> queryGroups(int userid);
    // 根据指定的groupid查询组群用户id列表, 除去userid自己, 主要用户群聊业务给群组其它成员群发消息
    vector<int> queryGroupUsers(int userid, int groupid);
private:

};

#endif
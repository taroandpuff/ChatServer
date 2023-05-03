#include "groupmodel.hpp"
#include "db.h"

    // 创建族群
bool GroupModel::createGroup(Group &group) {
   // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into AllGroup(groupname, groupdesc) values('%s', '%s')", 
        group.getName().c_str(), group.getDesc().c_str());
    
    MySQL mysql;
    if (mysql.connect()) {
        if (mysql.update(sql)) {
            group.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }   
    return false;
}
    // 加入组群
void GroupModel::addGroup(int userid, int groupid, string role) {
   // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into GroupUser values('%d', '%d', '%s')", 
        groupid, userid, role.c_str());
    
    MySQL mysql;
    if (mysql.connect()) {
        mysql.update(sql);
    }   
}
    // 查询用户所在组群信息 
vector<Group> GroupModel::queryGroups(int userid) {
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.groupname, a.groupdesc from AllGroup a join GroupUser b on a.id = b.groupid where b.userid = %d", userid);
    
    vector<Group> groupvec;
    MySQL mysql;
    if (mysql.connect()) {
        MYSQL_RES* res = mysql.query(sql);
        if (res != nullptr) {
            // 查出userid所有的群组信息
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr) {
                    Group group;
                    group.setId(atoi(row[0]));
                    group.setName(row[1]);
                    group.setDesc(row[2]);
                    groupvec.emplace_back(group);
            }
            mysql_free_result(res);
        }
    }    
    // 查询群组的用户信息
    for (auto& group : groupvec) {
        sprintf(sql, "select a.id, a.name, a.state, b.grouprole from User a\
         join GroupUser b on b.userid = a.id where b.groupid = %d", group.getId());

         MYSQL_RES* res = mysql.query(sql);
         if (res != nullptr) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                group.getUsers().push_back(user);
            }
            mysql_free_result(res);
         }
    }
    return groupvec;
}
    // 根据指定的groupid查询组群用户id列表, 除去userid自己, 主要用户群聊业务给群组其它成员群发消息
vector<int> GroupModel::queryGroupUsers(int userid, int groupid) {
   // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select userid from GroupUser where groupid = %d and userid != %d", 
        groupid, userid);
    
    vector<int> idVec;
    MySQL mysql;
    if (mysql.connect()) {
        MYSQL_RES* res = mysql.query(sql);
        if (res != nullptr) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                idVec.emplace_back(atoi(row[0]));
            }
            mysql_free_result(res);
        }
    }      
    return idVec;
}
//
// Created by amir on 19/11/2019.
//

#include "Session.h"


std::vector<BaseAction*>& Session::getActionsLog() { return actionsLog&}
User& Session::getActiveUser() { return  activeUser&}
std::vector<Watchable*>& Session::getContent() { return  content&}
const std::unordered_map<std::string,User*>& Session::getUserMap() { return userMap&}


void setActionsLog(std::vector<BaseAction*> newActionLog);
void setUserMap(std::unordered_map<std::string,User*> newUserMap);
void setActiveUser(User*){

}
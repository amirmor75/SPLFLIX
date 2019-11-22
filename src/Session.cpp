
// Created by tal on 19/11/2019.
//

#include "Session.h"
#include <../include/json.hpp>
#include <fstream>
#include <Watchable.h>
#include <User.h>

//Session Constructor
Session::Session(const std::string &configFilePath) {
    //activeUser=new User();
    using json= nlohmann::json;
    std::ifstream file(configFilePath);
    json j;
    file >> j;

    json movies;
    movies=j["movies"];
    for (auto& element : movies){
        //content.push_back(new Movie(content.size(),element["name"],element["length"],element["tags"]));
    }
    json series;
    series=j["tv_series"];
    int seasonIndex;
    for(auto& element : series) {
        seasonIndex = 1;
        std::cout << element["seasons"] << '\n';
        for (auto &season: element["seasons"]) {
            for (int i = 1; i <= season; i++) {
                //content.push_back(new Episode(content.size(), element["name"], element["episode_length"],i,seasonIndex, element["tags"]));
//                std::cout<<content.size()<<"  ";
//                std::cout<<element["name"]<<"  ";
//                std::cout<<element["episode_length"]<<"  ";
//                std::cout<<i<<"  ";
//                std::cout<<seasonIndex<<"  ";
//                std::cout<<element["tags"]<< '\n';
            }
            seasonIndex++;
        }
    }
}

//Session copy constructor
Session::Session(const Session &other){
    activeUser=other.activeUser->clone(); //need to implement clone func
    for(int i=0;i<other.content.size();i++){
        content.push_back(other.content.at(i));
    }
    for(int i=0;i<other.actionsLog.size();i++){
        actionsLog.push_back(other.actionsLog.at(i));
    }
    for (auto& x: other.userMap) {
        userMap.insert(x);
    }
}

Session::Session(Session&& other):
activeUser(other.activeUser),content(other.content), actionsLog(other.actionsLog), userMap(other.userMap) {
    other.activeUser= nullptr;
    for(int i=0;i<other.content.size();i++){
        other.content.at(i)= nullptr;
    }
    for(int i=0;i<other.actionsLog.size();i++){
        other.actionsLog.at(i)= nullptr;
    }
    for (auto& x: other.userMap) {
        x.second= nullptr;
    }
}

Session& Session::operator=(Session& other) {
    if(this != &other){
        delete activeUser;
        activeUser=other.activeUser->clone();
        for(int i=0;i<content.size();i++){
            delete content.at(i);
        }
        content.clear();
        for(int i=0;i<other.content.size();i++){
            content.push_back(other.content.at(i)->clone());
        }

        for(int i=0;i<actionsLog.size();i++){
            delete actionsLog.at(i);
        }
        actionsLog.clear();
        for(int i=0;i<other.actionsLog.size();i++){
            actionsLog.push_back(other.actionsLog.at(i)->clone());
        }

        for (auto& x: userMap) {
            delete x.second;
        }
        userMap.clear();
        for (auto& x: other.userMap) {
            userMap.insert(x);
        }
    }
    return (*this);
}

Session& Session::operator=(Session &&other) {
    if(this!=&other){
        delete activeUser;
        activeUser=other.activeUser;
        delete other.activeUser;

        for(int i=0;i<content.size();i++){
            delete content.at(i);
        }
        content=other.content; //shallow copy
        for(int i=0;i<other.content.size();i++){
            delete other.content.at(i);
        }

        for(int i=0;i<actionsLog.size();i++){
            delete actionsLog.at(i);
        }
        actionsLog=other.actionsLog; //shallow copy
        for(int i=0;i<other.actionsLog.size();i++){
            delete other.actionsLog.at(i);
        }

        for (auto& x: userMap) {
            delete x.second;
        }
        userMap=other.userMap; //shallow copy
        for (auto& x: other.userMap) {
            delete x.second;
        }

    }
    return (*this);
}

Session::~Session() {
    for(int i=0;i<content.size();i++){
        delete content[i];
    }
    for(int i=0;i<actionsLog.size();i++){
        delete actionsLog[i];
    }
    for (auto& x: userMap) {
        delete x.second;
    }
    delete activeUser;
}

const std::vector<BaseAction*>& Session::getActionsLog() { return actionsLog; }
const User& Session::getActiveUser() { return  *activeUser; }
const std::vector<Watchable*>& Session::getContent() { return  content; }
const std::unordered_map<std::string,User*>& Session::getUserMap() { return userMap; }

void Session::setActionsLog(std::vector<BaseAction *>& newActionLog) {
    for(auto& base: actionsLog){
        delete base;
    }
    actionsLog.clear();
    actionsLog=newActionLog;
}
void Session::setActiveUser(User* newUser) {
    if(newUser!=nullptr) {
        delete activeUser;
        activeUser = newUser;
    }
}
void Session::setUserMap(std::unordered_map<std::string, User *>& newUserMap) {
    for(auto& x: userMap){
        delete x.second;
    }
    userMap.clear();
    userMap=newUserMap;
}



// Created by tal on 19/11/2019.
//

#include "Session.h"
#include <../include/json.hpp>
#include <fstream>
#include <Watchable.h>
#include <User.h>

//Session Constructor
Session::Session(const std::string &configFilePath):indexOfContent(0) {
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
            }
            seasonIndex++;
        }
    }
}

//Session copy constructor
Session::Session(const Session &other){
    indexOfContent=other.indexOfContent;
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
activeUser(other.activeUser),content(other.content), actionsLog(other.actionsLog), userMap(other.userMap), indexOfContent(other.indexOfContent) {
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
        indexOfContent=other.indexOfContent;
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
        indexOfContent=other.indexOfContent;
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

const std::vector<BaseAction*>& Session::getActionsLog() { return actionsLog;}
const User& Session::getActiveUser() { return  *activeUser;}
const std::vector<Watchable*>& Session::getContent() { return  content;}
User* Session::getUserFromMap(std::string name) {
    return  userMap.at(name);

}

void Session::addToActionsLog(BaseAction* newAction) {
    this->actionsLog.push_back(newAction);
}
void Session::setActiveUser(User* user) {
    this->activeUser=user;
}
void Session::addToUserMap(std::string name, User* newUserMap) {
    this->userMap.insert({name,newUserMap});
}
std::string& Session::getCurrentCommand() { return currentCommand;}
void Session::setCurrentCommand(std::string& currentCommand) {this->currentCommand=currentCommand;}

bool Session::deleteFromUserMap(std::string name) {
    if(userMap.find(name)==userMap.end())
        return false;
    else{
        userMap.erase(name);
        return true;
    }
}

void Session::start() {} //should be implemnted sometime



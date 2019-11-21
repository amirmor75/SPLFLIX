
// Created by tal on 19/11/2019.
//

#include "Session.h"
#include <../include/json.hpp>
#include <fstream>
#include <Watchable.h>

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
        content.push_back(new Movie(content.size(),element["name"],element["length"],element["tags"]));
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
Session::Session(const Session &session) {

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



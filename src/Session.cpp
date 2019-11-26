
// Created by tal on 19/11/2019.
//

#include "Session.h"
#include <../include/json.hpp>
#include <fstream>
#include <Watchable.h>
#include <User.h>
#include <iostream>

//Session Constructor
Session::Session(const std::string &configFilePath):indexOfContent(0),currentCommand(""), isRunning(false) {
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
        for (int season: element["seasons"]) {
            for (int i = 1; i <= season; i++) {
                content.push_back(new Episode(content.size(), element["name"], element["episode_length"],seasonIndex,i, element["tags"]));
            }
            seasonIndex++;
        }
    }
}


//5 Rule S
//Session copy constructor
Session::Session(const Session &other): isRunning(other.isRunning), currentCommand(other.currentCommand), indexOfContent(other.indexOfContent){
    activeUser=other.activeUser->clone();
    for(int i=0;i<other.content.size();i++){
        content.push_back(other.content.at(i)->clone());
    }
    for(int i=0;i<other.actionsLog.size();i++){
        actionsLog.push_back(other.actionsLog.at(i)->clone());
    }
    for (auto& x: other.userMap) {
        userMap.insert({x.first,x.second->clone()});
    }
}

Session::Session(Session&& other):
activeUser(other.activeUser),content(other.content), actionsLog(other.actionsLog), userMap(other.userMap), indexOfContent(other.indexOfContent), currentCommand(other.currentCommand), isRunning(other.isRunning) {
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
        currentCommand=other.currentCommand;
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
            userMap.insert({x.first,x.second->clone()});
        }
    }
    return (*this);
}

Session& Session::operator=(Session &&other) {
    if(this!=&other){
        indexOfContent=other.indexOfContent;
        currentCommand=other.currentCommand;
        indexOfContent=other.indexOfContent;
        delete activeUser;
        activeUser=other.activeUser;

        for(int i=0;i<content.size();i++){
            delete content.at(i);
        }
        content=other.content; //shallow copy

        for(int i=0;i<actionsLog.size();i++){
            delete actionsLog.at(i);
        }
        actionsLog=other.actionsLog; //shallow copy

        for (auto& x: userMap) {
            delete x.second;
        }
        userMap=other.userMap; //shallow copy
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
//5 Rule F
const std::vector<BaseAction*>& Session::getActionsLog() { return actionsLog;}
User& Session::getActiveUser() const{ return  *activeUser;}
const std::vector<Watchable*>& Session::getContent() { return  content;}
User* Session::getUserFromMap(std::string name) { return  userMap.at(name); }
Watchable* Session::getContentByID(long id) const{
    for(auto& watch: content){
        if(watch->getId()==id)
            return watch;
    }
    return nullptr;
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
const int Session::getIndexOfContent() { return indexOfContent; }
bool Session::getIsRun() const { return isRunning; }
bool Session::setIsRun(bool run) { isRunning=run; }

bool Session::deleteFromUserMap(std::string name) {
    if(userMap.find(name)==userMap.end())
        return false;
    else{
        userMap.erase(name);
        return true;
    }
}

void Session::split(std::string &str, std::vector<std::string> &out) {
    size_t start;
    size_t end = 0;
    out.clear();
    while ((start = str.find_first_not_of(' ', end)) != std::string::npos)
    {
        end = str.find(' ', start);
        out.push_back(str.substr(start, end - start));
    }
}

void Session::start() {
    std::vector<std::string> command;
    int firstSpace;
    BaseAction* baseAction;
    std::cout<<"SPLFLIX is now on!"<<'\n';
    activeUser=new LengthRecommenderUser("default");
    //addToUserMap("default",activeUser);
    setIsRun(true);
    while(getIsRun()){
        std::getline(std::cin, currentCommand);
        if(command.size()>0)
            split(currentCommand,command);

        if(command.at(0).compare("createuser")==0) {
            firstSpace = currentCommand.find(" ");
            if (firstSpace < currentCommand.size()) {
                currentCommand = currentCommand.substr(firstSpace + 1);
                baseAction = new CreateUser();
                baseAction->act(*this);
                addToActionsLog(baseAction);
            }
        }
        else if(command.at(0).compare("changeuser")==0) {
            firstSpace = currentCommand.find(" ");
            if (firstSpace < currentCommand.size()) {
                currentCommand = currentCommand.substr(firstSpace + 1);
                baseAction = new ChangeActiveUser();
                baseAction->act(*this);
                addToActionsLog(baseAction);
            }
        }
        else if(command.at(0).compare("deleteuser")==0) {
            firstSpace = currentCommand.find(" ");
            if (firstSpace < currentCommand.size()) {
                currentCommand = currentCommand.substr(firstSpace + 1);
                baseAction = new DeleteUser();
                baseAction->act(*this);
                addToActionsLog(baseAction);
            }
        }
        else if(command.at(0).compare("dupuser")==0) {
            firstSpace = currentCommand.find(" ");
            if (firstSpace < currentCommand.size()) {
                currentCommand = currentCommand.substr(firstSpace + 1);
                baseAction = new DuplicateUser();
                baseAction->act(*this);
                addToActionsLog(baseAction);
            }
        }
        else if(command.at(0).compare("content")==0) {
            baseAction = new PrintContentList();
            baseAction->act(*this);
            addToActionsLog(baseAction);
        }
        else if(command.at(0).compare("watchhist")==0) {
            baseAction = new PrintWatchHistory();
            baseAction->act(*this);
            addToActionsLog(baseAction);
        }
        else if(command.at(0).compare("watch")==0) {
            firstSpace = currentCommand.find(" ");
            if (firstSpace <= currentCommand.size()) {
                currentCommand = currentCommand.substr(firstSpace + 1);
                baseAction = new Watch();
                baseAction->act(*this);
                addToActionsLog(baseAction);
            }
        }
        else if(command.at(0).compare("log")==0) {
            baseAction = new PrintActionsLog();
            baseAction->act(*this);
            addToActionsLog(baseAction);
        }
        else if(command.at(0).compare("exit")==0) {
            baseAction = new Exit();
            baseAction->act(*this);
            addToActionsLog(baseAction);
        }
    }

}



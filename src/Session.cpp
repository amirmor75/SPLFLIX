
// Created by tal on 19/11/2019.


#include "../include/Session.h"
#include "../include/json.hpp"
#include <fstream>
#include "../include/Watchable.h"
#include <iostream>

//Session Constructor
Session::Session(const std::string &configFilePath): content(), actionsLog(), userMap(), activeUser(),currentCommand(""), indexOfContent(0), isRunning(false) {
    using json= nlohmann::json;
    json j;
    std::ifstream file(configFilePath);
    file >> j;

    json movies=j["movies"];
    for (auto& element : movies){
        content.push_back(new Movie(content.size(),element["name"],element["length"],element["tags"]));
    }
    json series=j["tv_series"];
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
Session::Session(const Session &other): content(), actionsLog(), userMap(), activeUser(), currentCommand(other.currentCommand), indexOfContent(other.indexOfContent), isRunning(other.isRunning) {
    activeUser=other.activeUser->clone();
    for(Watchable* watch: other.content)
        content.push_back(watch->clone());

    for(BaseAction* action: other.actionsLog)
        actionsLog.push_back(action->clone());

    for (auto& x: other.userMap) {
        userMap.insert({x.first,x.second->clone()});
    }
}

Session::Session(Session&& other): content(other.content), actionsLog(other.actionsLog), userMap(other.userMap), activeUser(other.activeUser), currentCommand(other.currentCommand), indexOfContent(other.indexOfContent), isRunning(other.isRunning) {
    other.activeUser= nullptr;
    for(std::vector<Watchable*>::iterator iter=other.content.begin();iter!=other.content.end();iter++)
        *iter=nullptr;
    for(std::vector<BaseAction*>::iterator iter=other.actionsLog.begin();iter!=other.actionsLog.end();iter++)
        *iter=nullptr;
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
        for(Watchable* watch: content)
            delete watch;
        content.clear();
        for(Watchable* watch:other.content)
            content.push_back(watch->clone());

        for(BaseAction* base:actionsLog)
            delete base;
        actionsLog.clear();
        for(BaseAction* base:other.actionsLog)
            actionsLog.push_back(base->clone());

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
        indexOfContent=other.indexOfContent;
        currentCommand=other.currentCommand;
        indexOfContent=other.indexOfContent;
        delete activeUser;
        activeUser=other.activeUser;

        for(Watchable* watch: content)
            delete watch;
        content=other.content; //shallow copy

        for(BaseAction* base:actionsLog)
            delete base;
        actionsLog=other.actionsLog; //shallow copy

        for (auto& x: userMap) {
            delete x.second;
        }
        userMap=other.userMap; //shallow copy
    return (*this);
}

Session::~Session() {
    for(Watchable* watch: content)
        delete watch;
    for(BaseAction* base: actionsLog)
        delete base;
    for (auto& x: userMap) {
        delete x.second;
    }
}
//5 Rule F
const std::vector<BaseAction*>& Session::getActionsLog() { return actionsLog;}
User& Session::getActiveUser() const{ return  *activeUser;}
const std::vector<Watchable*>& Session::getContent() { return  content;}
User* Session::getUserFromMap(std::string name) {
    if(isUserExists(name))
        return  userMap.at(name);
    return nullptr;
}
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
void Session::setIsRun(bool run) { isRunning=run; }
void Session::deleteUser() { delete activeUser; }
bool Session::deleteFromUserMap(std::string name) {
    User* user=getUserFromMap(name);
    if(user== nullptr)
        return false;
    else{
        delete user;
        userMap.erase(name);
        return true;
    }
}
bool Session::isUserExists(std::string &name) {
    if(userMap.find(name)==userMap.end())
        return false;
    return true;
}
void Session::split(std::string &str, std::vector<std::string> &out) {
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(' ', end)) != std::string::npos)
    {
        end = str.find(' ', start);
        out.push_back(str.substr(start, end - start));
    }
}

void Session::start() {
    std::vector<std::string> command;
    size_t firstSpace;
    BaseAction* baseAction;
    std::cout<<"SPLFLIX is now on!"<<'\n';
    addToUserMap("default",new LengthRecommenderUser("default"));
    setActiveUser(getUserFromMap("default"));
    setIsRun(true);
    while(getIsRun()){
        std::getline(std::cin, currentCommand);
        if(currentCommand.size()>0) {
            command.clear();
            split(currentCommand, command);
        }

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
        else{
            std::cout<<"Unrecognized command- try again:"<<'\n';
        }
    }
}



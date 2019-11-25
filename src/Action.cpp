//
// Created by amir on 19/11/2019.
//

#include <Action.h>
#include "Session.h"
#include "User.h"


//+++ BaseAction +++
BaseAction::BaseAction(): errorMsg(""),status(PENDING){}
BaseAction::BaseAction(std::string errorMsg, ActionStatus status):errorMsg(errorMsg), status(status) {}
std::string BaseAction::getErrorMsgPublic() const { return errorMsg;}
ActionStatus BaseAction::getStatus() const { return status;}
void BaseAction::complete() {
    this->errorMsg="";
    status=COMPLETED;
}
void BaseAction::error(const std::string &errorMsg) {
    status=ERROR;
    this->errorMsg="ERROR <"+errorMsg+">";
}
std::string BaseAction::getErrorMsg() const { return errorMsg;}
std::vector<std::string>* BaseAction::split(std::string command) {
    std::vector<std::string>* words=new std::vector<std::string>();
    int index=0;
    std::string word="";
    for(auto& letter: command){
        if(letter!=' ')
        {
            if(words->size()<index) {
                words->push_back(word);
                word="";
            }
            word=word +letter;
        }
        else{
            index++;
        }
    }
    words->push_back(word);
    return words;
}


//+++ CreateUser +++
void CreateUser::act(Session &sess) {
    if(split(sess.getCurrentCommand())->size()==2) {
        std::string& command=sess.getCurrentCommand();
        std::string& name =  split(command)->at(0);
        std::string& algorithm=split(command)->at(1);
        User *user;
        if(sess.getUserFromMap(name)== nullptr) {
            if (command.compare("gen") == 0) {
                user = new GenreRecommenderUser(name);
                sess.addToUserMap(name, user);
                complete();
            } else if (command.compare("len") == 0) {
                user = new LengthRecommenderUser(name);
                sess.addToUserMap(name, user);
                complete();
            } else if (command.compare("rer") == 0) {
                user = new RerunRecommenderUser(name);
                sess.addToUserMap(name, user);
                complete();
            }
            else{
                error("invalid recommendation algorithm");
            }
        } else {
            error("This name is already exists");
        }
    } else
        error("invalid input");
}
std::string CreateUser::toString() const { return "CreateUser"; }
BaseAction* CreateUser::clone() {
    BaseAction* del=new CreateUser(getErrorMsg(),getStatus());
    return del;
}

//+++ ChangeActiveUser +++
void ChangeActiveUser::act(Session &sess) {
    if(split(sess.getCurrentCommand())->size()==1) {
        std::string& command=sess.getCurrentCommand();
        std::string& name =  split(command)->at(0);
        User *user = sess.getUserFromMap(name);
        if(user!=nullptr){
            sess.setActiveUser(user);
            complete();
        }
        else
            error("user "+name+" has never been created");
    }
    else
        error("invalid input");
}
std::string ChangeActiveUser::toString() const { return "ChangeActiveUser"; }
BaseAction* ChangeActiveUser::clone() {
    BaseAction* del=new ChangeActiveUser(getErrorMsg(),getStatus());
    return del;
}

//+++ DeleteUser +++
DeleteUser::DeleteUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void DeleteUser::act(Session &sess) {
    if(split(sess.getCurrentCommand())->size()==1) {
        std::string &name = split(sess.getCurrentCommand())->at(0);
        if (sess.deleteFromUserMap(name)) {
            complete();
        } else {
            error(name + " is not an exist user");
        }
    } else
        error("invalid input");
}
std::string DeleteUser::toString() const { return "Delete User"; }
BaseAction* DeleteUser::clone() {
    BaseAction* del=new DeleteUser(getErrorMsg(),getStatus());
    return del;
}

//+++ DuplicateUser +++
DuplicateUser::DuplicateUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void DuplicateUser::act(Session &sess) {
    std::string command=sess.getCurrentCommand();
    if(split(command)->size()==2) {
        std::string &name = split(command)->at(0);
        std::string &newName = split(command)->at(1);
        User* user=sess.getUserFromMap(name);
        User* newUser=sess.getUserFromMap(newName);
        if(newUser== nullptr) {
            if (user != nullptr) {
                User *user = user->duplicateUser(newName);
                sess.addToUserMap(newName, user);
                complete();
            } else {
                error(name + " is not an exist user");
            }
        } else{
            error(newName + " is an exist user");
        }
    } else
        error("invalid input");
}
std::string DuplicateUser::toString() const { return "DuplicateUser"; }
BaseAction* DuplicateUser::clone() {
    BaseAction* del=new DuplicateUser(getErrorMsg(),getStatus());
    return del;
}


//+++ PrintContentList +++
PrintContentList::PrintContentList(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void PrintContentList::act(Session &sess) {
    int index=1;
    std::string str="";
    for(auto& watch: sess.getContent()) {
        str=index+". ";
        str+=watch->toString()+" ";
        str+=watch->printAll();
        std::cout << str << '\n';
        index++;
    }
    complete();
}
std::string PrintContentList::toString() const { return "PrintContentList"; }
BaseAction* PrintContentList::clone() {
    BaseAction* del=new PrintContentList(getErrorMsg(),getStatus());
    return del;
}


//+++ PrintContentList +++
PrintWatchHistory::PrintWatchHistory(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void PrintWatchHistory::act(Session &sess) {
    int index=1;
    std::string str="";
    for(auto& watch: sess.getActiveUser().get_history()) {
        str=index+". ";
        str+=watch->toString();
        std::cout << str << '\n';
        index++;
    }
    complete();
}
std::string PrintWatchHistory::toString() const { return "PrintWatchHistory"; }
BaseAction* PrintWatchHistory::clone() {
    BaseAction* del=new PrintWatchHistory(getErrorMsg(),getStatus());
    return del;
}


//+++ Watch +++
Watch::Watch(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void Watch::act(Session &sess) {
    if(split(sess.getCurrentCommand())->size()==1) {
        std::string &idStr = split(sess.getCurrentCommand())->at(0);
        Watchable* watch=sess.getContentByID(stoi(idStr));
        if (watch!= nullptr) {
            std::string name="Watching "+watch->toString();
            std::cout<<name<<'\n';
            sess.getActiveUser().get_history().push_back(watch);
            Watchable* nextWatch=sess.getActiveUser().getRecommendation(sess);
            name="We recommend watching "+nextWatch->toString()+", continue watching? [y/n]";
            char answer;
            std::cin >> answer;
            if(answer=='y'){
                name=nextWatch->getId()+"";
                sess.setCurrentCommand(name);
                act(sess);
            }
            complete();
        } else {
            error(idStr + " is out of content bounds");
        }
    } else
        error("invalid input");
}
std::string Watch::toString() const { return "Watch"; }
BaseAction* Watch::clone() {
    BaseAction* del=new Watch(getErrorMsg(),getStatus());
    return del;
}


//+++ PrintActionsLog +++
PrintActionsLog::PrintActionsLog(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void PrintActionsLog::act(Session &sess) {
    std::string str,status,error;
    for(auto& action: sess.getActionsLog()) {
        str=action->toString()+" ";
        status=action->getStatus();
        if(status.compare("ERROR")==0)
            error=" "+action->getErrorMsgPublic();
        str+=status+error;
        std::cout << str << '\n';
    }
    complete();
}
std::string PrintActionsLog::toString() const { return "PrintActionsLog"; }
BaseAction* PrintActionsLog::clone() {
    BaseAction* del=new PrintActionsLog(getErrorMsg(),getStatus());
    return del;
}


//+++ Exit +++
Exit::Exit(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void Exit::act(Session &sess) {
    sess.setIsRun(false);
    complete();
}
std::string Exit::toString() const { return "Exit"; }
BaseAction* Exit::clone() {
    BaseAction* del=new Exit(getErrorMsg(),getStatus());
    return del;
}

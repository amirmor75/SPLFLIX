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
std::vector<std::string>& BaseAction::split(std::string command) {
    std::vector<std::string> words;
    int index=0;
    std::string word="";
    for(auto& letter: command){
        if(letter!=' ')
        {
            if(words.size()<index) {
                words.push_back(word);
                word="";
            }
            word=word +letter;
        }
        else{
            index++;
        }
    }
    words.push_back(word);
    return words;
}


//+++ CreateUser +++
void CreateUser::act(Session &sess) {
    // check if got a valid string if not make this error.
    // check valid command

    std::string& command=sess.getCurrentCommand();// assumption: gets only name and type of algo
    std::string name = command.substr(0,command.find(" "));
    User* user;
    if (command.substr(command.size()-4)=="gen") {
        user = new GenreRecommenderUser(name);
    }
    if (command.substr(command.size()-4)=="len") {
        user = new LengthRecommenderUser(name);
    }
    if (command.substr(command.size()-4)=="rer") {
        user = new RerunRecommenderUser(name);
    }
    sess.addToUserMap(name,user);
}

//+++ ChangeActiveUser +++
void ChangeActiveUser::act(Session &sess) {
    // check valid name and not space in beginning
    std::string& name= sess.getCurrentCommand();
    User* user=sess.getUserFromMap(name);
    sess.setActiveUser(user);
}

//+++ DeleteUser +++
DeleteUser::DeleteUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void DeleteUser::act(Session &sess) {
    if(split(sess.getCurrentCommand()).size()==1) {
        std::string &name = split(sess.getCurrentCommand()).at(0);
        if (sess.deleteFromUserMap(name)) {
            complete();
        } else {
            error(name + " is not an exist user");
        }
    } else
        error("invalid input");
}
std::string DeleteUser::toString() const { return "Delete User. status:"+getStatus(); }
BaseAction* DeleteUser::clone() {
    BaseAction* del=new DeleteUser(getErrorMsg(),getStatus());
    return del;
}

//+++ DuplicateUser +++
DuplicateUser::DuplicateUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void DuplicateUser::act(Session &sess) {
    if(split(sess.getCurrentCommand()).size()==2) {
        std::string &name = split(sess.getCurrentCommand()).at(0);
        std::string &newName = split(sess.getCurrentCommand()).at(1);
        User* u=sess.getUserFromMap(name);
        if (u!= nullptr) {
            User* user=u->duplicateUser(newName);
            sess.addToUserMap(newName,user);
            complete();
        } else {
            error(name + " is not an exist user");
        }
    } else
        error("invalid input");
}
std::string DuplicateUser::toString() const { return "Duplicate User. status:"+getStatus(); }
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
}
std::string PrintContentList::toString() const { return "PrintContentList. status:"+getStatus(); }
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
}
std::string PrintWatchHistory::toString() const { return "PrintContentList. status:"+getStatus(); }
BaseAction* PrintWatchHistory::clone() {
    BaseAction* del=new PrintWatchHistory(getErrorMsg(),getStatus());
    return del;
}
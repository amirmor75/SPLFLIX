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
    std::string& name= sess.getCurrentCommand();
    if(sess.deleteFromUserMap(name)){
        complete();
    }
    else{
        error(name+" is not an exist user");
    }
}
std::string DeleteUser::toString() const { return "Delete User. status:"+getStatus(); }
BaseAction* DeleteUser::clone() {
    BaseAction* del=new DeleteUser(getErrorMsg(),getStatus());
    return del;
}



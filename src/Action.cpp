//
// Created by amir on 19/11/2019.
//

#include <Action.h>
#include "Session.h"
#include "User.h"



BaseAction::BaseAction(): errorMsg(""),status(PENDING){}
std::string BaseAction::getErrorMsgPublic() const { return errorMsg;}
ActionStatus BaseAction::getStatus() const { return status;}
void BaseAction::complete() { this->errorMsg="";status=COMPLETED;}
void BaseAction::error(const std::string &errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
}
std::string BaseAction::getErrorMsg() const { return errorMsg;}


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
void ChangeActiveUser::act(Session &sess) {
    // check valid name and not space in beginning
    std::string& name= sess.getCurrentCommand();
    User* user=sess.getUserFromMap(name);
    sess.setActiveUser(user);
}

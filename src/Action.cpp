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
CreateUser::CreateUser() :BaseAction(){}
CreateUser::CreateUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void CreateUser::act(Session &sess) {
    std::string& command=sess.getCurrentCommand();
    std::vector<std::string> words;
    sess.split(command,words);
    if(words.size()==2) {
        std::string& name =  words.at(0);
        std::string& algorithm=words.at(1);
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
ChangeActiveUser::ChangeActiveUser() :BaseAction(){}
ChangeActiveUser::ChangeActiveUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void ChangeActiveUser::act(Session &sess) {
    std::vector<std::string> words;
    sess.split(sess.getCurrentCommand(),words);
    if(words.size()==1) {
        std::string& command=sess.getCurrentCommand();
        std::string& name =  words.at(0);
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
DeleteUser::DeleteUser() :BaseAction(){}
DeleteUser::DeleteUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void DeleteUser::act(Session &sess) {
    std::vector<std::string> words;
    sess.split(sess.getCurrentCommand(),words);
    if(words.size()==1) {
        std::string &name = words.at(0);
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
DuplicateUser::DuplicateUser() :BaseAction(){}
DuplicateUser::DuplicateUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void DuplicateUser::act(Session &sess) {
    std::string command=sess.getCurrentCommand();
    std::vector<std::string> words;
    sess.split(command,words);
    if(words.size()==2) {
        std::string &name = words.at(0);
        std::string &newName = words.at(1);
        User* user=sess.getUserFromMap(name);
        User* newUser=sess.getUserFromMap(newName);
        if(newUser== nullptr) {
            if (user != nullptr) {
                newUser = user->clone();
                newUser->setName(newName);
                sess.addToUserMap(newName, newUser);
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
PrintContentList::PrintContentList() :BaseAction(){}
PrintContentList::PrintContentList(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void PrintContentList::act(Session &sess) {
    int index=1;
    std::string str="";
    for(auto& watch: sess.getContent()) {
        printf("%d. %s %s \n",index,watch->toString().c_str(),watch->printAll().c_str());
        index++;
    }
    complete();
}
std::string PrintContentList::toString() const { return "PrintContentList"; }
BaseAction* PrintContentList::clone() {
    BaseAction* del=new PrintContentList(getErrorMsg(),getStatus());
    return del;
}


//+++ PrintWatchHistory +++
PrintWatchHistory::PrintWatchHistory() :BaseAction(){}
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
Watch::Watch() :BaseAction(){}
Watch::Watch(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void Watch::act(Session &sess) {
    std::vector<std::string> words;
    sess.split(sess.getCurrentCommand(),words);
    if(words.size()==1) {
        std::string &idStr = words.at(0);
        Watchable* watch=sess.getContentByID(stoi(idStr));
        std::cout<<watch->toString();
        if (watch!= nullptr) {
            sess.getActiveUser().addToHistory(watch);
            std::string name="Watching "+watch->toString();
            std::cout<<name<<'\n';
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
PrintActionsLog::PrintActionsLog() :BaseAction(){}
PrintActionsLog::PrintActionsLog(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void PrintActionsLog::act(Session &sess) {
    std::string str,status,error;
    for(auto& action: sess.getActionsLog()) {
        str=action->toString()+" ";
        switch (action->getStatus()){
            case ERROR:
                status="ERROR";
            case PENDING:
                status="PENDING";
            case COMPLETED:
                status="COMPLETED";
        }
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
Exit::Exit() :BaseAction(){}
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

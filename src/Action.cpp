//
// Created by amir on 19/11/2019.
//

#include "../include/Action.h"
#include <algorithm>
#include "../include/Session.h"
#include "../include/User.h"


//+++ BaseAction +++
BaseAction::BaseAction(): errorMsg(""),status(PENDING){}
BaseAction::BaseAction(std::string errorMsg, ActionStatus status):errorMsg(errorMsg), status(status) {}
std::string BaseAction::getErrorMsgPublic() const { return errorMsg;}
ActionStatus BaseAction::getStatus() const { return status;}
void BaseAction::complete() {
    status=COMPLETED;
    this->errorMsg="";
}
void BaseAction::error(const std::string &errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
}
std::string BaseAction::getErrorMsg() const { return errorMsg;}
bool BaseAction::isNumber(const std::string &s){
    return !s.empty() && std::find_if(s.begin(),
               s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}


//+++ CreateUser +++
CreateUser::~CreateUser() { }
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
        if(!sess.isUserExists(name)) { //this user doest exist
            if (algorithm.compare("gen") == 0) {
                user = new GenreRecommenderUser(name);
                sess.addToUserMap(name, user);
                complete();
            } else if (algorithm.compare("len") == 0) {
                user = new LengthRecommenderUser(name);
                sess.addToUserMap(name, user);
                complete();
            } else if (algorithm.compare("rer") == 0) {
                user = new RerunRecommenderUser(name);
                sess.addToUserMap(name, user);
                complete();
            }
            else{
                error("invalid recommendation algorithm");
                std::cout<<getErrorMsg()<<'\n';
            }
        } else {
            error("This name is already exists");
            std::cout<<getErrorMsg()<<'\n';
        }
    } else {
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string CreateUser::toString() const { return "CreateUser"; }
BaseAction* CreateUser::clone() {
    BaseAction* del=new CreateUser(getErrorMsg(),getStatus());
    return del;
}

//+++ ChangeActiveUser +++
ChangeActiveUser::~ChangeActiveUser() { }
ChangeActiveUser::ChangeActiveUser() :BaseAction(){}
ChangeActiveUser::ChangeActiveUser(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void ChangeActiveUser::act(Session &sess) {
    std::vector<std::string> words;
    sess.split(sess.getCurrentCommand(),words);
    if(words.size()==1) {
        std::string& name =  words.at(0);
        User *user = sess.getUserFromMap(name);
        if(user!=nullptr){
            sess.setActiveUser(user);
            complete();
        }
        else {
            error("user " + name + " is not exists");
            std::cout<<getErrorMsg()<<'\n';
        }
    }
    else {
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string ChangeActiveUser::toString() const { return "ChangeActiveUser"; }
BaseAction* ChangeActiveUser::clone() {
    BaseAction* del=new ChangeActiveUser(getErrorMsg(),getStatus());
    return del;
}

//+++ DeleteUser +++
DeleteUser::~DeleteUser() { }
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
            std::cout<<getErrorMsg()<<'\n';
        }
    } else {
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string DeleteUser::toString() const { return "DeleteUser"; }
BaseAction* DeleteUser::clone() {
    BaseAction* del=new DeleteUser(getErrorMsg(),getStatus());
    return del;
}

//+++ DuplicateUser +++
DuplicateUser::~DuplicateUser() { }
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
                std::cout<<getErrorMsg()<<'\n';
            }
        } else{
            error(newName + " is an exist user");
            std::cout<<getErrorMsg()<<'\n';
        }
    } else {
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string DuplicateUser::toString() const { return "DuplicateUser"; }
BaseAction* DuplicateUser::clone() {
    BaseAction* del=new DuplicateUser(getErrorMsg(),getStatus());
    return del;
}


//+++ PrintContentList +++
PrintContentList::~PrintContentList() { }
PrintContentList::PrintContentList() :BaseAction(){}
PrintContentList::PrintContentList(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void PrintContentList::act(Session &sess) {
    std::string& command=sess.getCurrentCommand();
    std::vector<std::string> words;
    sess.split(command,words);
    if(words.size()==1) {
        int index = 1;
        std::string str = "";
        for (auto &watch: sess.getContent()) {
            printf("%d. %s %s \n", index, watch->toString().c_str(), watch->printAll().c_str());
            index++;
        }
        complete();
    }
    else{
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string PrintContentList::toString() const { return "PrintContentList"; }
BaseAction* PrintContentList::clone() {
    BaseAction* del=new PrintContentList(getErrorMsg(),getStatus());
    return del;
}


//+++ PrintWatchHistory +++
PrintWatchHistory::~PrintWatchHistory() { }
PrintWatchHistory::PrintWatchHistory() :BaseAction(){}
PrintWatchHistory::PrintWatchHistory(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void PrintWatchHistory::act(Session &sess) {
    std::string& command=sess.getCurrentCommand();
    std::vector<std::string> words;
    sess.split(command,words);
    if(words.size()==1) {
        int index=1;
        for(auto& watch: sess.getActiveUser().get_history()) {
            printf("%d. %s \n",index,watch->toString().c_str());
            index++;
        }
        complete();
    }
    else{
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string PrintWatchHistory::toString() const { return "PrintWatchHistory"; }
BaseAction* PrintWatchHistory::clone() {
    BaseAction* del=new PrintWatchHistory(getErrorMsg(),getStatus());
    return del;
}


//+++ Watch +++
Watch::~Watch() { }
Watch::Watch() :BaseAction(){}
Watch::Watch(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void Watch::act(Session &sess) {
    std::vector<std::string> words;
    sess.split(sess.getCurrentCommand(), words);
    if (words.size() == 1) {
        std::string &idStr = words.at(0);
        if (isNumber(idStr)) {
            Watchable *watch = sess.getContentByID(stoi(idStr)-1);
            if (watch != nullptr) {
                sess.getActiveUser().addToHistory(watch->clone());
                std::string name = "Watching " + watch->toString();
                std::cout << name << '\n';
                Watchable *nextWatch = sess.getActiveUser().getRecommendation(sess);
                name = "We recommend watching " + nextWatch->toString() + ", continue watching? [y/n]";
                std::cout << name << '\n';
                std::string answer;
                std::getline(std::cin, answer);
                if (answer.compare("y")==0) {
                    name = std::to_string(nextWatch->getId()+1);
                    sess.setCurrentCommand(name);
                    act(sess);
                }
                complete();
            } else {
                error(idStr + " is out of content bounds");
                std::cout<<getErrorMsg()<<'\n';
            }
        } else {
            error(idStr + " is not an integer");
            std::cout<<getErrorMsg()<<'\n';
        }
    } else {
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string Watch::toString() const { return "Watch"; }
BaseAction* Watch::clone() {
    BaseAction* del=new Watch(getErrorMsg(),getStatus());
    return del;
}


//+++ PrintActionsLog +++
PrintActionsLog::~PrintActionsLog() { }
PrintActionsLog::PrintActionsLog() :BaseAction(){}
PrintActionsLog::PrintActionsLog(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void PrintActionsLog::act(Session &sess) {
    std::string& command=sess.getCurrentCommand();
    std::vector<std::string> words;
    sess.split(command,words);
    if(words.size()==1) {
        std::string str, status, error;
        for (auto &action: sess.getActionsLog()) {
            str = action->toString() + " ";
            switch (action->getStatus()) {
                case COMPLETED:
                    status = "COMPLETED";
                    break;
                case PENDING:
                    status = "PENDING";
                    break;
                case ERROR:
                    status = "ERROR";
                    break;
            }
            if (status.compare("ERROR") == 0)
                error = " " + action->getErrorMsgPublic();
            else
                error = "";
            str += status + error;
            std::cout << str << '\n';
        }
        complete();
    } else {
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string PrintActionsLog::toString() const { return "PrintActionsLog"; }
BaseAction* PrintActionsLog::clone() {
    BaseAction* del=new PrintActionsLog(getErrorMsg(),getStatus());
    return del;
}


//+++ Exit +++
Exit::~Exit() { }
Exit::Exit() :BaseAction(){}
Exit::Exit(std::string errorMsg, ActionStatus status):BaseAction(errorMsg,status) {}
void Exit::act(Session &sess) {
    std::string& command=sess.getCurrentCommand();
    std::vector<std::string> words;
    sess.split(command,words);
    if(words.size()==1) {
        sess.setIsRun(false);
        complete();
    }
    else {
        error("invalid input");
        std::cout<<getErrorMsg()<<'\n';
    }
}
std::string Exit::toString() const { return "Exit"; }
BaseAction* Exit::clone() {
    BaseAction* del=new Exit(getErrorMsg(),getStatus());
    return del;
}

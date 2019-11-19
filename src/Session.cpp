//
// Created by tal on 19/11/2019.
//

#include "Session.h"

Session::Session(const std::string &configFilePath) {

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
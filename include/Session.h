#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session(const Session& other);
    Session(Session&& other);
    ~Session();
    Session& operator=(Session& other);
    Session& operator=(Session&& other);
    void start();
    const std::vector<Watchable*>& getContent();
    Watchable* getContentByID(long id) const;
    const std::vector<BaseAction*>& getActionsLog();
    User* getUserFromMap(std::string name);
    User& getActiveUser() const;
    void addToActionsLog(BaseAction* newAction);
    void addToUserMap(std::string name,User* newUserMap);
    bool deleteFromUserMap(std::string name);
    void setActiveUser(User* user);
    std::string& getCurrentCommand();
    void setCurrentCommand(std::string& currentCommand);  
    const int getIndexOfContent();    


private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string currentCommand;//the command just written in the main
    int indexOfContent;

};
#endif

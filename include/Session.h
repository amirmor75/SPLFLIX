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
    Session(const Session& session);
    ~Session();
    void start();
    const std::vector<Watchable*>& getContent();
    const std::vector<BaseAction*>& getActionsLog();
    User* getUserFromMap(std::string name);
    const User& getActiveUser();
    void addToActionsLog(BaseAction* newAction);
    void addToUserMap(std::string name,User* newUserMap);
    void setActiveUser(User* user);
    std::string& getCurrentCommand();
    void setCurrentCommand(std::string& currentCommand);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string currentCommand;
};
#endif

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
    std::vector<Watchable*>& getContent();
    std::vector<BaseAction*>& getActionsLog();
    const std::unordered_map<std::string,User*>& getUserMap();
    User& getActiveUser();
    void setActionsLog(std::vector<BaseAction*> newActionLog);
    void setUserMap(std::unordered_map<std::string,User*> newUserMap);
    void setActiveUser(User*);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
};
#endif

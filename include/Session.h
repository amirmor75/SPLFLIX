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

    //5 Rule S

    Session(const Session& other);
    Session(Session&& other);
    ~Session();
    Session& operator=(Session& other);
    Session& operator=(Session&& other);
    //5 Rule F
  
    void start();
    std::vector<std::string>* split(std::string command);

    //getters S
    const std::vector<Watchable*>& getContent();
    Watchable* getContentByID(long id) const;
    const std::vector<BaseAction*>& getActionsLog();
    User* getUserFromMap(std::string name);
    User& getActiveUser() const;
    std::string& getCurrentCommand();
    const int getIndexOfContent();
    bool getIsRun() const;
    //getters F

    void addToActionsLog(BaseAction* newAction);
    void addToUserMap(std::string name,User* newUserMap);
    bool deleteFromUserMap(std::string name);

    //setters
    void setActiveUser(User* user);
    void setCurrentCommand(std::string& currentCommand);
    bool setIsRun(bool run);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string currentCommand;//the command just written in the main
    int indexOfContent;//index of the current content the activeUser just watch
    bool isRunning;//if the main loop is still running

};
#endif

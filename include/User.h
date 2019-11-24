#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Watchable.h"

class Watchable;
class Session;

class User{
public:
    User(const std::string &name);
    //5 Rule S
    ~User();
    User(const User& other);
    User(User&& other);
    User& operator=(User& other);
    User& operator=(User&& other);
    //5 Rule F
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual Watchable* getRecommendation(Watchable& w)=0;
    virtual Watchable* getRecommendation(Movie& s) = 0;
    virtual Watchable* getRecommendation(Episode& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual User* clone()=0;
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual Watchable* getRecommendation(Watchable& w);
    virtual Watchable* getRecommendation(Movie& s); //for double dispatch
    virtual Watchable* getRecommendation(Episode& s);
    User* clone();
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const std::string& name, int index);
    virtual Watchable* getRecommendation(Session& s);
    virtual Watchable* getRecommendation(Watchable& w);
    virtual Watchable* getRecommendation(Movie& s); //for double dispatch
    virtual Watchable* getRecommendation(Episode& s);
    User* clone();
private:
    int indexOfHistory;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const std::string& name, std::unordered_map<std::string,int> tags);
    virtual Watchable* getRecommendation(Session& s);
    virtual Watchable* getRecommendation(Watchable& w);
    virtual Watchable* getRecommendation(Movie& s); //we need to implement double dispatch
    virtual Watchable* getRecommendation(Episode& s);
    User* clone()=0;
    std::string mostPopTag();
private:
};

#endif

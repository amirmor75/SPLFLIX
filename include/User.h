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

    //given
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    //given

    //5 Rule S
    ~User();
    User(const User& other);
    User(User&& other);
    User& operator=(User& other);
    User& operator=(User&& other);
    //5 Rule F

   //visitor
    virtual Watchable* getRecommendation(Watchable& w)=0;
    virtual Watchable* getRecommendation(Movie& s) = 0;
    virtual Watchable* getRecommendation(Episode& s) = 0;
    //visitor
    virtual User* clone()=0;

    // all is given
    protected:
    std::vector<Watchable*> history;
    private:
    std::string name;
    //all is given
};


class LengthRecommenderUser : public User {
public:
    //given
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    //given

    //visitor
    virtual Watchable* getRecommendation(Watchable& w);
    virtual Watchable* getRecommendation(Movie& s); //for double dispatch
    virtual Watchable* getRecommendation(Episode& s);
    //visitor
    User* clone();
private:
};

class RerunRecommenderUser : public User {
public:
    //given
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    //given

    RerunRecommenderUser(const std::string& name, int index);
    virtual Watchable* getRecommendation(Watchable& w);
    virtual Watchable* getRecommendation(Movie& s); //for double dispatch
    virtual Watchable* getRecommendation(Episode& s);
    User* clone();
private:
    int indexOfHistory;
};

class GenreRecommenderUser : public User {
public:
    //given
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    //given
    GenreRecommenderUser(const std::string& name, std::unordered_map<std::string,int> tags);
    //visitor
    virtual Watchable* getRecommendation(Watchable& w);
    virtual Watchable* getRecommendation(Movie& s); //we need to implement double dispatch
    virtual Watchable* getRecommendation(Episode& s);
    //visitor
    User* clone()=0;
    std::string mostPopTag();
private:
};

#endif

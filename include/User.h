#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Watchable.h"

class Watchable;
class Movie;
class Episode;
class Session;

class User{
public:


    //given
    User(const std::string& name);
    virtual Watchable *getRecommendation(Session &s) const = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    //given
    //5 Rule S
    ~User();
    User(User& other);
    User(User&& other);
    User& operator=(User& other);
    User& operator=(User&& other);
    //5 Rule F
    User(const std::string &name,std::vector<Watchable*> history);
    virtual User* clone()=0;
    void addToHistory(Watchable* watch);
    void setName(std::string & name);

    // all is given
    protected:
    std::vector<Watchable*> history;
    int lastrecommended;
private:
    std::string name;
    //all is give
};


class LengthRecommenderUser : public User {
public:
    //given
    LengthRecommenderUser(const std::string& name);
    virtual Watchable *getRecommendation(Session &s) const;
    //given
    virtual User* clone();

private:
};

class RerunRecommenderUser : public User {
public:
    //given
    RerunRecommenderUser(const std::string& name);
    virtual Watchable *getRecommendation(Session &s) const ;
    //given
    virtual User* clone();      


private:
};

class GenreRecommenderUser : public User {
public:
    //given
    GenreRecommenderUser(const std::string& name);
    virtual Watchable *getRecommendation(Session &s) const;
    //given
    virtual User* clone();
    std::string mostPopTag(const std::unordered_map<std::string,int>& tagMap) const;    
private:
};

#endif

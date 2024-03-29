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
    User(std::vector<Watchable*>& hist, int lastRec, std::string& name);
    //given
    User(const std::string& name);
    virtual Watchable *getRecommendation(Session &s) = 0;
    std::vector<Watchable*> get_history() const;
    std::string& getName();
    //given
    //5 Rule S
    virtual ~User();
    User(User& other);
    User(User&& other);
    User& operator=(User& other);
    User& operator=(User&& other);
    //5 Rule F
    virtual User* clone()=0;
    void addToHistory(Watchable* watch);
    void setName(std::string & name);
    void setLastRec(long i);
    bool isInHistory(Watchable& watch) const;

    // all is given
    protected:
    std::vector<Watchable*> history;
    int lastRecommended;
    private:
    std::string name;
    //all is give
};


class LengthRecommenderUser : public User {
public:
    virtual ~LengthRecommenderUser();
    LengthRecommenderUser(std::vector<Watchable*>& hist, int lastRec, std::string& name);
    //given
    LengthRecommenderUser(const std::string& name);
    virtual Watchable *getRecommendation(Session &s);
    //given
    virtual User* clone();

private:
};

class RerunRecommenderUser : public User {
public:
    virtual ~RerunRecommenderUser();
    RerunRecommenderUser(std::vector<Watchable*>& hist, int lastRec, std::string& name);
    //given
    RerunRecommenderUser(const std::string& name);
    virtual Watchable *getRecommendation(Session &s) ;
    //given
    virtual User* clone();
private:
};

class GenreRecommenderUser : public User {
public:
    virtual ~GenreRecommenderUser();
    GenreRecommenderUser(std::vector<Watchable*>& hist, int lastRec, std::string& name);
    //given
    GenreRecommenderUser(const std::string& name);
    virtual Watchable *getRecommendation(Session &s);
    //given
    virtual User* clone();
    std::string mostPopTag(const std::unordered_map<std::string,int>& tagMap) const;    
private:
};

#endif

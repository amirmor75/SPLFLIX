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
    User(const std::string &name);
    User(const std::string &name,std::vector<Watchable*> history);
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual Watchable* getRecommendation(Watchable& w)=0;
    virtual Watchable* getRecommendation(Movie& s) = 0;
    virtual Watchable* getRecommendation(Episode& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual User* duplicateUser(std::string &name)=0;
    virtual User* clone()=0;
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    Watchable* getRecommendation(Session& s);
    Watchable* getRecommendation(Watchable& w);
    Watchable* getRecommendation(Movie& s);
    Watchable* getRecommendation(Episode& s);
    User* duplicateUser(std::string &name);
    User* clone();
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const std::string& name, int index);
    Watchable* getRecommendation(Session& s);
    Watchable* getRecommendation(Watchable& w);
    Watchable* getRecommendation(Movie& s);
    Watchable* getRecommendation(Episode& s);
    User* duplicateUser(std::string &name);
    User* clone();
private:
    int indexOfHistory;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    Watchable* getRecommendation(Session& s);
    Watchable* getRecommendation(Watchable& w);
    Watchable* getRecommendation(Movie& s);
    Watchable* getRecommendation(Episode& s);
    User* duplicateUser(std::string &name);
    User* clone();
    std::string mostPopTag();
private:
};

#endif

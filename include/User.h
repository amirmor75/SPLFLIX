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
    virtual void buildMe(User* u) const =0;   
    User(const std::string &name,std::vector<Watchable*> history);    
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual User* duplicateUser(std::string &name)=0;
    virtual User* clone()=0;
  
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
    virtual void buildMe(User* u) const;   
    virtual User* duplicateUser(std::string &name);
    virtual User* clone();
private:
};

class RerunRecommenderUser : public User {
public:
    //given
    RerunRecommenderUser(const std::string& name);
    virtual Watchable *getRecommendation(Session &s) const ;
    //given
    virtual void buildMe(User* u) const ;
    virtual User* clone();      
    User* duplicateUser(std::string &name);
    

private:
};

class GenreRecommenderUser : public User {
public:
    //given
    GenreRecommenderUser(const std::string& name);

    virtual Watchable *getRecommendation(Session &s) const;
    //given
    virtual void buildMe(User* u)const;
    virtual User* clone();
    std::string mostPopTag(const std::unordered_map<std::string,int>& tagMap) const;    
    User* duplicateUser(std::string &name);   
private:
};

#endif

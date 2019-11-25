#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
#include "User.h"


class Session;
class User;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>&  tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual std::string printAll() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual void recommendMe(User&  u)=0;
    virtual Watchable* clone()=0;

    //getters
    const long &getId() const;
    int getLength() const;
    const std::vector<std::string> & getTags() const;


private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    std::string printAll() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual void recommendMe(User&  u);
    Watchable* clone();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    std::string printAll() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual void recommendMe(User&  u);
    Watchable* clone();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif

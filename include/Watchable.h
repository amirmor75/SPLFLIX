#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
#include "User.h"


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>&  tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0
    virtual Watchable* clone()=0; //need to implement in sub classes
    const std::vector<std::string> & getTags() const;
    virtual void recommendMe(User&  u)=0;
    //getters
    const long & getId() const;
    int getLength() const;
    std::vector<std::string>& getTags() const;
    //getters
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(Movie& other);
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString(bool print_full=false) const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual void recommendMe(User&  u);
    virtual Movie* clone();
    //get
    std::string getName();
    //get

private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(Episode& other);
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString(bool print_full=false) const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual void recommendMe(User&  u);
    virtual Episode* clone();
    //getters
    std::string getSeriesName();
    int getEpisode();
    long getNextEpisodeId();
    //getters
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif

#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
#include "../include/User.h"

class Session;
class User;

class Watchable{
public:
    //given
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;

    virtual Watchable* getNextWatchable(Session&) const = 0;
    //given
    virtual Watchable* clone()=0;
    virtual bool isEpisode()=0;
    //getters
    const long& getId() const;
    int getLength() const;
    const std::vector<std::string> & getTags() const;
    //getters

    virtual std::string printAll() const = 0;    


    
private:
    //given
    const long id;
    int length;
    std::vector<std::string> tags;
    //given
};

class Movie : public Watchable{
public:
    //given
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    //given
    Movie(Movie &other);
    virtual Watchable* clone();
    virtual bool isEpisode();
    std::string printAll() const;
    //get
    std::string getName();
    //get
    
private:
    //given
    std::string name;
    //given
};

class Episode: public Watchable{
public:
    //given
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    //given
    Episode(Episode& other);
    virtual Watchable* clone();
    virtual bool isEpisode();
    std::string printAll() const;
    //getters
    std::string getSeriesName();
    int getEpisode();
    long getNextEpisodeId();
    int getSeason();
    //getters

private:
    //given
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
    //given

};

#endif

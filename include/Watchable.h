#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
#include "User.h"

class Session;
class Watchable{
public:
    //given
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    //given
    virtual Watchable* clone()=0;
    virtual void recommendMe(User&  u)=0;
    //getters
    const long & getId() const;
    int getLength() const;
    const std::vector<std::string> & getTags() const;
    //getters
private:
    //given
    const long id;
    int length;
    std::vector<std::string> tags;
    //given
};

class Episode: public Watchable{
public:
    //given
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);

    Episode(Episode &other);

    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    //given
    virtual Watchable* clone();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;

    int getSeason();

    int getEpisode();

    long getNextEpisodeId();

    std::string getSeriesName();

    void recommendMe(User &u);

    std::string toString(bool print_full) const;
};

class Movie : public Watchable{
public:
    //given
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    //given
    Movie(Movie &other);
    std::string toString(bool print_full) const;
    virtual void recommendMe(User&  u);
    virtual Watchable* clone();

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
    virtual void recommendMe(User&  u);
    virtual Watchable* clone();
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

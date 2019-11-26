//
// Created by amir on 19/11/2019.
//

#include "Watchable.h"
#include "Session.h"
#include <utility>
class Session;

//Watchable S
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length),tags(std::move(tags)){}
Watchable::~Watchable() = default;
const long & Watchable::getId() const { return id; }
int Watchable::getLength() const{ return length; }
const std::vector<std::string> & Watchable::getTags() const{ return tags; }
//Watchable F

//Movie S
Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags), name(name) {}
Movie::Movie(Movie &other) :Watchable(other.getId(),other.getLength(),other.getTags()),name(other.getName()){}
//dont know if getTags is good here it is not deep copy it is &

Watchable* Movie::getNextWatchable(Session &) const {
    return nullptr;
}
std::string Movie::getName() { return name;}

bool Movie::isEpisode() { return false;}
Watchable* Movie::clone() {
    return  new Movie(*this);
}
//Movie F

//Episode S
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags):Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName),nextEpisodeId(id+1){}
Episode::Episode(Episode &other):Watchable(other.getId(),other.getLength(),other.getTags()),season(other.getSeason()),episode(other.getEpisode()),seriesName(other.getSeriesName()){}
// at copy constructor i don't know if the other.getTags is giving me a deep copy of tags, i want my this to be independent.
Watchable* Episode::getNextWatchable(Session &s) const {
    const std::vector<Watchable*>& content=s.getContent();
    if(content.at(nextEpisodeId)->isEpisode())
        return content.at(nextEpisodeId);
    else
        return nullptr;
}
bool Episode::isEpisode() { return true;}
std::string Episode::getSeriesName() { return  seriesName;}
long Episode::getNextEpisodeId() { return  nextEpisodeId;}
int Episode::getEpisode() { return  episode;}
int Episode::getSeason() { return season;}
Watchable* Episode::clone() {return new Episode(*this);}
//Episode F



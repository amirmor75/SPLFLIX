//
// Created by amir on 19/11/2019.
//

#include "Watchable.h"
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
std::string Movie::toString(bool print_full) const {
    std::string str(getTags().begin(), getTags().end());
    return getId()+". "+name+" "+std::to_string(getLength())+" minutes"+ str;
}
Watchable* Movie::getNextWatchable(Session &) const {}
void Movie::recommendMe(User &u) {
    u.getRecommendation(*this);
}
std::string Movie::getName() { return name;}
Watchable* Movie::clone() {
    return  new Movie(*this);
}
//Movie F

//Episode S
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags):Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName){}
Episode::Episode(Episode &other):Watchable(other.getId(),other.getLength(),other.getTags()),season(other.getSeason()),episode(other.getEpisode()),seriesName(other.getSeriesName()){}
// at copy constructor i don't know if the other.getTags is giving me a deep copy of tags, i want my this to be independent.
std::string Episode::toString(bool print_full) const {
    std::string str(getTags().begin(), getTags().end());
    return getId()+". "+seriesName+" S"+std::to_string(season)+"E"+std::to_string(episode)+
         +" "+std::to_string(getLength())+" minutes"+ str;
}
void Episode::recommendMe(User &u) {u.getRecommendation(*this);}
std::string Episode::getSeriesName() { return  seriesName;}
long Episode::getNextEpisodeId() { return  nextEpisodeId;}
int Episode::getEpisode() { return  episode;}
int Episode::getSeason() { return season;}
Watchable* Episode::clone() {return new Episode(*this);}
//Episode F



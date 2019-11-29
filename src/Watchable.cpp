//
// Created by amir on 19/11/2019.
//

#include "../include/Watchable.h"
#include "../include/Session.h"
#include <utility>
class Session;

//Watchable S
Watchable::Watchable(long newid, int newlength, const std::vector<std::string>& newtags) : id(newid), length(newlength),tags(newtags){}
Watchable::~Watchable() = default;
int Watchable::getLength() const{ return length; }
const long& Watchable::getId() const { return id;}
const std::vector<std::string> & Watchable::getTags() const{ return tags; }
//Watchable F

//Movie S
Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags), name(name) {}
Movie::Movie(Movie &other) :Watchable(other.getId(),other.getLength(),other.getTags()),name(other.getName()){}
Watchable* Movie::getNextWatchable(Session &) const {
    return nullptr;
}
std::string Movie::getName() { return name;}

bool Movie::isEpisode() { return false;}
Watchable* Movie::clone() {
    return  new Movie(getId(),getName(),getLength(),getTags());
}
//Movie F

//Episode S
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags):Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(id+1) {}
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags, long nextEp) :Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(nextEp) {}
Episode::Episode(Episode &other):Watchable(other.getId(),other.getLength(),other.getTags()),seriesName(other.getSeriesName()),season(other.getSeason()),episode(other.getEpisode()), nextEpisodeId(other.nextEpisodeId){}
Watchable* Episode::getNextWatchable(Session &s) const {
    const std::vector<Watchable*>& content=s.getContent();
    if((size_t)nextEpisodeId<content.size() && content.at(nextEpisodeId)->isEpisode())
        return content.at(nextEpisodeId);
    else
        return nullptr;
}
bool Episode::isEpisode() { return true;}
std::string Episode::getSeriesName() { return  seriesName;}
long Episode::getNextEpisodeId() { return  nextEpisodeId;}
int Episode::getEpisode() { return  episode;}
int Episode::getSeason() { return season;}
Watchable* Episode::clone() {
    return new Episode(getId(),getSeriesName(),getLength(),getSeason(),getEpisode(),getTags(),getNextEpisodeId());
}
//Episode F

std::string Movie::toString() const {
    return name;
}
std::string Movie::printAll() const {
    std::string tagsList="[";
    for(auto& tag:getTags()){
        tagsList+=tag+", ";
    }
    tagsList[tagsList.size()-2]=']';
    return std::to_string(getLength())+" minutes "+ tagsList;
}
std::string Episode::toString() const {
    return seriesName+" S"+std::to_string(season)+"E"+std::to_string(episode);
}
std::string Episode::printAll() const {
    std::string tagsList="[";
    for(auto& tag:getTags()){
        tagsList+=tag+", ";
    }
    tagsList[tagsList.size()-2]=']';
    return std::to_string(getLength())+" minutes "+ tagsList;
}





//
// Created by amir on 19/11/2019.
//

#include "Watchable.h"

#include <utility>

//Watchable
Watchable::Watchable(long id, int length, const std::vector<std::string> tags): id(id), length(length),tags(std::move(tags)) {}
Watchable::~Watchable() = default;
const long & Watchable::getId() const { return id; }
const int Watchable::getLength() const { return length; }
const std::vector<std::string> & Watchable::getTags() const{ return tags; }


//Movie
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags), name(name) {}
std::string Movie::toString(bool print_full) const {
    std::string str(getTags().begin(), getTags().end());
    return getId()+". "+name+" "+std::to_string(getLength())+" minutes"+ str;
}
Watchable* Movie::getNextWatchable(Session &) const {}



//Episode
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags):Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName)
{}
std::string Episode::toString(bool print_full) const {
    std::string str(getTags().begin(), getTags().end());
    return getId()+". "+seriesName+" S"+std::to_string(season)+"E"+std::to_string(episode)+
         +" "+std::to_string(getLength())+" minutes"+ str;
}

Watchable* Episode::getNextWatchable(Session & s) const
{
}



//
// Created by amir on 19/11/2019.
//

#include "Watchable.h"
#include <utility>
class Session;

//Watchable
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length),tags(std::move(tags)){}
Watchable::~Watchable() = default;
int Watchable::getLength() const{ return length; }
const long& Watchable::getId() const { return id;}
const std::vector<std::string> & Watchable::getTags() const{ return tags; }
std::string Watchable::toString() const { return ""; }

//Movie
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags), name(name) {}

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
Watchable* Movie::getNextWatchable(Session &) const { return nullptr; }
void Movie::recommendMe(User &u) {
    u.getRecommendation(*this);
}
Watchable* Movie::clone() {
    Watchable* watch=new Movie(getId(),name,getLength(),getTags());
    return watch;
}



//Episode
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags):Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName)
{}
std::string Episode::toString() const {
    return (getId()+1)+". "+seriesName+" S"+std::to_string(season)+"E"+std::to_string(episode);
}
std::string Episode::printAll() const {
    std::string tagsList="[";
    for(auto& tag:getTags()){
        tagsList+=tag+", ";
    }
    tagsList[tagsList.size()-2]=']';
    return std::to_string(getLength())+" minutes "+ tagsList;
}
Watchable* Episode::getNextWatchable(Session & s) const
{ return nullptr;
}
void Episode::recommendMe(User &u) {
    u.getRecommendation(*this);
}
Watchable* Episode::clone() {
    Watchable* watch=new Episode(getId(),seriesName,getLength(),season,episode,getTags());
    return watch;
}



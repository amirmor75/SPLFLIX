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

//Movie
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags), name(name) {}

std::string Movie::toString(bool print_full) const {
    return name;
}
std::string Movie::printAll() {
    std::string tagsList="[";
    for(auto& tag:getTags()){
        tagsList+=tag+", ";
    }
    tagsList[tagsList.size()-2]=']';
    return std::to_string(getLength())+" minutes "+ tagsList;
}
Watchable* Movie::getNextWatchable(Session &) const {}
void Movie::recommendMe(User &u) {
    u.getRecommendation(*this);
}



//Episode
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags):Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName)
{}
std::string Episode::toString(bool print_full) const {
    return (getId()+1)+". "+seriesName+" S"+std::to_string(season)+"E"+std::to_string(episode);
}
std::string Episode::printAll() {
    std::string tagsList="[";
    for(auto& tag:getTags()){
        tagsList+=tag+", ";
    }
    tagsList[tagsList.size()-2]=']';
    return std::to_string(getLength())+" minutes "+ tagsList;
}
Watchable* Episode::getNextWatchable(Session & s) const
{
}
void Episode::recommendMe(User &u) {
    u.getRecommendation(*this);
}




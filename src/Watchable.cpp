//
// Created by amir on 19/11/2019.
//

#include "Watchable.h"

Watchable::Watchable(long id, int length, const std::vector<std::string> &tags): id(id), length(length),tags(tags)
{}






Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags), name(name) {}

Watchable* Movie::getNextWatchable(Session &) const {}

std::string Movie::toString(bool print_full) const {}

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags):Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName)
{}
Watchable* Episode::getNextWatchable(Session & s) const
{
    ss
}



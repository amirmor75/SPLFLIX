//
// Created by amir on 19/11/2019.
//

#include "Watchable.h"

class Session;


Watchable::Watchable(long id, int length, const std::vector<std::string> &tags): id(id), length(length),tags(tags)
{}
int Watchable::getLength() { return length;}

Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags), name(name) {}

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,const std::vector<std::string> &tags):Watchable(id,length,tags),season(season),episode(episode),seriesName(seriesName)
{}






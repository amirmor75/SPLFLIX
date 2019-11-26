//
// Created by amir on 19/11/2019.
//

#include <User.h>
#include <string>
#include <cstring>
#include "Watchable.h"
#include "Session.h"




//User S
User::User(const std::string &name): name(name),history(),lastrecommended(0){}
std::string User::getName() const { return name;}
std::vector<Watchable*> User::get_history() const {return history;}
//5 Rule S
User::~User() {
    for(Watchable* w:history){
        delete w;
    }
    history.clear();
    name= nullptr;
}
User::User(User &other){
    name=other.name;
    lastrecommended=other.lastrecommended;
    for(Watchable* w: other.get_history())
        this->history.push_back(w->clone());
}
User::User(User &&other){
    name=other.name;
    other.name= nullptr;

    lastrecommended=other.lastrecommended;
    other.lastrecommended=0;

    this->history=other.history;
    other.history.clear();
}
User& User::operator=(User &other) {
    if (this!=&other){
        for(Watchable* w: this->history)
            delete w;
        history.clear();
        this->name=other.name;
        lastrecommended=other.lastrecommended;
        for (int i = 0; i < other.history.size(); ++i)
            this->history.push_back(other.history.at(i)->clone());
    }
    return (*this);
}
User& User::operator=(User &&other) {

    name= nullptr;
    for(Watchable* w:history)
        delete w;
    history.clear();
    lastrecommended=0;

    this->name=other.name;
    this->lastrecommended=other.lastrecommended;
    this->history=other.history;

    other.history.clear();
    other.lastrecommended=0;
    other.name= nullptr;

    return *this;
}
//5 Rule F
void User::setName(std::string &newName) {
    name=newName;
}
//User F

//userLEN S
LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name){}
Watchable* LengthRecommenderUser::getRecommendation(Session &s) const {
    Watchable* nextEpisode=history.at(history.size()-1)->getNextWatchable(s);
    if(nextEpisode!= nullptr){
        return nextEpisode;
    } else {
        const std::vector<Watchable *> &content = s.getContent();
        if (content.size() != history.size()) {
            int sum = 0;
            for (Watchable *c : history) {
                sum = sum + c->getLength();
            }
            double avgLength = sum / history.size();
            Watchable *tempMin = nullptr;
            bool watched = false;
            bool tempIsNull = true;
            for (int i = 0; i < content.size(); ++i) {
                if (tempIsNull || abs(avgLength - content.at(i)->getLength()) < abs(avgLength - tempMin->getLength())) {
                    for (int k = 0; k < history.size() && !watched; ++k) {
                        if (this->history.at(k) == tempMin)
                            watched = true;
                    }
                    if (!watched) {
                        tempMin = content.at(i);
                        tempIsNull = false;
                    }
                }
            }
            return tempMin;
        }
    }
    return nullptr;
}

User* LengthRecommenderUser::clone() {
    return new LengthRecommenderUser(*this);
}
//userLEN F


//userRER S
RerunRecommenderUser::RerunRecommenderUser(const std::string &name):User(name) {};
Watchable* RerunRecommenderUser::getRecommendation(Session &s) const {
    Watchable* nextEpisode=history.at(history.size()-1)->getNextWatchable(s);
    if(nextEpisode!= nullptr){
        return nextEpisode;
    } else {
        if (history.size()!=0)
            return history.at(lastrecommended%history.size());
        else
            return nullptr;
    }

}
User* RerunRecommenderUser::clone() {
    return new RerunRecommenderUser(*this);
}
//userRER F




//userGEN S
GenreRecommenderUser::GenreRecommenderUser(const std::string &name):User(name){}
std::string GenreRecommenderUser::mostPopTag( const std::unordered_map<std::string,int>& amountOfTags ) const {
    std::string popTag="";
    int amount=0;
    for(auto& tag: amountOfTags)
    {
        if(amount<tag.second){
            amount=tag.second;
            popTag=tag.first;
        }
        else if(amount==tag.second){
            char array1[popTag.size()];
            std::strcpy(array1, popTag.c_str()); // copying the contents of the string to char array
            char array2[tag.first.size()];
            std::strcpy(array2, tag.first.c_str()); // copying the contents of the string to char array
            if(std::lexicographical_compare(array2,array2+ sizeof(array2),array1,array1+sizeof(array1))) {
                amount = tag.second;
                popTag = tag.first;
            }
        }
    }
    return popTag;
}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) const {


    Watchable* nextEpisode=history.at(history.size()-1)->getNextWatchable(s);
    if(nextEpisode!= nullptr){
        return nextEpisode;
    } else {
        //init of TagMap S
        std::unordered_map<std::string,int> tagMap;
        for(Watchable* watch: history){
            for(std::string tag: watch->getTags()){
                if(tagMap.find(tag)==tagMap.end())//this tag not exists
                    tagMap[tag]=1;
                else
                    tagMap[tag]++;
            }
        }
        //init of TagMap F
        const std::vector<Watchable *> &content = s.getContent();

        std::string s=mostPopTag(tagMap);//first poptag
        bool beenWatched= false;
        while (s.compare("")!=0 ) {
            for (Watchable *show:content) {
                for (Watchable *watched:history) {//checks if show been watched
                    if (watched==show)
                        beenWatched = true;
                }
                if (!beenWatched) {
                    for (std::string tag:show->getTags()) {//checks if show has relevant tag
                        if (s.compare(tag) == 0)
                            return show;
                    }
                }
            }
            tagMap.erase(s);
            s=mostPopTag(tagMap);
        }
    }
}

User* GenreRecommenderUser::clone() {
    return new GenreRecommenderUser(*this);
}
//userGEN F
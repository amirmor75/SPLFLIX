//
// Created by amir on 19/11/2019.
//

#include "../include/User.h"
#include <string>
#include <cstring>
#include "../include/Session.h"

//User S
User::User(const std::string &name):history(),lastRecommended(1), name(name){}
User::User(std::vector<Watchable*>& hist, int lastRec, std::string &newName):history(),lastRecommended(lastRec), name(newName) {
    for(Watchable* watch: hist) {
        addToHistory(watch->clone());
    }
}
std::vector<Watchable*> User::get_history() const {return history;}
//5 Rule S
User::~User() {
    for(Watchable* hist:history){
        delete hist;
    }
}
User::User(User &other):history(),lastRecommended(other.lastRecommended), name(other.name){
    if(this != &other) {
        for (Watchable* w: other.get_history()) {
            history.push_back(w->clone());
        }
    }
}
User::User(User &&other): history(other.history),lastRecommended(other.lastRecommended), name(other.name){
    for(size_t i=0;i<other.history.size();i++){
        other.history.at(i)= nullptr;
    }
    other.history.clear();
}
User& User::operator=(User &other) {
    this->name=other.name;
    lastRecommended=other.lastRecommended;
    if (this!=&other){
        for(Watchable* w: this->history)
            delete w;
        history.clear();
        for (size_t i = 0; i < other.history.size(); i++)
            this->history.push_back(other.history.at(i)->clone());
    }
    return (*this);
}

User& User::operator=(User &&other) {
    name= nullptr;
    for(Watchable* w:history)
        delete w;
    history.clear();
    lastRecommended=0;

    this->name=other.name;
    this->lastRecommended=other.lastRecommended;
    this->history=other.history;

    other.history.clear();
    other.lastRecommended=0;
    other.name= nullptr;

    return *this;
}
//5 Rule F

void User::setName(std::string &newName) {
    name=newName;
}
std::string& User::getName() { return name;}
void User::addToHistory(Watchable *watch) {
    history.push_back(watch);
}
bool User::isInHistory(Watchable &watch) const {
    bool watched=false;
    for (size_t k = 0; k < history.size() && !watched; k++) { //checks if content exists in the history
        if (this->history.at(k)->getId()==watch.getId()) {
            watched = true;
        }
    }
    return watched;
}
void User::setLastRec(long i) { lastRecommended=i;}

//User F

//userLEN S
LengthRecommenderUser::~LengthRecommenderUser() { }
LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name){}
LengthRecommenderUser::LengthRecommenderUser(std::vector<Watchable *> &hist, int lastRec, std::string &name): User(hist,lastRec,name) {}
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
            double avgLength = sum / history.size(); //the avg length of content in history

            Watchable *tempMin = nullptr;
            bool tempIsNull = true;
            for (size_t i = 0; i < content.size(); i++) {
                if (tempIsNull || abs(avgLength - content.at(i)->getLength()) < abs(avgLength - tempMin->getLength())) {
                    if (!isInHistory(*content.at(i))) {
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
    return new LengthRecommenderUser(history,lastRecommended,getName());
}
//userLEN F


//userRER S
RerunRecommenderUser::~RerunRecommenderUser() { }
RerunRecommenderUser::RerunRecommenderUser(std::vector<Watchable *>& hist, int lastRec, std::string& name): User(hist,lastRec,name)  {}
RerunRecommenderUser::RerunRecommenderUser(const std::string &name): User(name){}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) const {
    Watchable* nextEpisode=history.at(history.size()-1)->getNextWatchable(s);
    if(nextEpisode!= nullptr){
        return nextEpisode;
    } else {
        if (history.size()!=0) {
            int i=0;
            bool found=false;
            for(size_t k = 0; k < history.size() && !found; k++)
            {
                if(lastRecommended==history.at(k)->getId()) { // index of last recommended in history
                    i = k;
                    found=true;
                }
            }
            return history.at((i+1) % history.size());
        }
        else
            return nullptr;
    }
}

User* RerunRecommenderUser::clone() {
    return new RerunRecommenderUser(history,lastRecommended,getName());
}
//userRER F




//userGEN S
GenreRecommenderUser::~GenreRecommenderUser() { }
GenreRecommenderUser::GenreRecommenderUser(std::vector<Watchable *>& hist, int lastRec, std::string &name): User(hist,lastRec,name)  {}
GenreRecommenderUser::GenreRecommenderUser(const std::string &name):User(name){}
std::string GenreRecommenderUser::mostPopTag( const std::unordered_map<std::string,int>& tagMap ) const {
    std::string popTag="";
    int maxAmount=0;
    for(auto& tag: tagMap)
    {
        if(maxAmount<tag.second){//max is smaller than current
            maxAmount=tag.second;
            popTag=tag.first;
        }
        else if(maxAmount==tag.second){// same count, takes first lexi.
            char array1[popTag.size()];
            std::strcpy(array1, popTag.c_str()); // copying the contents of the string to char array
            char array2[tag.first.size()];
            std::strcpy(array2, tag.first.c_str()); // copying the contents of the string to char array
            if(std::lexicographical_compare(array2,array2+ sizeof(array2),array1,array1+sizeof(array1))) {
                maxAmount = tag.second;
                popTag = tag.first;
            }
        }
    }
    // printf("pop tag is %s \n",popTag.c_str());
    return popTag;
}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) const {
    Watchable* nextEpisode=history.at(history.size()-1)->getNextWatchable(s);// gets next episode of series or null.
    if(nextEpisode!= nullptr){// no next episode
        return nextEpisode;
    } else {// algs recommendation
        //init of TagMap S
        std::unordered_map<std::string,int> tagMap;
        for(Watchable* watch: history){
            for(std::string tag: watch->getTags()){// takes every watchable in history and takes tags.
                if(tagMap.count(tag)>0)//this exists
                    tagMap[tag]++;
                else// first appearence of tag
                    tagMap[tag]=1;
            }
        }
        //init of TagMap F

        std::string pop=mostPopTag(tagMap);//first poptag
        while (!pop.empty()) {
            for (Watchable *show:  s.getContent()) {
                if (!isInHistory(*show)) {
                    for (std::string tag: show->getTags()) {//checks if show has relevant tag
                        if (pop==tag)
                            return show;
                    }
                }
            }
            tagMap.erase(pop);
            pop=mostPopTag(tagMap);
        }
    }
    return nullptr;
}

User* GenreRecommenderUser::clone() {
    return new GenreRecommenderUser(history,lastRecommended,getName());
}
//userGEN F
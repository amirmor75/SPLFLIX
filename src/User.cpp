//
// Created by amir on 19/11/2019.
//

#include <User.h>
#include <string>
#include <cstring>
#include "Watchable.h"
#include "Session.h"

//+++ User +++
User::User(const std::string &name): name(name){}
std::string User::getName() const { return name;}
std::vector<Watchable*> User::get_history() const {return history;}
User* User::clone() {}

//+++ LengthRecommenderUser +++
LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name){}
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    for (int i = 0; i < history.size(); ++i) {

    }
    int  sum=0;
    for(Watchable *c : history)
    {
        sum=sum+ c-> getLength() ;
    }
    double avgLength=sum/history.size();
    const std::vector<Watchable*>& content= s.getContent();
    for(Watchable* show: content){
        if (show->getLength()==avgLength){
            bool watched=false;
            for(Watchable* seen: history){
                if (seen==show)
                    watched= true;
            }
            if(!watched)
                return show;
        }
    }
    return nullptr;
}
Watchable* LengthRecommenderUser::getRecommendation(Watchable &w) {}
Watchable* LengthRecommenderUser::getRecommendation(Movie &s) {}
Watchable* LengthRecommenderUser::getRecommendation(Episode &s) {}
User* LengthRecommenderUser::clone() {
    User* newUser=new LengthRecommenderUser(getName());
    return newUser;
}

//+++ RerunRecommenderUser +++
RerunRecommenderUser::RerunRecommenderUser(const std::string &name):User(name), indexOfHistory(0){}
RerunRecommenderUser::RerunRecommenderUser(const std::string &name, int index): User(name), indexOfHistory(index){}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    s.getContent().at((s.getIndexOfContent()+1)%s.getContent().size())->recommendMe(*this);
}
Watchable* RerunRecommenderUser::getRecommendation(Watchable &w) {

}
Watchable* RerunRecommenderUser::getRecommendation(Movie &s) {
    indexOfHistory++;
    return history.at((indexOfHistory-1)%history.size());
}
Watchable* RerunRecommenderUser::getRecommendation(Episode &s) {
    //if next episode exists then recommend it
    //else
    indexOfHistory++;
    return history.at((indexOfHistory-1)%history.size());
}
User* RerunRecommenderUser::clone() {
    User* newUser=new RerunRecommenderUser(getName(),indexOfHistory);
    return newUser;
}

//GenreRecommenderUser
GenreRecommenderUser::GenreRecommenderUser(const std::string &name):User(name){}
std::string GenreRecommenderUser::mostPopTag() {
    std::unordered_map<std::string,int> amountOfTags;
    for(auto& watch: history){
        for(auto& tag: watch->getTags()){
            if(amountOfTags.find(tag)==amountOfTags.end())//this tag not exists
                amountOfTags[tag]=1;
            else
                amountOfTags[tag]++;
        }
    }

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
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    std::string popTag=mostPopTag();
    for(auto& content: s.getContent()){
        //if this content content this tag and this content doesnt exists
        //in the history->return this content
    }
}
Watchable* GenreRecommenderUser::getRecommendation(Watchable &w) {}
Watchable* GenreRecommenderUser::getRecommendation(Movie &s) {}
Watchable* GenreRecommenderUser::getRecommendation(Episode &s) {}
User* GenreRecommenderUser::clone() {
    User* newUser=new GenreRecommenderUser(getName());
    return newUser;
}

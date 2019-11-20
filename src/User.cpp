//
// Created by amir on 19/11/2019.
//

#include <User.h>
#include <string>
#include "Watchable.h"
#include "Session.h"


User::User(const std::string &name): name(name)
{}
std::string User::getName() const { return name;}
std::vector<Watchable*> User::get_history() const {return history;}


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

RerunRecommenderUser::RerunRecommenderUser(const std::string &name): User(name){}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {

}




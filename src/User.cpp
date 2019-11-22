//
// Created by amir on 19/11/2019.
//

#include <User.h>
#include <string>
#include <cstring>

//User
User::User(const std::string &name): name(name)
{}
std::string User::getName() const { return name; }
std::vector<Watchable*> User::get_history() const { return history; }
User* User::clone() {}

// Subclasses of User:

//LengthRecommenderUser
LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name){}
Watchable* LengthRecommenderUser::getRecommendation(Session &s)
{

}
User* LengthRecommenderUser::clone() {
    User* newUser=new LengthRecommenderUser(getName());
    return newUser;
}

//RerunRecommenderUser
RerunRecommenderUser::RerunRecommenderUser(const std::string &name):User(name), indexOfHistory(0){}
RerunRecommenderUser::RerunRecommenderUser(const std::string &name, int index): User(name), indexOfHistory(index){}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    //if(s.getContent().at((s.getIndexOfContent()+1)%s.getContent().size()) instanceof Movie)
        //do something
    indexOfHistory++;
    return history.at((indexOfHistory-1)%history.size());
}
User* RerunRecommenderUser::clone() {
    User* newUser=new RerunRecommenderUser(getName(),indexOfHistory);
    return newUser;
}

//GenreRecommenderUser
GenreRecommenderUser::GenreRecommenderUser(const std::string &name):User(name){}
GenreRecommenderUser::GenreRecommenderUser(const std::string &name, const std::unordered_map<std::string, int> tags):User(name),amountOfTags(tags) {}
std::string GenreRecommenderUser::mostPopTag() {
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
            // copying the contents of the string to char array
            std::strcpy(array1, popTag.c_str());
            char array2[tag.first.size()];
            // copying the contents of the string to char array
            std::strcpy(array2, tag.first.c_str());
            if(std::lexicographical_compare(array2,array2+ sizeof(array2),array1,array1+sizeof(array1))) {
                amount = tag.second;
                popTag = tag.first;
            }
        }
    }
    return popTag;
}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    //i have know idea how to check if the last content was a movie or an episode
    std::string popTag=mostPopTag();
    std::vector<Watchable*> contents=s.getContent();
    for(auto& content: contents){
        //if this content content this tag and this content doesnt exists
        //in the history->return this content
    }
}
User* GenreRecommenderUser::clone() {
    User* newUser=new GenreRecommenderUser(getName(),amountOfTags);
    return newUser;
}

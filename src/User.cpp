//
// Created by amir on 19/11/2019.
//

#include <User.h>
#include <string>

//User
User::User(const std::string &name): name(name)
{}
std::string User::getName() const { return name; }
std::vector<Watchable*> User::get_history() const { return history; }
User* User::clone() {}


// Subclasses of User:

LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name)
{}
Watchable* LengthRecommenderUser::getRecommendation(Session &s)
{

}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name):User(name)
{}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {

}

GenreRecommenderUser::GenreRecommenderUser(const std::string &name):User(name)
{}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {

}


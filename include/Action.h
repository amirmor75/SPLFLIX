#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include <vector>

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};

class BaseAction{
public:
	BaseAction();
    BaseAction(std::string errorMsg,ActionStatus status);
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
	std::string getErrorMsgPublic() const;
	virtual BaseAction* clone()=0;
	bool isNumber(const std::string& s);


protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    CreateUser();
    CreateUser(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser();
    ChangeActiveUser(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class DeleteUser : public BaseAction {
public:
    DeleteUser();
    DeleteUser(std::string errorMsg,ActionStatus status);
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser();
    DuplicateUser(std::string errorMsg,ActionStatus status);
    virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class PrintContentList : public BaseAction {
public:
    PrintContentList();
    PrintContentList(std::string errorMsg,ActionStatus status);
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
    PrintWatchHistory(std::string errorMsg,ActionStatus status);
    virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class Watch : public BaseAction {
public:
    Watch();
    Watch(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class Exit : public BaseAction {
public:
    Exit();
    Exit(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};
#endif

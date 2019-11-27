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
    virtual ~BaseAction()= default;
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
    virtual ~CreateUser();
    CreateUser();
    CreateUser(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class ChangeActiveUser : public BaseAction {
public:
    virtual ~ChangeActiveUser();
    ChangeActiveUser();
    ChangeActiveUser(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class DeleteUser : public BaseAction {
public:
    virtual ~DeleteUser();
    DeleteUser();
    DeleteUser(std::string errorMsg,ActionStatus status);
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class DuplicateUser : public BaseAction {
public:
    virtual ~DuplicateUser();
    DuplicateUser();
    DuplicateUser(std::string errorMsg,ActionStatus status);
    virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class PrintContentList : public BaseAction {
public:
    virtual ~PrintContentList();
    PrintContentList();
    PrintContentList(std::string errorMsg,ActionStatus status);
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    virtual ~PrintWatchHistory();
    PrintWatchHistory();
    PrintWatchHistory(std::string errorMsg,ActionStatus status);
    virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class Watch : public BaseAction {
public:
    virtual ~Watch();
    Watch();
    Watch(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class PrintActionsLog : public BaseAction {
public:
    virtual ~PrintActionsLog();
    PrintActionsLog();
    PrintActionsLog(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class Exit : public BaseAction {
public:
    virtual ~Exit();
    Exit();
    Exit(std::string errorMsg,ActionStatus status);
    virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};
#endif

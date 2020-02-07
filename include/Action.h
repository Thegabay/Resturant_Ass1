#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;
extern Restaurant* backup;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    void updateArgs(std::string arguments);
    virtual BaseAction* makeCopy();
    std::string getArgs() const;
    virtual ~BaseAction();
protected:
    void complete();
    void error(std::string errorMsg);
    void setStatus(ActionStatus st);
    std::string getErrorMsg() const;
    std::string args;
private:
    std::string errorMsg;
    ActionStatus status; //current status
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
private:
    const int tableId;
    const std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
private:
    const int srcTable;
    const int dstTable;
    const int id;
    bool errorCheak(Restaurant &restaurant, int src, int dst, int customerId);
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy();
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual BaseAction* makeCopy(); //copy's all of the current actions.
private:

};


#endif

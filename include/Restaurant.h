#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const;
    std::vector<Dish>& getMenu();
    std::vector<Table*> getTables();
    virtual ~Restaurant();
    Restaurant(const Restaurant &other);
    Restaurant(Restaurant &&other);
    Restaurant& operator=(const Restaurant &other);
    Restaurant& operator=(Restaurant &&other);
    bool getOpen();
    void setOpen(bool b);
private:
    bool open;
    std::vector<Table*> tables; //vector of all the tables.
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    void clear();
    void copy(const Restaurant &other);
    DishType getDishType(std::string s);
    Customer* theConstracorCaller (std::string name, int id,std::string type);
    int customerId;
};

#endif

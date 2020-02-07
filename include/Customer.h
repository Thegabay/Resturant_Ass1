#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* makeCopy();
    virtual std::vector<int> getOrder();
    virtual ~Customer();
private:
    const std::string name;
    const int id;

};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual Customer* makeCopy();
    virtual std::vector<int> getOrder();
private:
    std::vector<int> c_order; //vector of the customer's order.
    const std::string customerType;
    bool isOrdered; //if the order was already found
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual Customer* makeCopy();
    virtual std::vector<int> getOrder();
    void setIsOrderd (bool b);
private:
    std::vector<int> c_order;
    const std::string customerType;
    bool isOrdered;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual std::vector<int> getOrder();
    virtual Customer* makeCopy();
private:
    std::vector<int> c_order; //vector of all of the orders
    std::vector<int> currOrder; //vector of the current customer's order.
    bool isOrdered;
    const std::string customerType;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual std::vector<int> getOrder();
    void sortVec (std::vector<std::pair<int,int>> &vec);
    virtual Customer* makeCopy();
    void setOrderIndex(int n);
private:
    std::vector<int> c_order;
    std::vector<int> currOrder;
    int orderIndex; //the number of the current order.
    const std::string customerType;
};


#endif

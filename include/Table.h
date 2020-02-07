#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    bool isOpenConst() const;
    void removeOrders(int id);
    void addOrders (OrderPair pair);
    virtual ~Table();
    Table(const Table &other);
    Table(Table &&other);
    Table& operator=(const Table &other);
    Table& operator=(Table &&other);
    void clear();
  private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList;
    void copy (const Table &other);
};


#endif

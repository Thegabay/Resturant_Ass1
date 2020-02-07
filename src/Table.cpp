#include "../include/Table.h"
#include <iostream>
#include <vector>

//class Table


    //Implement Rule of 5

    //copy constractor
    Table::Table(const Table &other)  {copy(other);}


    //move copy constractor
    Table::Table(Table &&other){
        capacity = other.getCapacity();
        open = other.isOpenConst();
        customersList = other.customersList;
        for (const auto &i : other.orderList)
            orderList.push_back(i);
    }

    //copy assignment operator
    Table& Table:: operator=(const Table &other){
        if(this!= &other){
            clear();
            copy(other);
        }
        return *this;
    }

    //move copy operator
    Table& Table::operator=(Table &&other){
        clear();
        capacity = other.getCapacity();
        open = other.isOpenConst();
        customersList = other.customersList;
        for (const auto &i : other.orderList)
            orderList.push_back(i);
        return *this;
    }

    //distractor
    Table::~Table() {clear();}

    //Auxiliary function that clear all the content of a table
    void Table::clear(){
        capacity = 0;
        closeTable();
        for (auto &i : customersList)
            delete i;
        customersList.clear();
        orderList.clear();
    }

    //Auxiliary function that copys all the content of a table
    void Table::copy(const Table &other) {
        capacity=other.getCapacity();
        open=other.isOpenConst();
        for (auto i : other.customersList)
            customersList.push_back(i->makeCopy());
        for (const auto &i : other.orderList)
            orderList.push_back(i);
    }

    //constractor
    Table :: Table(int t_capacity): capacity(t_capacity), open(false),customersList() ,orderList(){}


    int Table::getCapacity() const {return capacity;}

    void Table:: addCustomer(Customer* customer){
        customersList.push_back(customer);
    }


    void Table::removeCustomer(int id){
        int listSize = static_cast<int>(customersList.size());
        for(int i=0; i < listSize; i++)
            if (customersList[i]->getId() == id){
                customersList.erase(customersList.begin()+i);
            }
    }


    Customer*Table:: getCustomer(int id){
        for (auto &i : customersList)
            if (i->getId() == id)
                return i;

        return nullptr;
    }

    std::vector<Customer*>& Table:: getCustomers() {return customersList;}

    std::vector<OrderPair>& Table:: getOrders() {return orderList;}

    void Table::openTable() {open = true;}

    void Table::closeTable() {
    open = false;
    for (auto &i : customersList)
        delete i;
    customersList.clear();
    orderList.clear();
    }

    bool Table:: isOpen() {return open;}

    bool Table:: isOpenConst() const {return open;}

    //prints all of the orders of a sertin table using each customer order method.
   void Table::order(const std::vector<Dish> &menu) {
       std::vector<int> order_vector;
       for (auto &i : customersList) {
           i->order(menu);
           int orderSize = static_cast<int>(i->getOrder().size());
           for(int j=0; j< orderSize;j++){
               order_vector.push_back(i->getOrder()[j]);
               OrderPair p = std::make_pair(i->getId(),menu[order_vector.back()]);
               orderList.emplace_back(p);
               std::cout << i->getName() + " ordered " + p.second.getName() + "\n";
           }

       }
   }

    int Table::getBill() {
        int sum = 0;
        for (auto &i : orderList)
            sum += i.second.getPrice();
        return sum;

    }

    //adding customer order(only one) to the table he moved in to. used in MoveCustomer
    void Table::addOrders(OrderPair pair) {orderList.push_back(pair); }

    //delets of the customer's orders that is located by it's id using in MoveCustomer action.
    void Table::removeOrders(int id) {
        std::vector<OrderPair> temp;
        for (auto &i : orderList)
            if(i.first != id)
                temp.push_back(i);
        orderList.clear();
        for (const auto &i : temp)
            orderList.push_back(i);
    }




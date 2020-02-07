#include "../include/Action.h"
#include <iostream>
#include "../include/Restaurant.h"

//BaseAction class

//constractor
BaseAction::BaseAction() : errorMsg(""),status(PENDING){};

void BaseAction::act(Restaurant &restaurant) {}

ActionStatus BaseAction::getStatus() const {return status;}

void BaseAction::complete() {status=COMPLETED;}

void BaseAction::error(std::string errorMsg) {
    this->errorMsg=errorMsg;
    status=ERROR;
}

void BaseAction::setStatus(ActionStatus st) {this->status=st;}

std::string BaseAction::getErrorMsg() const {return errorMsg;}

std::string BaseAction::getArgs() const {return args;}

void BaseAction::updateArgs(std::string arguments){
    this->args=arguments;
}
BaseAction::~BaseAction()=default;

BaseAction* BaseAction::makeCopy() {return nullptr;}



//OpenTable class

//constractor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : BaseAction(),tableId(id),customers(customersList){}

//opens a table or prints an error about the table according to instructions (the first if cheaks all the cases that can throw an error)
void OpenTable::act(Restaurant &restaurant) {
    if((restaurant.getTable(tableId))==nullptr || (restaurant.getTable(tableId))->isOpen() || restaurant.getTable(tableId)->getCapacity()< static_cast<int>(customers.size())+ static_cast<int>(restaurant.getTable(tableId)->getCustomers().size())) {
        error("Table does not exist or is already open");
        std::cout << "Error: " << getErrorMsg() << '\n';
    }
    else{ //if there is no error than opens table.
        restaurant.getTable(tableId)->openTable();
        for (auto customer : customers)
            restaurant.getTable(tableId)->addCustomer(customer);
        complete();
    }
}
//prints table status when using log action.
std::string OpenTable:: toString() const {
    std::string s = "open " + std::to_string(tableId) + " ";
    if (getStatus() == ERROR) {
        s +=getArgs() + "Error: " + getErrorMsg();
    } else if (getStatus() == COMPLETED) {
        s += getArgs() + "Completed";
    }
    return s;
}

// makes a copy of a table and all of it's content.
BaseAction* OpenTable::makeCopy() {
    std::vector<Customer *> temp;
    for (auto customer : customers) {
        temp.push_back(customer);
    }
    std::vector<Customer *> &referance = temp; //copys all of the table's current actions and errors.
    OpenTable* action = new OpenTable(tableId,referance);
    action->updateArgs(this->getArgs());
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}

//class Order

//constractor
Order::Order(int id) : tableId(id){}

//make an order for a certain table if it possible
void Order::act(Restaurant &restaurant) {
    if((restaurant.getTable(tableId))==nullptr || !(restaurant.getTable(tableId))->isOpen() ) {
        error("Table does not exist or is not open");
        std::cout << "Error: " << getErrorMsg() << '\n';
    }
    else{
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    }

}

//print Order status when using log action
std::string Order:: toString() const {
    std::string s = "order " + std::to_string(tableId) + " ";
    if (getStatus() == ERROR) {
        s += "Error: " + getErrorMsg();
    } else if (getStatus() == COMPLETED) {
        s += "Completed";
    }
    return s;
}

// makes a copy of an order and all of it's content.
BaseAction* Order::makeCopy() {
    Order* action = new Order(tableId);
    action->updateArgs(this->getArgs());
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}


//MoveCustomer class

//constractor
MoveCustomer::MoveCustomer (int src, int dst, int customerId) : srcTable(src), dstTable(dst),id(customerId){}

//move customer from one table to another if possible and update them aswell
void MoveCustomer::act(Restaurant &restaurant) {
    if(!errorCheak(restaurant,srcTable,dstTable,id)){
        error("Cannot move customer");
        std::cout << "Error: " << getErrorMsg() << '\n';
    }
    else {
        restaurant.getTable(dstTable)->addCustomer(restaurant.getTable(srcTable)->getCustomer(id));
        restaurant.getTable(srcTable)->removeCustomer(id);
        std::vector<OrderPair> orderListCustomer;
        int tableOrderSize = static_cast<int>(restaurant.getTable(srcTable)->getOrders().size());
        for (int i = 0; i < tableOrderSize; i++) {
            if (restaurant.getTable(srcTable)->getOrders()[i].first == id) {
                restaurant.getTable(dstTable)->addOrders(restaurant.getTable(srcTable)->getOrders()[i]);
            }
         }
        restaurant.getTable(srcTable)->removeOrders(id);
         if(restaurant.getTable(srcTable)->getCustomers().size()==0)
             restaurant.getTable(srcTable)->closeTable();
        complete();
    }

}

//print MoveCustomer status when using log action
std::string MoveCustomer:: toString() const{
    std::string s = "move " ;
    if (getStatus() == ERROR) {
        s +=getArgs() + " Error: " + getErrorMsg();
    }
    else if(getStatus() == COMPLETED){
        s+=getArgs() + " Completed";
    }
    return s;
}

//returns true if all possile errors than can occour by moving a cutomer is false,used in act method
bool MoveCustomer::errorCheak(Restaurant &restaurant, int src, int dst, int customerId) {
    return (restaurant.getTable(src)!= nullptr && restaurant.getTable(dst)!= nullptr &&  restaurant.getTable(dst)->isOpen() && restaurant.getTable(src)->isOpen() && restaurant.getTable(src)->getCustomer(id)!=
                                                                                      nullptr && restaurant.getTable(dst)->getCapacity()!=
                                                                                                 static_cast<int>(restaurant.getTable(dst)->getCustomers().size()));
}

// makes a copy of an MoveCustomer and all of it's content.
BaseAction* MoveCustomer::makeCopy() {
    MoveCustomer* action = new MoveCustomer(srcTable,dstTable,id);
    action->updateArgs(this->getArgs());
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}


//Close class

//constractor
Close::Close(int id): tableId(id){}

//closes table if possible and prints the bill.
void Close::act(Restaurant &restaurant) {
    if (restaurant.getTable(tableId) == nullptr || !restaurant.getTable(tableId)->isOpen()) {
        error("Table does not exist or is not open");
        std::cout << "Error: " << getErrorMsg() << '\n';

    } else {
        std::string bill = std::to_string(restaurant.getTable(tableId)->getBill());
        restaurant.getTable(tableId)->closeTable();
        std::cout << "Table " + std::to_string(tableId) + " was closed. Bill " + bill + "NIS" +"\n";
        complete();
    }
}

//print Close status when using log action
std::string Close:: toString() const{
        std::string s = "close " ;
        if (getStatus() == ERROR) {
            s +=getArgs() + " Error: " + getErrorMsg();
        }
        else if(getStatus() == COMPLETED){
            s+=getArgs() + " Completed";
        }
        return s;
}

// makes a copy of an Close and all of it's content.
BaseAction* Close::makeCopy() {
    Close* action = new Close(tableId);
    action->updateArgs(this->getArgs());
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}

//CloseAll class

//constractor
CloseAll::CloseAll() {}

//closes all the open tables at once using Close and close the restaurant
void CloseAll::act(Restaurant &restaurant) {
    int tablesSize = static_cast<int>(restaurant.getTables().size());
    for (int i=0; i < tablesSize; i++) {
        if (restaurant.getTables()[i]->isOpen()) {
            Close *c = new Close(i);
            c->act(restaurant);
            delete c; //clears the space of c in memory, that we allocate temporary to close each table.
        }
    }
    restaurant.setOpen(false);
    complete();
}

//print CloseAll status when using log action
std::string CloseAll:: toString() const{
    std::string s = "closeAll " + getArgs() + " Completed";
    return s;
}

// makes a copy of an CloseAll and all of it's content.
BaseAction* CloseAll::makeCopy() {
    CloseAll* action = new CloseAll();
    action->getArgs()=getArgs();
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}


//PrintMenu class

//constractor
PrintMenu::PrintMenu(){}

//prints the restaurant menu
void PrintMenu:: act(Restaurant &restaurant){
    for (auto &i : restaurant.getMenu())
        std::cout << i.toString() << "\n" ;
    complete();
}

//print Printmenu status when using log action
std::string PrintMenu:: toString() const{
    std::string s = "menu" + getArgs() + " Completed";
    return s;
}

// makes a copy of an PrintMenu and all of it's content.
BaseAction* PrintMenu::makeCopy() {
    PrintMenu* action = new PrintMenu();
    action->getArgs()=getArgs();
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}

//PrintTableStatus

//constractor
PrintTableStatus::PrintTableStatus(int id): tableId(id){}

//prints all the table's current status:customers,orders and overall bill if open or close if table is close.
void PrintTableStatus:: act(Restaurant &restaurant){
    if (!restaurant.getTable(tableId)->isOpen())
        std::cout << "Table " << std::to_string(tableId) << " status: closed" << "\n";
    else{
        std::cout << "Table " << std::to_string(tableId) << " status: open" << "\n" << "Customers:" << "\n";
        for (auto &i : restaurant.getTable(tableId)->getCustomers())
            std::cout << i->getId() << " " << i->getName() << "\n";
        std::cout <<  "Orders:" << "\n";
        for (auto &i : restaurant.getTable(tableId)->getOrders())
            std::cout << i.second.getName() << " " << i.second.getPrice() << "NIS " << i.first << "\n";
        std::cout << "Current Bill: " << restaurant.getTable(tableId)->getBill() << "NIS" << "\n";
    }
    complete();
}

//print PrintTableStatus status when using log action
std::string PrintTableStatus:: toString() const{
    std::string s = "status " + std::to_string(tableId) + " Completed";
    return s;
}

// makes a copy of an PrintTableStatus and all of it's content.
BaseAction* PrintTableStatus::makeCopy() {
    PrintTableStatus* action = new PrintTableStatus(tableId);
    action->getArgs()=getArgs();
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}


//PrintActionLog class

//constractor
PrintActionsLog::PrintActionsLog(){}

//prints status for all the action that happened using there toString method
void PrintActionsLog::act(Restaurant &restaurant) {
    for (auto i : restaurant.getActionsLog())
        std::cout << i->toString() << "\n";
    complete();
}

//print PrintActionsLog status when using log action
std::string PrintActionsLog:: toString() const{
    std::string s = "log" + getArgs() + " Completed";
    return s;
}

// makes a copy of an PrintActionsLog and all of it's content.
BaseAction* PrintActionsLog::makeCopy() {
    PrintActionsLog* action = new PrintActionsLog();
    action->getArgs()=getArgs();
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}

//BackupRestaurant class

//constractor
BackupRestaurant::BackupRestaurant(){}

//allocate a new restaurant object in memory called backup that holds all the restaurant content at the moment.
//if a backup is exisst it update the content using restaurant copy assignment operator
void BackupRestaurant:: act(Restaurant &restaurant){
    if(backup== nullptr){
        backup = new Restaurant(restaurant);
        complete();
    }
    else{
        *backup=restaurant;
        complete();
    }
}

//print BackupRestaurant status when using log action
std::string BackupRestaurant:: toString() const{
    std::string s = "backup" + getArgs() + " Completed";
    return s;
}

// makes a copy of an BackupRestaurant and all of it's content.
BaseAction* BackupRestaurant::makeCopy() {
    BackupRestaurant* action = new BackupRestaurant();
    action->getArgs()=getArgs();
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}


//RestoreResturant class

//constractor
RestoreResturant::RestoreResturant() {}

//restore the restaurant content from backup if exists
void RestoreResturant:: act(Restaurant &restaurant){
    if(backup== nullptr){
        error("No backup available");
        std::cout << "Error: " << getErrorMsg() << '\n';
    }
    else{
        restaurant = *backup;
        complete();
    }
}

//print RestoreResturant status when using log action
std::string RestoreResturant:: toString() const{
    std::string s = "restore " ;
    if (getStatus() == ERROR) {
        s +=getArgs() + "Error: " + getErrorMsg();
    }
    else if(getStatus() == COMPLETED){
        s+=getArgs() + "Completed";
    }
    return s;
}

// makes a copy of an RestoreResturant and all of it's content.
BaseAction* RestoreResturant::makeCopy() {
    RestoreResturant* action = new RestoreResturant();
    action->getArgs()=this->getArgs();
    action->error(getErrorMsg());
    action->setStatus(getStatus());
    return action;
}


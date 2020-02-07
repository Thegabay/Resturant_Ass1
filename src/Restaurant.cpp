#include "../include/Restaurant.h"
#include <iostream>
#include <fstream>



//class Restaurant

//Implement Rule of 5

//copy constractor
Restaurant::Restaurant(const Restaurant &other){copy(other);}

//move copy constractor
Restaurant::Restaurant(Restaurant &&other) {
    open = other.getOpen();
    tables = other.getTables();
    for (const auto &i : other.getMenu())
        menu.push_back(i);
    actionsLog = other.getActionsLog();

}

//copy assignment operator
Restaurant& Restaurant::operator=(const Restaurant &other) {
    if(this!= &other){
        this->clear();
        this->copy(other);
    }
    return *this;
}

//move assignment operator
Restaurant& Restaurant::operator=(Restaurant &&other) {
    clear();
    open = other.getOpen();
    tables = other.getTables();
    for (const auto &i : other.getMenu())
        menu.push_back(i);
    actionsLog = other.getActionsLog();
    return *this;
}

//distractor
Restaurant::~Restaurant() {clear();}

//Auxiliary function that clear all the content of a restaurant
void Restaurant::clear() {
    open=false;
    for (auto &table : tables)
        delete table;
    tables.clear();
    menu.clear();
    for (auto i : actionsLog)
        delete i;
    actionsLog.clear();
}

//Auxiliary function that copys all the content of a restaurant
void Restaurant::copy(const Restaurant &other) {
    open=other.open;
    for (auto table : other.tables)
        tables.emplace_back(new Table(*table));
    for (const auto &i : other.menu)
        menu.push_back(i);
    for (auto i : other.actionsLog)
        actionsLog.push_back(i->makeCopy());
}

//empty constractor
Restaurant::Restaurant() {}

//constractor
Restaurant::Restaurant(const std::string &configFilePath) : customerId(0) {
    std::ifstream input;
    int counter =0;
    input.open(configFilePath);
    if(!input.is_open())
        std::cout << "exit" << std::endl;
    std::string line;
    while(!input.eof()) {
        getline(input,line);//reads data from the config file if open
        if(line!="") {//skip empty lines
            if (line.substr(0, 2) == "#n" || line.substr(0, 2) == "#N")
                counter = 1;
            else if (counter == 1) {
                counter = 2;
            }
            if (line.substr(0, 2) == "#T" || line.substr(0, 2) == "#t")
                counter = 3;
            else if (counter == 3) {
                std::string s;
                while (!line.empty()) {//allocate tables according to the input file
                    if (line[0] != ',') {
                        s += line[0];
                        line.erase(line.begin() + 0);
                    } else {
                        tables.push_back(new Table(std::stoi(s)));
                        line.erase(line.begin() + 0);
                        s.clear();
                    }
                }
                tables.push_back(new Table(std::stoi(s)));
                counter = 4;
            }
            if (line.substr(0, 2) == "#M" || line.substr(0, 2) == "#m")
                counter = 5;
            else if (counter == 5 && line != "") {
                std::vector<std::string> v;
                std::string s = "";
                for (char i : line) {//brake line that include dish info into words
                    if (i != ',')
                        s += i;
                    else {
                        v.push_back(s);
                        s = "";
                    }
                }
                v.push_back(s);
                Dish d = Dish(static_cast<int>(menu.size()), v[0], std::stoi(v[2]), getDishType(v[1]));//allocate dish according to the input file
                menu.push_back(d);
            }
        }
        }
        input.close();

    }





void Restaurant::start() { //opens the restaurant.
    open = true;
    std::cout << "Restaurant is now open!" << "\n";
    std::string action;
    int customerId = 0;
    std::vector<std::string> tokens; //hold the words from line
    while(action !="closeall"){//as long as we didn't type "closeall" the restaurant is operate.
        getline(std::cin,action);
        if(action != "closeall") {
            int found, found1 = 0;
            while (action != "\0") {//takes the line we get from "get line" file and separate it to words without spaces or comma's and save each word in tokens vector.
                found = action.find(" ");
                found1 = action.find(",");
                if (found == -1) { found = INT32_MAX; }
                if (found1 == -1) { found1 = INT32_MAX; }
                std::string temp;
                for (int f = 0; f < std::min(found, found1); f++) {
                    if (found == INT32_MAX && found1 == INT32_MAX) {
                        temp = action;
                        action.clear();
                        break;
                    }
                    temp += action[f];
                }
                tokens.push_back(temp);
                action.erase(0, std::min(found, found1) + 1);
            }
        }
        else{
            tokens.push_back(action);
        }

        //first word in tokens is the action,after recognizing the word, know what action to call.
        if(tokens[0] == "open"){
           std::vector<Customer *> customersList;
            std::string name;
            std::string s;
            std::string &args = s;
            int tokenSize = static_cast<int>(tokens.size());
            for(int i=2; i < tokenSize; i++) {
                if (i % 2 == 0){
                    name = tokens[i];
                    args+=tokens[i] + ",";
                }

                else if (i % 2 == 1){
                    customersList.emplace_back(theConstracorCaller(name, customerId, tokens[i]));
                    customerId++;
                    args+=tokens[i]+ " ";
                }

            }
            OpenTable* a = new OpenTable(std::stoi(tokens[1]),customersList);
            a->updateArgs(args);
            a->act(*this);
            actionsLog.emplace_back(a);
            if(a->getStatus()==ERROR){//if we can't open the table delete the customers from memory
                for(auto Customer : customersList)
                    delete Customer;
            }


        }

        if(tokens[0] == "order"){
            if(!menu.empty()) {
                Order *a = new Order(std::stoi(tokens[1]));
                a->act(*this);
                actionsLog.emplace_back(a);
            }
        }

        if(tokens[0] == "move"){
            MoveCustomer* a = new MoveCustomer(std::stoi(tokens[1]), std::stoi(tokens[2]),std::stoi(tokens[3]));
            std::string s = tokens[1] + " " + tokens[2] + " " + tokens[3];
            a->updateArgs(s);
            a->act(*this);
            actionsLog.emplace_back(a);
        }

        if(tokens[0] == "close"){
            Close* a = new Close(std::stoi(tokens[1]));
            a->updateArgs(tokens[1]);
            a->act(*this);
            actionsLog.emplace_back(a);
        }

        if(tokens[0] == "menu"){
            PrintMenu* a = new PrintMenu();
            a->act(*this);
            actionsLog.emplace_back(a);
        }

        if(tokens[0] == "status"){
            PrintTableStatus* a = new PrintTableStatus(std::stoi(tokens[1]));
            a->updateArgs(tokens[1]);
            a->act(*this);
            actionsLog.emplace_back(a);
        }

        if(tokens[0] == "log"){
            PrintActionsLog* a = new PrintActionsLog();
            a->act(*this);
            actionsLog.emplace_back(a);
        }

        if(tokens[0] == "backup"){
            BackupRestaurant* a = new BackupRestaurant();
            a->act(*this);
            actionsLog.emplace_back(a);
        }

        if(tokens[0] == "restore"){
            RestoreResturant* a = new RestoreResturant();
            a->act(*this);
            actionsLog.emplace_back(a);
        }
        tokens.clear();
    }
    if(tokens[0] == "closeall"){
        CloseAll* a = new CloseAll();
        actionsLog.emplace_back(a);
        a->act(*this);

    }
}


//calles the constructor for each customer by it's type.
Customer* Restaurant::theConstracorCaller(std::string name, int id, std::string type) {
    if(type == "veg")
        return(new VegetarianCustomer(name,id));
    else if(type == "chp")
        return(new CheapCustomer(name,id));
    else if(type == "spc")
        return(new SpicyCustomer(name,id));
    else return(new AlchoholicCustomer(name,id));
}

DishType Restaurant::getDishType(std::string s) {
    if(s == "VEG")
        return VEG;
    else if (s == "BVG")
        return BVG;
    else if(s == "ALC")
        return ALC;
    else
        return SPC;
}

int Restaurant::getNumOfTables() const {return tables.size();}

Table* Restaurant::getTable(int ind) {return tables[ind];}

const std::vector<BaseAction*>& Restaurant::getActionsLog() const {return actionsLog;}

std::vector<Dish>& Restaurant::getMenu() {return menu;}

std::vector<Table*> Restaurant::getTables() {return tables;}

bool Restaurant::getOpen() {return open;}

void Restaurant::setOpen(bool b) {open = b;}
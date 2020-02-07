#include "../include/Customer.h"
#include <iostream>
#include <vector>

//class Customer
//constractor
    Customer::Customer(std::string c_name, int c_id): name(c_name), id(c_id){}

    std::string Customer:: getName() const {return name;}

    int Customer::getId() const {return id;}

    std::vector<int> Customer::order(const std::vector<Dish> &menu) {
        std::vector<int> v;
        return v;
    }

    std::string Customer::toString() const {return "";}

    Customer* Customer::makeCopy() {return nullptr;}

    std::vector<int> Customer::getOrder() {
        std::vector<int> v;
    return v;
    }

    Customer::~Customer()=default;



    //class VegetarianCustomer
    //constractor
    VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id), customerType ("VEG"),isOrdered(false){}

    //return vector with the lowest id VEG meal and highest price BVG drink
    std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu){
        if(!isOrdered){ //if the order was found once, no need to look for it again. we will save it and return as a vector.
            int smallestIdVegDish = -1;
            int currentMostExpensiveBvrPrice=-1;
            int currentMostExpensiveBvrId = -1;
            for (const auto &i : menu) {//finds the  lowest id VEG meal and highest price BVG drink
                if(smallestIdVegDish == -1 && i.getType()==VEG)
                    smallestIdVegDish = i.getId();
                if(i.getType()==BVG && currentMostExpensiveBvrId == -1 ){// if its a BVG and still wasn't found.
                    currentMostExpensiveBvrId = i.getId();
                    currentMostExpensiveBvrPrice = i.getPrice();
                }
                //compare the current BVG that we chose and the next one in the menu.
                else if(i.getType()==BVG && (currentMostExpensiveBvrPrice < i.getPrice() || (currentMostExpensiveBvrPrice ==
                                                                                                  i.getPrice() && currentMostExpensiveBvrId > i.getId()))){

                    currentMostExpensiveBvrId = i.getId();
                    currentMostExpensiveBvrPrice = i.getPrice();
                }
            }
            c_order.push_back(smallestIdVegDish) ;
            c_order.push_back(currentMostExpensiveBvrId);
            isOrdered = true;
        }
        if(c_order[0] == -1 || c_order[1] == -1)//if the menu dosen't include VEG dish or BVG dish the customer won't order
            c_order.clear();
        return c_order;
    }

    //print coustomer info
    std::string VegetarianCustomer:: toString() const {
       return "this is a Vegetarian Customer his name is " + getName() + "and his id is: " + std::to_string(getId());
    }

    // makes a copy of vegetarian customer and all of it's content.
    Customer* VegetarianCustomer:: makeCopy(){
        VegetarianCustomer* c = new VegetarianCustomer(getName(), getId());
        int orderSize = static_cast<int>(getOrder().size());
        for(int i=0; i < orderSize; i++)
            c->getOrder().push_back(getOrder()[i]); //copying all of it's orders.
        return c;
    }


    std::vector<int> VegetarianCustomer:: getOrder() {return c_order;}



    //class CheapCustomer

    //constractor
    CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id), customerType("CHP"), isOrdered(false){}

    //return vector with the cheapest dish in the menu
    std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu){
        if(!isOrdered){ //if not found yet, than search, otherwise, it kept and there is no need to look for it again.
            int currentLowPrice = -1;
            int currentId = -1;
            for (const auto &i : menu) {
                if(currentLowPrice == -1 ){//take the first dish in menu
                    currentLowPrice = i.getPrice();
                    currentId = i.getId();
                }
                //cheak if the dish we chose is cheaper then the other dishes in the menu, and chose the cheapest
                else if((i.getPrice() < currentLowPrice ||(i.getPrice() == currentLowPrice &&  currentId > i.getId()))){
                    currentLowPrice = i.getPrice();
                    currentId = i.getId();
                }
            }
            if(currentId != -1)
                c_order.push_back(currentId);
            isOrdered = true;
        }
        else{
            c_order.clear();//if the order is not complete (menu is empty) the customer won't order
        }
        return c_order;
}

    //print coustomer info
    std::string CheapCustomer:: toString() const {
        return "this is a Cheap Customer his name is " + getName() + "and his id is: " + std::to_string(getId());
    }


    // makes a copy of cheap customer and all of it's content.
    Customer* CheapCustomer::makeCopy(){
        CheapCustomer* c = new CheapCustomer(getName(), getId());
        int orderSize = static_cast<int>(getOrder().size());
        for(int i=0; i < orderSize; i++)
            c->getOrder().push_back(getOrder()[i]);
        c->setIsOrderd(isOrdered);
        return c;
    }

    std::vector<int> CheapCustomer:: getOrder() {return c_order;}

    void CheapCustomer::setIsOrderd(bool b) {isOrdered=b;}


//class SpicyCustomer

    //constractor
    SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id), isOrdered(false), customerType("SPC") {}

    //return vector with the most expensive spicy dish and the cheapest beverages in the menu
    std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu){
        int currentMostExpensiveSpcId = -1;
        int currentMostExpensiveSpcPrice=-1;
        int currentMostCheapBvrId = -1;
        int currentMostCheapBvrPrice=-1;
        if(!isOrdered){//if not found yet, than search, otherwise, it kept and there is no need to look for it again.
            for (const auto &i : menu) {
                if(currentMostExpensiveSpcId == -1 && i.getType()==SPC){//chose the first SPC Dish
                    currentMostExpensiveSpcId = i.getId();
                    currentMostExpensiveSpcPrice = i.getPrice();
                }
                //compere curent SPC Dish with the other SPC dish in the menu and chose the most expensive
                else if(i.getType()==SPC && (currentMostExpensiveSpcPrice < i.getPrice() || (currentMostExpensiveSpcPrice == i.getPrice() && currentMostExpensiveSpcId > i.getId()))){
                    currentMostExpensiveSpcId = i.getId();
                    currentMostExpensiveSpcPrice = i.getPrice();
                }
                //chose the first BVG dish
                if(currentMostCheapBvrId == -1 && i.getType() == BVG){
                    currentMostCheapBvrId = i.getId();
                    currentMostCheapBvrPrice = i.getPrice();
                }
                //compere curent BVG Dish with the other BVG dish in the menu and chose the most cheap one
                else if(i.getType() == BVG && (currentMostCheapBvrPrice > i.getPrice() || (currentMostCheapBvrPrice == i.getPrice() && currentMostCheapBvrId > i.getId()))){
                    currentMostCheapBvrId = i.getId();
                    currentMostCheapBvrPrice = i.getPrice();
                }
            }
            c_order.push_back(currentMostExpensiveSpcId);
            c_order.push_back(currentMostCheapBvrId);
            if(currentMostCheapBvrId == -1 || currentMostExpensiveSpcId == -1)//if the menu dosen't include SPC dish or BVG dish the customer won't order
                return currOrder;
            else{
                currOrder.push_back(currentMostExpensiveSpcId);//first order will be only the SPC dish
                isOrdered = true;
                return currOrder;
            }
        }
        if(currOrder.size()==0)//if the customer didn't order the first time he won't order at all.
            return currOrder;
        else {//if ordered once (the SPC dish) he will order the BVG dish
            currOrder.pop_back();
            currOrder.push_back(c_order[1]);
            return currOrder;
        }
    }

    std::vector<int> SpicyCustomer:: getOrder() {return currOrder;}


    //print coustomer info
    std::string SpicyCustomer:: toString() const {
        return "this is a Spicy Customer his name is " + getName() + "and his id is: " +std::to_string(getId());
    }

    // makes a copy of spicy customer and all of it's content.
    Customer* SpicyCustomer:: makeCopy(){
        SpicyCustomer* c = new SpicyCustomer(getName(), getId());
        int orderSize = static_cast<int>(getOrder().size());
        for(int i=0; i < orderSize; i++)
            c->getOrder().push_back(getOrder()[i]);
        return c;
    }



//class AlchoholicCustomer

    //constractor
    AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(name,id), orderIndex(0), customerType("ALC"){}

    //return vector with all the alcoholic beverage from the cheapest to the most expensive
    std::vector<int> AlchoholicCustomer:: order(const std::vector<Dish> &menu){
        std::vector<std::pair<int,int>> v;
        if(orderIndex==0){
            for (const auto &i : menu)
                if(i.getType() == ALC)
                    v.push_back(std::make_pair(i.getId(), i.getPrice()));
            sortVec (v); //sends the alcoholic bvg vector to be sorted from cheap to most expensive.
            for (auto &i : v)
                c_order.push_back(i.first);
            v.clear();
            currOrder.push_back(c_order[orderIndex]);
            orderIndex++;
            return currOrder;
        } // if have'nt ordered all of the drinks, keep on ordering the next more expensive one.
        else if (orderIndex < static_cast<int>(c_order.size())){
            currOrder.pop_back();
            currOrder.push_back(c_order[orderIndex]);
            orderIndex++;
            return currOrder;
        }
        else{
            currOrder.clear();
            return currOrder;
        }

    }



    //gets a vector of alcoholic drinks the sort them from cheapest to most expensive by insertion sort.
    void AlchoholicCustomer:: sortVec (std::vector<std::pair<int,int>> &vec) {
        int i,j;
        std::pair<int,int> temp;
        int vecSize = static_cast<int>(vec.size());
        for(i=1; i < vecSize ;i++){
            j=i-1;
            temp = vec[i];
            while(j>=0 && (temp.second<vec[j].second || (temp.second==vec[j].second && temp.first < vec[j].first))){
                //comperes prices first, if price is the same, than comperes ids.
                    vec[j+1] = vec[j];
                    j--;
                }
                vec[j+1] = temp;
            }
        }

        //print coustomer info
        std::string AlchoholicCustomer::toString() const {
            return  "this is a Alchoholic Customer his name is " + getName() + "and his id is: " + std::to_string(getId());

        }

    // makes a copy of Alcoholic customer and all of it's content.
    Customer* AlchoholicCustomer:: makeCopy(){
        AlchoholicCustomer* c = new AlchoholicCustomer(this->getName(), this->getId() );
        for(int i=0; i < static_cast<int>(getOrder().size()); i++)
            c->getOrder().push_back(getOrder()[i]);
        c->setOrderIndex(orderIndex);
        return c;
    }

    void AlchoholicCustomer::setOrderIndex(int n) {orderIndex=n;}

    std::vector<int> AlchoholicCustomer::getOrder() {return currOrder;}


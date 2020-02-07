#include "../include/Dish.h"
#include <iostream>



    Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type): id(d_id), name(d_name),price(d_price),type(d_type){}

    int Dish::getId() const {return id;}

    int Dish:: getPrice() const{return price;}

    DishType Dish:: getType() const {return type;}

    //get dish type and return it by string for printing
    std::string Dish:: DishTypeToString (DishType t){
        if(t==VEG) return "VEG";
        if(t==BVG) return "BVG";
        if(t==ALC) return "ALC";
        return "SPC";

    }

    //print the dish info used in PrintMenu action
    std::string Dish:: toString() {
        return name + " " + DishTypeToString(type) + " " + std::to_string(price) + "NIS";
    }

    std::string Dish::getName() const {return name;}


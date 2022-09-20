#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "Bag.h"
#include "Continuity.h"
#include "NewArmy.h"
#include "AppleMonarchy.h"

int main(int argc, char* argv[]) {
    std::cout << "Application Name: " << argv[0] << std::endl;
    std::cout << "Reading JSON file: " << argv[1] << std::endl;

    // let's create a vector to hold our class objects
    // we will be using the base abstract class to show polymorphism
    std::vector<Bag*> acmeBagVector;

    // let's setup a pointer to base abstract class
    Bag* acmeBaseObj = nullptr;

    // Use filestream to open a JSON native file which was supplied as arguments
    std::fstream jsonFile;
    jsonFile.open(argv[1], std::ios::in);
    // parse the filestream as JSON using nlohmann library
    nlohmann::json jsonData = nlohmann::json::parse(jsonFile);

    // let's iterate over jsonData using an iterator container and load into the vector - acmeBagVector
    for(nlohmann::json::iterator it = jsonData.begin(); it != jsonData.end(); ++it){
        std::string brandSelection = it->at("BRAND");
        if(brandSelection == "CONTINUITY") {
            try {
                acmeBaseObj = new Continuity(*it);
                acmeBagVector.push_back(acmeBaseObj);
                //acmeBaseObj->display();
            }
            catch (std::runtime_error &runtime_error) {
                std::cout << "Exception occurred: " << runtime_error.what() << std::endl;
            }
        } else if(brandSelection == "NEW_ARMY"){
            try {
                acmeBaseObj = new NewArmy(*it);
                acmeBagVector.push_back(acmeBaseObj);
                //acmeBaseObj->display();
            }
            catch (std::runtime_error &runtime_error) {
                std::cout << "Exception occurred: " << runtime_error.what() << std::endl;
            }
        } else if(brandSelection == "APPLE_MONARCHY"){
            try {
                acmeBaseObj = new AppleMonarchy(*it);
                acmeBagVector.push_back(acmeBaseObj);
                //acmeBaseObj->display();
            }
            catch (std::runtime_error &runtime_error) {
                std::cout << "Exception occurred: " << runtime_error.what() << std::endl;
            }
        } else{
            // do nothing here!
        }
    }

    for(auto item:acmeBagVector){
        // Entry method to run the following -
        // 1) Calculate Sales Totals
        // 2) Prepare and create entry in CUSTOMER table if new/existing customers based on JSON inputs
        // 3) Update custId private date member if new customer
        // 4) Prepare and create entries for enterprise wide Sales table
        // 5) Prepare and create entry to transaction tables respective to each brand
        item->entryMethod();
    }

    return 0;
}

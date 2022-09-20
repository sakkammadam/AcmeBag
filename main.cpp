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

int main() {
    // let's create a vector to hold our class objects
    // we will be using the base abstract class to show polymorphism
    std::vector<Bag*> acmeBagVector;

    // let's setup a pointer to base abstract class
    Bag* acmeBaseObj = nullptr;

    // JSON native file
    std::fstream jsonFile;
    jsonFile.open("/home/sakkammadam/CLionProjects/cis_554/AcmeBag/sample_newarmy.json", std::ios::in);
    nlohmann::json jsonData = nlohmann::json::parse(jsonFile);
    //std::cout << std::setw(4) << jsonData << std::endl;
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
        }
    }

    for(auto item:acmeBagVector){
        // default customer ids...
        item->display();
        // after insertion and retrieval
        item->entryMethod();
        // updated customer ids...
        item->display();
    }

    // test
    return 0;
}

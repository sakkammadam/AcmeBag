#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "Bag.h"

int main() {
    // let's create a vector to hold our class objects
    // we will be using the base abstract class to show polymorphism
    std::vector<Bag*> acmeBagVector;

    // let's setup a pointer to base abstract class
    Bag* acmeBaseObj = nullptr;

    // JSON native file
    std::fstream jsonFile;
    jsonFile.open("/home/sakkammadam/CLionProjects/cis_554/AcmeBag/sample_mix_bad.json", std::ios::in);
    nlohmann::json jsonData = nlohmann::json::parse(jsonFile);
    std::cout << std::setw(4) << jsonData << std::endl;
    // let's iterate over jsonData using an iterator container
    for(nlohmann::json::iterator it = jsonData.begin(); it != jsonData.end(); ++it){
        try {
            acmeBaseObj = new Bag(*it);
            acmeBagVector.push_back(acmeBaseObj);
            acmeBaseObj->display();
        }
        catch(std::runtime_error &runtime_error) {
            std::cout << "Exception occurred: " << runtime_error.what() << std::endl;
        }
    }

    return 0;
}

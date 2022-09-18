#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "ShoppingBag.h"
#include "Bag.h"

int main() {
    // csv file parsing
    std::string fileName = "/home/sakkammadam/CLionProjects/cis_554/AcmeBag/sample.csv";
    // declare two vectors
    std::vector<std::vector<std::string>> fileContent;
    std::vector<std::string> fileRow;
    // declare strings to hold a csv line and individual csv words
    std::string line, word;
    // declare filestream to hold the file
    std::fstream file;
    // open the file
    file.open(fileName, std::ios::in);
    // if the file was opened successfully
    if(file.good()){
        // use the filestream and getline, read individual lines of the file
        while(getline(file, line)){
            // on reading every line, clear the fileRow vector
            fileRow.clear();
            // read each line as a stringstream object
            std::stringstream str(line);
            // using getline (again) to break down each line into words using delimiter (comma in this case)
            while(getline(str, word, '|')){
                // add each item to fileRow vector
                fileRow.push_back(word);
            }
            fileContent.push_back(fileRow);
        }
    } else {
        std::cout << "Could not open " << fileName << " file." << std::endl;
    }

    // let's create a vector to hold our class objects
    // we will be using the base abstract class to show polymorphism
    std::vector<ShoppingBag*> acmeVector;

    // let's setup a pointer to base abstract class
    ShoppingBag* baseObj = nullptr;

    // outputting the vectors onto console
    for(auto i=1; i < fileContent.size(); ++i){
        std::string purchase_ts = fileContent[i][0];
        std::string first_name = fileContent[i][1];
        std::string last_name = fileContent[i][2];
        std::string address = fileContent[i][3];
        std::string city = fileContent[i][4];
        std::string state = fileContent[i][5];
        std::string zip = fileContent[i][6];
        std::string base = fileContent[i][7];
        nlohmann::json purchase_details = nlohmann::json::parse(base);
        // std::cout << purchase_ts << '|' << first_name << '|' << last_name << '|' << address << '|' << city << '|' << state << '|' << zip << '|' << purchase_details << std::endl;
        baseObj = new ShoppingBag(purchase_ts,first_name,last_name,address,city,state,zip,purchase_details);
        acmeVector.push_back(baseObj);
        baseObj->display();
    }

    // let's create a vector to hold our class objects
    // we will be using the base abstract class to show polymorphism
    std::vector<Bag*> acmeBagVector;

    // let's setup a pointer to base abstract class
    Bag* acmeBaseObj = nullptr;

    // JSON native file
    std::fstream jsonFile;
    jsonFile.open("/home/sakkammadam/CLionProjects/cis_554/AcmeBag/sample.json", std::ios::in);
    nlohmann::json jsonData = nlohmann::json::parse(jsonFile);
    std::cout << std::setw(4) << jsonData << std::endl;
    // let's iterate over jsonData using an iterator container
    for(nlohmann::json::iterator it = jsonData.begin(); it != jsonData.end(); ++it){
        acmeBaseObj = new Bag(*it);
        acmeBagVector.push_back(acmeBaseObj);
        acmeBaseObj->display();
    }


    return 0;
}

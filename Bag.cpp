//
// Created by sakkammadam on 9/18/22.
//

#include "Bag.h"
#include <array>
#include <stdexcept>
#include <string>
#include "nlohmann/json.hpp"

// function to validate timestamp
int validate_json_timestamp(const std::string &timestampStr){
    // initialize empty variable of struct tm
    std::tm chkT{};
    // we will use sscanf to copy value of timestampStr to chk
    // sscanf returns the number of fields being copied
    //
    if(std::sscanf(
            timestampStr.data(),
            "%d-%d-%d %d:%d:%d",
            &chkT.tm_year,
            &chkT.tm_mon,
            &chkT.tm_mday,
            &chkT.tm_hour,
            &chkT.tm_min,
            &chkT.tm_sec
    ) != 6){
        throw std::runtime_error("Invalid timestamp: " + std::string(timestampStr));
    }
    // invalid checks for year
    if(chkT.tm_year > 9999){
        throw std::runtime_error("Invalid year: " + std::string(timestampStr) + std::string(" Year greater than 9999"));
    }
    // invalid month
    if(chkT.tm_mon < 1 || chkT.tm_mon > 12){
        throw std::runtime_error("Invalid month: " + std::string(timestampStr) + std::string(" Month is not valid!"));
    }

    // check if hour is valid
    if(chkT.tm_hour <0 || chkT.tm_hour > 23){
        throw std::runtime_error("Invalid hour: " + std::string(timestampStr) + std::string(" Hour is not valid!"));
    }

    // check if minute is valid
    if(chkT.tm_min <0 || chkT.tm_min > 59){
        throw std::runtime_error("Invalid minute: " + std::string(timestampStr) + std::string(" Minute is not valid!"));
    }

    // check if second is valid
    if(chkT.tm_sec <0 || chkT.tm_sec > 59){
        throw std::runtime_error("Invalid second: " + std::string(timestampStr) + std::string(" Second is not valid!"));
    }

    // check if leap year
    if(chkT.tm_mon == 2 && chkT.tm_mday == 29 &&
       (chkT.tm_year % 400 == 0 || (chkT.tm_year % 4 == 0 && chkT.tm_year % 100 != 0))
            ) {
        return 1;
    }

    // array dates associated with months
    static const std::array<int,13> daysPerMonth{
            0,31,28,31,30,31,30,31,31,30,31,30,31
    };
    // check if date is valid
    if(chkT.tm_mday < 0 || chkT.tm_mday > daysPerMonth[chkT.tm_mon]){
        throw std::runtime_error("Invalid date: " + std::string(timestampStr) + std::string(" Date is not valid!"));
    }
    // all edge cases covered!
    return 1;
}

// Default constructor
Bag::Bag():custPurchaseTime{" "},
    custFirstName{" "},
    custLastName{" "},
    custAddress{" "},
    custCity{" "},
    custState{" "},
    custZip{" "},
    custId{-999},
    custPurchases{nlohmann::json::parse(R"({"status": "default"})")},
    custTotal{0.00}
    {
    // Nothing in the body for default constructor
}

// Primary constructor
Bag::Bag(const nlohmann::json &purchase_json) {
    // let's validate the timestamp in the JSON
    try{
        if(purchase_json.contains("PURCHASE_TS")){
            validate_json_timestamp(purchase_json.at("PURCHASE_TS"));
        } else {
            throw std::runtime_error("PURCHASE_TS key not found: " + std::string(purchase_json));
        }
    }
    catch(std::runtime_error &runtime_error){
        std::cout << "Exception occurred - Bad Timestamp: " << runtime_error.what() << std::endl;
    }
    // if it looks good, lets assign the private data members
    // set purchase time
    setCustomerPurchaseTime(purchase_json);
    // set first name
    setCustomerFirstName(purchase_json);
    // set last name
    setCustomerLastName(purchase_json);
    // set address
    setCustomerAddress(purchase_json);
    // set city
    setCustomerCity(purchase_json);
    // set state
    setCustomerState(purchase_json);
    // set zip
    setCustomerZip(purchase_json);
    // set purchase details
    setCustomerPurchaseDetails(purchase_json);
    // set id
    setCustomerId(purchase_json);

}

// Setters
// This method will set the Customer's purchase time
void Bag::setCustomerPurchaseTime(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("PURCHASE_TS")){
        this->custPurchaseTime = purchase_json.at("PURCHASE_TS");
    } else {
        throw std::runtime_error("PURCHASE_TS key not found: " + std::string(purchase_json));
    }
}

// This method will set the Customer's first name
void Bag::setCustomerFirstName(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("FIRST_NAME")){
        this->custFirstName = purchase_json.at("FIRST_NAME");
    } else {
        this->custFirstName = " ";
    }
}

// This method will set the Customer's last name
void Bag::setCustomerLastName(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("LAST_NAME")){
        this->custLastName = purchase_json.at("LAST_NAME");
    } else {
        this->custLastName = " ";
    }
}

// This method will set the Customer's address
void Bag::setCustomerAddress(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("ADDRESS")){
        this->custAddress = purchase_json.at("ADDRESS");
    } else {
        this->custAddress = " ";
    }
}

// This method will set the Customer's city
void Bag::setCustomerCity(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("CITY")){
        this->custCity = purchase_json.at("CITY");
    } else {
        this->custCity = " ";
    }
}

// This method will set the Customer's state
void Bag::setCustomerState(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("STATE")){
        this->custState = purchase_json.at("STATE");
    } else {
        this->custState = " ";
    }
}

// This method will set the Customer's zip
void Bag::setCustomerZip(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("ZIP")){
        this->custZip = purchase_json.at("ZIP");
    } else {
        this->custZip = " ";
    }

}

// This method will set the Customer's Purchase details
void Bag::setCustomerPurchaseDetails(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("PURCHASE_DETAILS")){
        this->custPurchases = purchase_json.at("PURCHASE_DETAILS");
    } else {
        throw std::runtime_error("PURCHASE_DETAILS key not found: " + std::string(purchase_json));
    }
}

// This method will set the Customer id
void Bag::setCustomerId(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("CUSTOMER_ID")){
        this->custId = purchase_json.at("CUSTOMER_ID");
    } else {
        this->custId = -999;
    }
}

// Getters
// This method will return the customer's purchase time
std::string Bag::getCustomerPurchaseTime() {
    return this->custPurchaseTime;
}

// This method will return the customer's first name
std::string Bag::getCustomerFirstName() {
    return this->custFirstName;
}

// This method will return the customer's last name
std::string Bag::getCustomerLastName() {
    return this->custLastName;
}

// This method will return the customer's address
std::string Bag::getCustomerAddress() {
    return this->custAddress;
}

// This method will return the customer's city
std::string Bag::getCustomerCity() {
    return this->custCity;
}

// This method will return the customer's state
std::string Bag::getCustomerState() {
    return this->custState;
}

// This method will return the customer's zip
std::string Bag::getCustomerZip() {
    return this->custZip;
}

// This method will return the customer's purchases in JSON
nlohmann::json Bag::getCustomerPurchases() {
    return this->custPurchases;
}

// This method will return the customer id
int Bag::getCustomerId() {
    return this->custId;
}

// This method will calculate the total of customer's purchases
double Bag::getCustomerTotals() {
    // get all purchase details
    nlohmann::json purchaseDetails = getCustomerPurchases();
    // declare a sum variable
    double total = 0;
    // iterate over the purchase details
    for(nlohmann::json::iterator it = purchaseDetails.begin(); it != purchaseDetails.end(); ++it){
        total += static_cast<double>(it->at("UNIT_PRICE")) * static_cast<int>(it->at("UNITS"));
    }
    return total;
}

// Operational Methods
void Bag::display() {
    std::cout << getCustomerPurchaseTime()
    << '|' << getCustomerFirstName() << '|' << getCustomerLastName() << '|' << getCustomerAddress()
    << '|' << getCustomerCity() << '|' << getCustomerState() << '|' << getCustomerZip()
    << '|' << getCustomerTotals() << '|' << getCustomerPurchases() << std::endl;
}









//
// Created by sakkammadam on 9/17/22.
//

#include "ShoppingBag.h"
#include <array>
#include <stdexcept>
#include <string>
#include "nlohmann/json.hpp"


// function to validate timestamp
int validate_timestamp(const std::string &timestampStr){
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

// Constructors
// Initialization Constructor #1
// Usecase#1 - New Customer
ShoppingBag::ShoppingBag(
        const std::string &purchase_time,
        const std::string &first_name,
        const std::string &last_name,
        const std::string &address,
        const std::string &city,
        const std::string &state,
        const std::string &zip,
        const nlohmann::json &purchase_details){
    try{
        validate_timestamp(purchase_time);
    }
    catch(std::runtime_error &runtime_error){
        std::cout << "Exception occurred - Bad Timestamp: " << runtime_error.what() << std::endl;
    }
    // assign the private data members
    setCustomerPurchaseTime(purchase_time);
    setCustomerFirstName(first_name);
    setCustomerLastName(last_name);
    setCustomerAddress(address);
    setCustomerCity(city);
    setCustomerState(state);
    setCustomerZip(zip);
    setCustomerPurchaseDetails(purchase_details);
    // set these values to defaults!
    custId = -100;
    custTotal = 0;

}

// Initialization Constructor #2
// Usecase#2 - Existing Customer
ShoppingBag::ShoppingBag(
        const std::string &purchase_time,
        const int &customer_id,
        const nlohmann::json &purchase_details){
    try{
        validate_timestamp(purchase_time);
    }
    catch(std::runtime_error &runtime_error){
        std::cout << "Exception occurred - Bad Timestamp: " << runtime_error.what() << std::endl;
    }
    // assign the private data members
    setCustomerPurchaseTime(purchase_time);
    setCustomerPurchaseDetails(purchase_details);
    setCustomerId(customer_id);
    // set these values to defaults!
    custTotal = 0;

}


// Setters
// This method will set the Customer's purchase time
void ShoppingBag::setCustomerPurchaseTime(const std::string &purchase_time) {
    this->custPurchaseTime = purchase_time;
}

// This method will set the Customer's first name
void ShoppingBag::setCustomerFirstName(const std::string &first_name) {
    this->custFirstName = first_name;
}

// This method will set the Customer's last name
void ShoppingBag::setCustomerLastName(const std::string &last_name) {
    this->custLastName = last_name;
}

// This method will set the Customer's address
void ShoppingBag::setCustomerAddress(const std::string &address) {
    this->custAddress = address;
}

// This method will set the Customer's city
void ShoppingBag::setCustomerCity(const std::string &city) {
    this->custCity = city;
}

// This method will set the Customer's state
void ShoppingBag::setCustomerState(const std::string &state) {
    this->custState = state;
}

// This method will set the Customer's zip
void ShoppingBag::setCustomerZip(const std::string &zip) {
    this->custZip = zip;
}

// This method will set the Customer's Purchase details
void ShoppingBag::setCustomerPurchaseDetails(const nlohmann::json &purchase_details) {
    this->custPurchases = purchase_details;
}

// This method will set the Customer id
void ShoppingBag::setCustomerId(const int &customer_id) {
    this->custId = customer_id;
}


// Getters
// This method will return the customer's purchase time
std::string ShoppingBag::getCustomerPurchaseTime() {
    return this->custPurchaseTime;
}

// This method will return the customer's first name
std::string ShoppingBag::getCustomerFirstName() {
    return this->custFirstName;
}

// This method will return the customer's last name
std::string ShoppingBag::getCustomerLastName() {
    return this->custLastName;
}

// This method will return the customer's address
std::string ShoppingBag::getCustomerAddress() {
    return this->custAddress;
}

// This method will return the customer's city
std::string ShoppingBag::getCustomerCity() {
    return this->custCity;
}

// This method will return the customer's state
std::string ShoppingBag::getCustomerState() {
    return this->custState;
}

// This method will return the customer's zip
std::string ShoppingBag::getCustomerZip() {
    return this->custZip;
}

// This method will return the customer's purchases in JSON
nlohmann::json ShoppingBag::getCustomerPurchases() {
    return this->custPurchases;
}

// This method will return the customer id
int ShoppingBag::getCustomerId() {
    return this->custId;
}

// This method will calculate the total of customer's purchases
double ShoppingBag::getCustomerTotals() {
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

void ShoppingBag::display() {
    std::cout << getCustomerPurchaseTime()
    << '|' << getCustomerFirstName() << '|' << getCustomerLastName() << '|' << getCustomerAddress()
    << '|' << getCustomerCity() << '|' << getCustomerState() << '|' << getCustomerZip()
    << '|' << getCustomerTotals() << '|' << getCustomerPurchases() << std::endl;
 };


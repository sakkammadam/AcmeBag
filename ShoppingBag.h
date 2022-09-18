/*
 * Author: Sooraj Akkammadam
 * Details:
 *  This class file serves as the abstract base class for Acme's ShoppingBag implementation
 */

#ifndef ACMEBAG_SHOPPINGBAG_H
#define ACMEBAG_SHOPPINGBAG_H

#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

// Function to validate timestamps!
int validate_timestamp(const std::string &timestampStr);

class ShoppingBag {
public:
    // Default Constructor
    ShoppingBag();

    // Initialization Constructor #1
    // Usecase#1 - New Customer
    ShoppingBag(
            const std::string &purchase_time,
            const std::string &first_name,
            const std::string &last_name,
            const std::string &address,
            const std::string &city,
            const std::string &state,
            const std::string &zip,
            const nlohmann::json &purchase_details
    );

    // Initialization Constructor #2
    // Usecase#2 - Existing Customer
    ShoppingBag(
            const std::string &purchase_time,
            const int &customer_id,
            const nlohmann::json &purchase_details
            );

    // Setters
    // This method will set the Customer's purchase time
    void setCustomerPurchaseTime(const std::string &purchase_time);

    // This method will set the Customer's first name
    void setCustomerFirstName(const std::string &first_name);

    // This method will set the Customer's last name
    void setCustomerLastName(const std::string &last_name);

    // This method will set the Customer's address
    void setCustomerAddress(const std::string &address);

    // This method will set the Customer's city
    void setCustomerCity(const std::string &city);

    // This method will set the Customer's state
    void setCustomerState(const std::string &state);

    // This method will set the Customer's zip
    void setCustomerZip(const std::string &zip);

    // This method will set the Customer's Purchase details
    void setCustomerPurchaseDetails(const nlohmann::json &purchase_details);

    // This method will set the Customer id
    void setCustomerId(const int &customer_id);

    // Getters
    // This method will return the customer's purchase time
    std::string getCustomerPurchaseTime();

    // This method will return the customer's first name
    std::string getCustomerFirstName();

    // This method will return the customer's last name
    std::string getCustomerLastName();

    // This method will return the customer's address
    std::string getCustomerAddress();

    // This method will return the customer's city
    std::string getCustomerCity();

    // This method will return the customer's state
    std::string getCustomerState();

    // This method will return the customer's zip
    std::string getCustomerZip();

    // This method will return the customer's purchases in JSON
    nlohmann::json getCustomerPurchases();

    // This method will return the customer id
    int getCustomerId();

    // This method will calculate the total of customer's purchases
    double getCustomerTotals();

    // Operational Methods
    void display();


private:
    std::string custPurchaseTime;
    std::string custFirstName;
    std::string custLastName;
    std::string custAddress;
    std::string custCity;
    std::string custState;
    std::string custZip;
    int custId;
    nlohmann::json custPurchases;
    double custTotal;
};


#endif //ACMEBAG_SHOPPINGBAG_H

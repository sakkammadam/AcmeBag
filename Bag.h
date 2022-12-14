/*
 * Author: Sooraj Akkammadam
 * Details:
 *  This class file serves as the abstract base class for Acme's Bag implementation
 */

#ifndef ACMEBAG_BAG_H
#define ACMEBAG_BAG_H

#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

// Function to validate timestamps!
int validate_json_timestamp(const std::string &timestampStr);

// Function to read connection file
std::string read_connection();

class Bag {
    // Private data members!
private:
    std::string custBrand;
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

    // Public class functions
public:
    // Default Constructor
    Bag();

    // Initialization Constructor
    explicit Bag(const nlohmann::json &purchase_json);

    // Setters
    // This method will set the site brand the customer is on
    void setCustomerBrand(const nlohmann::json &purchase_json);

    // This method will set the Customer's purchase time
    void setCustomerPurchaseTime(const nlohmann::json &purchase_json);

    // This method will set the Customer's first name
    void setCustomerFirstName(const nlohmann::json &purchase_json);

    // This method will set the Customer's last name
    void setCustomerLastName(const nlohmann::json &purchase_json);

    // This method will set the Customer's address
    void setCustomerAddress(const nlohmann::json &purchase_json);

    // This method will set the Customer's city
    void setCustomerCity(const nlohmann::json &purchase_json);

    // This method will set the Customer's state
    void setCustomerState(const nlohmann::json &purchase_json);

    // This method will set the Customer's zip
    void setCustomerZip(const nlohmann::json &purchase_json);

    // This method will set the Customer's Purchase details
    void setCustomerPurchaseDetails(const nlohmann::json &purchase_json);

    // This method will set the Customer's Purchase totals
    virtual void setCustomerPurchaseTotals();

    // This method will set the Customer id
    void setCustomerId(const nlohmann::json &purchase_json);

    // Getters
    // This method will get the site brand the customer is on
    std::string getCustomerBrand();

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
    int getCustomerId() const;

    // This method will calculate the total of customer's purchases
    // Making it virtual so that different derived classes can return dedicated private cost data members
    virtual double getCustomerTotals() const;

    // Operational Methods
    // Simple display
    void display();

    // This method will leverage private data members and insert a record into CUSTOMER table in ACME schema
    void insertCustomerRecord();

    // This method will retrieve customerId from the database for new customers
    void retrieveCustomerIdDB();

    // This method will retrieve brandId from the database
    int retrieveBrandId();

    // This method will insert transaction totals from each brand into CUSTOMER_BRAND_TRANSACTIONS table in ACME schema
    // it will reference Customer id, their purchase timestamps and the total across their purchases for a brand
    void insertCustomerBrandSaleTotals();

    // This method will insert customer's transactions into brand specific tables within brand specific schemas
    // it will reference Customer id, their purchase timestamps and their purchases for a brand
    // We will make this a pure virtual method
    // It will receive its implementation from classes specific to brand
    virtual void insertCustomerTransactions() = 0;

    // Entry method!
    // This is being made virtual so that it's implementation can be overridden in derived class
    // And we want to do this at run-time through a pointer to the base class.
    virtual void entryMethod() final;

};


#endif //ACMEBAG_BAG_H

//
// Created by sakkammadam on 9/18/22.
//

#include "Bag.h"
#include <array>
#include <stdexcept>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "pqxx/pqxx"


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

std::string read_connection() {
    // declare strings
    std::string fileLine;
    std::string cnxFile = "/home/sakkammadam/CLionProjects/cis_554/AcmeBag/cnx.dat";
    // declare filestream to hold the file
    std::fstream fileRaw;
    fileRaw.open(cnxFile, std::ios::in);
    // use the filestream and getline, read individual lines of the file
    while (getline(fileRaw, fileLine)) {
        // read each line as a stringstream object
        std::stringstream str(fileLine);
    }
    return fileLine;
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
    custPurchases{nlohmann::json::parse(R"({"UNITS":0,"SKU":"DUMMY","UNIT_PRICE":0.00})")},
    custTotal{0.00}
    {
    // Nothing in the body for default constructor
}


// Primary constructor
Bag::Bag(const nlohmann::json &purchase_json) {
    // Set purchase time - will throw exception if key was not found in JSON
    setCustomerPurchaseTime(purchase_json);
    // Will throw exception if invalid timestamp
    validate_json_timestamp(getCustomerPurchaseTime());
    // Set brand - will throw exception if key was not found in JSON
    setCustomerBrand(purchase_json);
    // set first name - will throw exception if customerId key was not present and first name key was not found in JSON
    setCustomerFirstName(purchase_json);
    // set last name - will throw exception if customerId key was not present and last name key was not found in JSON
    setCustomerLastName(purchase_json);
    // set address - will throw exception if customerId key was not present and address key was not found in JSON
    setCustomerAddress(purchase_json);
    // set city - will throw exception if customerId key was not present and city key was not found in JSON
    setCustomerCity(purchase_json);
    // set state - will throw exception if customerId key was not present and state key was not found in JSON
    setCustomerState(purchase_json);
    // set zip - will throw exception if customerId key was not present and zip key was not found in JSON
    setCustomerZip(purchase_json);
    // set purchase details - will throw exception if purchase details key was not found in JSON
    setCustomerPurchaseDetails(purchase_json);
    // set id
    setCustomerId(purchase_json);
    // calculate purchase totals
    setCustomerPurchaseTotals();
}

// Setters
// This method will set the Customer's purchase time
void Bag::setCustomerBrand(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("BRAND")){
        this->custBrand = purchase_json.at("BRAND");
    } else {
        throw std::runtime_error("BRAND key not found: " + std::string(purchase_json.dump()));
    }
}


// This method will set the Customer's purchase time
void Bag::setCustomerPurchaseTime(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("PURCHASE_TS")){
        this->custPurchaseTime = purchase_json.at("PURCHASE_TS");
    } else {
        throw std::runtime_error("PURCHASE_TS key not found: " + std::string(purchase_json.dump()));
    }
}

// This method will set the Customer's first name
void Bag::setCustomerFirstName(const nlohmann::json &purchase_json) {
    // if json structure already has a customer id -> returning customer!
    if(purchase_json.contains("CUSTOMER_ID")){
        // assign default
        this->custFirstName = " ";
    } else {
        // now you should check if First Name was provided or not!
        if(purchase_json.contains("FIRST_NAME")){
            this->custFirstName = purchase_json.at("FIRST_NAME");
        } else {
            throw std::runtime_error("FIRST_NAME key not found: " + std::string(purchase_json.dump()));
        }
    }
}

// This method will set the Customer's last name
void Bag::setCustomerLastName(const nlohmann::json &purchase_json) {
    // if json structure already has a customer id -> returning customer!
    if(purchase_json.contains("CUSTOMER_ID")){
        // assign default
        this->custLastName = " ";
    } else {
        // now you should check if Last Name was provided or not!
        if(purchase_json.contains("LAST_NAME")){
            this->custLastName = purchase_json.at("LAST_NAME");
        } else {
            throw std::runtime_error("LAST_NAME key not found: " + std::string(purchase_json.dump()));
        }
    }
}

// This method will set the Customer's address
void Bag::setCustomerAddress(const nlohmann::json &purchase_json) {
    // if json structure already has a customer id -> returning customer!
    if(purchase_json.contains("CUSTOMER_ID")){
        // assign default
        this->custAddress = " ";
    } else {
        // now you should check if Address was provided or not!
        if(purchase_json.contains("ADDRESS")){
            this->custAddress = purchase_json.at("ADDRESS");
        } else {
            throw std::runtime_error("ADDRESS key not found: " + std::string(purchase_json.dump()));
        }
    }
}

// This method will set the Customer's city
void Bag::setCustomerCity(const nlohmann::json &purchase_json) {
    // if json structure already has a customer id -> returning customer!
    if(purchase_json.contains("CUSTOMER_ID")){
        // assign default
        this->custCity = " ";
    } else {
        // now you should check if City was provided or not!
        if(purchase_json.contains("CITY")){
            this->custCity = purchase_json.at("CITY");
        } else {
            throw std::runtime_error("CITY key not found: " + std::string(purchase_json.dump()));
        }
    }
}

// This method will set the Customer's state
void Bag::setCustomerState(const nlohmann::json &purchase_json) {
    // if json structure already has a customer id -> returning customer!
    if(purchase_json.contains("CUSTOMER_ID")){
        // assign default
        this->custState = " ";
    } else {
        // now you should check if State was provided or not!
        if(purchase_json.contains("STATE")){
            this->custState = purchase_json.at("STATE");
        } else {
            throw std::runtime_error("STATE key not found: " + std::string(purchase_json.dump()));
        }
    }
}

// This method will set the Customer's zip
void Bag::setCustomerZip(const nlohmann::json &purchase_json) {
    // if json structure already has a customer id -> returning customer!
    if(purchase_json.contains("CUSTOMER_ID")){
        // assign default
        this->custZip = " ";
    } else {
        // now you should check if State was provided or not!
        if(purchase_json.contains("ZIP")){
            this->custZip = purchase_json.at("ZIP");
        } else {
            throw std::runtime_error("ZIP key not found: " + std::string(purchase_json.dump()));
        }
    }
}

// This method will set the Customer's Purchase details
void Bag::setCustomerPurchaseDetails(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("PURCHASE_DETAILS")){
        nlohmann::json purchaseJSON = purchase_json.at("PURCHASE_DETAILS");
        // let's iterate over PURCHASE_DETAILS to validate if we have all the keys we need
        for(nlohmann::json::iterator it = purchaseJSON.begin(); it != purchaseJSON.end(); ++it){
            if(!it->contains("SKU") || !it->contains("UNITS") || !it->contains("UNIT_PRICE")){
                throw std::runtime_error("PURCHASE_DETAILS missing one of SKU/UNITS/UNIT_PRICE");
            }
        }
        // all cases satisfied, lets set the custPurchases private data member
        this->custPurchases = purchase_json.at("PURCHASE_DETAILS");
    } else {
        throw std::runtime_error("PURCHASE_DETAILS key not found: " + std::string(purchase_json.dump()));
    }
}

// This method will set the Customer's Purchase totals
void Bag::setCustomerPurchaseTotals(){
    // get all purchase details
    nlohmann::json purchaseDetails = getCustomerPurchases();
    // declare a sum variable
    double total = 0;
    // iterate over the purchase details
    for(nlohmann::json::iterator it = purchaseDetails.begin(); it != purchaseDetails.end(); ++it){
        total += static_cast<double>(it->at("UNIT_PRICE")) * static_cast<int>(it->at("UNITS"));
    }
    // let's assign to this private data member
    this->custTotal = total;
}

// This method will set the Customer id
void Bag::setCustomerId(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("CUSTOMER_ID")){
        this->custId =  static_cast<int>(purchase_json.at("CUSTOMER_ID"));
    } else {
        // Assign a simple default
        this->custId = -999;
    }
}

// Getters
// This method will return the brand the customer was on
std::string Bag::getCustomerBrand() {
    return this->custBrand;
}

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
int Bag::getCustomerId() const {
    return this->custId;
}

// This method will return the total of customer's purchases
double Bag::getCustomerTotals() const {
    return this->custTotal;
}

// Operational Methods
// Simple display
void Bag::display() {
    std::cout << getCustomerId() << '|' << getCustomerPurchaseTime()
    << '|' << getCustomerFirstName() << '|' << getCustomerLastName() << '|' << getCustomerAddress()
    << '|' << getCustomerCity() << '|' << getCustomerState() << '|' << getCustomerZip()
    << '|' << getCustomerTotals() << '|' << getCustomerPurchases() << std::endl;
}

// This method will leverage private data members and insert a record into CUSTOMER table in ACMEDB
void Bag::insertCustomerRecord() {
    try {
        // let's use the function to get the connection string
        std::string cnxDetails = read_connection();

        // let's use the connection string to set up a connection object using PQXX library
        pqxx::connection cnx{cnxDetails};

        // Using the connection object, we will create a transaction to run our SQL instructions against
        pqxx::work txn{cnx};

        // Let's set the brand flags while we are here
        char newArmyFlg = (this->getCustomerBrand() == "NEW_ARMY") ? '1' : '0';
        char appleMonarchyFlg = (this->getCustomerBrand() == "APPLE_MONARCHY") ? '1' : '0';
        char continuityFlg = (this->getCustomerBrand() == "CONTINUITY") ? '1' : '0';

        // Now we will build the SQL to be inserted using private data members
        std::string buildSQL =
                std::string("INSERT INTO ACME.CUSTOMER(FIRST_NAME, LAST_NAME, ADDRESS, CITY, STATE, ZIP, NA_CUST_FLG, AM_CUST_FLG, CN_CUST_FLG) VALUES(") +
                "'" + this->getCustomerFirstName() + "'" + "," +
                "'" + this->getCustomerLastName() + "'" + "," +
                "'" + this->getCustomerAddress() + "'" + "," +
                "'" + this->getCustomerCity() + "'" + "," +
                "'" + this->getCustomerState() + "'" + "," +
                "'" + this->getCustomerZip() + "'" + "," +
                "'" + newArmyFlg + "'" + "," +
                "'" + appleMonarchyFlg + "'" + ',' +
                "'" + continuityFlg + "'" + ") ON CONFLICT DO NOTHING" + ";";

        // We will supply the buildSQL and execute within the transaction
        pqxx::result insertRes{txn.exec(buildSQL)};

        if (!insertRes.empty()) {
            int totCol = insertRes.columns();
            for (auto row: insertRes) {
                for (int i = 0; i < totCol; i++) {
                    std::cout << row[i].c_str() << "|";
                }
                std::cout << std::endl;
            }
        }
        txn.commit();
    }
    catch(pqxx::sql_error const &e){
        std::cout << "SQL error: " << e.what() << std::endl;
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cout << "Query was:; " << e.query() << std::endl;
        std::cerr << "Query was:; " << e.query() << std::endl;
    }
}

// This method will retrieve customerId from the database for new customers
void Bag::retrieveCustomerIdDB() {
    try {
        // let's use the function to get the connection string
        std::string cnxDetails = read_connection();

        // let's use the connection string to set up a connection object using PQXX library
        pqxx::connection cnx{cnxDetails};

        // Using the connection object, we will create a transaction to run our SQL instructions against
        pqxx::work txn{cnx};

        // Now we will build the SQL to be selected using private data members
        // This will retrieve customerId
        std::string buildSQL =
                std::string("SELECT ID FROM ACME.CUSTOMER WHERE FIRST_NAME = ") +
                "'" + this->getCustomerFirstName() + "'" + " AND LAST_NAME = " +
                "'" + this->getCustomerLastName() + "'" + " AND ADDRESS = " +
                "'" + this->getCustomerAddress() + "'" + " AND CITY = " +
                "'" + this->getCustomerCity() + "'" + " AND STATE = " +
                "'" + this->getCustomerState() + "'" + " AND ZIP = " +
                "'" + this->getCustomerZip() + "'" + ";";

        // We will supply the buildSQL and execute within the transaction
        pqxx::result insertRes{txn.exec(buildSQL)};

        // Check result set...
        if (!insertRes.empty()) {
            int totCol = insertRes.columns();
            // iterate over the result set container..
            for (auto row: insertRes) {
                for (int i = 0; i < totCol; i++) {
                    // re-assigning the private data member custId to value from the DB
                    this->custId = row[i].as<int>();
                }
            }
        }
        txn.commit();
    }
    catch(pqxx::sql_error const &e){
        std::cout << "SQL error: " << e.what() << std::endl;
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cout << "Query was:; " << e.query() << std::endl;
        std::cerr << "Query was:; " << e.query() << std::endl;
    }
}

// This method will retrieve brandId from the database
int Bag::retrieveBrandId(){
    try {
        // let's use the function to get the connection string
        std::string cnxDetails = read_connection();

        // let's use the connection string to set up a connection object using PQXX library
        pqxx::connection cnx{cnxDetails};

        // Using the connection object, we will create a transaction to run our SQL instructions against
        pqxx::work txn{cnx};

        // Now we will build the SQL to retrieve the brandId using private data members
        std::string buildSQL =
                std::string("SELECT MAX(ID) FROM ACME.BRANDS WHERE DESCRIPTION = ") +
                "'" + this->getCustomerBrand() + "'" + ";";

        // We will supply the buildSQL and execute within the transaction
        pqxx::result insertRes{txn.exec(buildSQL)};

        // Check result set...
        if (!insertRes.empty()) {
            int totCol = insertRes.columns();
            // iterate over the result set container..
            for (auto row: insertRes) {
                for (int i = 0; i < totCol; i++) {
                    // let's return the brand id
                    return row[i].as<int>();
                }
            }
        }
        txn.commit();
    }
    catch(pqxx::sql_error const &e){
        std::cout << "SQL error: " << e.what() << std::endl;
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cout << "Query was:; " << e.query() << std::endl;
        std::cerr << "Query was:; " << e.query() << std::endl;
    }
    return 0;
}

// This method will insert transaction totals from each brand into CUSTOMER_BRAND_TRANSACTIONS table in ACME schema
// it will reference Customer id, their purchase timestamps and the total across their purchases for a brand
void Bag::insertCustomerBrandSaleTotals(){
    try {
        // let's use the function to get the connection string
        std::string cnxDetails = read_connection();

        // let's use the connection string to set up a connection object using PQXX library
        pqxx::connection cnx{cnxDetails};

        // Using the connection object, we will create a transaction to run our SQL instructions against
        pqxx::work txn{cnx};

        // Let's set the brand id
        int brandId = retrieveBrandId();

        // Now we will build the SQL to be inserted using private data members
        std::string buildSQL =
                std::string("INSERT INTO ACME.CUSTOMER_BRAND_SALES(PURCHASE_TS,PURCHASE_DT,CUSTOMER_ID,BRAND_ID,SALES_TOTAL) VALUES(") +
                "TO_TIMESTAMP(" + "'" + this->getCustomerPurchaseTime() + "'" + "," + "'" + "YYYY-MM-DD HH24:MI:SS" + "'" + ")" + "," +
                "TO_TIMESTAMP(" + "'" + this->getCustomerPurchaseTime() + "'" + "," + "'" + "YYYY-MM-DD HH24:MI:SS" + "'" + ")::DATE" + "," +
                std::to_string(this->getCustomerId()) + "," +
                std::to_string(brandId) + "," +
                std::to_string(this->getCustomerTotals()) + ") ON CONFLICT DO NOTHING" + ";";

        // We will supply the buildSQL and execute within the transaction
        pqxx::result insertRes{txn.exec(buildSQL)};

        if (!insertRes.empty()) {
            int totCol = insertRes.columns();
            for (auto row: insertRes) {
                for (int i = 0; i < totCol; i++) {
                    std::cout << row[i].c_str() << "|";
                }
                std::cout << std::endl;
            }
        }
        txn.commit();
    }
    catch(pqxx::sql_error const &e){
        std::cout << "SQL error: " << e.what() << std::endl;
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cout << "Query was:; " << e.query() << std::endl;
        std::cerr << "Query was:; " << e.query() << std::endl;
    }
}

// Entry method!
// This is being made virtual so that it's implementation can be overridden in derived class
// And we want to do this at run-time through a pointer to the base class.
void Bag::entryMethod() {
    // This will be the primary method for Bag Class
    // Flow -
    // 1) Check the customerId -
    //  a) If -999, this is a new customer, will create an entry in ACME.CUSTOMER table
    //      i) Perform additional checks to see if data is valid or not.
    //  b) If not -999, this is existing customer, will NOT create an entry in ACME.CUSTOMER table
    if(getCustomerId() == -999 && getCustomerFirstName() != " " && getCustomerLastName() != " " &&
    getCustomerAddress() != " " && getCustomerState() != " " && getCustomerCity() != " " &&
    getCustomerZip() != " "){
        // Insert records into Customer table
        insertCustomerRecord();
        // Retrieve customer id associated with insert
        // SA: I feel an ORM could help by removing this retrieval method
        retrieveCustomerIdDB();
        // Insert records into Customer Brand Sales table
        insertCustomerBrandSaleTotals();
    } else if (getCustomerId() != -999){
        // This is existing customer flow - insert records into Customer Brand Sales table
        insertCustomerBrandSaleTotals();
    } else {
        throw std::runtime_error("Unexpected behavior path!");
    }
}



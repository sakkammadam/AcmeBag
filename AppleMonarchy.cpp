//
// Created by sakkammadam on 9/19/22.
//

#include "AppleMonarchy.h"
#include "Continuity.h"
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "pqxx/pqxx"

AppleMonarchy::AppleMonarchy(const nlohmann::json &purchase_json) : Continuity(purchase_json) {
    // Initialization will follow Bag Constructor
    // We will setCustomerPoints
    setCustomerPoints(purchase_json);
}

// This method will set the customer points
void AppleMonarchy::setCustomerPoints(const nlohmann::json &purchase_json) {
    if(purchase_json.contains("CUSTOMER_POINTS")){
        this->custPoints =  static_cast<int>(purchase_json.at("CUSTOMER_POINTS"));
    } else {
        // Assign a simple default
        this->custPoints = 0;
    }
}

// This method with retrieve the customer points
int AppleMonarchy::getCustomerPoints() const{
    return this->custPoints;
}

// This method will translate customer points to discounts applied across all their purchases
double AppleMonarchy::translatePointsToDiscounts() const {
    if (getCustomerPoints() >= 0 && getCustomerPoints() < 25000){
        // points are not worth anything till after 25000
        return 0.0;
    } else if(getCustomerPoints() > 1000000){
        // points expire after 1000000
        return 0.00;
    } else {
        return getCustomerPoints()/1000000.0;
    }
}

// This method will calculate the sales totals across all transactions for the customer
// This method is override in AppleMonarchy to take into account customer points
void AppleMonarchy::setCustomerPurchaseTotals() {
    // get all purchase details
    nlohmann::json purchaseDetails = getCustomerPurchases();
    // declare a sum variable
    double total = 0;
    // declare net item cost variable
    double item_cost = 0;
    // declare discount offer variable
    double discount_offer = translatePointsToDiscounts();
    // declare discount calculation variable
    double discount_calc = 0;
    // declare final item cost variable after discounts have been applied
    double final_item_cost = 0;
    // iterate over the purchase details
    for(nlohmann::json::iterator it = purchaseDetails.begin(); it != purchaseDetails.end(); ++it){
        item_cost = static_cast<double>(it->at("UNIT_PRICE")) * static_cast<int>(it->at("UNITS"));
        discount_calc = discount_offer * item_cost;
        final_item_cost = item_cost - discount_calc;
        // handle any scenario where crazy high offer was provided
        if(final_item_cost < 0){
            final_item_cost = 0;
        }
        total += final_item_cost;
    }
    // let's assign to this private data member
    this->custTotalWDiscount = total;
}

// This method will reflect the new private data member - custTotalWDiscount
double AppleMonarchy::getCustomerTotals() const {
    return this->custTotalWDiscount;
}

void AppleMonarchy::insertCustomerTransactions() {
    try {
        // let's use the function to get the connection string
        std::string cnxDetails = read_connection();

        // let's use the connection string to set up a connection object using PQXX library
        pqxx::connection cnx{cnxDetails};

        // Using the connection object, we will create a transaction to run our SQL instructions against
        pqxx::work txn{cnx};

        // Let's get all the purchase details
        nlohmann::json purchaseDetails = getCustomerPurchases();

        // we will need to iterate over the purchase details to understand the components
        for(nlohmann::json::iterator it = purchaseDetails.begin(); it != purchaseDetails.end(); ++it){
            std::string sku = it->at("SKU");
            int units = it->at("UNITS");
            double unitPrice = it->at("UNIT_PRICE");
            double txnTotal = unitPrice*units;
            double discount_offer = translatePointsToDiscounts();
            // applying discounts
            txnTotal = (unitPrice*units) - ((unitPrice*units)*discount_offer);
            if(txnTotal < 0){
                txnTotal = 0;
            }
            // Now we will build the SQL to be inserted using private data members
            std::string buildSQL =
                    std::string("INSERT INTO APPLEMONARCHY.CUSTOMER_TRANSACTIONS(TRANSACTION_TS,TRANSACTION_DT,CUSTOMER_ID,SKU,UNITS,UNIT_PRICE,TRANSACTION_TOTAL,ORIGINAL_TOTAL,OFFERS,DISCOUNTS,POINTS) VALUES(") +
                    "TO_TIMESTAMP(" + "'" + this->getCustomerPurchaseTime() + "'" + "," + "'" + "YYYY-MM-DD HH24:MI:SS" + "'" + ")" + "," +
                    "TO_TIMESTAMP(" + "'" + this->getCustomerPurchaseTime() + "'" + "," + "'" + "YYYY-MM-DD HH24:MI:SS" + "'" + ")::DATE" + "," +
                    std::to_string(this->getCustomerId()) + "," +
                    "'" + sku + "'" + "," +
                    std::to_string(units) + "," +
                    std::to_string(unitPrice) + "," +
                    std::to_string(txnTotal)  + "," +
                    std::to_string(unitPrice*units)  + "," +
                    std::to_string(discount_offer)  + "," +
                    std::to_string((unitPrice*units)*discount_offer) + "," +
                    std::to_string(this->getCustomerPoints()) +
                    ") ON CONFLICT DO NOTHING" + ";";

            // We will supply the buildSQL and execute within the transaction
            pqxx::result insertRes{txn.exec(buildSQL)};

            // display the SQL
            std::cout << "Query executed for AppleMonarchy Customer transactions: " << buildSQL << std::endl;
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





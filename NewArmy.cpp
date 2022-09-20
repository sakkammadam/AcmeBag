//
// Created by sakkammadam on 9/19/22.
//

#include "NewArmy.h"
#include "Bag.h"
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "pqxx/pqxx"


NewArmy::NewArmy(const nlohmann::json &purchase_json) : Bag(purchase_json) {
    // Initialization will follow Bag Constructor
    // We will re-calculate the purchase totals
}

void NewArmy::setCustomerPurchaseTotals() {
    // get all purchase details
    nlohmann::json purchaseDetails = getCustomerPurchases();
    // declare a sum variable
    double total = 0;
    // declare net item cost variable
    double item_cost = 0;
    // declare discount offer variable
    double discount_offer = 0;
    // declare discount calculation variable
    double discount_calc = 0;
    // declare final item cost variable after discounts have been applied
    double final_item_cost = 0;
    // iterate over the purchase details
    for(nlohmann::json::iterator it = purchaseDetails.begin(); it != purchaseDetails.end(); ++it){
        item_cost = static_cast<double>(it->at("UNIT_PRICE")) * static_cast<int>(it->at("UNITS"));
        if(it->contains("DISCOUNT")){
            discount_offer = static_cast<double>(it->at("DISCOUNT"));
        } else {
            discount_offer = 0.00;
        }
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

double NewArmy::getCustomerTotals() const {
    return this->custTotalWDiscount;
}

void NewArmy::insertCustomerTransactions() {
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
            double discount_offer;
            if(it->contains("DISCOUNT")){
                discount_offer = static_cast<double>(it->at("DISCOUNT"));
            } else {
                discount_offer = 0.00;
            }
            // applying discounts
            txnTotal = (unitPrice*units) - ((unitPrice*units)*discount_offer);
            if(txnTotal < 0){
                txnTotal = 0;
            }
            // Now we will build the SQL to be inserted using private data members
            std::string buildSQL =
                    std::string("INSERT INTO NEWARMY.CUSTOMER_TRANSACTIONS(TRANSACTION_TS,TRANSACTION_DT,CUSTOMER_ID,SKU,UNITS,UNIT_PRICE,TRANSACTION_TOTAL,ORIGINAL_TOTAL,OFFERS,DISCOUNTS) VALUES(") +
                    "TO_TIMESTAMP(" + "'" + this->getCustomerPurchaseTime() + "'" + "," + "'" + "YYYY-MM-DD HH24:MI:SS" + "'" + ")" + "," +
                    "TO_TIMESTAMP(" + "'" + this->getCustomerPurchaseTime() + "'" + "," + "'" + "YYYY-MM-DD HH24:MI:SS" + "'" + ")::DATE" + "," +
                    std::to_string(this->getCustomerId()) + "," +
                    "'" + sku + "'" + "," +
                    std::to_string(units) + "," +
                    std::to_string(unitPrice) + "," +
                    std::to_string(txnTotal)  + "," +
                    std::to_string(unitPrice*units)  + "," +
                    std::to_string(discount_offer)  + "," +
                    std::to_string((unitPrice*units)*discount_offer) +
                    ") ON CONFLICT DO NOTHING" + ";";

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

//
// Created by sakkammadam on 9/19/22.
//

#include "Continuity.h"
#include "Bag.h"
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "pqxx/pqxx"

Continuity::Continuity(const nlohmann::json &purchase_json) : Bag(purchase_json) {
    // do nothing in the body
    // Initialization will follow Bag Constructor
}

void Continuity::insertCustomerTransactions() {
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

            // Now we will build the SQL to be inserted using private data members
            std::string buildSQL =
                    std::string("INSERT INTO CONTINUITY.CUSTOMER_TRANSACTIONS(TRANSACTION_TS,TRANSACTION_DT,CUSTOMER_ID,SKU,UNITS,UNIT_PRICE,TRANSACTION_TOTAL) VALUES(") +
                    "TO_TIMESTAMP(" + "'" + this->getCustomerPurchaseTime() + "'" + "," + "'" + "YYYY-MM-DD HH24:MI:SS" + "'" + ")" + "," +
                    "TO_TIMESTAMP(" + "'" + this->getCustomerPurchaseTime() + "'" + "," + "'" + "YYYY-MM-DD HH24:MI:SS" + "'" + ")::DATE" + "," +
                    std::to_string(this->getCustomerId()) + "," +
                    "'" + sku + "'" + "," +
                    std::to_string(units) + "," +
                    std::to_string(unitPrice) + "," +
                    std::to_string(txnTotal)  +
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


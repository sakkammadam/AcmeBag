//
// Created by sakkammadam on 9/19/22.
//

#ifndef ACMEBAG_NEWARMY_H
#define ACMEBAG_NEWARMY_H

#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include "Bag.h"

class NewArmy:
        public Bag{
private:
    double custTotalWDiscount;

public:
    // initialization constructor
    explicit NewArmy(const nlohmann::json &purchase_json);

    // This method will calculate the sales totals across all transactions for the customer
    // This method is override in NewArmy to take into account discounts presented to the customer
    virtual void setCustomerPurchaseTotals();

    virtual double getCustomerTotals() const;

    // this method will capture the customer's transactions within Continuity brand to its own table
    // CONTINUITY.CUSTOMER_TRANSACTIONS
    virtual void insertCustomerTransactions();

    // We will override the entrypoint method to take into account insertCustomerTransactions method
    // virtual void entryMethod();

};


#endif //ACMEBAG_NEWARMY_H

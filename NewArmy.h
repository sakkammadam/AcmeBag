//
// Created by sakkammadam on 9/19/22.
//

#ifndef ACMEBAG_NEWARMY_H
#define ACMEBAG_NEWARMY_H

#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include "Continuity.h"

class NewArmy:
        public Continuity{
private:
    // New private data member to take into account new total after applying discounts
    double custTotalWDiscount;

public:
    // initialization constructor
    explicit NewArmy(const nlohmann::json &purchase_json);

    // This method will calculate the sales totals across all transactions for the customer
    // This method is override in NewArmy to take into account discounts presented to the customer
    void setCustomerPurchaseTotals() override;

    // This method will reflect the new private data member - custTotalWDiscount
    double getCustomerTotals() const override;

    // this method will capture the customer's transactions within NewArmy brand to its own table
    // NEWARMY.CUSTOMER_TRANSACTIONS
    void insertCustomerTransactions() override;

};


#endif //ACMEBAG_NEWARMY_H

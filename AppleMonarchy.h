//
// Created by sakkammadam on 9/19/22.
//

#ifndef ACMEBAG_APPLEMONARCHY_H
#define ACMEBAG_APPLEMONARCHY_H

#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include "Continuity.h"


class AppleMonarchy:
        public Continuity {
private:
    // custPoints denotes the purchase points that Apple Monarchy provides its customers.
    // Customers can use these points across all their purchases
    int custPoints;

    // custTotalWDiscount denotes the new total after applying discounts
    double custTotalWDiscount;

public:
    // initialization constructor
    explicit AppleMonarchy(const nlohmann::json &purchase_json);

    // This method will set the customer points
    void setCustomerPoints(const nlohmann::json &purchase_json);

    // This method with retrieve the customer points
    int getCustomerPoints() const;

    // This method will translate customer points to discounts applied across all their purchases
    double translatePointsToDiscounts() const;

    // This method will calculate the sales totals across all transactions for the customer
    // This method is override in AppleMonarchy to take into account customer points
    void setCustomerPurchaseTotals() override;

    // This method will reflect the new private data member - custTotalWDiscount
    double getCustomerTotals() const override;

    // this method will capture the customer's transactions within AppleMonarchy brand to its own table
    // APPLEMONARCHY.CUSTOMER_TRANSACTIONS
    void insertCustomerTransactions() override;

};


#endif //ACMEBAG_APPLEMONARCHY_H

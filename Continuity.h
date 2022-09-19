//
// Created by sakkammadam on 9/19/22.
//

#ifndef ACMEBAG_CONTINUITY_H
#define ACMEBAG_CONTINUITY_H

#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include "Bag.h"

class Continuity:
        public Bag{
public:
    // initialization constructor
    explicit Continuity(const nlohmann::json &purchase_json);

    // this method will capture the customer's transactions within Continuity brand to its own table
    // CONTINUITY.CUSTOMER_TRANSACTIONS
    virtual void insertCustomerTransactions();

    // We will override the entrypoint method to take into account insertCustomerTransactions method
    virtual void entryMethod();

};


#endif //ACMEBAG_CONTINUITY_H

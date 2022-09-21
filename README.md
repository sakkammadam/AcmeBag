AcmeBag is a 3-tier class that takes JSON inputs and based on the brand key invokes different types of the Bag derived class - NewArmy, Continuity and AppleMonarchy.
All of the objects are then fed into a vector containing pointers of the base Bag Class.
Leverage polymorphism to drive dynamic linking at run time.

The common entryMethod will invoke the necessary constructor initializations and create SQL's to write into PostgreSQL database.

To build - 

g++ --std=c++17 main.cpp Bag.h Bag.cpp Continuity.h Continuity.cpp NewArmy.h NewArmy.cpp AppleMonarchy.h AppleMonarchy.cpp -lpqxx -lpq -o runAcme


To run - 

./runAcme <path_to_json_file>


Further documentation can be found at AcmeImplementationDocs.pdf

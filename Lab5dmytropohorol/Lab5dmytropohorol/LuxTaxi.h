#pragma once
#include "Taxi.h"

class LuxTaxi : public Taxi
{
public:
    LuxTaxi() : Taxi()
    {
        std::cout << "[LOG] LuxTaxi default constructor called.\n";
    }

    LuxTaxi(const char* InPassenger,
        const int* InDrivers, int InDriversCount,
        const char (*InAddresses)[MAX_STR_LEN], int InAddressesCount)
        : Taxi(InPassenger, InDrivers, InDriversCount, InAddresses, InAddressesCount)
    {
        std::cout << "[LOG] LuxTaxi param constructor called.\n";
    }

    // Polymorphic function override
    void ShowPolymorphic() const override
    {
        std::cout << "[LuxTaxi::ShowPolymorphic] This is a luxury taxi.\n";
    }

    // Lets show how to access 'Addresses' (protected in Taxi)
    // but unable to access 'Drivers' (private in Taxi).
    void ShowProtectedAccess()
    {
        std::cout << "[LuxTaxi::ShowProtectedAccess] We can read addresses...\n";
        for (int i = 0; i < GetAddressesCount(); i++)
            if (Addresses) // 'Addresses' is protected, so its avaiable here
                std::cout << "  LuxTaxi address[" << i << "] = " << Addresses[i] << "\n";

        // Uncommenting the following lines will fail to compile,
        // because 'Drivers' is private in 'Taxi':
        // for(int i=0; i<GetDriversCount(); i++)
        // {
        //     std::cout << "Driver state = " << Drivers[i] << "\n"; // ILLEGAL
        // }
    }
};

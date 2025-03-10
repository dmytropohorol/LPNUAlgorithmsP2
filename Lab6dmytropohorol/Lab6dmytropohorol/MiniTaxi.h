#pragma once

#include "Taxi.h"

class MiniTaxi : public Taxi
{
public:
	MiniTaxi() : Taxi()
	{
		std::cout << "[LOG] MiniTaxi default constructor.\n";
	}

	MiniTaxi(const char* InPassenger,
		const int* InDrivers, int InDriversCount,
		const char (*InAddresses)[MAX_STR_LEN], int InAddressesCount)
		: Taxi(InPassenger, InDrivers, InDriversCount, InAddresses, InAddressesCount)
	{
		std::cout << "[LOG] MiniTaxi param constructor.\n";
	}

	// override the abstract method from AbstractTaxi
	void ShowPolymorphic() const override
	{
		std::cout << "[MiniTaxi::ShowPolymorphic] This is a smaller taxi.\n";
	}
};


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
	void PrintInfo() const override
	{
		std::cout << "Mini taxi:\n"
			<< "Passenger: " << Passenger
			<< ", Total Taxi objects: " << Count
			<< std::endl;
	}
};


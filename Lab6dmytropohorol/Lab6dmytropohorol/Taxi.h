#pragma once

#include "AbstractTaxi.h"
#include <iostream>
#include <fstream>

// Possible driver states
const int DRIVER_FREE = 0;
const int DRIVER_BUSY = 1;

class Taxi : public AbstractTaxi
{
public:
	Taxi();
	Taxi(const char* InPassenger,
		const int* InDrivers, int InDriversCount,
		const char (*InAddresses)[MAX_STR_LEN], int InAddressesCount);
	Taxi(const Taxi& Other);
	~Taxi();

	// Return number of free drivers
	int Order();

	// Check if given address is in the array of addresses
	bool Order(const char* InAddress);

	// Orders the driver by address
	bool Order(int DriverIndex, const char* InAddress);

	void PrintInfo() const;

	void InputFromConsole();
	void PrintToConsole() const;

	void SaveToFile(const char* FileName) const;
	void LoadFromFile(const char* FileName);

	int  GetDriverState(int Index) const;
	void SetDriverState(int Index, int State);
	int GetDriversCount() const { return DriversCount; }
	void SetDriversCount(int NewCount);

	const char* GetAddress(int Index) const;
	void SetAddress(int Index, const char* NewAddress);
	int GetAddressesCount() const { return AddressesCount; }
	void SetAddressesCount(int NewCount);

	// Polymorphic function required by base class
	// This overrides the pure virtual method from AbstractTaxi
	void ShowPolymorphic() const override;

	// Implement IAutoNumber:
	int GetObjectNumber() const override { return ObjectNumber; }
	int GetTotalCount() const override { return Count; }

	// Implement IStringConvertible:
	std::string ToString() const override;
	void FromString(const std::string& InStr) override;

	friend std::istream& operator>>(std::istream& InStream, Taxi& Obj);
	friend std::ostream& operator<<(std::ostream& OutStream, const Taxi& Obj);

	friend std::ifstream& operator>>(std::ifstream& InFile, Taxi& Obj);
	friend std::ofstream& operator<<(std::ofstream& OutFile, const Taxi& Obj);

	bool operator==(const Taxi& Other) const;
	bool operator!=(const Taxi& Other) const { return !(*this == Other); }

	bool operator<(const Taxi& Other) const;
	bool operator>(const Taxi& Other) const { return Other < *this; }
	bool operator<=(const Taxi& Other) const { return !(Other < *this); }
	bool operator>=(const Taxi& Other) const { return !(*this < Other); }

	char Passenger[MAX_STR_LEN];

protected:
	char (*Addresses)[MAX_STR_LEN];

private:
	int* Drivers;

	int DriversCount;
	int AddressesCount;
};
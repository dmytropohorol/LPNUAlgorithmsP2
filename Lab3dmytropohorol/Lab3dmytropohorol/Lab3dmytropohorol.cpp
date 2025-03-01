#include "Lab3dmytropohorol.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <limits>

#pragma warning( disable : 4996)

int ReadStrictInt();
void ReadNonEmptyString(char* Buffer);

int Taxi::Count = 0;

Taxi::Taxi()
{
	std::cout << "[LOG] Taxi default constructor called.\n";

	std::strcpy(Passenger, "Unknown");
	Drivers = nullptr;
	Addresses = nullptr;

	DriversCount = 0;
	AddressesCount = 0;

	Count++;
}

Taxi::Taxi(const char* InPassenger,
	const int* InDrivers, int InDriversCount,
	const char (*InAddresses)[MAX_STR_LEN], int InAddressesCount)
{
	std::cout << "[LOG] Taxi parameterized constructor called.\n";

	InPassenger ? std::strncpy(Passenger, InPassenger, MAX_STR_LEN - 1) : std::strcpy(Passenger, "Unknown");
	Passenger[MAX_STR_LEN - 1] = '\0';

	if (InDrivers && InDriversCount > 0)
	{
		DriversCount = InDriversCount;
		Drivers = new int[DriversCount];
		for (int i = 0; i < DriversCount; i++)
		{
			Drivers[i] = InDrivers[i];
		}
	}
	else
	{
		DriversCount = 0;
		Drivers = nullptr;
	}

	if (InAddresses && InAddressesCount > 0)
	{
		AddressesCount = InAddressesCount;
		Addresses = new char[AddressesCount][MAX_STR_LEN];
		for (int i = 0; i < AddressesCount; i++)
		{
			std::strncpy(Addresses[i], InAddresses[i], MAX_STR_LEN - 1);
			Addresses[i][MAX_STR_LEN - 1] = '\0';
		}
	}
	else
	{
		AddressesCount = 0;
		Addresses = nullptr;
	}

	Count++;
}

Taxi::Taxi(const Taxi& Other)
{
	std::cout << "[LOG] Taxi copy constructor called.\n";

	std::strcpy(Passenger, Other.Passenger);

	DriversCount = Other.DriversCount;
	AddressesCount = Other.AddressesCount;

	if (DriversCount > 0 && Other.Drivers)
	{
		Drivers = new int[DriversCount];
		for (int i = 0; i < DriversCount; i++)
		{
			Drivers[i] = Other.Drivers[i];
		}
	}
	else
	{
		Drivers = nullptr;
		DriversCount = 0;
	}

	if (AddressesCount > 0 && Other.Addresses)
	{
		Addresses = new char[AddressesCount][MAX_STR_LEN];
		for (int i = 0; i < AddressesCount; i++)
		{
			std::strcpy(Addresses[i], Other.Addresses[i]);
		}
	}
	else
	{
		Addresses = nullptr;
		AddressesCount = 0;
	}

	Count++;
}

Taxi::~Taxi()
{
	std::cout << "[LOG] Taxi destructor called. Freeing memory.\n";

	delete[] Drivers;
	delete[] Addresses;

	Count--;
}

int Taxi::GetDriverState(int Index) const
{
	if (Index < 0 || Index >= DriversCount || !Drivers)
	{
		return -1; // invalid
	}
	return Drivers[Index];
}

void Taxi::SetDriverState(int Index, int State)
{
	if (Index < 0 || Index >= DriversCount || !Drivers)
	{
		return;
	}
	Drivers[Index] = State;
}

const char* Taxi::GetAddress(int Index) const
{
	if (Index < 0 || Index >= AddressesCount || !Addresses)
	{
		return nullptr;
	}
	return Addresses[Index];
}

void Taxi::SetAddress(int Index, const char* NewAddress)
{
	if (Index < 0 || Index >= AddressesCount || !Addresses || !NewAddress)
	{
		return;
	}
	std::strncpy(Addresses[Index], NewAddress, MAX_STR_LEN - 1);
	Addresses[Index][MAX_STR_LEN - 1] = '\0';
}

int Taxi::Order()
{
	std::cout << "[LOG] Taxi::Order() - counting free drivers.\n";

	if (!Drivers)
	{
		return 0;
	}
	int FreeDriversNum = 0;
	for (int i = 0; i < DriversCount; i++)
	{
		if (Drivers[i] == DRIVER_FREE)
		{
			FreeDriversNum++;
		}
	}
	return FreeDriversNum;
}

bool Taxi::Order(const char* InAddress)
{
	std::cout << "[LOG] Taxi::Order(const char*).\n";

	if (!Drivers || !Addresses || !InAddress || std::strlen(InAddress) == 0)
	{
		return false;
	}

	// confirm the address is known
	bool KnownAddress = false;
	for (int i = 0; i < AddressesCount; i++)
	{
		if (!std::strcmp(Addresses[i], InAddress))
		{
			KnownAddress = true;
			break;
		}
	}
	if (!KnownAddress)
	{
		return false;
	}

	// find a free driver
	for (int i = 0; i < DriversCount; i++)
	{
		if (Drivers[i] == DRIVER_FREE)
		{
			return Order(i, InAddress);
		}
	}
	return false; // no free drivers
}

bool Taxi::Order(int DriverIndex, const char* InAddress)
{
	std::cout << "[LOG] Taxi::Order(int, const char*).\n";

	if (!InAddress || !Drivers || !Addresses
		|| DriverIndex < 0 || DriverIndex >= DriversCount
		|| std::strlen(InAddress) == 0)
	{
		return false;
	}
	if (Drivers[DriverIndex] != DRIVER_FREE)
	{
		return false;
	}

	// check address
	for (int i = 0; i < AddressesCount; i++)
	{
		if (!std::strcmp(Addresses[i], InAddress))
		{
			Drivers[DriverIndex] = DRIVER_BUSY;
			return true;
		}
	}
	return false; // address not known
}

void Taxi::PrintInfo() const
{
	std::cout << " Passenger: " << Passenger
		<< ", DriversCount: " << DriversCount
		<< ", AddressesCount: " << AddressesCount
		<< ", Total Taxi objects: " << Count
		<< "\n";
}

void Taxi::InputFromConsole()
{
	std::cout << "Enter passenger name (no spaces, up to "
		<< (MAX_STR_LEN - 1) << " chars): ";
	ReadNonEmptyString(Passenger);

	std::cout << "How many drivers do you want to store? ";
	
	DriversCount = ReadStrictInt();
	if (DriversCount < 0)
	{
		DriversCount = 0;
	}

	delete[] Drivers;
	Drivers = nullptr;

	if (DriversCount > 0)
	{
		Drivers = new int[DriversCount];
		for (int i = 0; i < DriversCount; i++)
		{
			std::cout << "  Enter state for driver No." << i << " (0=FREE,1=BUSY): ";
			int State = ReadStrictInt();
			if (State != DRIVER_FREE && State != DRIVER_BUSY)
			{
				State = DRIVER_FREE;
			}
			Drivers[i] = State;
		}
	}

	std::cout << "How many addresses? ";
	AddressesCount = ReadStrictInt();
	if (AddressesCount < 0)
	{
		AddressesCount = 0;
	}

	delete[] Addresses;
	Addresses = nullptr;

	if (AddressesCount > 0)
	{
		Addresses = new char[AddressesCount][MAX_STR_LEN];
		for (int i = 0; i < AddressesCount; i++)
		{
			std::cout << "  Enter address #" << i << ": ";
			ReadNonEmptyString(Addresses[i]);
		}
	}
}

void Taxi::PrintToConsole() const
{
	std::cout << "\n--- Taxi Detailed Info ---\n"
		<< "Passenger: " << Passenger << "\n"
		<< "DriversCount: " << DriversCount << "\n";
	if (Drivers)
	{
		for (int i = 0; i < DriversCount; i++)
		{
			std::cout << "  Driver #" << i << " => "
				<< (Drivers[i] == DRIVER_FREE ? "FREE" : "BUSY")
				<< "\n";
		}
	}
	else
	{
		std::cout << "  No driver data.\n";
	}

	std::cout << "AddressesCount: " << AddressesCount << "\n";
	if (Addresses)
	{
		for (int i = 0; i < AddressesCount; i++)
		{
			std::cout << "  Address #" << i << ": " << Addresses[i] << "\n";
		}
	}
	else
	{
		std::cout << "  No address data.\n";
	}
	std::cout << std::endl;
}

void Taxi::SaveToFile(const char* FileName) const
{
	std::ofstream fout(FileName);
	if (!fout)
	{
		std::cerr << "Failed to open file for saving: " << FileName << "\n";
		return;
	}
	fout << Passenger << "\n";
	fout << DriversCount << "\n";
	for (int i = 0; i < DriversCount; i++)
	{
		fout << Drivers[i] << "\n";
	}
	fout << AddressesCount << "\n";
	for (int i = 0; i < AddressesCount; i++)
	{
		fout << Addresses[i] << "\n";
	}

	fout.close();
}

void Taxi::LoadFromFile(const char* FileName)
{
	std::ifstream fin(FileName);
	if (!fin)
	{
		std::cerr << "Failed to open file for loading: " << FileName << "\n";
		return;
	}

	fin.getline(Passenger, MAX_STR_LEN);
	fin >> DriversCount;
	delete[] Drivers;
	Drivers = nullptr;

	if (DriversCount < 0)
	{
		DriversCount = 0;
	}

	if (DriversCount > 0)
	{
		Drivers = new int[DriversCount];
		for (int i = 0; i < DriversCount; i++)
		{
			int st;
			fin >> st;
			if (st != DRIVER_FREE && st != DRIVER_BUSY)
			{
				st = DRIVER_FREE;
			}
			Drivers[i] = st;
		}
	}

	fin >> AddressesCount;
	fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	delete[] Addresses;
	Addresses = nullptr;
	if (AddressesCount < 0)
	{
		AddressesCount = 0;
	}

	if (AddressesCount > 0)
	{
		Addresses = new char[AddressesCount][MAX_STR_LEN];
		for (int i = 0; i < AddressesCount; i++)
		{
			fin.getline(Addresses[i], MAX_STR_LEN);
			if (!std::strlen(Addresses[i]))
			{
				std::strcpy(Addresses[i], "UnknownAddr");
			}
		}
	}

	fin.close();
}

void PressEnterToContinue()
{
	std::cout << "Press ENTER to continue...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << std::endl;
}

int main()
{
	Taxi* t1 = new Taxi(); // default
	Taxi* t2 = new Taxi(); // will create param or copy, etc.

	bool Running = true;
	while (Running)
	{
		std::cout << "\n=========================================\n"
			<< "          TAXI DEMO MENU (No MAX)\n"
			<< "=========================================\n"
			<< "  1) Create Default Taxi (t1)\n"
			<< "  2) Create Parameterized Taxi (t2)\n"
			<< "  3) Copy t2 to t1\n"
			<< "  4) InputFromConsole (t1)\n"
			<< "  5) PrintToConsole (t1)\n"
			<< "  6) Save t1 to file\n"
			<< "  7) Load t1 from file\n"
			<< "  8) Show number of FREE drivers in t1\n"
			<< "  9) Order by address (t1)\n"
			<< " 10) Order by driver index & address (t1)\n"
			<< " 11) PrintInfo (t1)\n"
			<< " 12) PrintInfo (t2)\n"
			<< "  0) Exit\n"
			<< "-----------------------------------------\n"
			<< "Enter your choice: ";

		int Choice = ReadStrictInt();

		switch (Choice)
		{
		case 1:
		{
			delete t1;
			t1 = new Taxi();
			std::cout << "[MAIN] Created t1 as default Taxi.\n";
			PressEnterToContinue();
			break;
		}
		case 2:
		{
			// Create a param t2 with 3 drivers + 2 addresses
			// to show how to pass them in the new constructor.

			const int DCount = 3;
			int SampleDrivers[DCount] = { DRIVER_FREE, DRIVER_BUSY, DRIVER_FREE };

			const int ACount = 2;
			char SampleAddresses[ACount][MAX_STR_LEN] = {
				"ParkLane", "Broadway"
			};

			delete t2;
			t2 = new Taxi("Alice", SampleDrivers, DCount, SampleAddresses, ACount);
			std::cout << "[MAIN] Created t2 with parameterized constructor.\n";
			PressEnterToContinue();
			break;
		}
		case 3:
		{
			// Copy t2 -> t1
			delete t1;
			t1 = new Taxi(*t2);
			std::cout << "[MAIN] Copied t2 into t1.\n";
			PressEnterToContinue();
			break;
		}
		case 4:
		{
			t1->InputFromConsole();
			std::cout << "[MAIN] Called t1.InputFromConsole().\n";
			PressEnterToContinue();
			break;
		}
		case 5:
		{
			t1->PrintToConsole();
			PressEnterToContinue();
			break;
		}
		case 6:
		{
			std::cout << "Enter file name to save t1: ";
			char filename[260];
			ReadNonEmptyString(filename);
			t1->SaveToFile(filename);
			PressEnterToContinue();
			break;
		}
		case 7:
		{
			std::cout << "Enter file name to load into t1: ";
			char filename[260];
			ReadNonEmptyString(filename);
			t1->LoadFromFile(filename);
			PressEnterToContinue();
			break;
		}
		case 8:
		{
			int FreeNumber = t1->Order();
			std::cout << "[MAIN] t1.Order() => " << FreeNumber << " free drivers.\n";
			PressEnterToContinue();
			break;
		}
		case 9:
		{
			std::cout << "Enter address to order: ";
			char address[256];
			ReadNonEmptyString(address);
			bool success = t1->Order(address);
			std::cout << (success
				? "[MAIN] Successfully assigned the first free driver.\n"
				: "[MAIN] Order by address failed.")
				<< std::endl;
			PressEnterToContinue();
			break;
		}
		case 10:
		{
			std::cout << "Enter driver index: ";
			int idx = ReadStrictInt();
			std::cout << "Enter address: ";
			char address[256];
			ReadNonEmptyString(address);
			bool success = t1->Order(idx, address);
			std::cout << (success
				? "[MAIN] Assigned that driver to address.\n"
				: "[MAIN] Order failed.")
				<< std::endl;
			PressEnterToContinue();
			break;
		}
		case 11:
		{
			t1->PrintInfo();
			PressEnterToContinue();
			break;
		}
		case 12:
		{
			t2->PrintInfo();
			PressEnterToContinue();
			break;
		}
		case 0:
			Running = false;
			break;
		default:
			std::cout << "Invalid choice.\n";
			PressEnterToContinue();
		}
	}

	std::cout << "\nExiting... final Taxi count = " << Taxi::GetCount() << std::endl;
	return 0;
}

// -------------------------------------------
// Utility: Input Validation
// -------------------------------------------

bool IsPureInteger(const char* Str)
{
	if (!Str[0]) return false;
	for (int i = 0; Str[i] != '\0'; i++)
		if (!std::isdigit(static_cast<unsigned char>(Str[i]))) 
			return false;
	return true;
}

void ReadNonEmptyString(char* Buffer)
{
	while (true)
	{
		std::cin.getline(Buffer, MAX_STR_LEN);
		if (std::strlen(Buffer) > 0) return;
		std::cout << "  Invalid (empty) input. Please try again: ";
	}
}

int ReadStrictInt()
{
	char Line[MAX_STR_LEN];
	while (true)
	{
		std::cin.getline(Line, MAX_STR_LEN);
		if (IsPureInteger(Line)) return std::atoi(Line);
		std::cout << "  Invalid integer format. Please try again: ";
	}
}
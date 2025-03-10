#include "Taxi.h"

#pragma warning( disable : 4996)

// Helper functions for copying arrays
int* CopyDrivers(const int* source, int count)
{
	if (source && count > 0)
	{
		int* dest = new int[count];
		for (int i = 0; i < count; i++)
		{
			dest[i] = source[i];
		}
		return dest;
	}
	return nullptr;
}

char (*CopyAddresses(const char source[][MAX_STR_LEN], int count))[MAX_STR_LEN]
{
	if (source && count > 0)
	{
		char (*dest)[MAX_STR_LEN] = new char[count][MAX_STR_LEN];
		for (int i = 0; i < count; i++)
		{
			std::strncpy(dest[i], source[i], MAX_STR_LEN - 1);
			dest[i][MAX_STR_LEN - 1] = '\0';
		}
		return dest;
	}
	return nullptr;
}

void AllocateDrivers(int*& Drivers, int& Count, int NewCount)
{
	delete[] Drivers;
	Drivers = nullptr;
	Count = (NewCount > 0 ? NewCount : 0);
	if (Count > 0)
	{
		Drivers = new int[Count];
		for (int i = 0; i < Count; i++)
			Drivers[i] = DRIVER_FREE;
	}
}

int ReadStrictInt();
void ReadNonEmptyString(char* Buffer);

Taxi::Taxi()
{
	std::cout << "[LOG] Taxi default constructor called. Current number of taxi's classes - " << ++Count << std::endl;
	std::strcpy(Passenger, "Unknown");
	Drivers = nullptr;
	Addresses = nullptr;
	DriversCount = 0;
	AddressesCount = 0;
}

Taxi::Taxi(const char* InPassenger,
	const int* InDrivers, int InDriversCount,
	const char (*InAddresses)[MAX_STR_LEN], int InAddressesCount)
{
	std::cout << "[LOG] Taxi parameterized constructor called. Current number of taxi's classes - " << ++Count << std::endl;

	InPassenger ? std::strncpy(Passenger, InPassenger, MAX_STR_LEN - 1) : std::strcpy(Passenger, "Unknown");
	Passenger[MAX_STR_LEN - 1] = '\0';
	
	DriversCount = (InDrivers && InDriversCount > 0) ? InDriversCount : 0;
	Drivers = CopyDrivers(InDrivers, DriversCount);
	AddressesCount = (InAddresses && InAddressesCount > 0) ? InAddressesCount : 0;
	Addresses = CopyAddresses(InAddresses, AddressesCount);
}

Taxi::Taxi(const Taxi& Other)
{
	std::cout << "[LOG] Taxi copy constructor called. Current number of taxi's classes - " << ++Count << std::endl;

	std::strcpy(Passenger, Other.Passenger);

	DriversCount = Other.DriversCount;
	AddressesCount = Other.AddressesCount;
	
	Drivers = CopyDrivers(Other.Drivers, DriversCount);
	Addresses = CopyAddresses(Other.Addresses, AddressesCount);
}

Taxi::~Taxi()
{
	std::cout << "[LOG] Taxi destructor called. Current number of taxi's classes - " << --Count << ". Freeing memory.\n";
	delete[] Drivers;
	delete[] Addresses;
}

int Taxi::GetDriverState(int Index) const
{
	if (Index < 0 || Index >= DriversCount || !Drivers) return -1;
	return Drivers[Index];
}

void Taxi::SetDriverState(int Index, int State)
{
	if (Index < 0 || Index >= DriversCount || !Drivers) return;
	Drivers[Index] = State;
}

const char* Taxi::GetAddress(int Index) const
{
	if (Index < 0 || Index >= AddressesCount || !Addresses)
		return nullptr;
	return Addresses[Index];
}

void Taxi::SetAddress(int Index, const char* NewAddress)
{
	if (Index < 0 || Index >= AddressesCount || !Addresses || !NewAddress) return;
	std::strncpy(Addresses[Index], NewAddress, MAX_STR_LEN - 1);
	Addresses[Index][MAX_STR_LEN - 1] = '\0';
}

void Taxi::ShowPolymorphic() const
{
	std::cout << "[Taxi::ShowPolymorphic] This is a standard taxi.\n";
}

int Taxi::Order()
{
	if (!Drivers) return 0;
	int FreeDriversNum = 0;
	for (int i = 0; i < DriversCount; i++)
		if (Drivers[i] == DRIVER_FREE)
			FreeDriversNum++;
	return FreeDriversNum;
}

bool Taxi::Order(const char* InAddress)
{
	if (!Drivers || !Addresses || !InAddress || std::strlen(InAddress) == 0)
		return false;
	// confirm the address is known
	bool KnownAddress = false;
	for (int i = 0; i < AddressesCount; i++)
	{
		if (!std::strcmp(Addresses[i], InAddress))
		{
			KnownAddress = true; break;
		}
	}
	if (!KnownAddress)
		return false;
	// find a free driver
	for (int i = 0; i < DriversCount; i++)
		if (Drivers[i] == DRIVER_FREE)
			return Order(i, InAddress);
	return false; // no free drivers
}

bool Taxi::Order(int DriverIndex, const char* InAddress)
{
	if (!InAddress || !Drivers || !Addresses
		|| DriverIndex < 0 || DriverIndex >= DriversCount
		|| !std::strlen(InAddress))
		return false;
	if (Drivers[DriverIndex] != DRIVER_FREE)
		return false;
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
	std::cout << "How many drivers do you want to store: ";
	DriversCount = ReadStrictInt();
	if (DriversCount < 0)
		DriversCount = 0;
	AllocateDrivers(Drivers, DriversCount, DriversCount);
	for (int i = 0; i < DriversCount; i++)
	{
		std::cout << "Enter state for driver No." << i << " (0=FREE,1=BUSY): ";
		int State = ReadStrictInt();
		if (State != DRIVER_FREE && State != DRIVER_BUSY)
			State = DRIVER_FREE;
		Drivers[i] = State;
	}
	std::cout << "How many addresses: ";
	AddressesCount = ReadStrictInt();
	if (AddressesCount < 0)
		AddressesCount = 0;
	delete[] Addresses;
	Addresses = nullptr;
	if (AddressesCount > 0)
	{
		Addresses = new char[AddressesCount][MAX_STR_LEN];
		for (int i = 0; i < AddressesCount; i++)
		{
			std::cout << "Enter address #" << i << ": ";
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
		for (int i = 0; i < DriversCount; i++)
			std::cout << "Driver #" << i << " => "
			<< (Drivers[i] == DRIVER_FREE ? "FREE" : "BUSY")
			<< "\n";
	else
		std::cout << "No driver data.\n";
	std::cout << "AddressesCount: " << AddressesCount << "\n";
	if (Addresses)
		for (int i = 0; i < AddressesCount; i++)
			std::cout << "Address #" << i << ": " << Addresses[i] << "\n";
	else
		std::cout << "No address data.\n";
	std::cout << std::endl;
}

void Taxi::SaveToFile(const char* FileName) const
{
	std::ofstream fout(FileName);
	if (!fout)
		std::cerr << "Failed to open file for saving: " << FileName << "\n"; return;
	fout << Passenger << "\n";
	fout << DriversCount << "\n";
	for (int i = 0; i < DriversCount; i++)
		fout << Drivers[i] << "\n";
	fout << AddressesCount << "\n";
	for (int i = 0; i < AddressesCount; i++)
		fout << Addresses[i] << "\n";
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
	if (DriversCount < 0)
		DriversCount = 0;
	AllocateDrivers(Drivers, DriversCount, DriversCount);
	for (int i = 0; i < DriversCount; i++)
	{
		int st;
		fin >> st;
		if (st != DRIVER_FREE && st != DRIVER_BUSY)
			st = DRIVER_FREE;
		Drivers[i] = st;
	}
	fin >> AddressesCount;
	fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	delete[] Addresses;
	Addresses = nullptr;
	if (AddressesCount < 0) AddressesCount = 0;
	if (AddressesCount > 0)
	{
		Addresses = new char[AddressesCount][MAX_STR_LEN];
		for (int i = 0; i < AddressesCount; i++)
		{
			fin.getline(Addresses[i], MAX_STR_LEN);
			if (!std::strlen(Addresses[i]))
				std::strcpy(Addresses[i], "UnknownAddr");
		}
	}
	fin.close();
}

std::istream& operator>>(std::istream& InStream, Taxi& Obj)
{
	char Temp[MAX_STR_LEN];
	std::cout << "[Console >>] Enter passenger name (no spaces): ";
	InStream >> Temp;
	std::strncpy(Obj.Passenger, Temp, MAX_STR_LEN - 1);
	Obj.Passenger[MAX_STR_LEN - 1] = '\0';
	std::cout << "[Console >>] Drivers number: ";
	int dc;
	InStream >> dc;
	if (!InStream || dc < 0)
	{
		dc = 0;
		InStream.clear();
	}
	std::cout << dc;
	Obj.SetDriversCount(dc);
	for (int i = 0; i < Obj.GetDriversCount(); i++)
	{
		std::cout << "  Driver #" << i << " state (0=FREE,1=BUSY): ";
		int st;
		InStream >> st;
		if (!InStream || (st != DRIVER_FREE && st != DRIVER_BUSY))
		{
			st = DRIVER_FREE;
			InStream.clear();
		}
		Obj.SetDriverState(i, st);
	}
	std::cout << "[Console >>] How many addresses: ";
	int ac;
	InStream >> ac;
	if (!InStream || ac < 0)
	{
		ac = 0;
		InStream.clear();
	}
	Obj.SetAddressesCount(ac);
	InStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	for (int i = 0; i < Obj.GetAddressesCount(); i++)
	{
		std::cout << "  Enter address #" << i << ": ";
		char lineBuf[MAX_STR_LEN];
		lineBuf[0] = '\0';
		InStream.getline(lineBuf, MAX_STR_LEN);
		if (std::strlen(lineBuf) == 0)
			std::strcpy(lineBuf, "UnknownAddr");
		Obj.SetAddress(i, lineBuf);
	}
	return InStream;
}

std::ostream& operator<<(std::ostream& OutStream, const Taxi& Obj)
{
	OutStream << "\n-- Taxi Info (ostream<<) --\n"
		<< "Passenger: " << Obj.Passenger << "\n"
		<< "DriversCount: " << Obj.GetDriversCount() << "\n";
	for (int i = 0; i < Obj.GetDriversCount(); i++)
	{
		int st = Obj.GetDriverState(i);
		OutStream << "  Driver #" << i
			<< (st == DRIVER_FREE ? " => FREE" : " => BUSY")
			<< "\n";
	}
	OutStream << "AddressesCount: " << Obj.GetAddressesCount() << "\n";
	for (int i = 0; i < Obj.GetAddressesCount(); i++)
	{
		const char* addr = Obj.GetAddress(i);
		OutStream << "  Address #" << i << ": "
			<< (addr ? addr : "[NULL]") << "\n";
	}
	return OutStream;
}

std::ifstream& operator>>(std::ifstream& InFile, Taxi& Obj)
{
	if (!InFile) return InFile;
	char passengerBuf[MAX_STR_LEN];
	passengerBuf[0] = '\0';
	InFile.getline(passengerBuf, MAX_STR_LEN);
	if (std::strlen(passengerBuf) == 0)
		std::strcpy(passengerBuf, "Unknown");
	std::strncpy(Obj.Passenger, passengerBuf, MAX_STR_LEN - 1);
	Obj.Passenger[MAX_STR_LEN - 1] = '\0';
	int dc;
	InFile >> dc;
	if (!InFile || dc < 0)
	{
		dc = 0;
		InFile.clear();
	}
	Obj.SetDriversCount(dc);
	for (int i = 0; i < dc; i++)
	{
		int st;
		InFile >> st;
		if (st != DRIVER_FREE && st != DRIVER_BUSY)
			st = DRIVER_FREE;
		Obj.SetDriverState(i, st);
	}
	InFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	int ac;
	InFile >> ac;
	if (!InFile || ac < 0)
	{
		ac = 0;
		InFile.clear();
	}
	Obj.SetAddressesCount(ac);
	InFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	for (int i = 0; i < ac; i++)
	{
		char addrBuf[MAX_STR_LEN];
		addrBuf[0] = '\0';
		InFile.getline(addrBuf, MAX_STR_LEN);
		if (std::strlen(addrBuf) == 0)
			std::strcpy(addrBuf, "UnknownAddr");
		Obj.SetAddress(i, addrBuf);
	}
	return InFile;
}

std::ofstream& operator<<(std::ofstream& OutFile, const Taxi& Obj)
{
	if (!OutFile) return OutFile;
	OutFile << Obj.Passenger << "\n";
	int dc = Obj.GetDriversCount();
	OutFile << dc << "\n";
	for (int i = 0; i < dc; i++)
		OutFile << Obj.GetDriverState(i) << "\n";
	int ac = Obj.GetAddressesCount();
	OutFile << ac << "\n";
	for (int i = 0; i < ac; i++)
	{
		const char* A = Obj.GetAddress(i);
		OutFile << (A ? A : "UnknownAddr") << "\n";
	}
	return OutFile;
}

bool Taxi::operator==(const Taxi& Other) const
{
	// match passenger name & # of free drivers
	if (std::strcmp(Passenger, Other.Passenger) != 0)
		return false;
	int freeThis = 0; // compare free drivers
	for (int i = 0; i < GetDriversCount(); i++)
		if (GetDriverState(i) == DRIVER_FREE)
			freeThis++;
	int freeOther = 0;
	for (int i = 0; i < Other.GetDriversCount(); i++)
		if (Other.GetDriverState(i) == DRIVER_FREE)
			freeOther++;
	return freeThis == freeOther;
}

bool Taxi::operator<(const Taxi& Other) const
{
	// compare passenger name lexicly
	int cmp = std::strcmp(Passenger, Other.Passenger);
	if (cmp < 0) return true;
	if (cmp > 0) return false;
	int freeThis = 0; // if equal names, compare free drivers ascending
	for (int i = 0; i < GetDriversCount(); i++)
		if (GetDriverState(i) == DRIVER_FREE)
			freeThis++;
	int freeOther = 0;
	for (int j = 0; j < Other.GetDriversCount(); j++)
		if (Other.GetDriverState(j) == DRIVER_FREE)
			freeOther++;
	return freeThis < freeOther;
}

void Taxi::SetDriversCount(int NewCount)
{
	AllocateDrivers(Drivers, DriversCount, NewCount);
}

void Taxi::SetAddressesCount(int NewCount)
{
	if (NewCount < 0) NewCount = 0;
	delete[] Addresses;
	Addresses = nullptr;
	if (NewCount == 0)
	{
		AddressesCount = 0; return;
	}
	AddressesCount = NewCount;
	Addresses = new char[AddressesCount][MAX_STR_LEN];
	for (int i = 0; i < AddressesCount; i++) std::strcpy(Addresses[i], "UnknownAddr");
}
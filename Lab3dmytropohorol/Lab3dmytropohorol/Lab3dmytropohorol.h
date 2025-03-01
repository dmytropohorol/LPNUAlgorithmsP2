#pragma once

const int MAX_STR_LEN = 64;

// Possible driver states
const int DRIVER_FREE = 0;
const int DRIVER_BUSY = 1;

class Taxi
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

    const char* GetAddress(int Index) const;
    void SetAddress(int Index, const char* NewAddress);

    int GetDriversNum() const { return DriversCount; }
    int GetAddressesNum() const { return AddressesCount; }

    static const int GetCount() { return Count; };

    char Passenger[MAX_STR_LEN];

protected:
    char (*Addresses)[MAX_STR_LEN];

private:
    int* Drivers;

    int DriversCount;
    int AddressesCount;

    static int Count;
};

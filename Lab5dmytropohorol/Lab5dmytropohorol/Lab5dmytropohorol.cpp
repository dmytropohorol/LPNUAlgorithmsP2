#include "Taxi.h"
#include "LuxTaxi.h"
#include <iostream>
#include <fstream>

int ReadStrictInt();
void ReadNonEmptyString(char* Buffer);
void PressEnterToContinue();

int main()
{
    // 1) Create standard Taxi
    int sampleDrivers[3] = { DRIVER_FREE, DRIVER_BUSY, DRIVER_FREE };
    char sampleAddresses[2][MAX_STR_LEN] = { "MainStreet", "Broadway" };
    Taxi t1("Alice", sampleDrivers, 3, sampleAddresses, 2);
    t1.PrintToConsole();

    // 2) Create a LuxTaxi
    LuxTaxi t2("Bob", sampleDrivers, 3, sampleAddresses, 2);
    t2.PrintToConsole();
    t2.ShowProtectedAccess();  // show we can access 'Addresses'

    PressEnterToContinue();

    // 3) Demonstrate Polymorphism via AbstractTaxi*
    std::cout << "\n--- Polymorphism Demo with AbstractTaxi* ---\n";
    AbstractTaxi* arr[2];
    arr[0] = &t1;  // a 'Taxi'
    arr[1] = &t2;  // a 'LuxTaxi'

    for (int i = 0; i < 2; i++)
        arr[i]->ShowPolymorphic();

    PressEnterToContinue();

    // 4) Overloaded console input => Read a new Taxi from console
    Taxi t3;
    std::cout << "\nRead t3 from console using operator>> ...\n";
    std::cin >> t3;

    std::cout << "\nPrint t3 using operator<<:\n";
    std::cout << t3 << std::endl;

    // 5) Overloaded file output
    std::ofstream fout("taxi_data.txt");
    if (fout.is_open()) {
        fout << t3;
        fout.close();
        std::cout << "[MAIN] Wrote t3 to 'taxi_data.txt'.\n";
    }

    // 6) Overloaded file input
    Taxi t4;
    std::ifstream fin("taxi_data.txt");
    if (fin.is_open()) {
        fin >> t4;
        fin.close();
        std::cout << "[MAIN] Read t4 from 'taxi_data.txt'.\n";
    }
    std::cout << "Now t4 is:\n";
    std::cout << t4 << std::endl;

    // 7) Compare t1 and t4
    if (t1 == t4)
        std::cout << "t1 == t4\n";
    else if (t1 < t4)
        std::cout << "t1 < t4\n";
    else
        std::cout << "t1 > t4 or not equal\n";

    return 0;
}

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
    while (true) {
        std::cin.getline(Buffer, MAX_STR_LEN);
        if (std::strlen(Buffer) > 0) return;
        std::cout << "Invalid (empty) input. Please try again: ";
    }
}

int ReadStrictInt()
{
    char Line[MAX_STR_LEN];
    while (true) {
        std::cin.getline(Line, MAX_STR_LEN);
        if (IsPureInteger(Line)) return std::atoi(Line);
        std::cout << "Invalid integer format. Please try again: ";
    }
}

void PressEnterToContinue()
{
    std::cout << "Press ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl;
}

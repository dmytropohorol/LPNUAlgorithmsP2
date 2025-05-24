#include "Taxi.h"
#include "LuxTaxi.h"
#include "MiniTaxi.h"
#include <iostream>
#include <fstream>
#include <string>

int ReadStrictInt();
void ReadNonEmptyString(char* Buffer);
void PressEnterToContinue();
void SortByObjectNumber(IAutoNumber*[], int);
void StreamConversion(std::istream&, std::ostream&, IStringConvertible*);

int main()
{
	// A) Basic usage of Taxi
	int sampleDrivers[3] = { DRIVER_FREE, DRIVER_BUSY, DRIVER_FREE };
	char sampleAddresses[2][MAX_STR_LEN] = { "MainStreet","Broadway" };

	Taxi t1("Alice", sampleDrivers, 3, sampleAddresses, 2);
	t1.PrintToConsole();

	Taxi t2;
	std::cout << "\nRead t2 from console using operator>> ...\n";
	std::cin >> t2;
	std::cout << "\nPrint t2 using operator<<:\n" << t2 << "\n";

	// B) Additional derived classes
	MiniTaxi mt("Bob", sampleDrivers, 3, sampleAddresses, 2);
	LuxTaxi  lt("Charlie", sampleDrivers, 3, sampleAddresses, 2);

	// C) Polymorphism: store them in AbstractTaxi* array, call PrintInfo()
	//    Also, they are IAutoNumber => so can store them in an array to sort by ID
	AbstractTaxi* absArr[3];
	absArr[0] = &t1;  // a Taxi
	absArr[1] = &mt;  // a MiniTaxi
	absArr[2] = &lt;  // a LuxTaxi

	std::cout << "\n--- Polymorphic ShowPolymorphic() ---\n";
	for (AbstractTaxi* ptr : absArr)
		ptr->PrintInfo();

	// D) Show IAutoNumber usage => each has a unique ID, we can sort them
	IAutoNumber* numArr[3];
	numArr[0] = &t1; // a Taxi
	numArr[1] = &mt; // a MiniTaxi
	numArr[2] = &lt; // a LuxTaxi

	std::cout << "\n--- Unique object IDs before sorting ---\n";
	for (IAutoNumber* ptr : numArr)
		std::cout << "ID=" << ptr->GetObjectNumber() << ", total=" << ptr->GetTotalCount() << "\n";

	// sort them by ID ascending
	SortByObjectNumber(numArr, 3);
	std::cout << "--- After sorting by ID ---\n";
	for (IAutoNumber* ptr : numArr)
		std::cout << "ID=" << ptr->GetObjectNumber() << ", total=" << ptr->GetTotalCount() << "\n";

	// E) Show IStringConvertible usage => we can do input => parse => output
	IStringConvertible* convPtr = &lt; // pick the LuxTaxi
	std::cout << "\n--- Demonstrate StreamConversion(...) on LuxTaxi ---\n";
	StreamConversion(std::cin, std::cout, convPtr);

	std::cout << "\nEnd of program.\n";
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

void SortByObjectNumber(IAutoNumber* arr[], int size)
{
	// basic bubble sort or std::sort with lambda
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1 - i; j++) {
			if (arr[j]->GetObjectNumber() > arr[j + 1]->GetObjectNumber()) {
				IAutoNumber* Temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = Temp;
			}
		}
	}
}

void StreamConversion(std::istream& in, std::ostream& out, IStringConvertible* obj)
{
	out << "Enter a line to parse => ";
	std::string line;
	std::getline(in, line);
	obj->FromString(line);
	out << "Converted object => " << obj->ToString() << "\n";
}
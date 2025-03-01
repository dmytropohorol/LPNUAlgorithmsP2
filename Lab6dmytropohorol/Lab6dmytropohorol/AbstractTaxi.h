#pragma once
#include <iostream>

const int MAX_STR_LEN = 64;

class IAutoNumber
{
public:
	virtual ~IAutoNumber() = default;
	// Returns the unique ID of this object
	virtual int GetObjectNumber() const = 0;
	// Returns how many total objects of this class exist
	virtual int GetTotalCount() const = 0;
};

class IStringConvertible
{
public:
	virtual ~IStringConvertible() = default;
	// Convert object to string
	virtual std::string ToString() const = 0;
	// Parse the string to set the object's state
	virtual void FromString(const std::string& InStr) = 0;
};

class AbstractTaxi : public IAutoNumber, public IStringConvertible
{
public:
	virtual ~AbstractTaxi() = default;
	// A pure virtual function to be overridden by all derived classes
	virtual void ShowPolymorphic() const = 0;

	static const int GetCount() { return Count; };

protected:
	static int Count;
	int ObjectNumber;
};
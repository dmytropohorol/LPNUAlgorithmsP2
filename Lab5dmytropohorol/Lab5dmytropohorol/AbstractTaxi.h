#pragma once
#include <iostream>

const int MAX_STR_LEN = 64;

class AbstractTaxi
{
public:
    virtual ~AbstractTaxi() = default;
    // A pure virtual function to be overridden by all derived classes
    virtual void ShowPolymorphic() const = 0;

    static const int GetCount() { return Count; };

protected:
    static int Count;
};
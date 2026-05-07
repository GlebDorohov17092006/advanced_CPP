#include "typemap.h"
#include <iostream>
#include <string>
#include <cassert>

struct DataA
{
    std::string value;
    bool operator==(const DataA &other) const { return value == other.value; }
};
struct DataB
{
    int value;
    bool operator==(const DataB &other) const { return value == other.value; }
};

int main()
{
    typemap::TypeMap<int, double, DataA, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14159);
    myTypeMap.AddValue<DataA>({"Hello, TypeMap!"});
    myTypeMap.AddValue<DataB>({10});

    assert(myTypeMap.GetValue<int>() == 42);
    assert(myTypeMap.GetValue<double>() == 3.14159);
    assert(myTypeMap.GetValue<DataA>().value == "Hello, TypeMap!");
    assert(myTypeMap.GetValue<DataB>().value == 10);

    assert(myTypeMap.Contains<int>() == true);
    assert(myTypeMap.Contains<double>() == true);
    assert(myTypeMap.Contains<DataA>() == true);
    assert(myTypeMap.Contains<DataB>() == true);

    assert(myTypeMap.Size() == 4);
    assert(myTypeMap.Capacity() == 4);

    myTypeMap.RemoveValue<double>();
    assert(myTypeMap.Contains<double>() == false);
    assert(myTypeMap.Size() == 3);

    myTypeMap.Clear();
    assert(myTypeMap.Size() == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include "typemap.h"
#include <iostream>
#include <string>
#include <cassert>

struct DataA
{
    std::string value;

    bool operator==(const DataA &other) const
    {
        return value == other.value;
    }
};

struct DataB
{
    int value;

    bool operator==(const DataB &other) const
    {
        return value == other.value;
    }
};

struct DataC
{
    double value;

    bool operator==(const DataC &other) const
    {
        return value == other.value;
    }
};

struct DataD
{
    char value;

    bool operator==(const DataD &other) const
    {
        return value == other.value;
    }
};

void test_add_and_get()
{
    std::cout << "Test 1: Add and get elements" << std::endl;

    typemap::TypeMap<int, double, DataA, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14159);
    myTypeMap.AddValue<DataA>({"Hello, TypeMap!"});
    myTypeMap.AddValue<DataB>({10});

    assert(myTypeMap.GetValue<int>() == 42);
    assert(myTypeMap.GetValue<double>() == 3.14159);
    assert(myTypeMap.GetValue<DataA>().value == "Hello, TypeMap!");
    assert(myTypeMap.GetValue<DataB>().value == 10);

    std::cout << "  OK" << std::endl;
}

void test_contains()
{
    std::cout << "Test 2: Contains check" << std::endl;

    typemap::TypeMap<int, double, DataA, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<DataA>({"Test"});

    assert(myTypeMap.Contains<int>() == true);
    assert(myTypeMap.Contains<double>() == false);
    assert(myTypeMap.Contains<DataA>() == true);
    assert(myTypeMap.Contains<DataB>() == false);

    std::cout << "  OK" << std::endl;
}

void test_remove()
{
    std::cout << "Test 3: Remove elements" << std::endl;

    typemap::TypeMap<int, double, DataA, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14);
    myTypeMap.AddValue<DataA>({"Test"});

    assert(myTypeMap.Contains<int>() == true);
    assert(myTypeMap.Contains<double>() == true);
    assert(myTypeMap.Contains<DataA>() == true);

    myTypeMap.RemoveValue<double>();

    assert(myTypeMap.Contains<int>() == true);
    assert(myTypeMap.Contains<double>() == false);
    assert(myTypeMap.Contains<DataA>() == true);

    std::cout << "  OK" << std::endl;
}

void test_exception_on_missing()
{
    std::cout << "Test 4: Exception on missing value" << std::endl;

    typemap::TypeMap<int, double, DataA, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);

    bool exception_caught = false;
    try
    {
        myTypeMap.GetValue<double>();
    }
    catch (const std::bad_optional_access &)
    {
        exception_caught = true;
    }

    assert(exception_caught == true);

    std::cout << "  OK" << std::endl;
}

void test_try_get_value()
{
    std::cout << "Test 5: TryGetValue safe access" << std::endl;

    typemap::TypeMap<int, double, DataA, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);

    auto intValue = myTypeMap.TryGetValue<int>();
    assert(intValue.has_value() == true);
    assert(intValue->get() == 42);

    auto doubleValue = myTypeMap.TryGetValue<double>();
    assert(doubleValue.has_value() == false);

    std::cout << "  OK" << std::endl;
}

void test_size_and_capacity()
{
    std::cout << "Test 6: Size and capacity" << std::endl;

    typemap::TypeMap<int, double, DataA, DataB> myTypeMap;

    assert(myTypeMap.Capacity() == 4);
    assert(myTypeMap.Size() == 0);
    assert(myTypeMap.Empty() == true);

    myTypeMap.AddValue<int>(42);
    assert(myTypeMap.Size() == 1);
    assert(myTypeMap.Empty() == false);

    myTypeMap.AddValue<double>(3.14);
    assert(myTypeMap.Size() == 2);

    myTypeMap.AddValue<DataA>({"Test"});
    assert(myTypeMap.Size() == 3);

    myTypeMap.RemoveValue<double>();
    assert(myTypeMap.Size() == 2);

    myTypeMap.Clear();
    assert(myTypeMap.Size() == 0);
    assert(myTypeMap.Empty() == true);

    std::cout << "  OK" << std::endl;
}

void test_multiple_type_maps()
{
    std::cout << "Test 7: Multiple TypeMaps with different types" << std::endl;

    typemap::TypeMap<int, std::string> map1;
    typemap::TypeMap<double, DataC, char> map2;

    map1.AddValue<int>(100);
    map1.AddValue<std::string>("Hello");

    map2.AddValue<double>(99.99);
    map2.AddValue<DataC>({123.456});
    map2.AddValue<char>('Z');

    assert(map1.GetValue<int>() == 100);
    assert(map1.GetValue<std::string>() == "Hello");
    assert(map2.GetValue<double>() == 99.99);
    assert(map2.GetValue<DataC>().value == 123.456);
    assert(map2.GetValue<char>() == 'Z');

    assert(map1.Capacity() == 2);
    assert(map2.Capacity() == 3);

    std::cout << "  OK" << std::endl;
}

void test_const_correctness()
{
    std::cout << "Test 8: Const correctness" << std::endl;

    typemap::TypeMap<int, DataA> myTypeMap;
    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<DataA>({"Const test"});

    const auto &constMap = myTypeMap;

    assert(constMap.Contains<int>() == true);
    assert(constMap.Contains<DataA>() == true);

    assert(constMap.GetValue<int>() == 42);
    assert(constMap.GetValue<DataA>().value == "Const test");

    auto optValue = constMap.TryGetValue<int>();
    assert(optValue.has_value() == true);
    assert(optValue->get() == 42);

    std::cout << "  OK" << std::endl;
}

void test_compile_time_checks()
{
    std::cout << "Test 9: Compile-time checks" << std::endl;

    typemap::TypeMap<int, double, DataA> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14);
    myTypeMap.AddValue<DataA>({"Compile test"});

    static_assert(std::is_same_v<decltype(myTypeMap.GetValue<int>()), int &>);
    static_assert(std::is_same_v<decltype(myTypeMap.GetValue<double>()), double &>);
    static_assert(std::is_same_v<decltype(myTypeMap.GetValue<DataA>()), DataA &>);

    std::cout << "  OK" << std::endl;
}

void test_example_from_spec()
{
    std::cout << "Test 10: Example from specification" << std::endl;

    typemap::TypeMap<int, DataA, double, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14);
    myTypeMap.AddValue<DataA>({"Hello, TypeMap!"});
    myTypeMap.AddValue<DataB>({10});

    assert(myTypeMap.GetValue<int>() == 42);
    assert(myTypeMap.GetValue<double>() == 3.14);
    assert(myTypeMap.GetValue<DataA>().value == "Hello, TypeMap!");
    assert(myTypeMap.GetValue<DataB>().value == 10);

    assert(myTypeMap.Contains<int>() == true);
    assert(myTypeMap.Contains<DataA>() == true);
    assert(myTypeMap.Contains<double>() == true);
    assert(myTypeMap.Contains<DataB>() == true);

    myTypeMap.RemoveValue<double>();

    assert(myTypeMap.Contains<double>() == false);

    std::cout << "  OK" << std::endl;
}

int main()
{
    std::cout << "========== TypeMap Tests ==========" << std::endl;
    std::cout << std::endl;

    test_add_and_get();
    test_contains();
    test_remove();
    test_exception_on_missing();
    test_try_get_value();
    test_size_and_capacity();
    test_multiple_type_maps();
    test_const_correctness();
    test_compile_time_checks();
    test_example_from_spec();

    std::cout << std::endl;
    std::cout << "========== All tests passed! ==========" << std::endl;

    return 0;
}
#include <iostream>
#include <string>
#include <iomanip>
#include "test_framework.h"

int SimpleTest(void *data)
{
    std::cout << "This is simple test case" << std::endl;

    return TEST_SUCCESS;
}

const TestFamily* strings_init()
{
    TestFamily *testFamily = new TestFamily("strings", static_cast<int>(10));

    TEST_DEF(simple_test, SimpleTest);

    return testFamily;
}

#ifndef TEST_FW_H
#define TEST_FW_H

#include <iostream>
#include <list>
#include <iterator>
#include <map>

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

#define TEST_SUCCESS 0
#define TEST_FAILURE 1
#define TEST_SKIPPED 2

//Internal Errors
#define TEST_NOT_FOUND -1

typedef int (*testFunction)(void *data);

struct Test {
    std::string testName;
    void *data;
    int flags;
    int status;
    testFunction testFunc;
};

struct TestData {
public:
    int totalRun;
    int totalSkipped;
    int totalFailed;
    int totalPass;
public:
    TestData(): totalRun(0), totalSkipped(0), totalFailed(0), totalPass(0) {};
    ~TestData() {};
private:
        TestData(const TestData&);
        TestData& operator=(const TestData &);
};

#define STR_VALUE(str) #str
#define FUNCTION_NAME(str) STR_VALUE(str)

#define TEST_DEF(name, func) \
    do { \
        struct Test *testInstance = new Test; \
        std::string base = FUNCTION_NAME(name); \
        testInstance->testFunc = func; \
        testInstance->testName.assign(base); \
        testFamily->AddTest(testInstance); \
    }while(0); \

class TestFamily {
public:
    explicit TestFamily(std::string name, int maxTests): maxTests(maxTests),
        familyName(name) {};

    void ShowTests() const{
        int index = 1;

        for (auto i = testList.begin(); i != testList.end();
                ++i) {
            std::cout << index++ << " " << (*i)->testName << std::endl;
        }

        std::cout << std::endl;
    }

    const std::string& GetFamilyName() const { return familyName; }

    void AddTest(struct Test *test){
        if(!test)
            return;

        testList.push_back(test);
    }

    int RunSingleTest(const std::string& testName, struct TestData &data)
        const{
        for (auto i = testList.begin(); i != testList.end(); ++i) {

            if ((*i)->testName == testName) {

                std::cout << "Running - " << (*i)->testName << ":"
                    << std::endl;

                auto status = (*i)->testFunc(nullptr);

                data.totalRun++;
                if  (status == TEST_SUCCESS)
                    data.totalPass++;
                else if (status == TEST_FAILURE)
                    data.totalFailed++;
                else if (status == TEST_SKIPPED)
                    data.totalSkipped++;

                return status;
            }
        }

        return TEST_NOT_FOUND;
    }

    int RunSingleTest(const std::string& testName, struct TestData &data,
            int& timeDuration) const{
        for (auto i = testList.begin(); i != testList.end(); ++i) {

            if ((*i)->testName == testName) {

                std::cout << "Running - " << (*i)->testName << ":"
                    << std::endl;

                std::chrono::time_point<std::chrono::system_clock> begin =
                    std::chrono::system_clock::now();

                auto status = (*i)->testFunc(nullptr);

                std::chrono::time_point<std::chrono::system_clock> end =
                    std::chrono::system_clock::now();

                data.totalRun++;

                if  (status == TEST_SUCCESS) {

                    data.totalPass++;
                    timeDuration =
                        std::chrono::duration_cast<std::chrono::milliseconds>
                        (end - begin).count();
                }
                else if (status == TEST_FAILURE)
                    data.totalFailed++;
                else if (status == TEST_SKIPPED)
                    data.totalSkipped++;

                return status;
            }
        }

        return TEST_NOT_FOUND;
    }

    void RunAllTests(struct TestData& data) const {
        for (auto i = testList.begin(); i != testList.end(); ++i) {
            RunSingleTest((*i)->testName, data);
        }
    }

    ~TestFamily() {
        //std::cout << "Deleting: "<< familyName <<  std::endl;

        for (auto i = testList.begin(); i != testList.end(); ++i)
            delete *i;
    };

private:
    TestFamily(const TestFamily&);
    TestFamily operator=(const TestFamily &);
    std::list<const struct Test*> testList;
    int maxTests;
    std::string familyName;
};
#endif /* ifndef TEST_FW_H */

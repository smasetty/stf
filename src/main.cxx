#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "test_framework.h"
#include "test_groups.h"
#include "log.h"

typedef std::map<std::string, const TestFamily*> TestFamilies;
namespace flags {
    bool runAllTests = false;
    bool listTests = false;
    bool help = false;
    bool runSingleTest = false;
    bool profileTime = false;
    std::string familyName;
    std::string testName;
}

static bool ParseValue(const char *str, const char* flag, const char **value)
{
    const char *flagStr = ("--" + std::string(flag)).c_str();

    int flagLen = strlen(flagStr);

    if (strncmp(flagStr, str, flagLen)) return false;

    if (str[flagLen] != '\0') {
        if (value != nullptr)
            *value = &str[flagLen + 1];
        else
            return false;
    }

    return true;
}

bool ParseBool(const char *str, const char *flag)
{
    if (str == NULL || flag == NULL)
        return false;

    return ParseValue(str, flag, nullptr);
}

bool ParseString(const char*str, const char*flag, const char **strPtr)
{
    if (str == NULL || flag == NULL)
        return false;

    return ParseValue(str, flag, strPtr);
}

void PrintHelpAndExit()
{
    fprintf(stdout,
            "stf"
            "\t[--list] - List all available tests\n"
            "\t[--all] - Run all available tests\n"
            "\t[--test=group.test_name] - Run a test(test_name) from family(group)\n"
            "\t[--profile] - Profiles time take for a test in milliseconds resolution,\n\t\t\t only valid with the --test option\n"
            "\t[--help] - Print help\n");
    exit(0);
}

void ProcessTestName(const char *testName, std::vector<std::string>& tokens)
{
    std::string test(testName);

    std::string::size_type lastPos = test.find_first_not_of(".", 0);
    std::string::size_type pos = test.find_first_of(".");

    while (std::string::npos != lastPos || std::string::npos != pos)
    {
        tokens.push_back(test.substr(lastPos, pos - lastPos));
        lastPos = test.find_first_not_of(".", pos);
        pos = test.find_first_of(".", lastPos);
    }

    return;
}

void ProcessCommandLine(int argc, char *argv[])
{
    if (argc < 2){
        std::cout << "Use --help for more instructions" << std::endl;
        PrintHelpAndExit();
    }

    for (int i = 1; i < argc; i++) {
        const char *returnValue;

        if (ParseBool(argv[i], "help"))
            PrintHelpAndExit();
        else if (ParseBool(argv[i], "list"))
            flags::listTests = true;
        else if (ParseBool(argv[i], "all"))
            flags::runAllTests = true;
        else if (ParseString(argv[i], "test", &returnValue)) {
            std::vector<std::string> tokens;

            flags::runSingleTest = true;

            ProcessTestName(returnValue, tokens);

            if (tokens.size() != 2)
                PrintHelpAndExit();

            auto it = tokens.begin();
            flags::familyName.assign(*it);

            it++;
            flags::testName.assign(*it);

            //std::cout << flags::familyName << std::endl;
            //std::cout << flags::testName << std::endl;
        }
        else if(ParseString(argv[i], "profile", &returnValue)) {
            flags::profileTime = true;
        }
        else {
            PrintHelpAndExit();
        }
    }
}

void PrintRunData(const TestData &runData) {

    //TODO:: change this to use the new logging API
    std::cout << "----------------------"  << std::endl;
    std::cout << std::setw(14) << std::left << "Total tests:" << runData.totalRun << std::endl;
    std::cout << std::setw(14) << std::left << "Passed:" << runData.totalPass << std::endl;
    std::cout << std::setw(14) << std::left << "Failed:" << runData.totalFailed << std::endl;
    std::cout << std::setw(14) << std::left << "Skipped:" << runData.totalSkipped << std::endl;
    std::cout << "----------------------" << std::endl;
}

int main(int argc, char *argv[])
{
    ProcessCommandLine(argc, argv);

    TestFamilies families;
    TestData runData;

    std::unique_ptr<Logger> log = std::make_unique<Logger>();
    log->SetOutputStream(&std::cout);
    log->GetLog(LOG_HIGH)<< "This is a sample log" << std::endl;

    for (int i = 0; i < ARRAY_SIZE(groups); i++) {
        const TestFamily *family;

        family = groups[i].init();
        groups[i].name = family->GetFamilyName().c_str();
        families[groups[i].name] = family;
    }

    if (flags::listTests) {
        for (auto it = families.begin(); it != families.end(); it++) {
            const TestFamily *family = it->second;

            std::cout << "Group: " << it->first << std::endl;
            family->ShowTests();
        }

        goto done;
    }

    if (flags::runAllTests) {
        for (auto it = families.begin(); it != families.end(); it++) {
            const TestFamily *family = it->second;

            std::cout << "Group: " << it->first << std::endl;
            family->RunAllTests(runData);
        }

        goto done;
    }

    if (flags::runSingleTest) {
        for (auto it = families.begin(); it != families.end(); it++) {
            if (it->first == flags::familyName) {
                int timeDuration;
                int status;
                const TestFamily *family = it->second;

                if (flags::profileTime)
                    status = family->RunSingleTest(flags::testName,
                            runData, timeDuration);
                else
                    status = family->RunSingleTest(flags::testName,
                            runData);

                if (flags::profileTime && status != TEST_NOT_FOUND)
                    std::cout << "The test took: " << timeDuration << " ms" << \
                        std::endl;
            }
        }
    }

done:
    PrintRunData(runData);
    for (auto it = families.begin(); it != families.end(); it++) {
        delete it->second;
    }

    return 0;
}

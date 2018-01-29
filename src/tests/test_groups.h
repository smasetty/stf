#ifndef TESTGROUPS_H
#define TESTGROUPS_H value

extern const TestFamily* strings_init(void);

typedef const TestFamily* (*InitFunction)(void);

struct TestGroups {
    const char *name;
    InitFunction init;
} groups[] = 
{
    {.init = strings_init},
};

#endif /* ifndef TESTGROUPS_H */

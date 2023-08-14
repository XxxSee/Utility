#ifndef STRUCT_H
#define STRUCT_H

#include <QString>
#include <string>
#include "datautility/qfeild.h"
#include "datautility/feild.h"
#include "memorypool/memorypool.h"

struct test1
{
    QString a;
    int b;
    double c;
    char d;
};

struct test2
{
    std::string a;
    int b;
    double c;
    char d;
};
C_DECLARE_FEILD(test2)

struct test3
{
    int a;
    double b;
    char c;
};

#endif // STRUCT_H

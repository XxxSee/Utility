#ifndef MSG_H
#define MSG_H

#include "sigslot/cobject.h"
#include <string>

struct AMsg : public Msg
{
    int num;
    std::string text;
};
extern MsgType mtA;

struct BMsg : public Msg
{
    std::string name;
    std::string text;
};
extern MsgType mtB;

#endif // MSG_H

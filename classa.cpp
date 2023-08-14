#include "classa.h"

MsgType mtA = static_cast<MsgType>(CObject::registerMsg());

ClassA::ClassA()
{

}

void ClassA::test()
{
    AMsg data;
    data.mt = mtA;
    data.num = 10;
    data.text = "hello, i am ClassA";
    publish(data);
}

void ClassA::testSlot(std::string a, int b)
{
    std::cout << "ClassA: " <<a << " " << b << std::endl;
}

bool ClassA::handleMsg(const Msg &data)
{
    if (data.mt == mtB) {
        const BMsg& bMsg = static_cast<const BMsg &>(data);
        std::cout << "ClassA: " << bMsg.name << " " << bMsg.text << std::endl;
    }
    return true;
}

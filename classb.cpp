#include "classb.h"

MsgType mtB = static_cast<MsgType>(CObject::registerMsg());
ClassB::ClassB()
{

}

void ClassB::test()
{
    BMsg bMsg;
    bMsg.mt = mtB;
    bMsg.name = "bMsg";
    bMsg.text = "hello world";
    publish(bMsg);
}

void ClassB::testSlot(std::string a, double b, int c)
{
    std::cout << "ClassB: " << a << " " << b << " " << c << std::endl;
}

bool ClassB::handleMsg(const Msg &data)
{
    if (data.mt == mtA) {
        const AMsg& aMsg = static_cast<const AMsg &>(data);
        std::cout << "ClassB: " << aMsg.num << " " << aMsg.text << std::endl;
    }
    return true;
}

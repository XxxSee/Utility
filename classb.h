#ifndef CLASSB_H
#define CLASSB_H

#include "msg.h"
#include "sigslot/csignal.h"

class ClassB : public CObject
{
    C_OBJECT
public:
    ClassB();
    void test();
    void testSlot(std::string a, double b, int c);
    CSignal<void(std::string,int)> mSigB;
protected:
    bool handleMsg(const Msg &data) override;
};

#endif // CLASSB_H

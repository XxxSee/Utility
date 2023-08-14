#ifndef CLASSA_H
#define CLASSA_H

#include "msg.h"
#include "sigslot/csignal.h"

class ClassA : public CObject
{
    C_OBJECT
public:
    ClassA();
    void test();
    void testSlot(std::string a, int b);
    CSignal<void(std::string,double,int)> mSigA;
protected:
    bool handleMsg(const Msg &data) override;
};

#endif // CLASSA_H

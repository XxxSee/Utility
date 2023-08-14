#include "cobject.h"

uint64_t CObject::mType = 0;

CObject::CObject()
{

}

CObject::~CObject()
{

}

void CObject::connect(CObject *sender, MsgType mt, CObject *receiver)
{
    sender->subscribe(mt,receiver);
}

void CObject::connect(CObject *sender, MsgType mt)
{
    connect(sender,mt,this);
}

uint64_t CObject::registerMsg()
{
    mType++;
    return mType;
}

bool CObject::msg(const Msg &data)
{
    bool ret = false;
    std::lock_guard<std::mutex> locker(mMtx);
    ret = handleMsg(data);
    return ret;
}

void CObject::subscribe(MsgType , CObject *)
{
    //child override
}

void CObject::publish(const Msg &)
{
    //child override
}

bool CObject::handleMsg(const Msg &)
{
    return true;
}

void MsgActivator::active(const Msg &data)
{
    std::vector<CObject*> &subers = mSubers[data.mt];
    for(CObject *sub : subers) {
        sub->msg(data);
    }
}

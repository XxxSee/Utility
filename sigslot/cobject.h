#ifndef COBJECT_H
#define COBJECT_H

#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

#define C_OBJECT \
    MsgActivator cobject_msg_activator; \
    std::mutex cobject_msg_mtx; \
    void subscribe(MsgType mt, CObject *receiver) override \
    { \
        std::lock_guard<std::mutex> locker(cobject_msg_mtx); \
        cobject_msg_activator.mSubers[mt].push_back(receiver); \
    } \
    void publish(const Msg &data) override \
    { \
        std::lock_guard<std::mutex> locker(cobject_msg_mtx); \
        cobject_msg_activator.active(data); \
    } \

enum MsgType {mtNone=0};

struct Msg
{
    MsgType mt;
    Msg(MsgType m = mtNone):mt(m){}
};

class CObject;
class MsgActivator
{
public:
    void active(const Msg &data);
    std::map<MsgType,std::vector<CObject*>> mSubers;
};

class CObject
{
public:
    CObject();
    virtual ~CObject();
    static void connect(CObject *sender, MsgType mt, CObject *receiver);
    void connect(CObject *sender, MsgType mt);
    static uint64_t registerMsg();
    virtual bool msg(const Msg &data);
protected:
    virtual void subscribe(MsgType mt, CObject *receiver);
    virtual void publish(const Msg &data);
    virtual bool handleMsg(const Msg &data);
private:
    static uint64_t mType;
    std::mutex mMtx;
};

#endif // COBJECT_H

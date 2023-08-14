#include "cthreadpool.h"

std::unique_ptr<CThreadPool> CThreadPool::mIns(nullptr);
std::mutex gCThreadPoolMtx;

CThreadPool *CThreadPool::ins()
{
    if (!mIns.get()) {
        std::lock_guard<std::mutex> locker(gCThreadPoolMtx);
        if (!mIns.get()) {
            mIns.reset(new CThreadPool);
        }
    }
    return mIns.get();
}

CThreadPool::CThreadPool() :
    mRun(false),
    mNum(0)
{

}

CThreadPool::~CThreadPool()
{
    stop();
}

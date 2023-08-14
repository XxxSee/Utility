#include "threadpool.h"
#include <QMutex>
#include <QMutexLocker>

QScopedPointer<ThreadPool> ThreadPool::mIns(nullptr);
QMutex gThreadPoolMtx;

ThreadPool *ThreadPool::ins()
{
    if (!mIns.data()) {
        QMutexLocker locker(&gThreadPoolMtx);
        if (!mIns.data()) {
            mIns.reset(new ThreadPool);
        }
    }
    return mIns.data();
}

ThreadPool::ThreadPool(QObject *parent) :
    QObject(parent),
    mRun(false),
    mNum(0)
{
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::start(int num)
{
    num >0 ? mNum=num : mNum=10;
    stop();
    mRun.store(true);
    for (int i{0}; i<mNum; i++) {
        QSharedPointer<Thread> th(new Thread(this));
        mThread.push_back(th);
        th->start();
    }
}

void ThreadPool::append(std::function<void()> f)
{
    std::unique_lock<std::mutex> locker(mtx);
    mTask.push_back(f);
    locker.unlock();
    mcv.notify_all();
}

void ThreadPool::wait()
{
    while(mRun.load() && !mTask.empty()) {
        QThread::msleep(100);
    }
    int flag = 0;
    while (mRun.load()) {
        flag = 0;
        for (int i{0}; i<mThread.size() && mRun.load(); i++) {
            flag |= mThread[i]->mFlag;
        }
        if (flag == 0)
            break;
    }
}

void ThreadPool::stop()
{
    mRun.store(false);
    mcv.notify_all();
    for(int i{0}; i<mThread.size(); i++) {
        mThread[i]->quit();
        mThread[i]->wait();
    }
    mThread.clear();
    mTask.clear();
}

Thread::Thread(ThreadPool *parent) :
    QThread(nullptr),
    mPool(parent),
    mFlag(0)
{

}

Thread::~Thread()
{
    quit();
    wait();
}

void Thread::run()
{
    while (mPool->mRun.load()) {
        std::unique_lock<std::mutex> locker(mPool->mtx);
        mPool->mcv.wait(locker,[&](){return !mPool->mRun.load() || !mPool->mTask.empty();});
        if (!mPool->mRun.load())
            break;
        std::function<void()> fun = mPool->mTask.front();
        mPool->mTask.pop_front();
        locker.unlock();
        mFlag = 1;
        fun();
        mFlag = 0;
    }
}

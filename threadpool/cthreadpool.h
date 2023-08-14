#ifndef CTHREADPOOL_H
#define CTHREADPOOL_H

#include <thread>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

class CThreadPool
{
public:
    static CThreadPool* ins();
    CThreadPool();
    ~CThreadPool();
    void start(int num = 10)
    {
        num <=0 ? mNum=10 : mNum=num;
        stop();
        mRun.store(true);
        for (int i{0}; i<mNum; i++) {
            mThread.emplace_back(new std::thread(std::bind(&CThreadPool::run,this,i)));
            mState.push_back(0);
        }
    }
    void append(std::function<void()> f)
    {
        std::unique_lock<std::mutex> locker(mtx);
        mTask.push_back(f);
        locker.unlock();
        mcv.notify_all();
    }
    void wait()
    {
        while(mRun.load() && !mTask.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        int flag = 0;
        while (mRun.load()) {
            flag = 0;
            for (int i{0}; i<mState.size() && mRun.load(); i++) {
                flag |= mState[i];
            }
            if (flag == 0)
                break;
        }
    }
    void stop()
    {
        mRun.store(false);
        mcv.notify_all();
        for(int i{0}; i<mThread.size(); i++) {
            mThread[i]->join();
        }
        mThread.clear();
        mState.clear();
        mTask.clear();
    }
    std::atomic_bool mRun;
protected:
    void run(int index)
    {
        while (mRun.load()) {
            std::unique_lock<std::mutex> locker(mtx);
            mcv.wait(locker,[&](){return !mRun.load() || !mTask.empty();});
            if (!mRun.load())
                break;
            std::function<void()> fun = mTask.front();
            mTask.pop_front();
            locker.unlock();
            mState[index] = 1;
            fun();
            mState[index] = 0;
        }
    }
private:
    static std::unique_ptr<CThreadPool> mIns;
    int mNum;
    std::mutex mtx;
    std::condition_variable mcv;
    std::vector<std::unique_ptr<std::thread>> mThread;
    std::vector<int> mState;
    std::deque<std::function<void()>> mTask;
};

#endif // CTHREADPOOL_H

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QObject>
#include <QThread>
#include <QAtomicInteger>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QVector>
#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool;
class Thread : public QThread
{
public:
    Thread(ThreadPool *parent);
    ~Thread();
protected:
    void run() override;
private:
    friend class ThreadPool;
    ThreadPool *mPool;
    int mFlag;
};

class ThreadPool : public QObject
{
    Q_OBJECT
public:
    static ThreadPool *ins();
    explicit ThreadPool(QObject *parent = nullptr);
    ~ThreadPool();
    void start(int num = 10);
    void append(std::function<void()> f);
    void wait();
    void stop();
    QAtomicInteger<bool> mRun;
private:
    friend class Thread;
    static QScopedPointer<ThreadPool> mIns;
    int mNum;
    std::mutex mtx;
    std::condition_variable mcv;
    QVector<QSharedPointer<Thread>> mThread;
    std::deque<std::function<void()>> mTask;
};

#endif // THREADPOOL_H

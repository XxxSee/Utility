#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QBuffer>
#include "struct.h"
#include "sigslot/cobject.h"
#include "threadpool/cthreadpool.h"
#include "threadpool/threadpool.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    CObject::connect(&mClassA,mtA,&mClassB);
    CObject::connect(&mClassB,mtB,&mClassA);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_dataBtn_clicked()
{
    qDebug() << "************qt data serialize*************";
    QFeildStruct<test1> feild;
    test1 d1;
    feild.setValue(d1,"a","hello qfeild");
    feild.setValue(d1,"b",1);
    feild.setValue(d1,"c",2.2);
    feild.setValue(d1,"d",'d');
    for(int i{0}; i<feild.count(); i++) {
        qDebug() << feild.header(i) << ":" << feild.value(d1,i);
    }
    QSerialize serial;
    serial.serialize(d1);
    QSerialize deserial(serial.data());
    test1 d3;
    deserial.deserialize(d3);
    for(int i{0}; i<feild.count(); i++) {
        qDebug() << feild.header(i) << ":" << feild.value(d3,i);
    }

    qDebug() << "************cpp data serialize*************";
    FeildStruct<test2> feild2;
    test2 d2;
    feild2.setValue(d2,"a",std::string("hello feild"));
    feild2.setValue(d2,"b",1);
    feild2.setValue(d2,"c",2.2);
    feild2.setValue(d2,"d",'d');
    for(int i{0}; i<feild2.count(); i++) {
        std::cout << feild2.header(i) << " : " << feild2.value(d2,i) << std::endl;
    }
    test2 d4;
    Serialize serial3;
    serial3.serialize(d2);
    Serialize serial4(serial3.buff(),serial3.size());
    serial4.deserialize(d4);
    for(int i{0}; i<feild2.count(); i++) {
        std::cout << feild2.header(i) << " : " << feild2.value(d4,i) << std::endl;
    }
}

void test_fun(int a, std::string b)
{
    std::cout << a << " " << b << std::endl;
}

void Widget::on_sigBtn_clicked()
{
    mClassA.test();
    mClassB.test();

    CSignal<void(int, std::string)> sig;
    sig.connect(test_fun);
    sig(10,"hello world");

    mClassB.mSigB.connect(&ClassA::testSlot,&mClassA);
    mClassB.mSigB("hello ClassA",10);

    mClassA.mSigA.connect(&ClassB::testSlot,&mClassB);
    mClassA.mSigA.connect(&ClassB::testSlot,&mClassB);
    mClassA.mSigA("hello ClassB",1.1,2);
    mClassA.mSigA.disconnect(&ClassB::testSlot,&mClassB);
    mClassA.mSigA("hello ClassB2",1.1,2);
    mClassA.mSigA.disconnect(&ClassB::testSlot,&mClassB);
    mClassA.mSigA("hello ClassB3",1.1,2);
}

void cthread_fun(int index)
{
    uint64_t num = 0;
    qDebug() << QThread::currentThreadId() << "begin" << index << num;
    for (int i{0}; i<10000 && CThreadPool::ins()->mRun.load(); i++) {
        for (int j{0}; j<100000 && CThreadPool::ins()->mRun.load(); j++) {
            num++;
        }
    }
    qDebug() << QThread::currentThreadId() << "end" << index << num;
}

void thread_fun(int index)
{
    uint64_t num = 0;
    qDebug() << QThread::currentThreadId() << "begin" << index << num;
    for (int i{0}; i<10000 && ThreadPool::ins()->mRun.load(); i++) {
        for (int j{0}; j<100000 && ThreadPool::ins()->mRun.load(); j++) {
            num++;
        }
    }
    qDebug() << QThread::currentThreadId() << "end" << index << num;
}

void Widget::on_threadBtn_clicked()
{
    CThreadPool::ins()->start();
    CThreadPool::ins()->append(std::bind(&cthread_fun,1));
    CThreadPool::ins()->append(std::bind(&cthread_fun,2));
    ThreadPool::ins()->start();
    ThreadPool::ins()->append(std::bind(&thread_fun,3));
    ThreadPool::ins()->append(std::bind(&thread_fun,4));
}

MemoryPool<test3,10000> memPool;
class test_Object
{
public:
    test_Object(int a, double b):mA(a),mB(b){}
    ~test_Object(){
        qDebug() << "~test_Object";
    }
    void print() {
        qDebug() << mA << mB;
    }
private:
    int mA;
    double mB;
};

ObjectPool<test_Object,10000> objPool;

#include <cmath>
void Widget::on_memBtn_clicked()
{
    QVector<test3*> datas;
    test3 *d = nullptr;
    for(int i{0}; i<1000000; i++) {
        d = memPool.mem_malloc();
        if (d) {
            d->a = i;
            d->b = i*1.;
            d->c = 'a';
            datas << d;
        }
    }
    qDebug() << datas[999]->a << datas[9999]->a << datas[99999]->a;
    for (int i{0}; i<datas.size(); i++) {
        memPool.mem_free(datas[i]);
    }
    datas.clear();
    for(int i{0}; i<1000000; i++) {
        d = memPool.mem_malloc();
        if (d) {
            d->a = 999999-i;
            d->b = i*1.;
            d->c = 'a';
            datas << d;
        }
    }
    qDebug() << datas[999]->a << datas[9999]->a << datas[99999]->a;
    memPool.freeAll();
    datas.clear();

    test_Object *obj = objPool.construct(1,2.2);
    obj->print();
    objPool.destruct(obj);
    obj->print();
}

#pragma once

#include <QVariant>
#include <QMap>
#include <QList>
#include <QSharedPointer>
#include <QDataStream>
#include <QDebug>
#include <QBuffer>
#include <QByteArray>

template<class CT>
class QFeildStruct
{
    class BasicFeild
    {
    public:
        using PTR = QSharedPointer<BasicFeild>;
        virtual ~BasicFeild(){}
        virtual void setValue(CT &d, const QVariant &val) = 0;
        virtual QVariant getValue(const CT &d) = 0;
        virtual void write(QDataStream &stream, const CT &d) = 0;
        virtual void read(QDataStream &stream, CT &d) = 0;
    };
    template<class DT>
    class DataFeild : public BasicFeild
    {
    public:
        DataFeild(DT CT::*ptr) : mPtr(ptr){}
        void setValue(CT &d, const QVariant &val) override
        {
            d.*mPtr = val.value<DT>();
        }
        QVariant getValue(const CT &d) override
        {
            return QVariant::fromValue<DT>(d.*mPtr);
        }
        void write(QDataStream &stream, const CT &d) override
        {
            QVariant val = d.*mPtr;
            stream << val;
        }
        void read(QDataStream &stream, CT &d) override
        {
            QVariant val;
            stream >> val;
            d.*mPtr = val.value<DT>();
        }
    private:
        DT CT::*mPtr;
    };
public:
    QFeildStruct(){init();}
    ~QFeildStruct(){}
    void setValue(CT &d, const QString &name, const QVariant &val)
    {
        if (mFeilds.contains(name)) {
            mFeilds[name]->setValue(d,val);
        }
    }
    void setValue(CT &d, int index, const QVariant &val)
    {
        if (mFeildIndex.contains(index)) {
            if (mFeilds.contains(mFeildIndex[index])) {
                mFeilds[mFeildIndex[index]]->setValue(d,val);
            }
        }
    }
    QVariant value(const CT &d, const QString &name)
    {
        if (mFeilds.contains(name)) {
            return mFeilds[name]->getValue(d);
        }
        return QVariant();
    }
    QVariant value(const CT &d, int index)
    {
        if (mFeildIndex.contains(index)) {
            if (mFeilds.contains(mFeildIndex[index])) {
                return mFeilds[mFeildIndex[index]]->getValue(d);
            }
        }
        return QVariant();
    }
    QString header(int index)
    {
        if (mFeildIndex.contains(index)) {
            return mFeildIndex[index];
        }
        return QString();
    }
    int count()
    {
        return mFeilds.count();
    }
    void write(QDataStream &stream, const CT &d)
    {
        auto it = mFeilds.begin();
        for (;it!=mFeilds.end();it++){
            it.value()->write(stream,d);
        }
    }
    void read(QDataStream &stream, CT &d)
    {
        auto it = mFeilds.begin();
        for (;it!=mFeilds.end();it++){
            it.value()->read(stream,d);
        }
    }
protected:
    void init();
    template<class DT>
    void addFeild(const QString &name, DT CT::*ptr)
    {
        mFeildIndex[mFeilds.size()] = name;
        mFeilds[name] = QSharedPointer<DataFeild<DT>>(new DataFeild<DT>(ptr));
    }
private:
    QMap<QString,typename BasicFeild::PTR> mFeilds;
    QMap<int,QString> mFeildIndex;
};

template<class CT>
QDataStream& operator<<(QDataStream &stream, const CT &d)
{
    QFeildStruct<CT> feild;
    feild.write(stream,d);
    return stream;
}

template<class CT>
QDataStream& operator>>(QDataStream &stream, CT &d)
{
    QFeildStruct<CT> feild;
    feild.read(stream,d);
    return stream;
}

class QSerialize
{
public:
    QSerialize(){}
    QSerialize(const QByteArray &arr):mData(arr){}
    ~QSerialize(){}
    template<class CT>
    void serialize(const CT &d)
    {
        QBuffer buffer(&mData);
        if (buffer.open(QIODevice::WriteOnly)) {
            QDataStream stream(&buffer);
            stream << d;
            buffer.close();
        }
    }
    template<class CT>
    void deserialize(CT &d)
    {
        QBuffer buffer(&mData);
        if (buffer.open(QIODevice::ReadOnly)) {
            QDataStream stream(&buffer);
            stream >> d;
            buffer.close();
        }
    }
    QByteArray &data()
    {
        return mData;
    }
private:
    QByteArray mData;
};

#pragma once

#include <memory>
#include <string>
#include <map>
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <cassert>

#define C_DECLARE_FEILD(T) \
    template<> \
    struct StreamFeild<T> \
    { \
        using type = FeildStruct<T>; \
    }; \


template<class CT>
class StreamFeild{};

class Any
{
private:
    class BasicData
    {
    public:
        using PTR = std::unique_ptr<BasicData>;
        BasicData():p(nullptr){}
        virtual ~BasicData(){}
        virtual BasicData *clone() = 0;
        virtual void print(std::ostream &out) = 0;
        virtual void write(std::ostream &out) = 0;
        virtual void read(std::istream &in) = 0;
        char *p; //only arr head address
    };
    template<class T>
    class Data : public BasicData
    {
    public:
        Data(const T &data):mData(data){}
        BasicData *clone() override
        {
            return new Data(mData);
        }
        void print(std::ostream &out) override
        {
            out << mData;
        }
        void write(std::ostream &out) override
        {
            int size = sizeof(mData);
            out.write((char*)&size,sizeof(int));
            out.write((char*)&mData,size);
        }
        void read(std::istream &in) override
        {
            int size = 0;
            in.read((char*)&size,sizeof(int));
            in.read((char*)&mData,size);
        }
        T mData;
    };
    template<>
    class Data<std::string> : public BasicData
    {
    public:
        Data(const std::string &data):mData(data){}
        BasicData *clone() override
        {
            return new Data(mData);
        }
        void print(std::ostream &out) override
        {
            out << mData;
        }
        void write(std::ostream &out) override
        {
            size_t size = mData.length()+1;
            out.write((char*)&size,sizeof(size_t));
            out.write(mData.data(),size);
        }
        void read(std::istream &in) override
        {
            size_t size = 0;
            in.read((char*)&size,sizeof(size_t));
            mData.resize(size);
            in.read(&mData[0],size);
        }
        std::string mData;
    };
    template<int N>
    class Data<char[N]> : public BasicData
    {
    public:
        Data(const char data[N])
        {
            memcpy(mData,data,N);
            p = mData;
        }
        BasicData *clone() override
        {
            return new Data(mData);
        }
        void print(std::ostream &out) override
        {
            out << mData;
        }
        void write(std::ostream &out) override
        {
            int size = N;
            out.write((char*)&size,sizeof(int));
            out.write((char*)&mData,size);
        }
        void read(std::istream &in) override
        {
            int size = 0;
            in.read((char*)&size,sizeof(int));
            in.read((char*)&mData,size);
        }
        char mData[N];
    };
    typename BasicData::PTR mPtr;
public:
    Any():mPtr(nullptr) {}
    template<class T>
    Any(const T &data)
    {
        mPtr.reset(new Data<T>(data));
    }
    template<class T>
    Any & operator=(const T &data)
    {
        mPtr.reset(new Data<T>(data));
        return *this;
    }
    template<class T>
    Any(const Any &data)
    {
        if (data.mPtr.get()) {
            mPtr.reset(data.mPtr->clone());
        }
    }
    template<class T>
    Any & operator=(const Any &data)
    {
        if (mPtr.get() == data.mPtr.get())
            return *this;
        if (!data.mPtr.get()) {
            mPtr.reset();
            return *this;
        }
        mPtr.reset(data.mPtr->clone());
        return *this;
    }
    template<class T>
    T data() const
    {
        if (mPtr.get())
            return ((Data<T>*)mPtr.get())->mData;
        else
            return 0;
    }
    template<>
    char* data() const
    {
        if (mPtr.get())
            return mPtr->p;
        else
            return nullptr;
    }
    bool isEmpty()
    {
        return !mPtr.get();
    }
    template<class T>
    void read(std::istream &in)
    {
        if (mPtr.get()) {
            mPtr->read(in);
        }
        else {
            T tmp;
            mPtr.reset(new Data<T>(tmp));
            mPtr->read(in);
        }
    }
    template<class T>
    void write(std::ostream &out)
    {
        if (mPtr.get()) {
            mPtr->write(out);
        }
    }
    template<class T>
    static Any fromData(const T &data)
    {
        return Any(data);
    }
protected:
    friend std::ostream& operator<<(std::ostream &out, const Any &an)
    {
        if (an.mPtr.get()) {
            an.mPtr->print(out);
        }
        return out;
    }
};

template<class CT>
class FeildStruct
{
    class BasicFeild
    {
    public:
        using PTR = std::shared_ptr<BasicFeild>;
        virtual ~BasicFeild(){}
        virtual void setValue(CT &d, const Any &val) = 0;
        virtual Any getValue(const CT &d) = 0;
        virtual void read(std::istream &in, CT &d) = 0;
        virtual void write(std::ostream &out, const CT &d) = 0;
    };
    template<class DT>
    class DataFeild : public BasicFeild
    {
    public:
        DataFeild(DT CT::*ptr):mPtr(ptr){}
        void setValue(CT &d, const Any &val) override
        {
            d.*mPtr = val.data<DT>();
        }
        Any getValue(const CT &d) override
        {
            return Any::fromData<DT>(d.*mPtr);
        }
        void read(std::istream &in, CT &d) override
        {
            Any val;
            val.read<DT>(in);
            d.*mPtr = val.data<DT>();
        }
        void write(std::ostream &out, const CT &d) override
        {
            Any val = d.*mPtr;
            val.write<DT>(out);
        }
    private:
        DT CT::*mPtr;
    };
public:
    FeildStruct(){init();}
    ~FeildStruct(){}
    void setValue(CT &d, const std::string &name, const Any& val)
    {
        auto it = mFeilds.find(name);
        if (it != mFeilds.end()) {
            mFeilds[name]->setValue(d,val);
        }
    }
    void setValue(CT &d, int index, const Any& val)
    {
        auto itI = mFeildIndex.find(index);
        if (itI != mFeildIndex.end()) {
            auto it = mFeilds.find(mFeildIndex[index]);
            if (it != mFeilds.end()) {
                mFeilds[mFeildIndex[index]]->setValue(d,val);
            }
        }
    }
    Any value(const CT &d, const std::string &name)
    {
        auto it = mFeilds.find(name);
        if (it != mFeilds.end()) {
            return mFeilds[name]->getValue(d);
        }
        return Any();
    }
    Any value(const CT &d, int index)
    {
        auto itI = mFeildIndex.find(index);
        if (itI != mFeildIndex.end()) {
            auto it = mFeilds.find(mFeildIndex[index]);
            if (it != mFeilds.end()) {
                return mFeilds[mFeildIndex[index]]->getValue(d);
            }
        }
        return Any();
    }
    std::string header(int index)
    {
        auto itI = mFeildIndex.find(index);
        if (itI != mFeildIndex.end()) {
            return mFeildIndex[index];
        }
        return "";
    }
    int count()
    {
        return (int)mFeilds.size();
    }
    void read(std::istream &in, CT &d)
    {
        for (auto it = mFeilds.begin(); it != mFeilds.end(); it++) {
            it->second->read(in, d);
        }
    }
    void write(std::ostream &out, const CT &d)
    {
        for (auto it = mFeilds.begin(); it != mFeilds.end(); it++) {
            it->second->write(out, d);
        }
    }
protected:
    void init();
    template<class DT>
    void addFeild(const std::string &name, DT CT::*ptr)
    {
        mFeildIndex[mFeilds.size()] = name;
        mFeilds[name] = std::shared_ptr<DataFeild<DT>>(new DataFeild<DT>(ptr));
    }
private:
    std::map<std::string, typename BasicFeild::PTR> mFeilds;
    std::map<size_t, std::string> mFeildIndex;
};

template<class CT, class SF = typename StreamFeild<CT>::type>
std::ostream & operator<<(std::ostream &o, const CT &d) {
    SF fs;
    fs.write(o, d);
    return o;
}

template<class CT, class SF = typename StreamFeild<CT>::type>
std::istream & operator>>(std::istream &in, CT &d) {
    SF fs;
    fs.read(in, d);
    return in;
}

class Serialize
{
public:
    Serialize():mBuff(nullptr),mSize(0){}
    Serialize(char *buff, size_t size)
    {
        assert(buff);
        mBuff = new char[size];
        memset(mBuff,0,size);
        memcpy(mBuff,buff,size);
        mSize = size;
    }
    ~Serialize()
    {
        if (mBuff) {
            delete [] mBuff;
            mBuff = nullptr;
        }
    }
    template<class CT>
    void serialize(const CT &d)
    {
        if (mBuff) {
            delete [] mBuff;
            mBuff = nullptr;
        }
        std::stringbuf sb;
        std::ostream os(&sb);
        os << d;
        mBuff = new char[sb.str().length()];
        memset(mBuff,0,sb.str().length());
        sb.sgetn(mBuff,sb.str().length());
        mSize = sb.str().length();
    }
    template<class CT>
    void deserialize(CT &d)
    {
        if (mBuff) {
            std::stringbuf sb;
            sb.sputn(mBuff,mSize);
            std::istream in(&sb);
            in >> d;
        }
    }
    char* buff()
    {
        return mBuff;
    }
    size_t size()
    {
        return mSize;
    }
private:
    char* mBuff;
    size_t mSize;
};

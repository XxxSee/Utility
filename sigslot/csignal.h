#pragma once

#include <functional>
#include <map>
#include <string>

template<class FT>
class CSignal
{
public:
    using MAPPAIR = std::pair<std::string,std::function<FT>>;
    void connect(FT fun)
    {
        std::string key = typeid(FT).name();
        mFun.insert(MAPPAIR(key,fun));
    }
    void disconnect(FT fun)
    {
        std::string key = typeid(FT).name();
        auto it = mFun.find(key);
        if (it != mFun.end())
            mFun.erase(it);
    }
    void disconnect()
    {
        mFun.clear();
    }

    template<class CT>
    void connect(void (CT::*fun)(), CT *d)
    {
        std::function<FT> f = std::bind(fun,d);
        std::string key = typeid(f).name();
        mFun.insert(MAPPAIR(key,f));
    }
    template<class CT>
    void disconnect(void (CT::*fun)(), CT *d)
    {
        std::function<FT> f = std::bind(fun,d);
        std::string key = typeid(f).name();
        auto it = mFun.find(key);
        if (it != mFun.end())
            mFun.erase(it);
    }
    void operator()()
    {
        for(auto it=mFun.begin(); it!=mFun.end(); it++) {
            (it->second)();
        }
    }

    template<class CT, class Arg1>
    void connect(void (CT::*fun)(Arg1), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1);
        std::string key = typeid(f).name();
        mFun.insert(MAPPAIR(key,f));
    }
    template<class CT, class Arg1>
    void disconnect(void (CT::*fun)(Arg1), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1);
        std::string key = typeid(f).name();
        auto it = mFun.find(key);
        if (it != mFun.end())
            mFun.erase(it);
    }
    template<class Arg1>
    void operator()(Arg1 arg1)
    {
        for(auto it=mFun.begin(); it!=mFun.end(); it++) {
            (it->second)(arg1);
        }
    }

    template<class CT, class Arg1, class Arg2>
    void connect(void (CT::*fun)(Arg1, Arg2), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2);
        std::string key = typeid(f).name();
        mFun.insert(MAPPAIR(key,f));
    }
    template<class CT, class Arg1, class Arg2>
    void disconnect(void (CT::*fun)(Arg1, Arg2), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2);
        std::string key = typeid(f).name();
        auto it = mFun.find(key);
        if (it != mFun.end())
            mFun.erase(it);
    }
    template<class Arg1, class Arg2>
    void operator()(Arg1 arg1, Arg2 arg2)
    {
        for(auto it=mFun.begin(); it!=mFun.end(); it++) {
            (it->second)(arg1,arg2);
        }
    }

    template<class CT, class Arg1, class Arg2, class Arg3>
    void connect(void (CT::*fun)(Arg1, Arg2, Arg3), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
        std::string key = typeid(f).name();
        mFun.insert(MAPPAIR(key,f));
    }
    template<class CT, class Arg1, class Arg2, class Arg3>
    void disconnect(void (CT::*fun)(Arg1, Arg2, Arg3), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
        std::string key = typeid(f).name();
        auto it = mFun.find(key);
        if (it != mFun.end())
            mFun.erase(it);
    }
    template<class Arg1, class Arg2, class Arg3>
    void operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3)
    {
        for(auto it=mFun.begin(); it!=mFun.end(); it++) {
            (it->second)(arg1,arg2,arg3);
        }
    }

    template<class CT, class Arg1, class Arg2, class Arg3, class Arg4>
    void connect(void (CT::*fun)(Arg1, Arg2, Arg3, Arg4), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
        std::string key = typeid(f).name();
        mFun.insert(MAPPAIR(key,f));
    }
    template<class CT, class Arg1, class Arg2, class Arg3, class Arg4>
    void disconnect(void (CT::*fun)(Arg1, Arg2, Arg3, Arg4), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
        std::string key = typeid(f).name();
        auto it = mFun.find(key);
        if (it != mFun.end())
            mFun.erase(it);
    }
    template<class Arg1, class Arg2, class Arg3, class Arg4>
    void operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
    {
        for(auto it=mFun.begin(); it!=mFun.end(); it++) {
            (it->second)(arg1,arg2,arg3,arg4);
        }
    }

    template<class CT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
    void connect(void (CT::*fun)(Arg1, Arg2, Arg3, Arg4, Arg5), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
        std::string key = typeid(f).name();
        mFun.insert(MAPPAIR(key,f));
    }
    template<class CT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
    void disconnect(void (CT::*fun)(Arg1, Arg2, Arg3, Arg4, Arg5), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5);
        std::string key = typeid(f).name();
        auto it = mFun.find(key);
        if (it != mFun.end())
            mFun.erase(it);
    }
    template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
    void operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
    {
        for(auto it=mFun.begin(); it!=mFun.end(); it++) {
            (it->second)(arg1,arg2,arg3,arg4,arg5);
        }
    }

    template<class CT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
    void connect(void (CT::*fun)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
        std::string key = typeid(f).name();
        mFun.insert(MAPPAIR(key,f));
    }
    template<class CT, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
    void disconnect(void (CT::*fun)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), CT *d)
    {
        std::function<FT> f = std::bind(fun,d,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4,std::placeholders::_5,std::placeholders::_6);
        std::string key = typeid(f).name();
        auto it = mFun.find(key);
        if (it != mFun.end())
            mFun.erase(it);
    }
    template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
    void operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6)
    {
        for(auto it=mFun.begin(); it!=mFun.end(); it++) {
            (it->second)(arg1,arg2,arg3,arg4,arg5,arg6);
        }
    }
private:
    std::multimap<std::string,std::function<FT>> mFun;
};

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <memory>
#include <QDebug>

template<class CT, int count, int size=sizeof(CT)>
class MemoryPool
{
    union buff_node{
        buff_node *next;
        char buff[size];
    };
    struct mem_block{
        mem_block *next;
        buff_node node[count];
    };
public:
    MemoryPool() : mFreeNode(nullptr), mMemBlock(nullptr)
    {

    }
    ~MemoryPool()
    {
        mem_block *p = nullptr;
        while (mMemBlock) {
            p = mMemBlock->next;
            free(mMemBlock);
            mMemBlock = p;
        }
    }
    CT *mem_malloc()
    {
        if (!mFreeNode) {
            mem_block *newBlock = (mem_block*)malloc(sizeof(mem_block));
            newBlock->next = nullptr;
            if (!mMemBlock) {
                mMemBlock = newBlock;
            }
            else {
                mMemBlock->next = newBlock;
                mMemBlock = newBlock;
            }

            mFreeNode = &mMemBlock->node[0];
            for (int i{0}; i<count-1; i++) {
                mMemBlock->node[i].next = &mMemBlock->node[i+1];
            }
            mMemBlock->node[count-1].next = nullptr;
        }
        CT *freeNode = (CT*)mFreeNode;
        mFreeNode = mFreeNode->next;
        return freeNode;
    }
    void mem_free(void *p)
    {
        buff_node *freeNode = (buff_node*)p;
        freeNode->next = mFreeNode;
        mFreeNode = freeNode;
    }
    void freeAll()
    {
        mFreeNode = nullptr;
        mem_block *p = nullptr;
        while (mMemBlock) {
            p = mMemBlock->next;
            free(mMemBlock);
            mMemBlock = p;
        }
    }
private:
    buff_node *mFreeNode;
    mem_block *mMemBlock;
    int mSize;
};

template<class CT, int count>
class ObjectPool : protected MemoryPool<CT,count>
{
public:
    ObjectPool(){}
    ~ObjectPool(){}
    CT *construct()
    {
        CT *p = mem_malloc();
        CT *pp = new (p) CT();
        return pp;
    }
    template<class Arg1>
    CT *construct(Arg1 arg1)
    {
        CT *p = mem_malloc();
        CT *pp = new (p) CT(arg1);
        return pp;
    }
    template<class Arg1, class Arg2>
    CT *construct(Arg1 arg1, Arg2 arg2)
    {
        CT *p = mem_malloc();
        CT *pp = new (p) CT(arg1, arg2);
        return pp;
    }
    template<class Arg1, class Arg2, class Arg3>
    CT *construct(Arg1 arg1, Arg2 arg2, Arg3 arg3)
    {
        CT *p = mem_malloc();
        CT *pp = new (p) CT(arg1, arg2, arg3);
        return pp;
    }
    void destruct(void *p)
    {
        CT *pp = (CT*)p;
        pp->~CT();
        mem_free(p);
    }
};

#endif // MEMORYPOOL_H

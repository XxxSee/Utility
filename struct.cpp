#include "struct.h"
#include "datautility/qfeild.h"
#include "datautility/feild.h"

template<>
void QFeildStruct<test1>::init()
{
    addFeild("a",&test1::a);
    addFeild("b",&test1::b);
    addFeild("c",&test1::c);
    addFeild("d",&test1::d);
}

template<>
void FeildStruct<test2>::init()
{
    addFeild("a",&test2::a);
    addFeild("b",&test2::b);
    addFeild("c",&test2::c);
    addFeild("d",&test2::d);
}

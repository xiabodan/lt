//Reflex.cpp
#include <iostream>
#include <map>
#include "Reflex.h"

static map< string, ClassInfo*> *classInfoMap = NULL;
bool Object::Register(ClassInfo* ci)
{
    if (!classInfoMap)   {
        classInfoMap = new map< string, ClassInfo*>(); //这里我们是通过map来存储这个映射的。
    }
    if (ci)  {
        if (classInfoMap->find(ci->m_className) == classInfoMap->end()){
            classInfoMap->insert(map< string, ClassInfo*>::value_type(ci->m_className, ci)); // 类名 <-> classInfo
        }
    }
    return true;
}

Object* Object::CreateObject(string name)
{
    map< string, ClassInfo*>::const_iterator iter = classInfoMap->find(name);
    if (classInfoMap->end() != iter)  {
        return iter->second->CreateObject();//当传入字符串name后,通过name找到info,然后调用对应的CreatObject()即可
    }
    return NULL;
}

// new class B
class B : public Object
{
public:
    B(){ cout << hex << (long)this << " B constructor!" << endl; }
    ~B(){ cout << hex << (long)this << " B destructor!" << endl; }
    virtual ClassInfo* GetClassInfo() const{ return &ms_classinfo; }
    static Object* CreateObject() { return new B; }
protected:
    static ClassInfo ms_classinfo;
};
ClassInfo B::ms_classinfo("B", B::CreateObject);

// new class D
class D: public Object // 当增加新的类D时不用更改基础类,只需要
                       //  1.继承Object类。
                       //  2.重载一个CreatObject()函数，里面 return  new 自身类。
                       //  3.拥有一个classInfo的成员并且用类名和CreatObject初始化。
{
public:
    D(){ cout << hex << (long)this << " D constructor!" << endl; }
    ~D(){ cout << hex << (long)this << " D destructor!" << endl; }
    virtual ClassInfo* GetClassInfo() const{ return &ms_classinfo; }
    static Object* CreateObject() { return new D; }
protected:
    static ClassInfo ms_classinfo;
};
ClassInfo D::ms_classinfo("D", D::CreateObject);

// new class F use macro
class F: public Object
{
    DECLARE_CLASS(F)
public:
    F(){ cout << hex << (long)this << " F constructor!" << endl; }
    ~F(){ cout << hex << (long)this << " F destructor!" << endl; }
};
IMPLEMENT_CLASS(F)

int main() {
    Object* obj = Object::CreateObject("B");
    B* b = dynamic_cast<B*> (obj);
    cout << b->GetClassInfo()->IsDynamic() <<endl;
    delete obj;

    obj = Object::CreateObject("D");
    delete obj;

    obj = Object::CreateObject("F");
    delete obj;

    return 0;
}

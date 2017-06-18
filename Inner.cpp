#include <iostream>
#include <stddef.h>

#define METHOD_PROLOGUE(theClass, localClass) \
    theClass* pThis = ((theClass*)((char*)(this) - \
    offsetof(theClass, m_x##localClass))); \

using namespace std;

class Outer
{
public:
    Outer(){m_outerInt=0;}
private:
    int m_outerInt;
public:
    //内部类定义开始
    class Inner
    {
        public:
            Inner(){m_innerInt=1;}
        private:
            int m_innerInt;
        public:
            void DisplayIn(){cout<<m_innerInt<<endl;}
            // 在此函数中访问外部类实例数据
            void setOut()
            {
                METHOD_PROLOGUE(Outer,Inner);
                pThis->m_outerInt=10;
            }
    } m_xInner;
    //End内部类
public:
    void DisplayOut(){cout<<m_outerInt<<endl;}
};

int main()
{
    Outer out;
    out.DisplayOut();
    out.m_xInner.setOut();
    out.DisplayOut();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#define NAME_STR_LEN  32

#define offsetof(type, member) (size_t)&(((type*)0)->member)

#define container_of(ptr, type, member) ({ \
                const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct student
{
    int  id;
    char name[NAME_STR_LEN];
    int  age;
}student;
typedef struct student_info
{
    int  id;
    char name[NAME_STR_LEN];
    int  age;
    student st;
}student_info;


int main()
{
    size_t off_set = 0;
    off_set = offsetof(student_info, id);
    printf("id offset: %u\n",off_set);
    off_set = offsetof(student_info, name);
    printf("name offset: %u\n",off_set);
    off_set = offsetof(student_info, age);
    printf("age offset: %u\n",off_set);
    student_info *stu = (student_info *)malloc(sizeof(student_info));
    stu->age = 10;
    student_info *ptr = container_of(&(stu->age), student_info, age);
    student_info* st = container_of(&(stu->st), student_info, st);
    printf("age:%d\n", ptr->age);
    printf("stu address:%p\n", stu);
    printf("ptr address:%p\n", ptr);
    printf("st  address:%p\n", st );
    printf("stu->age:%p\n", &stu->age);

    return 0;
}

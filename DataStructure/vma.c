#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/resource.h>

int n[10];

int function() {
    int a[100000];
    return 1;
}

int cc = 100;

void *malloc1(size_t size);

int main() {
    char* c = malloc(1);
    char* b = malloc(100);
    char* g = malloc(100);
    char* e = malloc(100);
    int i = 0;
    void* f = malloc(1000000000);
    memset(f, 0,     1000000000-1);
    char* gg = malloc1(10);
    memset(gg, 11, 10);
    char m1 = (char)*gg;
    char m2 = (char)*gg;
    printf("gg %d %d\n", m1, m2);
    int a = cc;
    int d;
    int m[1000];
    cc = 1000;
    printf("heap add =%p  %p\n", c, b);
    function();
    struct rlimit *limit = (struct rlimit *)malloc(sizeof(struct rlimit));
    getrlimit(RLIMIT_AS, limit);
    printf("soft limit: %ld, hard limit: %ld\n", limit->rlim_cur, limit->rlim_max);
    while (1) {
        sleep(1);
    }
    return 0;
}

#define BLOCK_SIZE 24
void *first_block=NULL;

typedef struct s_block *t_block;
struct s_block {
    size_t size;  /* 数据区大小 */
    t_block next; /* 指向下个块的指针 */
    int free;     /* 是否是空闲块 */
    int padding;  /* 填充4字节，保证meta块长度为8的倍数 */
    char data[1]  /* 这是一个虚拟字段，表示数据块的第一个字节，长度不应计入meta */
};

/* First fit */
t_block find_block(t_block *last, size_t size) {
    t_block b = first_block;
    while(b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return b;
}

#define BLOCK_SIZE 24 /* 由于存在虚拟的data字段，sizeof不能正确计算meta长度，这里手工设置 */

t_block extend_heap(t_block last, size_t s) {
    t_block b;
    b = sbrk(0);
    if(sbrk(BLOCK_SIZE + s) == (void *)-1)
        return NULL;
    b->size = s;
    b->next = NULL;
    if(last)
        last->next = b;
    b->free = 0;
    return b;
}

void split_block(t_block b, size_t s) {
    t_block new;
    new = b->data + s;
    new->size = b->size - s - BLOCK_SIZE ;
    new->next = b->next;
    new->free = 1;
    b->size = s;
    b->next = new;
}

size_t align8(size_t s) {
        if(s & 0x7 == 0)
                    return s;
            return ((s >> 3) + 1) << 3;
}


/* other functions... */

void *malloc1(size_t size) {
    t_block b, last;
    size_t s;
    /* 对齐地址 */
    s = align8(size);
    if(first_block) {
        /* 查找合适的block */
        last = first_block;
        b = find_block(&last, s);
        if(b) {
            /* 如果可以，则分裂 */
            if ((b->size - s) >= ( BLOCK_SIZE + 8))
                split_block(b, s);
            b->free = 0;
        } else {
            /* 没有合适的block，开辟一个新的 */
            b = extend_heap(last, s);
            if(!b)
                return NULL;
        }
    } else {
        b = extend_heap(NULL, s);
        if(!b)
            return NULL;
        first_block = b;
    }
    return b->data;
}

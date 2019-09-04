 #include <stdio.h>
 #include <machine/malloc.h>

struct NextPage{
    struct page* next;
    bool (*condition)(void);
};

struct nextpage{
    struct NextPage* page_1;
    struct NextPage* page_2;
    struct NextPage* page_3;
    struct NextPage* page_4;
    struct NextPage* page_5;
    struct NextPage* page_6;
    struct NextPage* page_7;
    struct NextPage* page_8;
    struct NextPage* page_9;
    struct NextPage* page_10;
};


struct page{
    struct page* nextpage;
    void (*drawFun)(void);
    int id;
};


void newNextPage(){
    struct page mypage;
    struct page
    {
        /* data */
    };
    
    struct mypage.nextpage
    
}
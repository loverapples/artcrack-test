#include <iostream>
//#include<Windows.h>
#include <sys/time.h>
#include "art.c"
#include "random.h"
using namespace std;
//整型到字符型
unsigned char* int_to_char(int* a,unsigned char *key)
{
    free(key);
    unsigned char* p = (unsigned char*)a;
    key =  (unsigned char*)malloc(sizeof(char)*5);
    char s[5];
    key[0] = *(p+3);
    key[1] = *(p+2);
    key[2] = *(p+1);
    key[3] = *p;
    key[4] = '\0';
    return key;
}
int main()
{
    art_tree t;
    int res = art_tree_init(&t);
    unsigned char *key = NULL;
    uintptr_t line = 1;
    int i,a,b,count;
    int N = 7000040;
    Random r;
    timeval start,end,inittime;
    gettimeofday(&start,NULL);
    for(i = 0; i < N; i++)
    {
        res = rand()*rand()%1000000000;
        art_insert(&t,int_to_char(&res,key),4,(void*)line);
    }
    gettimeofday(&inittime,NULL);
    cout<<(inittime.tv_sec-start.tv_sec)*1000+(inittime.tv_usec-start.tv_usec)/1000<<endl;
    //init(data,N,N+100);
    for(i = 0; i < 200; i++)
    {
        do
        {
            a = r.nextInt(N);
            b = r.nextInt(N);
            if (a > b) swap(a, b);
        }
        while (a==b);
        art_view_search(&t,int_to_char(&a,key),int_to_char(&b,key));
    }
    /*
    	a = 126926;
    	b = 130990;
    	art_view_search(&t,int_to_char(&a,key),int_to_char(&b,key));*/
    gettimeofday(&end,NULL);
    cout<<(end.tv_sec-inittime.tv_sec)*1000+(end.tv_usec-inittime.tv_usec)/1000<<endl;
    cout<<"----------查询-----------"<<endl;
    cout<<t.size<<"!"<<endl;
}
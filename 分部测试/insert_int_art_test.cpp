#include <iostream>
#include "art.c"
//测试整型数据的添加到art中
 
using namespace std;
unsigned char* tt(int* a,unsigned char *u){
	free(u);
	unsigned char* p = (unsigned char*)a;
	u =  (unsigned char*)malloc(sizeof(char)*5);
	char s[5];
	u[0] = *(p+3);
	u[1] = *(p+2);
	u[2] = *(p+1);
	u[3] = *p;
	u[4] = '\0';
	printf("%d\n",*(p));
	//printf("%s\n",y);
	
	return u;
	
}
int main(){
	art_tree t;
	unsigned char *p = NULL;
    int res = art_tree_init(&t);
    char buf[512];
    int inttest[6] = {1094861636,1094927172,222,111,114,20}; 
    int i=0;
    uintptr_t line = 1;
    art_insert(&t,tt(inttest,p),4,(void*)line);
    art_insert(&t,tt(inttest+1,p),4,(void*)line);
	printf("%d\n",t.size);
	scan(t.root);
	
/*
	while(i < 6){
    	//len = strlen(ss[i]);
    	//unsigned char* p = (unsigned char*)inttest[i];
//	printf("%p\n",&inttest[i]);
    	art_insert(&t,t,4,(void*)line);
    	line++;
    	i++;
    }*/
    
}
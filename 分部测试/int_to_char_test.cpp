//整型转化为字符型，妈的，以前的无符号数白删除了，还得改回来，mmp 
#include <iostream>
using namespace std;
unsigned char* t(int* a,unsigned char *u){
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
	int a[2] = {
		1961636,9
	};
	int *p = a;
 	char *s = (char*)p;
	printf("%d\n",*s);
	s++;
	//if(*s > 0 )
	printf("%d\n",*s);
	s++;
	printf("%d\n",*s);
	s++;
	printf("%d\n",*s);
	s++;
	printf("%d\n",*s);
	unsigned char *as = NULL;
	unsigned char *sss = t(a,as);
	printf("%s\n",sss);
	
}
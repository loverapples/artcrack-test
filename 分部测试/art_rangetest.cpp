#include <iostream>
#include "art.c"
using namespace std;
//���͵��ַ��� 
unsigned char* int_to_char(int* a,unsigned char *key){
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

/*
int char_to_int(unsigned char *u){
	int result;
	result = u[3]+u[2]*256+u[1]*256*256+u[0]*256*256*256;
	return result;
}*/

int main(){
	art_tree t;
	//unsigned char *p = NULL;
    int res = art_tree_init(&t);
	int a[14] = {
		12,2,6,3,45,77,5,8,46,21,89,65,22,44
	};
	int i,*p;
	unsigned char *s,*key = NULL,*sss;
	uintptr_t line = 1;
	p = a ;
	for(i = 0;i < 14; i++){
		s = (unsigned char*)p;
		sss = int_to_char(p,key);
		art_insert(&t,sss,4,(void*)line);
		//printf("%s\n",sss);
		//printf("%d\n",char_to_int(sss));
		p++;	
	}
	printf("%d\n",t.size);
	int range[2] = {12,22};
	//printf("%c\n",int_to_char(range+1,as));
	scan(t.root);
	art_view_search(&t,int_to_char(range,key),int_to_char(range+1,key));
/*
	scan(t.root);
	art_node4 *q = (art_node4*)t.root;
	scan(q->children[0]);
	art_node4 *qq = (art_node4*)q->children[0];
	scan(qq->children[0]);*/
	

	
}
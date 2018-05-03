#include <iostream>
#include "art.c"
using namespace std;
//scanº¯Êı²âÊÔ 
int main(){
	unsigned char key[4]= {
		65,66,67,68
	};
	art_tree t;
    int res = art_tree_init(&t);
    int len,i=0,line;
    while(i < 66){
    	art_insert(&t,key,4,(void*)line);
    	key[2]++;
    	i++;
    }
    printf("%d\n",t.size);
    unsigned char low[4] = {
    	65,66,67,68
    };
    unsigned char high[4] = {
    	65,66,67,68
    }
    
	//scan(t.root);
}
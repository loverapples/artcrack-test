#include <iostream>
#include "art.c"
using namespace std;
//scanº¯Êı²âÊÔ 
int main(){
	unsigned char key[4]= {
		0,0,1,0
	};
	unsigned char key2[4]= {
		0,0,10,0
	};
	art_tree t;
    int res = art_tree_init(&t);
    int len,i=0,line;
    while(i < 66){
    	art_insert(&t,key,4,(void*)line);
    	art_insert(&t,key2,4,(void*)line);
    	key[2]++;
    	key2[3]++;
    	i++;
    }
    printf("%d\n",t.size);
    unsigned char low[4] = {
    	0,0,1,5
    };
    unsigned char high[4] = {
    	0,0,10,20
    };
    
    art_view_search(&t,low,high);
	scan(t.root);
}
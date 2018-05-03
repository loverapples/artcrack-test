#include <iostream>
#include "art.c"
using namespace std;
//带前缀的范围查找 
int main(){
	unsigned char key[4]= {
		65,66,67,68
	};
	art_tree t;
    int res = art_tree_init(&t);
    int len,i=0,line;
    while(i < 45){
    	art_insert(&t,key,4,(void*)line);
    	//key[2]+= 2;
    	key[2]++;
    	i++;
    }
    printf("%d\n",t.size);
    scan(t.root);
    printf("%d\n",t.root->partial_len);
    char low[4] = {
    	65,65,65,65
    };
    char high[4] = {
    	65,66,70,69
    };
    art_view_search(&t,low,high);
    
}
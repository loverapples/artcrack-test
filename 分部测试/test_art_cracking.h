#include <iostream>
#include <queue>
#include <deque>

#include "art.c"
#include "mycrack.h"

using namespace std;

#define INGDATASET 0
#define CHARDATASET 1
#define SCAN 0
#define SORT 1
#define CRACK 2
#define ART 3
#define ART_CRACK 4
char mytype[100][520];
uintptr_t line = 0;
//queue<char[520],deque<char[520]> t;
void load(int dataset){
	int len;
    char buf[512];
    FILE *f = fopen("word.txt", "r");
	cout<<"hello"<<endl;
    
    while (fgets(buf, sizeof buf, f)) {
        len = strlen(buf);
        buf[len-1] = '\0';
        //cout<<buf<<endl;
        strcpy(mytype[line],buf);
        line++;
    }
}


void rangeSearch(int dataset,int algo,float seleclivity){
	if(dataset == CHARDATASET){
		
	}
}

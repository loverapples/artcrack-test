#include<stdio.h>
#include <memory.h>
#include <stdlib.h>
int rangeflag[2] = {0,100};

typedef struct crackingflag{  
	int value;  
 	int location; 
	struct crackingflag *next;  
}FLAG,*pFLAG;



void crackInThree(int data[],int posL,int posH,int low,int high,int flag[]);
void crackInTwo(int data[],int posL,int posH,int med,int inc);
void crackingSearch(int low,int high);
void printFlag(struct crackingflag cf[]);
pFLAG createFlag(int lvalue,int llocation,int hvalue,int hlocation);
void addFlag();

int main(){
	int data[14] = {13,16,4,9,2,12,7,1,19,3,14,11,8,6};
	int a[6] = {2,5,8,6,3,9};
	
	//printFlag(flag);
	//crackInThree(data,0,13,10,14,rangeflag);
	//crackInTwo(data,0,13,10,0);
	crackInTwo(a,0,5,8,0);
	pFLAG p = createFlag(0,0,5,8);
	printFlag(p);
	int i ;
	for	(i = 0;i < 6;i++){
		printf("%d,",a[i]);
	}
	printf("\n");

/*
	crackInThree(data,0,13,11,14,rangeflag);
		for	(i = 0;i < 14;i++){
		printf("%d,",data[i]);
	}*/
	return 0;
}


void crackInThree(int data[],int posL,int posH,int low,int high,int flag[]){
	int x1 = posL;
	int x2 = posH;
	int x3,i;

	while(data[x2] >= high && x1 < x2){
		x2--;
	}
	x3 = x2;
		for	(i = 0;i < 14;i++){
		printf("%d,",data[i]);
	}
	printf("\n");
	while(data[x3] > low && x3 > x1){
		if(data[x3] >= high){
			int temp;
			temp = data[x2];data[x2] = data[x3];data[x3] = temp;
			//temp = x2;x2 = x3;x3 = temp; 
			x2--;
		}
		x3--;
	}
	for	(i = 0;i < 14;i++){
		printf("%d,",data[i]);
	}
	printf("\n");
	while(x1 < x3){
		if(data[x1] < low){
			x1++;
		}else{
			int temp;
			temp = data[x1];data[x1] = data[x3];data[x3] = temp;
			//x1++;
			//temp = x1;x1 = x3;x3 = temp; 
			while(data[x3] > low && x3 >= x1){
				if(data[x3] >= high){
					int temp;
					temp = data[x2];data[x2] = data[x3];data[x3] = temp;
					//temp = x2;x2 = x3;x3 = temp;  
					x2--;	
				}
				x3--;
			}
		}
	} 
//	int i;

}
void crackInTwo(int data[],int posL,int posH,int med,int inc){
	int x1 = posL;
	int x2 = posH;
	int len = sizeof(data)/sizeof(int);
	while(x1 < x2){
		if(data[x1] < med)
			x1++;
		else{
			while(data[x2] >= med && x2 >x1)
				x2--;
			int temp;
			temp = data[x1];data[x1] = data[x2];data[x2] = temp;
			x1++;
			x2--;
		}
		
	}
	printf("x1 = %d,x2 = %d\n",x1,x2);
	if(x1 == x2 && x1 == len-1){
		printf("the result is %d\n",x1);
	}else if(x1 == x2){
		printf("the result is %d\n",x1);
	}
}
void crackingSearch(int low,int high){
	
}
//打印标记位信息 
void printFlag(pFLAG p){
	p = p->next;
	while(p !=NULL){
		printf("The value is %d ,The location is %d\n",p->value,p->location);
		p = p->next;	
	}
}

void addFlag(){

}
pFLAG createFlag(int lvalue,int llocation,int hvalue,int hlocation){
	pFLAG list;
	pFLAG pHead = (pFLAG)malloc(sizeof(FLAG));
 	if(NULL == pHead)  
    {  
        printf("Memory allocation failure");  
        exit(-1);  
    }else{
    	pFLAG pTail = pHead;
    	pHead->next = NULL;
    	pFLAG p = (pFLAG)malloc(sizeof(FLAG));
    	if(NULL == p)  
    	{  
        	printf("Memory allocation failure");  
        	exit(-1);  
    	}
    	p->value = lvalue;
    	p->location = llocation;
    	p->next = NULL;
    	pTail->next = p;
    	pTail = p;
    	p = (pFLAG)malloc(sizeof(FLAG));
    	if(NULL == p)  
    	{  
        	printf("Memory allocation failure");  
        	exit(-1);  
    	}
    	p->value = hvalue;
    	p->location = hlocation;
    	p->next = NULL;
    	pTail->next = p;
    	pTail = p;
    }
    return pHead;
	 
}

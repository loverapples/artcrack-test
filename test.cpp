#include <iostream>
#include <sys/time.h>
//#include "mycrack.h"
//#include "art.c"
#include "random.h"
#include "sort.h"
#include "tester.h"
#include "art_crack.h"
#include "scan.h" 
using namespace std;
/**
*�����ֱ�Ϊ����������ѯ�������㷨����ѯģʽ 
*/
void view_query(int,int,int);
void generateDiffRandV(int a[], int n);
int *arr,N,Q,S;       
int querycount;
art_tree t;
int test[20] = {
	4,8,12,36,5,9,34,36,85,75,32,45,12,65,88,33,11,44,10,77
};
int *p = test;
int main(int argc, char *argv[]){
	char *datanum = argv[1];
	char *querynum = argv[2];
	char *algorithm = argv[3];
	char *workload = argv[4];
	char *selection = argv[5];
	
	cout<<"dataset: "<<argv[1]<<endl;
	cout<<"query:   "<<argv[2]<<endl;
	cout<<"algorithm: "<<argv[3]<<endl;
	cout<<"workload: "<<argv[4]<<endl;	
	cout<<"selection: "<<argv[5]<<endl;	
	
	N = static_cast<int>(atof(datanum));
	double s = static_cast<double>(atof(selection));
	S = static_cast<int>(N*s);
	Q = static_cast<int>(atof(querynum));
	querycount = Q;
	//��ʼ��art�� 
	
    int *tempdata = new int[N];
    int *data = new int[N];

/*
    for(int i = 0;i < N;i++)
    	if(i/2 == 0)
    		tempdata[i] = i;
   		else
   			tempdata[i] = i + 3*N;

   	//��������ƶ��������� 
   	
   	random_shuffle(tempdata,tempdata+N);
   	
    for(int i = 0;i < N;i++)
    	data[i] = tempdata[i];

/*
    for(int i = 0;i < N/2;i++)
    	data[i+N/2] = tempdata[i];*/
    	
   //	delete tempdata;
   	//������������� 



   	for(int i = 0; i < N; i++){
		data[i] = rand()*rand()%(2*N);
	}
	
   	timeval starttime,endtime,inittime;
   	Random r;
   	//int* arr;
   	int alflag;
   	gettimeofday(&starttime,NULL);
	if(strcmp(algorithm,"SORT") == 0){		
		sort_init(data,N,N+100);
		alflag = SORT;
		//print_arr();
	}else if(strcmp(algorithm,"CRACK") == 0){
		crack_init(data,N,N+100);
		alflag = CRACK;
	}else if(strcmp(algorithm,"ART") == 0){
		uintptr_t line = 1;
		unsigned char *key = NULL;
		art_tree_init(&t);
 		for(int i = 0; i < N; i++){
			art_insert(&t,int_to_char(data+i,key),4,(void*)line);
		}		
		alflag = ART;
	}else if(strcmp(algorithm,"ART_CRACK") == 0){
		art_crack_init(data,N,N+100);
		art_tree_init(&t);
		alflag = ART_CRACK;
	}else if(strcmp(algorithm,"SCAN") == 0){
		//cout<<"scan doing"<<endl;
		scan_init(data,N,N+100);
		alflag = SCAN;
	}

	int rate = 0;
	char buf[52] = {0};
	const char arr[4] = {'-', '\\', '|', '/'}; // ע�⣺'\'�ַ��ı�ʾ	
	if(strcmp(workload,"RANDOM") == 0){
		//�������ab�� 
		int a,b;
		for(int i = 0;i < Q  ;i++){
			if(i == 1)
				gettimeofday(&inittime,NULL);
			//printf("%.2lf%%\r", i * 100.0/ Q); 
		 	do {
        		a = r.nextInt(N);
        		b = r.nextInt(N);
        		if (a > b) swap(a, b);
 			} while (a == b);
 			view_query(alflag,a,b);
 			if(rate <= 100){
			 	buf[rate/2] = '#';
			 	printf("[%-50s] [%d]%% [%c]\r", buf, rate, arr[rate%4]);
			 	rate = static_cast<int>(i*1.0/Q*100);
			 }
		}
	}else if(strcmp(workload,"SEQ_OVER_W") == 0){
		//a���Ų�ѯ�������Ӷ����ӣ�bΪa-N�м�ĳ������Χ���ܻ����ص� 
		int a,b;
		for(int i = 0;i < Q  ;i++){
			a = 10 + i * 20;
    		if (a + 5 > N) break;
      		b = a + r.nextInt(N-a) + 1;
      		cout<<a<<","<<b<<endl;
      		view_query(alflag,a,b);
		}	
	}else if(strcmp(workload,"SEQ_INV_W") == 0){
		//����һ�����෴���� 
		int a,b;
		for(int i = 0;i < Q  ;i++){
			a = 10 + i * 20;
    		if (a + 5 > N) break;
      		b = a + r.nextInt(N-a) + 1;
      		view_query(alflag,N-b,N-a);
		}
	}else if(strcmp(workload,"SEQ_NO_OVER_W") == 0){
		//��ѡ��ȵ������޽��� 
		int a,b,temp = 0;
		for(int i = 0;i < Q  ;i++){
			if(i == 1)
				gettimeofday(&inittime,NULL);
			a = temp + 10;
    		if (a + 5 > N){
		    	querycount = i;
		    	break;
		    } 
      		b = a + S + 1;
      		//cout<<a<<","<<b<<endl;
      		view_query(alflag,a,b);
      		temp = b;
			if(rate <= 100){
			 	buf[rate/2] = '#';
			 	printf("[%-50s] [%d]%% [%c]\r", buf, rate, arr[rate%4]);
			 	rate = static_cast<int>(i*1.0/Q*100);
			 }
		}
		
	}else if(strcmp(workload,"CONS_RAND_W") == 0){
		//��1000��������ȷ����Χ 
		int a,b, R[1000];
		for (int i=0; i<1000; i++) R[i] = r.nextInt(N);
		for(int i = 0;i < Q  ;i++){
    		do {
      			a = R[r.nextInt(1000)];
      			b = R[r.nextInt(1000)];
    		} while (a == b);
    		if (a > b) swap(a,b);
    		view_query(alflag,a,b);
		}
	}else if(strcmp(workload,"ZOOM_IN_W") == 0){
		//��ѯÿ��С100 
		int a,b,L = N/2 - 500,R = N/2 + 500;
		for(int i = 0;i < Q  ;i++){
    		if (L<1 || R>N) break;
    		a = L; L -= 100;  // make the range bigger
    		b = R; R += 100;
    		view_query(alflag,a,b);
		}
		
	}else if(strcmp(workload,"ZOOM_OUT_W") == 0){
		//��ѯÿ�ζ�100 
		int a,b,L = N/3,R = 2*N/3;
		for(int i = 0;i < Q  ;i++){
    		if (L >= R || L<0 || R>N) break;
    		a = L; L += 100;  // make the range smaller
    		b = R; R -= 100;
    		view_query(alflag,a,b);
		}
	}else if(strcmp(workload,"SKEW_W") == 0){
		//28ԭ�� 
		int a,b;
		for(int i = 0;i < Q  ;i++){
    		if (i >= 10000) break;
    		if (i < 8000){
      			do {
        			a = r.nextInt(N/5);
        			b = r.nextInt(N/5);
      			} while (a == b);
    		} else {
      			do {
        		a = N/5 + r.nextInt((N*4)/5);
        		b = N/5 + r.nextInt((N*4)/5);
      			} while (a == b);
    		}
    		if (a > b) swap(a,b);
    		view_query(alflag,a,b);
		}
	}		
	gettimeofday(&endtime,NULL);
	cout<<endl<<"��ʼ��ʱ��: ";
	cout<<(inittime.tv_sec-starttime.tv_sec)*1000+(inittime.tv_usec-starttime.tv_usec)/1000
							       <<"ms"<<endl;	
	cout<<"��ѯʱ��: "; 
	cout<<(endtime.tv_sec-inittime.tv_sec)*1000+(endtime.tv_usec-inittime.tv_usec)/1000
	 								<<"ms"<<endl;
	//cout<<"hello world"<<endl;
	cout<<"size: "<<t.size<<endl;
	cout<<"�������"<<insertcount<<endl;
	cout<<"art��ѯ����"<<viewcount<<endl;
	cout<<"��ѯ����"<<querycount<<endl; 
	
}
void view_query(int flag,int a,int b){
	unsigned char *key1 = NULL;
	switch(flag){
		case SCAN:
			//cout<<a<<endl;
			scan_view_query(a,b);
			break;
		case SORT:
			sort_view_query(a,b);
			break;
		case CRACK:
			crack_view_query(a,b);
			break;
		case ART:			
			art_view_search(&t,int_to_char(&a,key1),int_to_char(&b,key1));
			break;
		case ART_CRACK:
			art_crack_view_query(&t,a,b);
			break;
		default:
			break;
	}
}
void generateDiffRandV(int a[], int n)
{
    int *flag =new int[n];
    static int flag_once = 0;
    int i, index;
    
    for(i = 0; i < n; i++) 
		flag[i] = i+1;
    if(!flag_once){
        srand(time(0));
        flag_once = 1;
    }
    
    for(i = 0; i < n;){
    	if(i % 10000 == 0)
    	cout<<i<<endl;
        index = rand() % n;
        if(flag[index] != 0){
            a[i++] = flag[index]-1;
            flag[index] = 0;
        }
    }
    cout<<"end"<<endl;
    delete flag;
}


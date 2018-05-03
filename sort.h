#include <math.h>
using namespace std;
//extern int *arr, sorted,  N;
int sorted,allocN;
int sort_N,*sort_arr;
void sort_init(int *a, int n, int cap){
  sort_arr = new int[allocN =sort_N= n];  // for updates expansion
  for (int i=0; i<sort_N; i++)
    sort_arr[i] = a[i];    // copy all
  sorted = 0;
}
int sort_view_query(int a, int b){
  if (!sorted){
    //n_touched += N;
    sort(sort_arr, sort_arr+sort_N);
    sorted = 1;
  }
  int temp;
  assert(a <= b);
  int i1 = std::lower_bound(sort_arr, sort_arr+sort_N, a) - sort_arr;
  int i2 = std::lower_bound(sort_arr+i1, sort_arr+sort_N, b) - sort_arr;
  for(int i = i1;i<i2;i++)
  	if(sort_arr[i]){
	  	
	  }
  //n_touched += (int) (log(N) / log(2) + 0.5);
  return i2 - i1;
}
void print_arr(){
	for (int i=0; i<sort_N; i++)
    	cout<<sort_arr[i]<<endl;
}
int* copyarr(int* arr1){
	delete arr1;
	arr1 = new int[sort_N];
 	for (int i=0; i<sort_N; i++)
    	arr1[i] = sort_arr[i];
 	return arr1;
} 

/*
int main(){
	int i,a,b,count;
	int N = 100000000;
	int *data = new int[N];
	Random r;
	DWORD start_time = GetTickCount(); 
	for(i = 0; i < N; i++){
		//if(i%1000000 == 0)
		//cout<<i<<endl;
		data[i] = rand()*rand()%200000000;
	}
	DWORD init_time = GetTickCount();
	cout<<"建立数组时间"<<init_time-start_time<<"ms!"<<endl;
	init(data,N,N+100);
	DWORD end_init_time = GetTickCount();
	cout<<"初始化时间"<<end_init_time - init_time<<"ms!"<<endl;
	for(i = 0;i < 10000  ;i++){
		 	do {
        a = r.nextInt(N);
        b = r.nextInt(N);
        if (a > b) swap(a, b);
      } while (a==b);

		count = view_query(a,b);
	}
	DWORD end_time = GetTickCount();
	cout<<"查询时间"<<end_time- end_init_time<<"ms!"<<endl;
	cout<<"----------第一次查询-----------"<<endl;	
	//ount = view_query(88, 2000);
	for(i=0;i<N;i++){		
		if(i%10 == 0)
			cout<<endl;
		cout<<arr[i]<<",";
	}
	cout<<endl;
	
	
}
*/

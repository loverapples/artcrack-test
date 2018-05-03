#include <iostream>
#include <map>
#include<Windows.h> 
#include "art.c"
#include "random.h"
using namespace std;
#ifndef REP
#define REP(i,n) for (int i=0,_n=n; i<_n; i++)
#endif

#ifndef FORE
#define FORE(i,a) for (__typeof(a.begin()) i=a.begin(); i!=a.end(); i++)
#endif
struct CIndex {
  int pos;      // the cracker index position
  int holes;    // the number of holes in front
  bool sorted;  // is this piece sorted
  int prev_pos() const { return pos - holes; }
};
typedef int value_type;                   // the data element value type (can be int or long long)
typedef map<value_type, CIndex> ci_type;  // cracker[cracker_value] = (cracker_index, cracker_holes)
typedef ci_type::iterator ci_iter;        // the iterator type for the cracker
typedef map<int,int> rangemark;           //the range of art
typedef rangemark::iterator rg_iter;
int n_cracks,n_touched; 
//定义阶段
//整型转化到字符型 
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
//art范围插入 
void view_insert(art_tree *t,int *arr,int low,int high){
	int *p = &arr[low];
	uintptr_t line = 1;
	unsigned char *key = NULL;
	for(;low < high;low++){
		art_insert(t,int_to_char(p,key),4,(void*)line);
		p++;
	}
}
int partition(value_type *arr, value_type v, int L, int R){
  return std::partition(arr+L, arr+R, bind2nd(less<value_type>(), v)) - arr;
} 
ci_iter find_piece(ci_type &ci, int N, value_type v, int &L, int &R){
  L = 0; R = N;
  //map有序，找到v的位置 
  ci_iter it = ci.lower_bound(v); 

  // handling off by one search
  if (it == ci.end()){
    if (it != ci.begin()){
      it--;
      L = it->second.pos;
      it++;
    }
  } else if (it == ci.begin()){
    if (v < it->first){
      R = it->second.prev_pos();
    } else {
      L = it->second.pos;
      it++;
      if (it != ci.end()) R = it->second.prev_pos();
    }
  } else if (v < it->first){
    R = it->second.prev_pos();
    it--;
    L = it->second.pos;
    it++;
  } else {
    L = it->second.pos;
    it++;
    if (it != ci.end()) R = it->second.prev_pos();
  }
  return it;
}
int add_crack(ci_type &ci, int &N, value_type v, int p){
  if (p==0 || p >= N) return p;
  ci_iter i = ci.lower_bound(v);
  if (i != ci.end()){
    if (i->second.pos == p) return p;
    ci_iter j = i;
    if (j->first == v) j++;
    if (j != ci.end())
      if (j->second.prev_pos() == p)
        return p;
  }
  if (i != ci.begin()){
    i--;
    if (i->second.pos == p) return p;
  }
  if (ci.count(v)){
    assert(ci[v].pos == p);
    return p;
  }
  ci[v] = (CIndex){ p,0, false };
  return p;
}
void print_crackers(ci_type &ci){
	cout<<"打印分裂标记"<<endl; 
  FORE(i,ci) fprintf(stderr,"ch[%d] = %d, %d (%d)\n",
    i->first,i->second.pos,i->second.holes,i->second.sorted);
}
void print_range(rangemark &r){
	cout<<"打印范围标记"<<endl; 
	for(map<int,int>::iterator it = r.begin();
							it != r.end();it++){
		cout<<"["<<it->first<<","<<it->second<<"]"<<endl;					
	}
}


// two way split in one scan
//对arr数组进行操作，方法是三分裂 
void split_ab(value_type *arr, int L, int R, value_type a, value_type b, int &i1, int &i2){
  i1 = L; R--;
  while (L<=R){
    while (L<=R && arr[L] < b){
      if (arr[L] < a) swap(arr[L], arr[i1++]);
      L++;
    }
    while (L<=R && arr[R] >= b) R--;
    if (L<R) swap(arr[L], arr[R]);
  }
  i2 = L;
}
//查询准备阶段 
ci_type ci; 
int *arr, N;              // the dataset array
int  msize;         // for materialization


//art初始化
art_tree t;
int res = art_tree_init(&t);
rangemark rg;



void init(int *a, int n, int cap){
  ci.clear();
  msize = 0;
  N = n;
  //marr = new int[cap];
  arr = new int[cap];     // for updates expansion
  for (int i=0; i<N; i++) arr[i] = a[i];  // copy all
}

int crack(value_type a, value_type b){
//找到范围所属的片在哪，经过修改之后，l1，l2,r1,r2
//被保存下来
	cout<<"执行分裂"<<endl;
  	int L1, R1, i1; find_piece(ci, N, a, L1, R1);
  	int L2, R2, i2; find_piece(ci, N, b, L2, R2);
	//rg_iter iterlow = rg.lower_bound(a);
	//rg_iter iterhigh = rg.upper_bound(b);
  n_touched += R1 - L1;   // examine the left tuple
  if (L1==L2){            // a and b is on the same piece
    assert(R1 == R2);
    split_ab(arr, L1,R1, a, b, i1, i2);// 3-split in one scan 
    //将范围插入art中 
	//view_insert(&t,arr,i1,i2);  
    //cout<<i1<<","<<i2<<endl;
  } else {                // a and b is on different piece
    n_touched += R2 - L2; // examine the right piece
    i1 = partition(arr, a, L1, R1);
    i2 = partition(arr, b, L2, R2);
  }
  add_crack(ci, N, a, i1);
  add_crack(ci, N, b, i2);

  return i2 - i1;    // return number of qualified tuples
}
void insert_range(int low,int high){	
	ci_iter lowloc = ci.find(low);
	ci_iter highloc = ci.find(high);
	int l = lowloc->second.pos;
	int h = highloc->second.pos;
	view_insert(&t,arr,l,h);	
} 

int view_query(int a, int b){
	
	rg_iter iterlow = rg.lower_bound(a);
	rg_iter iterhigh = rg.upper_bound(b);
	int cnt,min = a,max = b;
	ci_iter citer;
	int L1, R1, i1; 
	unsigned char *key;	
	if(iterhigh == iterlow || rg.empty()){

		if(iterhigh == rg.begin() || iterlow	== rg.end()){
			//在查询范围外，先分裂在加到树上 
			
			cnt = crack(a,b);
			cout<<"crack from "<<a<<" to "<<b<<endl;
			citer = find_piece(ci,N,a,L1,R1);
			view_insert(&t,arr,L1,R1);
			rg[a] = b;
			rg[b] = a;
		}else if(iterlow->first > iterlow->second){
			// 在查询范围内直接调用art范围查询。 
				
			art_view_search(&t,int_to_char(&a,key),int_to_char(&b,key));
		}
				
	}else{
		int l,temp,h = 0;
		l = a;
		rg_iter iter;
		if(iterlow->second < min) 
			min = iterlow->second; //为之后更新做准备
		for(iter = iterlow;iter != iterhigh;iter++){ 
			if(iter->first < iter->second ){
				h = iter->first;
				
				cnt = crack(l,h);
				citer = find_piece(ci,N,l,L1,R1);
				view_insert(&t,arr,L1,R1);
			}else{
				temp = iter->first;
				art_view_search(&t,int_to_char(&l,key),int_to_char(&temp,key));
				l = iter->first;
				if(h > l){
				cnt = crack(l,h);
				citer = find_piece(ci,N,l,L1,R1);
				view_insert(&t,arr,L1,R1);	
				}
					
			}			
		}
		if( iter == rg.end()){
			h = b ;
			cnt = crack(l,h);
			citer = find_piece(ci,N,l,L1,R1);
			view_insert(&t,arr,L1,R1);
		}else{
			if(iter->first < iter->second ){
				max = b;
				cnt = crack(l,h);
				citer = find_piece(ci,N,l,L1,R1);
				view_insert(&t,arr,L1,R1);
				
			}else{
				max = iter->first;
			}
			
		}
		rg.erase(iterlow,iterhigh);
		rg[min] = max;
		rg[max] = min; 
	}
	n_cracks += ci.size();
	return cnt;
	

}


int main(){
	int i,a,b,count;
	int N = 40000000;
	int *data = new int[N];
	Random r;
	DWORD start_time = GetTickCount(); 
	for(i = 0; i < N; i++){
		//if(i%1000000 == 0)
		//cout<<i<<endl;
		data[i] = rand()*rand()%200000000;
	}
	init(data,N,N+100);
	for(i = 0;i < 1;i++){
		 	do {
        a = r.nextInt(N);
        b = r.nextInt(N);
        if (a > b) swap(a, b);
      } while (a==b);
      count = view_query(a,b);
	}


	cout<<"----------查询-----------"<<endl;
	DWORD end_time = GetTickCount();
	cout<<end_time-start_time<<"ms!"<<endl;
	print_range(rg);
	print_crackers(ci);
	cout<<t.size<<endl;
	
/*
	cout<<"查询之后数组"<<endl; 
	for(i=0;i<N;i++)
		cout<<arr[i]<<",";
	cout<<endl;
	print_crackers(ci);
	print_range(rg);
	scan(t.root);*/
	//scan(t.root);
	//count = view_query(100,250);
/*
	cout<<"----------第二次查询-----------"<<endl;
	count = view_query(4,70);
	
	cout<<"查询之后数组"<<endl; 
	for(i=0;i<N;i++)
		cout<<arr[i]<<",";
	cout<<endl;
	print_crackers(ci);
	print_range(rg);
	
	cout<<"----------art-----------"<<endl;
	scan(t.root);
	count = view_query(22,100);
	for(i=0;i<N;i++)
		cout<<arr[i]<<",";
	cout<<endl;*/
	
}
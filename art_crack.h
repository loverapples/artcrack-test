#include <iostream>
#include <map>
#include "art.c"
#include "mycrack.h"
typedef map<int,int> rangemark;           //the range of art
typedef rangemark::iterator rg_iter;
//art范围插入 
int insertcount,viewcount;   
void view_insert(art_tree *t,int *arr,int low,int high){
	++insertcount;
	int *p = &arr[low];
	uintptr_t line = 1;
	unsigned char *key = NULL;
	for(;low < high;low++){
		art_insert(t,int_to_char(p,key),4,(void*)line);
		p++;
	}
} 



// two way split in one scan
//对arr数组进行操作，方法是三分裂 


int *art_crack_arr, art_crack_N;              // the dataset array



//art初始化
rangemark rg;



void art_crack_init(int *a, int n, int cap){
  ci.clear();
  art_crack_N = n;
  art_crack_arr = new int[cap];     // for updates expansion
  for (int i=0; i<art_crack_N; i++) art_crack_arr[i] = a[i];  // copy all
}


int art_crack(value_type a, value_type b){
//找到范围所属的片在哪，经过修改之后，l1，l2,r1,r2
//被保存下来
	//cout<<"crack from "<<a<<" to "<<b<<endl;
  	int L1, R1, i1; find_piece(ci, art_crack_N, a, L1, R1);
  	int L2, R2, i2; find_piece(ci, art_crack_N, b, L2, R2);
  if (L1==L2){            // a and b is on the same piece
    assert(R1 == R2);
    split_ab(art_crack_arr, L1,R1, a, b, i1, i2);// 3-split in one scan 
    //将范围插入art中 
	//view_insert(&t,arr,i1,i2);  
    //cout<<i1<<","<<i2<<endl;
  } else {                // a and b is on different piece
    //n_touched += R2 - L2; // examine the right piece
    i1 = partition(art_crack_arr, a, L1, R1);
    i2 = partition(art_crack_arr, b, L2, R2);
  }
  add_crack(ci, art_crack_N, a, i1);
  add_crack(ci, art_crack_N, b, i2);

  return i2 - i1;    // return number of qualified tuples
}

void insert_range(art_tree *t,int low,int high){	
	ci_iter lowloc = ci.find(low);
	ci_iter highloc = ci.find(high);
	int l = lowloc->second.pos;
	int h = highloc->second.pos;
	view_insert(t,art_crack_arr,l,h);	
} 

int art_crack_view_query(art_tree *t,int a, int b){
	
	rg_iter iterlow = rg.lower_bound(a);
	rg_iter iterhigh = rg.lower_bound(b);
	int cnt,min = a,max = b;
	ci_iter citer,iterl,iterh;
	int L1, R1, i1,L2,R2; 
	unsigned char *key;	
	if(iterhigh == iterlow || rg.empty()){

		if(iterhigh == rg.begin() || iterlow == rg.end()){
			//在查询范围外，先分裂在加到树上 			
			cnt = art_crack(a,b);
			//cout<<"*********************cnt = "<<cnt<<endl;
			citer = find_piece(ci,art_crack_N,a,L1,R1);
			if(cnt != 0)
				view_insert(t,art_crack_arr,L1,R1);
			rg[a] = b;
			rg[b] = a;
		}else if(iterlow->first > iterlow->second){
			// 在查询范围内直接调用art范围查询。 
			//cout<<"call art"<<endl;	
			++viewcount;
			art_view_search(t,int_to_char(&a,key),int_to_char(&b,key));
		}
				
	}else{
		int l,temp,h = 0;
		l = a;
		rg_iter iter;
		if(iterlow->second < min) 
			min = iterlow->second; //保存最小值
		//循环处理分开的区间 
		for(iter = iterlow;iter != iterhigh;iter++){ 
			if(iter->first < iter->second ){
				h = iter->first;
				cnt = art_crack(l,h);
				citer = find_piece(ci,art_crack_N,l,L1,R1);
				view_insert(t,art_crack_arr,L1,R1);
			}else{
				temp = iter->first;	
				++viewcount;			
				art_view_search(t,int_to_char(&l,key),int_to_char(&temp,key));
				l = iter->first;
				if(h > l){
					cnt = art_crack(l,h);
					citer = find_piece(ci,art_crack_N,l,L1,R1);
					view_insert(t,art_crack_arr,L1,R1);	
				}
					
			}			
		}
		//迭代器遍历完毕 
		if( iter == rg.end()){
			h = b ;
			cnt = art_crack(l,h);
			iterl = find_piece(ci,art_crack_N,l,L1,R1);
			if(cnt != 0)
				view_insert(t,art_crack_arr,L1,R1);
		}else{
			if(iter->first < iter->second ){
				max = b;
				cnt = art_crack(l,h);				
				citer = find_piece(ci,art_crack_N,l,L1,R1);
				view_insert(t,art_crack_arr,L1,R1);
				
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

void print_art_crack_arr(){
	for(int i = 0; i < art_crack_N; i++){
		if(i%10 == 0)
			cout<<endl;
		cout<<art_crack_arr[i]<<",";
	}
}
void print_range(rangemark &r){
	for(map<int,int>::iterator it = r.begin();
							it != r.end();it++){
		cout<<"["<<it->first<<","<<it->second<<"]"<<endl;					
	}
}
void printall(const art_tree &t,rangemark	&r){
	cout<<"数组"<<endl;
	print_art_crack_arr();
	cout<<"打印范围标记"<<endl; 
	print_range(r);
	cout<<"打印分裂标记"<<endl;
	print_crackers(ci);
	cout<<"扫描art"<<endl;
	scan(t.root);
	cout<<"art节点数: "<<t.size<<endl; 
	
}
/*
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
	*/
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
	
//}

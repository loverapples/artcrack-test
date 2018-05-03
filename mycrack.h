#include <map>
#include <set>
#include <vector>
#include "hash.h"

#ifndef REP
#define REP(i,n) for (int i=0,_n=n; i<_n; i++)
#endif

#ifndef FORE
#define FORE(i,a) for (__typeof(a.begin()) i=a.begin(); i!=a.end(); i++)
#endif
using namespace std;
//定义索引的数据结构 
struct CIndex {
  int pos;      // the cracker index position
  int holes;    // the number of holes in front
  bool sorted;  // is this piece sorted
  int prev_pos() const { return pos - holes; }
};

int QQQ,n_cracks,n_trash,n_rippled;

typedef int value_type;                   // the data element value type (can be int or long long)
typedef map<value_type, CIndex> ci_type;  // cracker[cracker_value] = (cracker_index, cracker_holes)
typedef ci_type::iterator ci_iter;        // the iterator type for the cracker

//std::partition会将区间[first,last)中的元素重新排列，满足判断条件pred的元素会被放在区间的前段，
//不满足pred的元素会被放在区间的后段。该算法不能保证元素的初始相对位置
int partition(value_type *arr, value_type v, int L, int R){
  return std::partition(arr+L, arr+R, bind2nd(less<value_type>(), v)) - arr;
}

ci_iter find_piece(ci_type &ci, int N, value_type v, int &L, int &R){
  L = 0; R = N;
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
//  fprintf(stderr, "add crack v = %d, p = %d\n", v,p);
  ci[v] = (CIndex){ p,0, false };
  return p;
}
void print_crackers(ci_type &ci){
  FORE(i,ci) fprintf(stderr,"ch[%d] = %d, %d (%d)\n",
    i->first,i->second.pos,i->second.holes,i->second.sorted);
}

void print_all(ci_type &ci, value_type *arr, int &N, multiset<int> &pending_insert, multiset<int> &pending_delete, int line){
  fprintf(stderr,"PRINT ALL %d\n",line);
  REP(i,N) fprintf(stderr,"arr[%d] = %d\n",i,arr[i]);
  print_crackers(ci);
//  FORE(i,pending_delete) fprintf(stderr,"pending_delete = %d\n",*i);
//  FORE(i,pending_insert) fprintf(stderr,"pending_insert = %d\n",*i);
  fprintf(stderr,"----\n");
}

// two way split in one scan
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


// three way cracks
void split_abc(value_type *arr, int L, int R,
    value_type a, value_type b, value_type c, int &i1, int &i2, int &i3){
  assert(a<=b && b<=c);
  i1 = i2 = L; R--;
  while (L<=R){
    while (L<=R && arr[L] < c){
      if (arr[L] < b){
        swap(arr[L], arr[i2++]);
        if (arr[i2-1] < a) swap(arr[i2-1], arr[i1++]);
      }
      L++;
    }
    while (L<=R && arr[R] >= c) R--;
    if (L<R) swap(arr[L], arr[R]);
  }
  i3 = L;
}

bool piece_is_empty(ci_type &ci, ci_iter &it2){
  if (it2 == ci.end()) return false;
  ci_iter it3 = it2; it3++;
  if (it3 == ci.end()) return false;
  int R2 = it3->second.prev_pos();

  if (it2->second.pos < R2) return false;
  assert(it2->second.pos == R2);
  int holes = it2->second.holes;
  //fprintf(stderr,"ERASE %d\n",R2);
  ci.erase(it2++);
  if (it2 != ci.end()){
    it2->second.holes += holes;
    ci_iter it3 = it2; it3++;
  }
  return true;
}
multiset<int> pins, pdel; // pending updates (insert / delete)
int *crack_arr, crack_N;              // the dataset array
ci_type ci;               // the cracker index

void crack_init(int *a, int n, int cap){
  ci.clear();
  crack_N= n;
  crack_arr = new int[cap];     // for updates expansion
  for (int i=0; i<crack_N; i++) crack_arr[i] = a[i];  // copy all
}

int crack(value_type a, value_type b){
	//cout<<"crack from "<<a<<" to "<<b<<endl;	
 // merge_ripple(ci, arr, N, pins, pdel, a, b);  // merge qualified updates
  int L1, R1, i1; find_piece(ci, crack_N, a, L1, R1);
  int L2, R2, i2; find_piece(ci, crack_N, b, L2, R2);
 // n_touched += R1 - L1;   // examine the left tuple
  if (L1==L2){            // a and b is on the same piece
    assert(R1 == R2);
    split_ab(crack_arr, L1,R1, a, b, i1, i2);  // 3-split in one scan 
  } else {                // a and b is on different piece
    //n_touched += R2 - L2; // examine the right piece
    i1 = partition(crack_arr, a, L1, R1);
    i2 = partition(crack_arr, b, L2, R2);
  }
  for(int i = i1;i < i2;i++)
  	if(crack_arr[i]){
	  	
	  }
  add_crack(ci, crack_N, a, i1);
  add_crack(ci, crack_N, b, i2);

  return i2 - i1;    // return number of qualified tuples
}

int crack_view_query(int a, int b){
  int cnt = crack(a,b);
  n_cracks += ci.size();
  return cnt;
}

int count_query(int a, int b){
  crack_view_query(a,b);

  int L=0, cnt=0;
  ci_iter it1, it2;
  if (ci.count(a)){
    it1 = ci.lower_bound(a);
    L = it1->second.pos;
  } else {
    int L1,R1;
    it1 = find_piece(ci, crack_N, a, L1, R1);
    for (int i=L1; i<R1; i++)
      if (crack_arr[i] >= a && crack_arr[i] < b) cnt++;
    L = R1;
    it1++;
  }
  assert(it1 != ci.end());
  it1++;

  if (ci.count(b)){
    it2 = ci.lower_bound(b);
  } else {
    int L2, R2;
    it2 = find_piece(ci, crack_N, b, L2, R2);
    for (int i=L2; i<R2; i++)
      if (crack_arr[i] >= a && crack_arr[i] < b) cnt++;
    assert(it1 != it2);
    it2--;
  }

  while (true){
    cnt += it1->second.prev_pos() - L;
    L = it1->second.pos;
    if (it1 == it2) break;
    it1++;
  }
  return cnt;
}
void print_arr1(){
	for (int i=0; i<crack_N; i++)
    	cout<<crack_arr[i]<<endl;
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
	
}*/

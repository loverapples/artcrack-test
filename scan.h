
int *scan_arr, scan_N;          // the dataset array

void scan_init(int *a, int n, int cap){
  scan_arr = new int[scan_N = n];    // for updates expansion
  for (int i=0; i<scan_N; i++)
    scan_arr[i] = a[i];           // copy all
}

int scan_view_query(int a, int b){
  for (int i=0; i<scan_N; i++) 
    if (scan_arr[i] >= a && scan_arr[i] < b);{
		//cout<<"hello "<<endl;		
	} 
  return 0;   
}



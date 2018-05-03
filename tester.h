//算法标记 
#define SCAN      0
#define SORT      1
#define CRACK     2
#define ART       3
#define ART_CRACK 4
//查询模式标记 
// a and b is selected uniformly at random and a < b
#define RANDOM    0
  // a will be incremented by 10 every subsequent query
  // the range may overlap with the next queries
#define SEQ_OVER_W 1
  // the opposit direction from the seq_over_w
#define SEQ_INV_W 2
 // sequential with no overlap with the subsequent query ranges
#define SEQ_NO_OVER_W 3
  // pick 1000 integers and produce range queries with endpoints 
  // using the 1000 picked integers
#define CONS_RAND_W 4
  // start at the [middle - 100500, middle + 100500), 
  // then zoom in by making the query range smaller by 100 on each query
#define ZOOM_IN_W 5
  // start at the [middle - 500, middle + 500),
  // then zoom out by making the query range larger by 100 each query
#define ZOOM_OUT_W 6
  //where 80 percent of the queries falls within 20 percent of the value range and
  //20 percent of the queries falls within the 80 percent of the value range
#define SKEW_W 7


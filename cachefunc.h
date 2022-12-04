#pragma once
#include <stdbool.h>

//cache line의 정보를 포함하는 구조체
typedef struct {
  unsigned int tag;
  bool valid;
  bool dirty;
}Cache;

//simulation 결과를 나타내는 구조체
typedef struct {
  unsigned long long load_total;
  unsigned long long store_total;
  unsigned long long load_hit;
  unsigned long long store_hit;
  unsigned long long instructions;
} SimResult;

//cache의 설정을 나타내는 구조체.
typedef struct {
  unsigned num_sets; 
  unsigned associativity; //set당 line개수
  unsigned block_size;
  unsigned write_allocate; 
  unsigned replacement_policy;
  unsigned line_size; //set당 line 개수 * 한 line의 data 크기(byte)
  unsigned cache_size; //set 개수 * set 당 line 개수 * 한 line의 data 크기(byte)
  unsigned miss_penalty;
  unsigned tag_size;
  unsigned offset_bits;
  unsigned set_size; 
} CacheInfo;



bool is_hit(Cache* cache, int associativity, int setsize, unsigned int tag, unsigned int index);
int sim(Cache* cache, CacheInfo *cacheinfo, unsigned int tag, unsigned int index, bool read);
void replacement(Cache* cache, CacheInfo *cacheinfo, unsigned int tag, unsigned int index);
void random_policy(Cache* cache, int associativity, int setsize, unsigned int tag, unsigned int index);
void FIFO_policy(Cache* cache, int associativity, int setsize, unsigned int tag, unsigned int index);

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "cachefunc.h"

int sim(Cache* cache, CacheInfo *cacheinfo, unsigned int tag, unsigned int index, bool read) {
  int misses = 0;

  // miss발생한 경우
  if(!is_hit(cache, cacheinfo->associativity, cacheinfo->set_size, tag, index)) {
    if(read || cacheinfo->write_allocate) {
      replacement(cache, cacheinfo, tag, index);
    }
    misses = 1;
  }

  return misses;
}

void replacement(Cache* cache, CacheInfo *cacheinfo, unsigned int tag, unsigned int index) {
  // Direct mapped인 경우
  if(cacheinfo->associativity==1) {
    cache[index].tag = tag;
    cache[index].valid = true;
    return;
  }

  if(cacheinfo->replacement_policy) {
    FIFO_policy(cache, cacheinfo->associativity, cacheinfo->set_size, tag, index);
  } else {
    random_policy(cache, cacheinfo->associativity, cacheinfo->set_size, tag, index);
  }
}

void random_policy(Cache* cache, int associativity, int setsize, unsigned int tag, unsigned int index) {
  
  if (associativity == 1) {
    index = 0;
  }

  srand(time(NULL));
  unsigned int offset = rand() % setsize;
  //내보낼 cache선택
  cache[index * associativity + offset].tag = tag;
  cache[index * associativity + offset].valid = true;
}

void FIFO_policy(Cache* cache, int associativity, int setsize, unsigned int tag, unsigned int index) {
  
  if (associativity == 1) {
    index = 0;
  }

  static unsigned int count = 0;
  //내보낼 cache선택
  cache[index * associativity + count].tag = tag;
  cache[index * associativity + count].valid = true;

  count = (count + 1) % setsize; // count = 0 1 2 3 4...setsize
}

bool is_hit (Cache* cache, int associativity, int setsize, unsigned int tag, unsigned int index) {

  if (associativity == 1) {
    index = 0;
  }

  for(int i = 0; i < setsize; i++)
  {
    if(cache[index * associativity + i].tag == tag && cache[index * associativity + i].valid) {
      return true;
    }
  }
  return false;
}
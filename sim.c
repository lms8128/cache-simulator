#include "sim.h"
#include "cachefunc.h"

CacheInfo sim_option(){

  CacheInfo cacheinfo;

  printf("number of sets in cache(2^n, n>=0) : ");
  scanf("%u", &cacheinfo.num_sets);
  
  printf("number of blocks in each set(2^n, n>=0) : ");
  scanf("%u", &cacheinfo.associativity);

  printf("number of bytes in each block(2^n, n>=2) : ");
  scanf("%u", &cacheinfo.block_size);

  printf("write allocate(=1) OR no write allocate(=0) : ");
  scanf("%u", &cacheinfo.write_allocate);

  printf("replacement policy (Random = 0, FIFO = 1) : ");
  scanf("%u", &cacheinfo.replacement_policy);

  cacheinfo.cache_size = cacheinfo.line_size*cacheinfo.num_sets;
  cacheinfo.line_size = cacheinfo.block_size*cacheinfo.associativity;
  cacheinfo.miss_penalty = 100;
  cacheinfo.offset_bits = (int) log2(cacheinfo.line_size);

  if (cacheinfo.associativity == 1) {
    cacheinfo.set_size = cacheinfo.num_sets;
  }
  else if (cacheinfo.associativity > 1) {
    cacheinfo.set_size = (cacheinfo.associativity > cacheinfo.num_sets) ? cacheinfo.num_sets:cacheinfo.associativity;
  }
  else {
    cacheinfo.set_size = 1;
  }

  float ratio = cacheinfo.num_sets / cacheinfo.set_size;
  int index_size = 0;

  if(ratio < 1) {
    index_size = 0;
  } else {
    index_size = (int) ceil(log2(ratio));
  }
  
  //trace파일에서 주소 크기로 32bit를 사용하고 있음
  cacheinfo.tag_size = 32 - index_size - cacheinfo.offset_bits;

  return cacheinfo;
}

SimResult simulate(CacheInfo *cacheinfo){
  SimResult result;
  result.load_total = 0;
  result.store_total = 0;
  result.load_hit = 0;
  result.store_hit = 0;
  result.instructions = 0;

  Cache *cache = malloc(sizeof(Cache)*cacheinfo->cache_size);

  //trace파일 설정
  FILE *trace_file;
  trace_file = fopen("gcc.trace", "r");

  if (trace_file == NULL) {
    fprintf(stderr, "Cannot open file.");
    exit(1);
  }

  char inst_type;
  unsigned int address;
  int inst_since_last;
  int sim_result;

  char buffer[10000];
  while(fgets(buffer, 10000, trace_file)) {

    //trace파일의 정보를 변수에 저장
    sscanf(buffer, "%c %x %d", &inst_type, &address, &inst_since_last);

    //tag, index를 계산하고 simulate
    sim_result = sim(cache, cacheinfo, calculate_tag(cacheinfo, address), calculate_index(cacheinfo, address), inst_type == 'l');

    //simulation결과를 result구조체에 저장
    if (inst_type == 'l') {
      result.load_hit += (1 - sim_result);
      result.load_total++;
    }
    else {
      result.store_hit += (1 - sim_result);
      result.store_total++;
    }

    //miss penalty를 더함
    if (sim_result) {
      result.instructions += cacheinfo->miss_penalty;
    }

    result.instructions += inst_since_last + 1;
  }

  fclose(trace_file);
  free(cache);
  return result;
}

void sim_print(SimResult results){
  printf("Total loads : %llu\n", results.load_total);
  printf("Total stores : %llu\n", results.store_total);
  printf("Load hits : %llu\n", results.load_hit);
  printf("Load misses : %llu\n", results.load_total - results.load_hit);
  printf("Store hits : %llu\n", results.store_hit);
  printf("Store misses : %llu\n", results.store_total - results.store_hit);
  printf("Total cycles : %llu\n", results.instructions);
}

unsigned int calculate_tag(CacheInfo *cacheinfo, unsigned int address){
  return address >> (32 - cacheinfo->tag_size);
}


unsigned int calculate_index(CacheInfo *cacheinfo, unsigned int address){

  if ((cacheinfo->tag_size + cacheinfo->offset_bits) == 32) {
    return 0;
  }

  address <<= cacheinfo->tag_size;
  return address >> (cacheinfo->offset_bits + cacheinfo->tag_size);
}
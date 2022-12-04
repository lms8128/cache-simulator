#include "cachefunc.h"
#include "sim.h"
#include <string.h>

int main(int argc, char *argv[])
{
  CacheInfo cacheinfo;
  SimResult results;

  cacheinfo = sim_option();
  results = simulate(&cacheinfo);
  sim_print(results);  
  
  return 0;
}

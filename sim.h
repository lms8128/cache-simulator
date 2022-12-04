#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "cachefunc.h"

CacheInfo sim_option();
SimResult simulate(CacheInfo *cacheinfo);
unsigned int calculate_tag(CacheInfo *cacheinfo, unsigned int address);
unsigned int calculate_index(CacheInfo *cacheinfo, unsigned int address);
void sim_print(SimResult results);
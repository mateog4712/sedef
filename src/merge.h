/// 786

/******************************************************************************/

#pragma once

/******************************************************************************/

#include <string>

#include "common.h"
#include "hit.h"

/******************************************************************************/

std::vector<Hit> merge(std::vector<Hit> &hits, const int merge_dist);

void merge_main(int argc, char **argv);


#ifndef LAPTIMER_SPLITS_H_
#define LAPTIMER_SPLITS_H_

#include "lapTimer_types.h"


void addSplit(SplitGroup_t* splits, int64_t time);
void removeSplit(SplitGroup_t* splits, uint8_t position);

#endif
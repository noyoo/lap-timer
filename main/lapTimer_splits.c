#include "lapTimer_splits.h"

void addSplit(SplitGroup_t* splits, int64_t time) {
    splits->_position++;
    splits->_split[splits->_position] = time;
}

void removeSplit(SplitGroup_t* splits, uint8_t position) {
    for (int i = position; i < splits->_position; i++) {
        splits->_split[i] = splits->_split[i + 1];
    }
    splits->_position--;
}

void clearAllSplits(SplitGroup_t* splits){
    splits->_position = -1;
}
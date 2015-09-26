#ifndef   _HISTORY_HEURISTIC_H
#define   _HISTORY_HEURISTIC_H

#include "ChessDefine.h"

class HistoryHeuristic {

public:
    HistoryHeuristic();
    virtual ~HistoryHeuristic();

    void ResetHistoryTable();

    int GetHistoryScore(STONEMOVE *move);
    void EnterHistoryScore(STONEMOVE *move, int depth);

    void MergeSort(STONEMOVE *source, int n);

protected:
    void MergePass(STONEMOVE *source, STONEMOVE *target, int s, int n);
    void Merge(STONEMOVE *source, STONEMOVE *target, int l,int m, int r);
    
    STONEMOVE m_TargetBuff[GRID_COUNT]; 
    int m_HistoryTable[GRID_NUM][GRID_NUM];

};


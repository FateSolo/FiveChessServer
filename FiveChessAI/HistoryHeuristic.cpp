#include "HistoryHeuristic.h"

HistoryHeuristic::HistoryHeuristic() {

}

HistoryHeuristic::~HistoryHeuristic() {

}

void HistoryHeuristic::ResetHistoryTable() {
    memset(m_HistoryTable, 10, GRID_COUNT * sizeof(int));
}

void HistoryHeuristic::GetHistoryScore(STONEMOVE *move) {
    move->score = m_HistoryTable[move->x][move->y];
}

void HistoryHeuristic::EnterHistoryScore(STONEMOVE *move, int depth) {
    m_HistoryTable[move->x][move->y] += 2 << depth;
}

void HistoryHeuristic::MergeSort(STONEMOVE *source, int n) {
    int s = 1;

    while(s < n) {
        MergePass(source, m_TargetBuff, s, n);
        s += s;
        
        MergePass(m_TargetBuff, source, s, n);
        s += s;
    }
}

void HistoryHeuristic::MergePass(STONEMOVE *source, STONEMOVE *target, int s, int n) {
    int i = 0;
    
    while(i <= n - 2 * s) {
        Merge(source, target, i, i + s - 1, i + 2 * s - 1);
        i = i + 2 * s;
    }
    
    if (i + s < n) {
        Merge(source, target, i, i + s - 1, n - 1);
    }
    else {
        for (int j = i; j <= n - 1; j++) {
            target[j] = source[j];
        }
    }
}

void HistoryHeuristic::Merge(STONEMOVE *source, STONEMOVE *target, int l, int m, int r) {
    int i = l;
    int j = m + 1;
    int k = l;
        
    while((i <= m) && (j <= r)) {
        if (source[i].score >= source[j].score) {
            target[k++] = source[i++];
        }
        else {
            target[k++] = source[j++];
        }
    }
    
    if(i > m) {
        for (int q = j; q <= r; q++) {
            target[k++] = source[q];
        }
    }
    else { 
        for (int q = i; q <= m; q++) {
            target[k++] = source[q];
        }
    }
}


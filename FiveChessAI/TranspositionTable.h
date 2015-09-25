#ifndef   _TRANSPOSITION_TABLE_H
#define   _TRANSPOSITION_TABLE_H

#include "ChessDefine.h"

class TranspositionTable {

public:
    TranspositionTable();
    virtual ~TranspositionTable();

    void CalculateInitHashKey(unsigned char CurPosition[GRID_NUM][GRID_NUM]);

    void Hash_MakeMove(STONEMOVE* move, unsigned char CurPosition[GRID_NUM][GRID_NUM]);
    void Hash_UnMakeMove(STONEMOVE* move, unsigned char CurPosition[GRID_NUM][GRID_NUM]);

    int LookUpHashTable(int alpha, int beta, int depth, int TableNo);
    void EnterHashTable(ENTRY_TYPE entry_type, int eval, int depth, int TableNo);

protected:
    void InitializeHashKey();

    HashItem *m_pTT[2];

    unsigned int m_nHashKey32[2][GRID_NUM][GRID_NUM];
    unsigned long m_ulHashKey64[2][GRID_NUM][GRID_NUM];

    unsigned int m_HashKey32;
    unsigned long m_HashKey64;

};

#endif


#include "TranspositionTable.h"

unsigned int rand32() {
    return rand() ^ ((unsigned int) rand() << 31);
}

unsigned long rand64() {
    return rand() ^ ((unsigned int) rand() << 31) ^ ((unsigned long) rand() << 62);
}

TranspositionTable::TranspositionTable() {
    InitializeHashKey();
}

TranspositionTable::~TranspositionTable() {
    delete m_pTT[0];
    delete m_pTT[1];
}

void TranspositionTable::InitializeHashKey() {    
    srand((unsigned int) time(0));
    
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < GRID_NUM; i++) {
            for (int j = 0; j < GRID_NUM; j++) {
                m_nHashKey32[k][i][j] = rand32();
                m_ulHashKey64[k][i][j] = rand64();
            }
        }
    }

    m_pTT[0] = new HashItem[1024 * 1024];
    m_pTT[1] = new HashItem[1024 * 1024];
}

void TranspositionTable::CalculateInitHashKey(unsigned char CurPosition[GRID_NUM][GRID_NUM]) {
    int nStoneType;

    m_HashKey32 = 0;
    m_HashKey64 = 0;

    for (int i = 0; i < GRID_NUM; i++) {
        for (int j = 0; j < GRID_NUM; j++) {
            nStoneType = CurPosition[i][j];

            if (nStoneType != NOSTONE) {
                m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nStoneType][i][j]; 
                m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nStoneType][i][j]; 
            }
        }
    }
}

void TranspositionTable::Hash_MakeMove(STONEMOVE* move, unsigned char CurPosition[GRID_NUM][GRID_NUM]) {
    int type = CurPosition[move->y][move->x];

    m_HashKey32 = m_HashKey32 ^ m_nHashKey32[type][move->y][move->x]; 
    m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type][move->y][move->x]; 
}

void TranspositionTable::Hash_UnMakeMove(STONEMOVE* move, unsigned char CurPosition[GRID_NUM][GRID_NUM]) {
    int type = CurPosition[move->y][move->x];

    m_HashKey32 = m_HashKey32 ^ m_nHashKey32[type][move->y][move->x]; 
    m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type][move->y][move->x]; 
}

int TranspositionTable::LookUpHashTable(int alpha, int beta, int depth, int TableNo) {
    int x;
    HashItem *pht;

    x = m_HashKey32 & 0xFFFFF;
    pht = &m_pTT[TableNo][x];

    if (pht->depth >= depth && pht->checksum == m_HashKey64) {
        switch (pht->entry_type) {
        case exact: 
            return pht->eval;

        case lower_bound:
            if (pht->eval >= beta) {
                return pht->eval;
            }
            else { 
                break;
            }

        case upper_bound:
            if (pht->eval <= alpha) {
                return pht->eval;
            }
            else {
                break;
            }
        }
    }

    return 66666;
}

void TranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, int eval, int depth, int TableNo) {
    int x;
    HashItem * pht;

    x = m_HashKey32 & 0xFFFFF;
    pht = &m_pTT[TableNo][x];

    pht->checksum = m_HashKey64;
    pht->entry_type = entry_type;
    pht->eval = eval;
    pht->depth = depth;
}


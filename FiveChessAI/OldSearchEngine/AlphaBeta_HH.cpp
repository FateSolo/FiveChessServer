#include "AlphaBeta_HH.h"

AlphaBeta_HH::AlphaBeta_HH() {

}

AlphaBeta_HH::~AlphaBeta_HH() {

}

int AlphaBeta_HH::SearchAGoodMove(unsigned char position[GRID_COUNT]) {
    int x, y;

    memcpy(CurPosition, position, GRID_COUNT);

    ResetHistoryTable();

    AlphaBetaSearch(m_nMaxDepth, -20000, 20000);

    x = m_BestMove.x;
    y = m_BestMove.y;

    return 100 * x + y;
}

int AlphaBeta_HH::AlphaBetaSearch(int depth, int alpha, int beta) {
    int count, i, score, side;

    side = (m_nMaxDepth - depth) % 2;

    if (!depth) {
        return m_pEval->Evaluate(CurPosition, side);
    }

    count = m_pMG->CreatePossibleMove(CurPosition, depth);

    for (i = 0; i < count; i++) {
        GetHistoryScore(&m_pMG->m_MoveList[depth][i]);
    }
    MergeSort(m_pMG->m_MoveList[depth], count);

    int bestmove = -1;

    for (i = 0; i < count; i++) {
        MakeMove(&m_pMG->m_MoveList[depth][i], side);

        score = -AlphaBetaSearch(depth - 1, -beta, -alpha);

        UnMakeMove(&m_pMG->m_MoveList[depth][i]);

        if (score > alpha) {
            alpha = score;
            bestmove = i;

            if (depth == m_nMaxDepth) {
                m_BestMove = m_pMG->m_MoveList[depth][i];
            }
        }

        if (alpha >= beta) {
            bestmove = i;
            break;
        }
    }
    
    if (bestmove != -1) {
        EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);
    }
    
    return alpha;
}


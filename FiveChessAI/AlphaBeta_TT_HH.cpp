#include "AlphaBeta_TT_HH.h"

AlphaBeta_TT_HH::AlphaBeta_TT_HH() {

}

AlphaBeta_TT_HH::~AlphaBeta_TT_HH() {

}

int AlphaBeta_TT_HH::SearchAGoodMove(unsigned char position[GRID_COUNT]) {
    int x, y;

    memcpy(CurPosition, position, GRID_COUNT);

    CalculateInitHashKey(CurPosition);
    ResetHistoryTable();

    AlphaBetaSearch(m_nMaxDepth, -20000, 20000);

    x = m_BestMove.x;
    y = m_BestMove.y;

    return 100 * x + y;
}

int AlphaBeta_TT_HH::AlphaBetaSearch(int depth, int alpha, int beta) {
    int count, i, score, side;

    side = (m_nMaxDepth - depth) % 2;

    score = LookUpHashTable(alpha, beta, depth, side); 
    if (score != 66666) {
        return score;
    }

    if (!depth) {
        score = m_pEval->Evaluate(CurPosition, side);
        EnterHashTable(exact, score, depth, side);
        return score;
    }

    count = m_pMG->CreatePossibleMove(CurPosition, depth);

    for (i = 0; i < count; i++) {
        GetHistoryScore(&m_pMG->m_MoveList[depth][i]);
    }
    MergeSort(m_pMG->m_MoveList[depth], count);

    int eval_is_exact = 0;
    int bestmove = -1;

    for (i = 0; i < count; i++) {
        MakeMove(&m_pMG->m_MoveList[depth][i], side);
        Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);

        score = -AlphaBetaSearch(depth - 1, -beta, -alpha);

        Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], CurPosition); 
        UnMakeMove(&m_pMG->m_MoveList[depth][i]);

        if (score > alpha) {
            alpha = score;
            eval_is_exact = 1;
            bestmove = i;

            if (depth == m_nMaxDepth) {
                m_BestMove = m_pMG->m_MoveList[depth][i];
            }
        }

        if (alpha >= beta) {
            EnterHashTable(lower_bound, alpha, depth, side);
            bestmove = i;
            break;
        }
    }
    
    if (eval_is_exact == 1) {
        EnterHashTable(exact, alpha, depth, side);
    }
    else {
        EnterHashTable(upper_bound, alpha, depth, side);
    }
    
    if (bestmove != -1) {
        EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);
    }
    
    return alpha;
}


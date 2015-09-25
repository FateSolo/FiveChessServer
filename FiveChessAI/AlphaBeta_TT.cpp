#include "AlphaBeta_TT.h"

AlphaBeta_TT::AlphaBeta_TT() {

}

AlphaBeta_TT::~AlphaBeta_TT() {

}

int AlphaBeta_TT::SearchAGoodMove(unsigned char position[GRID_COUNT]) {
    int x, y;

    memcpy(CurPosition, position, GRID_COUNT);

    CalculateInitHashKey(CurPosition);

    AlphaBetaSearch(m_nMaxDepth, -20000, 20000);

    x = m_BestMove.x;
    y = m_BestMove.y;

    return 100 * x + y;
}

int AlphaBeta_TT::AlphaBetaSearch(int depth, int alpha, int beta) {
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

    int eval_is_exact = 0;

    for (i = 0; i < count; i++) {
        MakeMove(&m_pMG->m_MoveList[depth][i], side);
        Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);

        score = -AlphaBetaSearch(depth - 1, -beta, -alpha);

        Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], CurPosition); 
        UnMakeMove(&m_pMG->m_MoveList[depth][i]);

        if (score > alpha) {
            alpha = score;
            eval_is_exact = 1;

            if (depth == m_nMaxDepth) {
                m_BestMove = m_pMG->m_MoveList[depth][i];
            }
        }

        if (alpha >= beta) {
            EnterHashTable(lower_bound, alpha, depth, side);
            break;
        }
    }
    
    if (eval_is_exact == 1) {
        EnterHashTable(exact, alpha, depth, side);
    }
    else {
        EnterHashTable(upper_bound, alpha, depth, side);
    }

    return alpha;
}


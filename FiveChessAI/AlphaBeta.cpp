#include "AlphaBeta.h"

AlphaBeta::AlphaBeta() {

}

AlphaBeta::~AlphaBeta() {

}

int AlphaBeta::SearchAGoodMove(unsigned char position[GRID_COUNT]) {
    int x, y;

    memcpy(CurPosition, position, GRID_COUNT);

    AlphaBetaSearch(m_nMaxDepth, -20000, 20000);

    x = m_BestMove.x;
    y = m_BestMove.y;

    return 100 * x + y;
}

int AlphaBeta::AlphaBetaSearch(int depth, int alpha, int beta) {
    int count, i, score, side;

    side = (m_nMaxDepth - depth) % 2;

    if (!depth) {
        return m_pEval->Evaluate(CurPosition, side);
    }

    count = m_pMG->CreatePossibleMove(CurPosition, depth);

    for (i = 0; i < count; i++) {
        MakeMove(&m_pMG->m_MoveList[depth][i], side);

        score = -AlphaBetaSearch(depth - 1, -beta, -alpha);

        UnMakeMove(&m_pMG->m_MoveList[depth][i]);

        if (score > alpha) {
            alpha = score;

            if (depth == m_nMaxDepth) {
                m_BestMove = m_pMG->m_MoveList[depth][i];
            }
        }

        if (alpha >= beta) {
            break;
        }
    }
    
    return alpha;
}

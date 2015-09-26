#include "FinalSearchEngine.h"

FinalSearchEngine::FinalSearchEngine() {

}

FinalSearchEngine::~FinalSearchEngine() {

}

int FinalSearchEngine::SearchAGoodMove(unsigned char position[GRID_COUNT]) {
    int x, y;

    memcpy(CurPosition, position, GRID_COUNT);

    InitLimitBorder();
    CalculateInitHashKey(CurPosition);
    ResetHistoryTable();

    AlphaBetaSearch(m_nMaxDepth, -20000, 20000);

    x = m_BestMove.x;
    y = m_BestMove.y;

    return 100 * x + y;
}

int FinalSearchEngine::AlphaBetaSearch(int depth, int alpha, int beta) {
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

    count = m_pMG->CreatePossibleMove(CurPosition, depth, top, bottom, left, right);

    for (i = 0; i < count; i++) {
        GetHistoryScore(&m_pMG->m_MoveList[depth][i]);
    }
    MergeSort(m_pMG->m_MoveList[depth], count);

    int eval_is_exact = 0;
    int bestmove = -1;

    for (i = 0; i < count; i++) {
        SetLimitBorder(&m_pMG->m_MoveList[depth][i]);
        MakeMove(&m_pMG->m_MoveList[depth][i], side);
        Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);

        score = -AlphaBetaSearch(depth - 1, -beta, -alpha);

        Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], CurPosition); 
        UnMakeMove(&m_pMG->m_MoveList[depth][i]);
        ResetLimitBorder();

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

void FinalSearchEngine::InitLimitBorder() {
    int flag = 0;

    top = bottom = left = right = -100;

    for (int i = 0; i < GRID_NUM && flag != 4; i++) {
        for (int j = 0; j < GRID_NUM && flag != 4; j++) {
            if (CurPosition[i][j] != NOSTONE && top == -100) {
                top = i - 2;
                flag++;
            }

            if (CurPosition[GRID_NUM - i - 1][j] != NOSTONE && bottom == -100) {
                bottom = GRID_NUM - i + 1;
                flag++;
            }

            if (CurPosition[j][i] != NOSTONE && left == -100) {
                left = i - 2;
                flag++;
            }

            if (CurPosition[j][GRID_NUM - i - 1] != NOSTONE && right == -100) {
                right = GRID_NUM - i + 1;
                flag++;
            }
        }
    }

    CheckLimitBorder();
}

void FinalSearchEngine::SetLimitBorder(STONEMOVE *move) {
    old_top = top;
    old_bottom = bottom;
    old_left = left;
    old_right = right;

    if (move->y - 2 < top) {
        top = move->y - 2;
    }

    if (move->y + 2 > bottom) {
        bottom = move->y + 2;
    }

    if (move->x - 2 < left) {
        left = move->x - 2;
    }

    if (move->x + 2 > right) {
        right = move->x + 2;
    }

    CheckLimitBorder();
}

void FinalSearchEngine::ResetLimitBorder() {
    top = old_top;
    bottom = old_bottom;
    left = old_left;
    right = old_right;
}

void FinalSearchEngine::CheckLimitBorder() {
    if (top < 0) {
        top = 0;
    }

    if (bottom >= GRID_NUM) {
        bottom = GRID_NUM - 1;
    }

    if (left < 0) {
        left = 0;
    }

    if (right >= GRID_NUM) {
        right = GRID_NUM - 1;
    }
}


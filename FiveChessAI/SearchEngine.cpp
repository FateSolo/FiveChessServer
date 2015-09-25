#include "SearchEngine.h"

SearchEngine::SearchEngine() {

}

SearchEngine::~SearchEngine() {
    delete m_pMG;
    delete m_pEval;
}

void SearchEngine::SetSearchDepth(int nDepth) {
    m_nMaxDepth = nDepth;
}

void SearchEngine::SetMoveGenerator(MoveGenerator *pMG) {
    m_pMG = pMG;
}

void SearchEngine::SetEvaluator(Evaluation *pEval) {
    m_pEval = pEval;
}

void SearchEngine::MakeMove(STONEMOVE *move, int type) {
    CurPosition[move->y][move->x] = type;
}

void SearchEngine::UnMakeMove(STONEMOVE *move) {
    CurPosition[move->y][move->x] = NOSTONE;
}


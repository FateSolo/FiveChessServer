#ifndef   _SEARCH_ENGINE_H 
#define   _SEARCH_ENGINE_H 

#include "MoveGenerator.h"
#include "Evaluation.h"

class SearchEngine {

public:
    SearchEngine();
    virtual ~SearchEngine();

    virtual int SearchAGoodMove(unsigned char position[GRID_COUNT]) = 0;

    void SetSearchDepth(int nDepth);
    void SetMoveGenerator(MoveGenerator *pMG);
    void SetEvaluator(Evaluation *pEval);

protected:
    void MakeMove(STONEMOVE *move, int type);
    void UnMakeMove(STONEMOVE *move);

    MoveGenerator *m_pMG;
    Evaluation *m_pEval;

    unsigned char CurPosition[GRID_NUM][GRID_NUM];
    STONEMOVE m_BestMove;

    int m_nMaxDepth;

};

#endif


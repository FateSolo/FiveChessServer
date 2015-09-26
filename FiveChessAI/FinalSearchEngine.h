#ifndef   _FINAL_SEARCH_ENGINE_H 
#define   _FINAL_SEARCH_ENGINE_H 

#include "SearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"

class FinalSearchEngine : 
    public SearchEngine,
    public TranspositionTable,
    public HistoryHeuristic {

public:
    FinalSearchEngine();
    ~FinalSearchEngine();

    int SearchAGoodMove(unsigned char position[GRID_COUNT]);

private:
    int AlphaBetaSearch(int depth, int alpha, int beta);

    void InitLimitBorder();

    void SetLimitBorder(STONEMOVE *move);
    void ResetLimitBorder();

    void CheckLimitBorder();

    int top;
    int bottom;
    int left;
    int right;

    int old_top;
    int old_bottom;
    int old_left;
    int old_right;

};

#endif


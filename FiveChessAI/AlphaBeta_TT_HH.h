#ifndef   _ALPHA_BETA_TT_HH_H 
#define   _ALPHA_BETA_TT_HH_H 

#include "SearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"

class AlphaBeta_TT_HH : 
    public SearchEngine,
    public TranspositionTable,
    public HistoryHeuristic {

public:
    AlphaBeta_TT_HH();
    ~AlphaBeta_TT_HH();

    int SearchAGoodMove(unsigned char position[GRID_COUNT]);

private:
    int AlphaBetaSearch(int depth, int alpha, int beta);

};

#endif


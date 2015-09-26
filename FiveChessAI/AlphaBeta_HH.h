#ifndef   _ALPHA_BETA_HH_H 
#define   _ALPHA_BETA_HH_H 

#include "SearchEngine.h"
#include "HistoryHeuristic.h"

class AlphaBeta_HH : 
    public SearchEngine,
    public HistoryHeuristic {

public:
    AlphaBeta_HH();
    ~AlphaBeta_HH();

    int SearchAGoodMove(unsigned char position[GRID_COUNT]);

private:
    int AlphaBetaSearch(int depth, int alpha, int beta);

};

#endif


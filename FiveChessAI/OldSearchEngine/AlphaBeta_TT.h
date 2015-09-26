#ifndef   _ALPHA_BETA_TT_H 
#define   _ALPHA_BETA_TT_H 

#include "SearchEngine.h"
#include "TranspositionTable.h"

class AlphaBeta_TT : 
    public SearchEngine,
    public TranspositionTable {

public:
    AlphaBeta_TT();
    ~AlphaBeta_TT();

    int SearchAGoodMove(unsigned char position[GRID_COUNT]);

private:
    int AlphaBetaSearch(int depth, int alpha, int beta);

};

#endif


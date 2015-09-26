#ifndef   _ALPHA_BETA_H 
#define   _ALPHA_BETA_H 

#include "SearchEngine.h"

class AlphaBeta : public SearchEngine {

public:
    AlphaBeta();
    ~AlphaBeta();

    int SearchAGoodMove(unsigned char position[GRID_COUNT]);

private:
    int AlphaBetaSearch(int depth, int alpha, int beta);

};

#endif


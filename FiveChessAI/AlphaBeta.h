#include "SearchEngine.h"

class AlphaBeta : public SearchEngine {

public:
	AlphaBeta();
	~AlphaBeta();

	int SearchAGoodMove(unsigned char position[GRID_COUNT]);

private:
	int AlphaBetaSearch(int depth, int alpha, int beta);

};


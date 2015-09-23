#include "ChessDefine.h"

class MoveGenerator {

public:
	MoveGenerator();
	~MoveGenerator();

	int CreatePossibleMove(unsigned char position[GRID_NUM][GRID_NUM], int nPly);

	STONEMOVE m_MoveList[5][GRID_COUNT];

private:
	void AddMove(int nToX, int nToY, int nPly);

	int m_nMoveCount;
	
};


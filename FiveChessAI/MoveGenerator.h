#ifndef   _MOVE_GENERATOR_H 
#define   _MOVE_GENERATOR_H 

#include "ChessDefine.h"

class MoveGenerator {

public:
    MoveGenerator();
    ~MoveGenerator();

    int CreatePossibleMove(unsigned char position[GRID_NUM][GRID_NUM], int nPly);
    int CreatePossibleMove(unsigned char position[GRID_NUM][GRID_NUM], int nPly, int top, int bottom, int left, int right);

    STONEMOVE m_MoveList[5][GRID_COUNT];

private:
    void AddMove(int nToX, int nToY, int nPly);

    int m_nMoveCount;
    
};

#endif


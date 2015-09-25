#include "MoveGenerator.h"

MoveGenerator::MoveGenerator() {

}

MoveGenerator::~MoveGenerator() {

}

void MoveGenerator::AddMove(int nToX, int nToY, int nPly) {
    m_MoveList[nPly][m_nMoveCount].x = nToX;
    m_MoveList[nPly][m_nMoveCount].y = nToY;

    m_nMoveCount++;
}

int MoveGenerator::CreatePossibleMove(unsigned char position[GRID_NUM][GRID_NUM], int nPly) {
    m_nMoveCount = 0;

    for (int i = 0; i < GRID_NUM; i++) {
        for (int j = 0; j < GRID_NUM; j++) {
            if (position[i][j] == NOSTONE) {
                AddMove(j, i, nPly);
            }
        }
    }

    return m_nMoveCount;
}


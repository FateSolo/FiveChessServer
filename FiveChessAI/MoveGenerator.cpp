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
    return CreatePossibleMove(position, nPly, 0, GRID_NUM - 1, 0, GRID_NUM - 1);
}

int MoveGenerator::CreatePossibleMove(unsigned char position[GRID_NUM][GRID_NUM], int nPly, int top, int bottom, int left, int right) {
    m_nMoveCount = 0;

    for (int i = top; i <= bottom; i++) {
        for (int j = left; j <= right; j++) {
            if (position[i][j] == NOSTONE) {
                AddMove(j, i, nPly);
            }
        }
    }

    return m_nMoveCount;
}


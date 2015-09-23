#include "Evaluation.h"

Evaluation::Evaluation() {

}

Evaluation::~Evaluation() {

}

int Evaluation::Evaluate(unsigned char position[GRID_NUM][GRID_NUM], int bIsBlackTurn) {
    int i, j, k, WValue = 0, BValue = 0;
    unsigned char nStoneType;

    memset(m_TypeRecord, TOBEANALYSIS, 4 * GRID_COUNT);
    memset(m_TypeCount, 0, 2 * 20 * sizeof(int));

    for (i = 0; i < GRID_NUM; i++) {
        for (j = 0; j < GRID_NUM; j++) {
            if (position[i][j] != NOSTONE) {
                if (m_TypeRecord[i][j][0] == TOBEANALYSIS) {
                    AnalysisHorizon(position, i, j);
                }

                if (m_TypeRecord[i][j][1] == TOBEANALYSIS) {
                    AnalysisVertical(position, i, j);
                }

                if (m_TypeRecord[i][j][2] == TOBEANALYSIS) {
                    AnalysisLeft(position, i, j);
                }

                if (m_TypeRecord[i][j][3] == TOBEANALYSIS) {
                    AnalysisRight(position, i, j);
                }
            }
        }
    }

    for (i = 0; i < GRID_NUM; i++) {
        for (j = 0; j < GRID_NUM; j++) {
            for (k = 0; k < 4; k++) {
                nStoneType = position[i][j];

                if (nStoneType != NOSTONE) {
                    switch (m_TypeRecord[i][j][k]) {
                    case FIVE:
                        m_TypeCount[nStoneType][FIVE]++;
                        break;

                    case FOUR:
                        m_TypeCount[nStoneType][FOUR]++;
                        break;

                    case SFOUR:
                        m_TypeCount[nStoneType][SFOUR]++;
                        break;

                    case THREE:
                        m_TypeCount[nStoneType][THREE]++;
                        break;

                    case STHREE:
                        m_TypeCount[nStoneType][STHREE]++;
                        break;

                    case TWO:
                        m_TypeCount[nStoneType][TWO]++;
                        break;

                    case STWO:
                        m_TypeCount[nStoneType][STWO]++;
                        break;
                    }
                }
            }
        }
    }

    if (bIsBlackTurn) {
        if (m_TypeCount[BLACK][FIVE]) {
            return -9999;
        }

        if (m_TypeCount[WHITE][FIVE]) {
            return 9999;
        }
    }
    else {
        if (m_TypeCount[BLACK][FIVE]) {
            return 9999;
        }

        if (m_TypeCount[WHITE][FIVE]) {
            return -9999;
        }
    }

    if (m_TypeCount[WHITE][SFOUR] > 1) {
        m_TypeCount[WHITE][FOUR]++;
    }

    if (m_TypeCount[BLACK][SFOUR] > 1) {
        m_TypeCount[BLACK][FOUR]++;
    }

    if (bIsBlackTurn) {
        if (m_TypeCount[WHITE][FOUR]) {
            return 9990;
        }

        if (m_TypeCount[WHITE][SFOUR]) {
            return 9980;
        }

        if (m_TypeCount[BLACK][FOUR]) {
            return -9970;
        }

        if (m_TypeCount[BLACK][SFOUR] && m_TypeCount[BLACK][THREE]) {
            return -9960;
        }

        if (m_TypeCount[WHITE][THREE] && !m_TypeCount[BLACK][SFOUR]) {
            return 9950;
        }

        if (m_TypeCount[BLACK][THREE] > 1 && !m_TypeCount[WHITE][THREE] && !m_TypeCount[WHITE][STHREE]) {
            return -9940;
        }

        if (m_TypeCount[WHITE][THREE] > 1) {
            WValue += 2000;
        }
        else if (m_TypeCount[WHITE][THREE]) {
            WValue += 200;
        }

        if (m_TypeCount[BLACK][THREE] > 1) {
            BValue += 500;
        }
        else if (m_TypeCount[BLACK][THREE]) {
            BValue += 100;
        }

        if (m_TypeCount[WHITE][STHREE]) {
            WValue += m_TypeCount[WHITE][STHREE] * 10;
        }

        if (m_TypeCount[BLACK][STHREE]) {
            BValue += m_TypeCount[BLACK][STHREE] * 10;
        }

        if (m_TypeCount[WHITE][TWO]) {
            WValue += m_TypeCount[WHITE][TWO] * 4;
        }

        if (m_TypeCount[BLACK][STWO]) {
            BValue += m_TypeCount[BLACK][TWO] * 4;
        }

        if (m_TypeCount[WHITE][STWO]) {
            WValue += m_TypeCount[WHITE][STWO];
        }

        if (m_TypeCount[BLACK][STWO]) {
            BValue += m_TypeCount[BLACK][STWO];
        }
    }
    else {
        if (m_TypeCount[BLACK][FOUR]) {
            return 9990;
        }

        if (m_TypeCount[BLACK][SFOUR]) {
            return 9980;
        }

        if (m_TypeCount[WHITE][FOUR]) {
            return -9970;
        }

        if (m_TypeCount[WHITE][SFOUR] && m_TypeCount[WHITE][THREE]) {
            return -9960;
        }

        if (m_TypeCount[BLACK][THREE] && !m_TypeCount[WHITE][SFOUR]) {
            return 9950;
        }

        if (m_TypeCount[WHITE][THREE] > 1 && !m_TypeCount[BLACK][THREE] && !m_TypeCount[BLACK][STHREE]) {
            return -9940;
        }

        if (m_TypeCount[BLACK][THREE] > 1) {
            BValue += 2000;
        }
        else if (m_TypeCount[BLACK][THREE]) {
            BValue += 200;
        }

        if (m_TypeCount[WHITE][THREE] > 1) {
            WValue += 500;
        }
        else if (m_TypeCount[WHITE][THREE]) {
            WValue += 100;
        }

        if (m_TypeCount[WHITE][STHREE]) {
            WValue += m_TypeCount[WHITE][STHREE] * 10;
        }

        if (m_TypeCount[BLACK][STHREE]) {
            BValue += m_TypeCount[BLACK][STHREE] * 10;
        }

        if (m_TypeCount[WHITE][TWO]) {
            WValue += m_TypeCount[WHITE][TWO] * 4;
        }

        if (m_TypeCount[BLACK][STWO]) {
            BValue += m_TypeCount[BLACK][TWO] * 4;
        }

        if (m_TypeCount[WHITE][STWO]) {
            WValue += m_TypeCount[WHITE][STWO];
        }

        if (m_TypeCount[BLACK][STWO]) {
            BValue += m_TypeCount[BLACK][STWO];
        }
    }

    for (i = 0; i < GRID_NUM; i++) {
        for (j = 0; j < GRID_NUM; j++) {
            nStoneType = position[i][j];

            if (nStoneType != NOSTONE) {
                if (nStoneType == BLACK) {
                    BValue += PosValue[i][j];
                }
                else {
                    WValue += PosValue[i][j];
                }
            }
        }
    }

    if (!bIsBlackTurn) {
        return BValue - WValue;
    }
    else {
        return WValue - BValue;
    }
}

void Evaluation::AnalysisHorizon(unsigned char position[GRID_NUM][GRID_NUM], int i, int j) {
    AnalysisLine(position[i], GRID_NUM, j);

    for (int s = 0; s < GRID_NUM; s++) {
        if (m_LineRecord[s] != TOBEANALYSIS) {
            m_TypeRecord[i][s][0] = m_LineRecord[s];
        }
    }
}

void Evaluation::AnalysisVertical(unsigned char position[GRID_NUM][GRID_NUM], int i, int j) {
    unsigned char tempArray[GRID_NUM];

    for (int k = 0; k < GRID_NUM; k++) {
        tempArray[k] = position[k][j];
    }

    AnalysisLine(tempArray, GRID_NUM, i);

    for (int s = 0; s < GRID_NUM; s++) {
        if (m_LineRecord[s] != TOBEANALYSIS) {
            m_TypeRecord[s][j][1] = m_LineRecord[s];
        }
    }
}

void Evaluation::AnalysisLeft(unsigned char position[GRID_NUM][GRID_NUM], int i, int j) {
    unsigned char tempArray[GRID_NUM];
    int x, y, k;

    if (i < j) {
        y = 0;
        x = j - i;
    }
    else {
        x = 0;
        y = i - j;
    }

    for (k = 0; k < GRID_NUM; k++) {
        if (x + k > 14 || y + k > 14) {
            break;
        }

        tempArray[k] = position[y + k][x + k];
    }

    AnalysisLine(tempArray, k, j - x);

    for (int s = 0; s < k; s++) {
        if (m_LineRecord[s] != TOBEANALYSIS) {
            m_TypeRecord[y + s][x + s][2] = m_LineRecord[s];
        }
    }
}

void Evaluation::AnalysisRight(unsigned char position[GRID_NUM][GRID_NUM], int i, int j) {
    unsigned char tempArray[GRID_NUM];
    int x, y, k;

    if (14 - i < j) {
        y = 14;
        x = j - 14 + i;
    }
    else {
        x = 0;
        y = i + j;
    }

    for (k = 0; k < GRID_NUM; k++) {
        if (x + k > 14 || y - k < 0) {
            break;
        }

        tempArray[k] = position[y - k][x + k];
    }

    AnalysisLine(tempArray, k, j - x);

    for (int s = 0; s < k; s++) {
        if (m_LineRecord[s] != TOBEANALYSIS) {
            m_TypeRecord[y - s][x + s][3] = m_LineRecord[s];
        }
    }
}

void Evaluation::AnalysisLine(unsigned char *position, int GridNum, int StonePos) {
    unsigned char StoneType;
    unsigned char AnalyLine[30];
    int nAnalyPos;
    int LeftEdge, RightEdge;
    int LeftRange, RightRange;

    if (GridNum < 5) {
        memset(m_LineRecord, ANALYSISED, GridNum);
        return;
    }

    nAnalyPos = StonePos;

    memset(m_LineRecord, TOBEANALYSIS, 30);
    memset(AnalyLine, 0x0F, 30);
    memcpy(&AnalyLine, position, GridNum);

    GridNum--;
    StoneType = AnalyLine[nAnalyPos];
    LeftEdge = nAnalyPos;
    RightEdge = nAnalyPos;

    while (LeftEdge > 0) {
        if (AnalyLine[LeftEdge - 1] != StoneType) {
            break;
        }

        LeftEdge--;
    }

    while (RightEdge < GridNum) {
        if (AnalyLine[RightEdge + 1] != StoneType) {
            break;
        }

        RightEdge++;
    }

    LeftRange = LeftEdge;
    RightRange = RightEdge;

    while (LeftRange > 0) {
        if (AnalyLine[LeftRange - 1] == !StoneType) {
            break;
        }

        LeftRange--;
    }

    while (RightRange < GridNum) {
        if (AnalyLine[RightRange + 1] == !StoneType) {
            break;
        }

        RightRange++;
    }

    if (RightRange - LeftRange < 4) {
        for (int k = LeftRange; k <= RightRange; k++) {
            m_LineRecord[k] = ANALYSISED;
        }

        return;
    }

    for (int k = LeftEdge; k <= RightEdge; k++) {
        m_LineRecord[k] = ANALYSISED;
    }

    if (RightEdge - LeftEdge > 3) {
        m_LineRecord[nAnalyPos] = FIVE;
        return;
    }

    if (RightEdge - LeftEdge == 3) {
        bool LeftFour = false;

        if (LeftEdge > 0) {
            if (AnalyLine[LeftEdge - 1] == NOSTONE) {
                LeftFour = true;
                m_LineRecord[nAnalyPos] = SFOUR;
            }
        }

        if (RightEdge < GridNum) {
            if (AnalyLine[RightEdge + 1] == NOSTONE) {
                if (LeftFour) {
                    m_LineRecord[nAnalyPos] = FOUR;
                }
                else {
                    m_LineRecord[nAnalyPos] = SFOUR;
                }
            }
        }

        return;
    }

    if (RightEdge - LeftEdge == 2) {
        bool LeftThree = false;

        if (LeftEdge > 0) {
            if (AnalyLine[LeftEdge - 1] == NOSTONE) {
                if (LeftEdge > 1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge]) {
                    m_LineRecord[LeftEdge] = SFOUR;
                    m_LineRecord[LeftEdge - 2] = ANALYSISED;
                }
                else {
                    LeftThree = true;
                }
            }
            else if (RightEdge == GridNum || AnalyLine[RightEdge + 1] != NOSTONE) {
                return;
            }
        }

        if (RightEdge < GridNum) {
            if (AnalyLine[RightEdge + 1] == NOSTONE) {
                if (RightEdge < GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge]) {
                    m_LineRecord[RightEdge] = SFOUR;
                    m_LineRecord[RightEdge + 2] = ANALYSISED;
                }
                else {
                    if (LeftThree) {
                        m_LineRecord[RightEdge] = THREE;
                    }
                    else {
                        m_LineRecord[RightEdge] = STHREE;
                    }
                }
            }
            else {
                if (m_LineRecord[LeftEdge] == SFOUR) {
                    return;
                }
                if (LeftThree) {
                    m_LineRecord[nAnalyPos] = STHREE;
                }
            }
        }
        else {
            if (m_LineRecord[LeftEdge] == SFOUR) {
                return;
            }
            if (LeftThree) {
                m_LineRecord[nAnalyPos] = STHREE;
            }
        }

        return;
    }

    if (RightEdge - LeftEdge == 1) {
        bool LeftTwo = false;

        if (LeftEdge > 2) {
            if (AnalyLine[LeftEdge - 1] == NOSTONE) {
                if (LeftEdge > 2 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge]) {
                    if (AnalyLine[LeftEdge - 3] == AnalyLine[LeftEdge]) {
                        m_LineRecord[LeftEdge - 3] = ANALYSISED;
                        m_LineRecord[LeftEdge - 2] = ANALYSISED;
                        m_LineRecord[LeftEdge] = SFOUR;
                    }
                    else if (AnalyLine[LeftEdge - 3] == NOSTONE) {
                        m_LineRecord[LeftEdge - 2] = ANALYSISED;
                        m_LineRecord[LeftEdge] = STHREE;
                    }
                }
                else {
                    LeftTwo = true;
                }
            }
        }

        if (RightEdge < GridNum) {
            if (AnalyLine[RightEdge + 1] == NOSTONE) {
                if (RightEdge < GridNum - 2 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge]) {
                    if (AnalyLine[RightEdge + 3] == AnalyLine[RightEdge]) {
                        m_LineRecord[RightEdge + 3] = ANALYSISED;
                        m_LineRecord[RightEdge + 2] = ANALYSISED;
                        m_LineRecord[RightEdge] = SFOUR;
                    }
                    else if (AnalyLine[RightEdge + 3] == NOSTONE) {
                        m_LineRecord[RightEdge + 2] = ANALYSISED;

                        if (LeftTwo) {
                            m_LineRecord[RightEdge] = THREE;
                        }
                        else {
                            m_LineRecord[RightEdge] = STHREE;
                        }
                    }
                }
                else {
                    if (m_LineRecord[LeftEdge] == SFOUR) {
                        return;
                    }

                    if (m_LineRecord[LeftEdge] == STHREE) {
                        m_LineRecord[LeftEdge] = THREE;
                        return;
                    }

                    if (LeftTwo) {
                        m_LineRecord[nAnalyPos] = TWO;
                    }
                    else {
                        m_LineRecord[nAnalyPos] = STWO;
                    }
                }
            }
            else {
                if (m_LineRecord[LeftEdge] == SFOUR) {
                    return;
                }
                
                if (LeftTwo) {
                    m_LineRecord[nAnalyPos] = STWO;
                }
            }
        }
    }
}


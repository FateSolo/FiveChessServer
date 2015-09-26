#include <string.h>

#define GRID_NUM        15
#define GRID_COUNT      GRID_NUM * GRID_NUM

bool isWin;

void AnalysisLine(unsigned char position[GRID_NUM], int GridNum, int StonePos) {
    if (GridNum < 5 || isWin) {
        return;
    }

    int left = StonePos, right = StonePos;

    while (left > 0) {
        if (position[left - 1] != position[StonePos]) {
            break;
        }

        left--;
    }

    while (right < GridNum) {
        if (position[right + 1] != position[StonePos]) {
            break;
        }

        right++;
    }

    if (right - left > 3) {
        isWin = true;
    }
}

bool CheckIsWin(unsigned char position[GRID_COUNT], int x, int y) {
    isWin = false;

    int i, j, k;
    unsigned char CurPosition[GRID_NUM][GRID_NUM], tempArray[GRID_NUM];

    memcpy(CurPosition, position, GRID_COUNT);

    AnalysisLine(CurPosition[y], GRID_NUM, x);

    for (k = 0; k < GRID_NUM; k++) {
        tempArray[k] = CurPosition[k][x];
    }

    AnalysisLine(tempArray, GRID_NUM, y);

    if (y < x) {
        i = 0;
        j = x - y;
    }
    else {
        j = 0;
        i = y - x;
    }

    for (k = 0; k < GRID_NUM; k++) {
        if (i + k > 14 || j + k > 14) {
            break;
        }

        tempArray[k] = CurPosition[i + k][j + k];
    }

    AnalysisLine(tempArray, k, x - j);

    if (14 - y < x) {
        i = 14;
        j = x - 14 + y;
    }
    else {
        j = 0;
        i = x + y;
    }

    for (k = 0; k < GRID_NUM; k++) {
        if (j + k > 14 || i - k < 0) {
            break;
        }

        tempArray[k] = CurPosition[i - k][j + k];
    }

    AnalysisLine(tempArray, k, x - j);

    return isWin;
}

extern "C" {
    bool GetIsWin(unsigned char position[GRID_COUNT], int x, int y) {
        return CheckIsWin(position, x, y);
    }
}


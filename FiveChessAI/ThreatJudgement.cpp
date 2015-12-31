#include <string.h>

#define GRID_NUM        15
#define GRID_COUNT      GRID_NUM * GRID_NUM
#define NOSTONE         255

int threat;

void AnalysisLine(unsigned char position[GRID_NUM], int GridNum, int StonePos) {
    if (GridNum < 5) {
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

    if (right - left == 3) {
        if (left > 0 && position[left - 1] == NOSTONE) {
            threat = left - 1;
            return;
        }

        if (right < GridNum && position[right + 1] == NOSTONE) {
            threat = right + 1;
            return;
        }
    }

    if (right - left == 2) {
        if (left > 1 && position[left - 1] == NOSTONE) {
            if (position[left - 2] == position[left]) {
                threat = left - 1;
                return;              
            }
        }

        if (right < GridNum - 1 && position[right + 1] == NOSTONE) {
            if (position[right + 2] == position[right]) {
                threat = right + 1;
                return;
            }
        }
    }

    if (right - left == 1) {
        if (left > 2 && position[left - 1] == NOSTONE) {
            if (position[left - 2] == position[left] && position[left - 3] == position[left]) {
                threat = left - 1;
                return;
            }
        }

        if (right < GridNum - 2 && position[right + 1] == NOSTONE) {
            if (position[right + 2] == position[right] && position[right + 3] == position[right]) {
                threat = right + 1;
                return;
            }
        }
    }

    if (right - left == 0) {
        if (left > 3 && position[left - 1] == NOSTONE) {
            if (position[left - 2] == position[left] && position[left - 3] == position[left] && position[left - 4] == position[left]) {
                threat = left - 1;
                return;
            }
        }

        if (right < GridNum - 3 && position[right + 1] == NOSTONE) {
            if (position[right + 2] == position[right] && position[right + 3] == position[right] && position[right + 4] == position[right]) {
                threat = right + 1;
                return;
            }
        }
    }
}

int CheckThreat(unsigned char position[GRID_COUNT], int x, int y) {
    threat = -1;

    int i, j, k;
    unsigned char CurPosition[GRID_NUM][GRID_NUM], tempArray[GRID_NUM];

    memcpy(CurPosition, position, GRID_COUNT);

    AnalysisLine(CurPosition[y], GRID_NUM, x);

    if (threat != -1) {
        return 100 * threat + y;
    }

    for (k = 0; k < GRID_NUM; k++) {
        tempArray[k] = CurPosition[k][x];
    }

    AnalysisLine(tempArray, GRID_NUM, y);

    if (threat != -1) {
        return 100 * x + threat;
    }

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

    if (threat != -1) {
        if (y < x) {
            return 100 * (threat + x - y) + threat;
        }
        else {
            return 100 * threat + threat + y - x;
        }
    }

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

    if (threat != -1) {
        if (14 - y < x) {
            return 100 * (threat + x + y - 14) + 14 - threat;
        }
        else {
            return 100 * threat + x + y - threat;
        }
    }

    return threat;
}

extern "C" {
    int GetThreat(unsigned char position[GRID_COUNT], int old_x, int old_y, int x, int y) {
        int a = CheckThreat(position, old_x, old_y);

        if (a != -1) {
            return a;
        }

        return CheckThreat(position, x, y);
    }
}
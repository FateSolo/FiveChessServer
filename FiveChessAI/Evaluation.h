#ifndef   _EVALUATION_H 
#define   _EVALUATION_H 

#include "ChessDefine.h"

class Evaluation {

public:
    Evaluation();
    ~Evaluation();

    int Evaluate(unsigned char position[GRID_NUM][GRID_NUM], int bIsBlackTurn);
    
private:
    void AnalysisHorizon(unsigned char position[GRID_NUM][GRID_NUM], int i, int j);
    void AnalysisVertical(unsigned char position[GRID_NUM][GRID_NUM], int i, int j);
    void AnalysisLeft(unsigned char position[GRID_NUM][GRID_NUM], int i, int j);
    void AnalysisRight(unsigned char position[GRID_NUM][GRID_NUM], int i, int j);

    void AnalysisLine(unsigned char *position, int GridNum, int StonePos);

    unsigned char m_TypeRecord[GRID_NUM][GRID_NUM][4];
    unsigned char m_LineRecord[30];
    int m_TypeCount[2][20];

};

#endif


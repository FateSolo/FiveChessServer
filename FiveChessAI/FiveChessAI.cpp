#include "AlphaBeta.h"
#include "AlphaBeta_TT.h"
#include "AlphaBeta_HH.h"
#include "AlphaBeta_TT_HH.h"

extern "C" {
    int GetAGoodMove(unsigned char position[GRID_COUNT], int level, int type) {
        SearchEngine *se;

        switch(type) {
        case 0:
            se = new AlphaBeta;

        case 1:
            se = new AlphaBeta_TT;

        case 2:
            se = new AlphaBeta_HH;

        case 3:
            se = new AlphaBeta_TT_HH;
        }

        se->SetSearchDepth(level);

        MoveGenerator *mg = new MoveGenerator;
        Evaluation *eval = new Evaluation;

        se->SetMoveGenerator(mg);
        se->SetEvaluator(eval);

        int result = se->SearchAGoodMove(position);

        delete se;

        return result;
    }
}


#include "AlphaBeta.h"
#include "AlphaBeta_TT.h"

extern "C" {
    int GetAGoodMove(unsigned char position[GRID_COUNT], int level, int type) {
        SearchEngine *se;

        switch(type) {
        case 0:
            se = new AlphaBeta;

        case 1:
            se = new AlphaBeta_TT;
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


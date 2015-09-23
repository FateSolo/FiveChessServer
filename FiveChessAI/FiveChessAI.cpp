#include "AlphaBeta.h"

extern "C" {
    int GetAGoodMove(unsigned char position[GRID_COUNT]) {
        SearchEngine *se = new AlphaBeta;

        MoveGenerator *mg = new MoveGenerator;
        Evaluation *eval = new Evaluation;

        se->SetMoveGenerator(mg);
        se->SetEvaluator(eval);

        se->SetSearchDepth(2);

        int result = se->SearchAGoodMove(position);

        delete se;

        return result;
    }
}
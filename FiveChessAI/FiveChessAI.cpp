#include "FinalSearchEngine.h"

extern "C" {
    int GetAGoodMove(unsigned char position[GRID_COUNT], int level) {
        SearchEngine *se = new FinalSearchEngine;

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


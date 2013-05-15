#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "lbfgsxx.h"

using namespace std;

typedef size_t size_type;

struct FEvaluate
{
    FEvaluate() = default;
    lbfgsfloatval_t operator () (
        const lbfgsfloatval_t* pVar,
        lbfgsfloatval_t* pGrad,
        const int N
        )
    {
        lbfgsfloatval_t fx = 0.0;
        const lbfgsfloatval_t* x = pVar;
        lbfgsfloatval_t* g = pGrad;
        for (int i=0; i<N; i+=2)
        {
            lbfgsfloatval_t t1 = 1.0 - x[i];
            lbfgsfloatval_t t2 = 10.0 * (x[i+1] - x[i] * x[i]);
            g[i+1] = 20.0 * t2;
            g[i] = -2.0 * (x[i] * g[i+1] + t1);
            fx += t1 * t1 + t2 * t2;
        }
        return fx;
    }
};

struct FProgress
{
    FProgress() = default;
    int operator () (
        const lbfgsfloatval_t* pVar,
        const lbfgsfloatval_t* pGrad,
        int N,
        const lbfgsfloatval_t ObjValue,
        const lbfgsfloatval_t Xnorm,
        const lbfgsfloatval_t Gnorm,
        int IterNum,
        int EvalNum,
        const lbfgsfloatval_t Step
        )
    {
        printf("Iteration %d:\n", IterNum);
        printf("  fx = %f, x[0] = %f, x[1] = %f\n", ObjValue, pVar[0], pVar[1]);
        printf("  xnorm = %f, gnorm = %f, step = %f\n", Xnorm, Gnorm, Step);
        printf("\n");
        return 0;
    }
};

void InitX(vector<lbfgsfloatval_t>& vX)
{
    const size_type N = 100;
    vX.clear();
    vX.resize(N);
    for (size_type i=0; i+1<vX.size(); i+=2)
    {
        vX[i] = -1.2;
        vX[i+1] = 1.0;
    }
}

int main(int argc, char* argv[])
{
    //  let t[1] = 1-x[1], t[2] = 10*(x[2]-x[1]^2)
    //  then f = t[1]^2 + t[2]^2
    //  g = < 2*t[1]*(-1)+2*t[2]*10*(-2)*x[1], 2*t[2]*10 >
    //    = < -2*(t[1]+g[2]*x[1]),             20*t[2] >
    //  the minimum should be: x[0]=1, x[1]=1, fx=0 
    vector<lbfgsfloatval_t> vX;
    //  initialize vector
    InitX(vX);
    //  initialize LBFGS parameter 
    lbfgs_parameter_t lparam;
    lbfgs_parameter_init(&lparam);
    //  run
    CLbfgsxx<FEvaluate, FProgress> lbfgsxx(&*vX.begin(), vX.size(), &lparam, FEvaluate(), FProgress());
    int ret = lbfgsxx.Run();
    if (ret != LBFGS_SUCCESS)
    {
        lbfgsxx.PrintError(ret);
    }
    lbfgsfloatval_t fx = lbfgsxx.ObjValue();
    //  output
    printf("L-BFGS optimization terminated with status code = %d\n", ret);
    printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, vX[0], vX[1]);
    return 0;
}


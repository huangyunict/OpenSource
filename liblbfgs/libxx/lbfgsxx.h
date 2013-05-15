#ifndef LBFGSXX_H_2011_12_20
#define LBFGSXX_H_2011_12_20

#include <cassert>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "lbfgs.h"

/////////////////////////////////////////////////////////////////////////////////

class FDefaultLBFGSProgress
{
private:
    int m_Verbose;     //  <=0: silent;  1: default;   >2: debug
public:
    explicit FDefaultLBFGSProgress(int verbose = 1) : m_Verbose(verbose)
    {
    }
    int operator () (
        const lbfgsfloatval_t* pVar,
        const lbfgsfloatval_t* pGrad,
        int N,
        const lbfgsfloatval_t Fx,
        const lbfgsfloatval_t Xnorm,
        const lbfgsfloatval_t Gnorm,
        int IterNum,
        int EvalNum,
        const lbfgsfloatval_t Step
        )
    {
        if (m_Verbose >= 1)
        {
            std::cerr << "Iteration " << IterNum << ": eval_num=" << EvalNum << std::endl;
            std::cerr << "  fx=" << Fx << ", xnorm=" << Xnorm << ", gnorm=" << Gnorm << ", step=" << Step << std::endl;
        }
        if (m_Verbose >= 2)
        {
            std::cerr << "  x["<<N<<"]=" ;
            std::copy(pVar, pVar + N, std::ostream_iterator<lbfgsfloatval_t>(std::cerr, " "));
            std::cerr << std::endl;
        }
        return 0;
    }
};

/////////////////////////////////////////////////////////////////////////////////

//  return the final value
template <typename _FEvaluate, typename _FProgress = FDefaultLBFGSProgress>
class CLbfgsxx
{
private:
    lbfgsfloatval_t* m_pVar;
    int m_N;
    lbfgsfloatval_t m_ObjValue;
    const lbfgs_parameter_t* m_pParam;
    _FEvaluate m_fEvaluate;
    _FProgress m_fProgress;
private:
    //  return objective function for current variables
    static lbfgsfloatval_t LBFGSXX_Evaluate (
        void* pInstance,              //  pointer to CLbfgsxx
        const lbfgsfloatval_t* pVar,  //  pointer to variable array
        lbfgsfloatval_t* pGrad,       //  pointer to gradient array
        const int N,                  //  dimension
        const lbfgsfloatval_t Step    //  current step of line search routine
        )
    {
        CLbfgsxx* p = reinterpret_cast<CLbfgsxx*>(pInstance);
        return p->m_fEvaluate(pVar, pGrad, N);
    }
    static int LBFGSXX_Progress (
        void* pInstance,
        const lbfgsfloatval_t* pVar,
        const lbfgsfloatval_t* pGrad,
        const lbfgsfloatval_t ObjValue,
        const lbfgsfloatval_t Xnorm,
        const lbfgsfloatval_t Gnorm,
        const lbfgsfloatval_t Step,
        int N,
        int IterNum,
        int EvalNum
        )
    {
        CLbfgsxx* p = reinterpret_cast<CLbfgsxx*>(pInstance);
        return p->m_fProgress(pVar, pGrad, N, ObjValue, Xnorm, Gnorm, IterNum, EvalNum, Step);
    }
public:
    static void PrintError(int ErrorNum)
    {
        switch (ErrorNum)
        {
        //case LBFGS_SUCCESS:
        case LBFGS_CONVERGENCE:
            std::cerr<<"Success, reaches convergence."<<std::endl;
            break;
        case LBFGS_STOP:
            std::cerr<<"Stopped."<<std::endl;
            break;
        case LBFGS_ALREADY_MINIMIZED:
            std::cerr<<"The initial variables already minimize the objective function."<<std::endl;
            break;
        case LBFGSERR_UNKNOWNERROR:
            std::cerr<<"Unknown error."<<std::endl;
            break;
        case LBFGSERR_LOGICERROR:
            std::cerr<<"Logic error."<<std::endl;
            break;
        case LBFGSERR_OUTOFMEMORY:
            std::cerr<<"Insufficient memory."<<std::endl;
            break;
        case LBFGSERR_CANCELED:
            std::cerr<<"The minimization process has been canceled."<<std::endl;
            break;
        case LBFGSERR_INVALID_N:
            std::cerr<<"Invalid number of variables specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_N_SSE:
            std::cerr<<"Invalid number of variables (for SSE) specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_X_SSE:
            std::cerr<<"The array x must be aligned to 16 (for SSE)."<<std::endl;
            break;
        case LBFGSERR_INVALID_EPSILON:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::epsilon specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_TESTPERIOD:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::past specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_DELTA:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::delta specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_LINESEARCH:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::linesearch specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_MINSTEP:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::max_step specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_MAXSTEP:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::max_step specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_FTOL:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::ftol specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_WOLFE:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::wolfe specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_GTOL:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::gtol specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_XTOL:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::xtol specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_MAXLINESEARCH:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::max_linesearch specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_ORTHANTWISE:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::orthantwise_c specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_ORTHANTWISE_START:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::orthantwise_start specified."<<std::endl;
            break;
        case LBFGSERR_INVALID_ORTHANTWISE_END:
            std::cerr<<"Invalid parameter lbfgs_parameter_t::orthantwise_end specified."<<std::endl;
            break;
        case LBFGSERR_OUTOFINTERVAL:
            std::cerr<<"The line-search step went out of the interval of uncertainty."<<std::endl;
            break;
        case LBFGSERR_INCORRECT_TMINMAX:
            std::cerr<<"A logic error occurred; alternatively, the interval of uncertainty became too small."<<std::endl;
            break;
        case LBFGSERR_ROUNDING_ERROR:
            std::cerr<<"A rounding error occurred; alternatively, no line-search step satisfies the sufficient decrease and curvature conditions."<<std::endl;
            break;
        case LBFGSERR_MINIMUMSTEP:
            std::cerr<<"The line-search step became smaller than lbfgs_parameter_t::min_step."<<std::endl;
            break;
        case LBFGSERR_MAXIMUMSTEP:
            std::cerr<<"The line-search step became larger than lbfgs_parameter_t::max_step."<<std::endl;
            break;
        case LBFGSERR_MAXIMUMLINESEARCH:
            std::cerr<<"The line-search routine reaches the maximum number of evaluations."<<std::endl; 
            break;
        case LBFGSERR_MAXIMUMITERATION:
            std::cerr<<"The algorithm routine reaches the maximum number of iterations."<<std::endl;
            break;
        case LBFGSERR_WIDTHTOOSMALL:
            std::cerr<<"Relative width of the interval of uncertainty is at most lbfgs_parameter_t::xtol."<<std::endl;
            break;
        case LBFGSERR_INVALIDPARAMETERS:
            std::cerr<<"A logic error (negative line-search step) occurred."<<std::endl;
            break;
        case LBFGSERR_INCREASEGRADIENT:
            std::cerr<<"The current search direction increases the objective function value."<<std::endl;
            break;
        default:
            std::cerr<<"Undefined error number, please check your codes."<<std::endl;
            assert(0);
            break;
        }
    }

public:
    explicit CLbfgsxx(
        lbfgsfloatval_t* pVar,
        int N,
        const lbfgs_parameter_t* pParam = NULL,
        _FEvaluate fEvaluate = _FEvaluate(), 
        _FProgress fProgress = _FProgress()) 
        : m_pVar(pVar), m_N(N), m_ObjValue(), m_pParam(pParam), m_fEvaluate(fEvaluate), m_fProgress(fProgress)
    {
        assert(m_N > 0);
    }
    int Run()
    {
#if     defined(USE_SSE) && (defined(__SSE__) || defined(__SSE2__))
        //  since SSE need call lbfgs_malloc and lbfgs_free, the memory spaces may be bigger
        static_assert(0, "LBFGSXX do not support SSE");
#endif
        int ret = lbfgs(
            m_N,
            m_pVar,
            &m_ObjValue,
            LBFGSXX_Evaluate, 
            LBFGSXX_Progress, 
            this, 
            m_pParam);
        return ret;
    }
    //  get the value of objective function
    lbfgsfloatval_t ObjValue() const
    {
        return m_ObjValue;
    }
};

#endif


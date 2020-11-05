#include <iostream>
#include <string>

#include "fitsio.h"

#include "fits_io.h"
#include "api.h"
#include "numeric_utils.h"
#include "normal_io.h"
#include "lsfit.h"

#include "stdafx.h"
#include "interpolation.h"

using namespace std;
using namespace alglib;


static void function_cx_1_func(const real_1d_array &c, const real_1d_array &x, double &func, void *ptr)
{
    // this callback calculates f(c,x)=exp(-c0*sqr(x0))
    // where x is a position on X-axis and c is adjustable parameter
    func = exp(-c[0] * pow(x[0], 2));
}

void test_exp_func_fit()
{
    real_2d_array x = "[[-1],[-0.8],[-0.6],[-0.4],[-0.2],[0],[0.2],[0.4],[0.6],[0.8],[1.0]]";
    real_1d_array y = "[0.223130, 0.382893, 0.582748, 0.786628, 0.941765, 1.000000, 0.941765, 0.786628, 0.582748, 0.382893, 0.223130]";
    real_1d_array c = "[0.3]";
    real_2d_array test;
    vector<double> v;
    test.setcontent(v.size() / 2, 2, v.data());

    double diffstep = 1e-4;
    lsfitstate state;
    ae_int_t info;
    lsfitreport report;

    lsfitcreatef(x, y, c, diffstep, state);
    lsfitfit(state, function_cx_1_func);
    lsfitresults(state, info, c, report);
}


static void func2d(const real_1d_array &c, const real_1d_array &x, double &func, void *ptr)
{
    // this callback calculates f(c,x)=exp(-c0*sqr(x0))
    // where x is a position on X-axis and c is adjustable parameter

    // func = exp(-c[0]*pow(x[0],2));
    func = exp(-c[0] * pow(x[0], 2) + c[1] * x[1]);
}

void test_exp_func_multi_params()
{
    lsfitstate state;
    ae_int_t info;
    lsfitreport report;

    vector<double> vx = {-1, -1, -0.8, -0.8, -0.6, -0.6, -0.4, -0.4, -0.2, -0.2, 0, 0, 0.2, 0.2, 0.4, 0.4, 0.6, 0.6, 0.8, 0.8, 1, 1};
    vector<double> vy = vx;

    real_2d_array x;
    real_1d_array y = "[0.082085,0.16398177,0.29760148, 0.49066189, 0.73491532, 1, 1.23614789, 1.38818897, 1.41623225, 1.312587, 1.10517092]";
    real_1d_array c = "[0.0, 0.0]";

    x.setcontent(vx.size() / 2, 2, vx.data());

    try
    {
        lsfitcreatef(x, y, c, 1e-4, state);
    }
    catch (ap_error &e)
    {
        cout << e.msg << endl;
    }

    lsfitfit(state, func2d);
    lsfitresults(state, info, c, report);
    cout << c[0] << "  " << c[1] << endl;
}

void func_more(const real_1d_array &c, const real_1d_array &x, double &func, void *ptr)
{
    // this callback calculates f(c,x)=exp(-c0*sqr(x0))
    // where x is a position on X-axis and c is adjustable parameter

    // func = exp(-c[0]*pow(x[0],2));
    func = c[0]*x[0] + c[1];
}


void test_lsfitlinear(){
    ae_int_t info;
    lsfitreport report;
    real_2d_array x = "[[1, -1],[1, -0.8],[1, -0.6],[1, -0.4],[1, -0.2],[1, 0],[1, 0.2],[1, 0.4],[1, 0.6],[1, 0.8],[1, 1.0]]";
    // real_1d_array y = "[0.223130, 0.382893, 0.582748, 0.786628, 0.941765, 1.000000, 0.941765, 0.786628, 0.582748, 0.382893, 0.223130]";
    real_1d_array y = "[0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2]";
    real_1d_array c = "[0.3]";

    lsfitlinear(y, x, info, c, report);

    cout << c[0] << "  " << c[1] << endl;
    
}
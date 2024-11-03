/*
Copyright (c) 2024 Eugene Kirian

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <float.h>
#include <limits.h>

#if _MSC_VER <=1800
#ifndef __HUGE_VALUE
#define __HUGE_VALUE  1e+300
#endif

#define INFINITY   ((float)(__HUGE_VALUE * __HUGE_VALUE))
#define NAN        ((float)(INFINITY * 0.0F))
#endif

void basic_float_operations_examples()
{
    const static float values1[] = { 0.0f, 0.5f, 1.0f, 2.0f, 64.0f, -1.0f, -10.0f };
    const static float values2[] = { 1.0f, 2.2f, (float)M_PI, (float)M_E, (float)M_LOG2E, (float)M_LN2, (float)M_SQRT2 };

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const float result = values1[i] + values2[j];

            printf("%f + %f = %f\n", values1[i], values2[j], result);
        }
    }

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const float result = values1[i] - values2[j];

            printf("%f - %f = %f\n", values1[i], values2[j], result);
        }
    }

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const float result = values1[i] * values2[j];

            printf("%f * %f = %f\n", values1[i], values2[j], result);
        }
    }

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const float result = values1[i] / values2[j];

            printf("%f / %f = %f\n", values1[i], values2[j], result);
        }
    }

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const bool result = values1[i] == values2[j];

            printf("%f == %f = %s\n", values1[i], values2[j], result ? "TRUE" : "FALSE");
        }
    }
}

void basic_double_operations_examples()
{
    const double values1[] = { 0.0, 0.5, 1.0, 2.0, 64.0, -1.0, -10.0 };
    const double values2[] = { 1.0, 2.2, M_PI, M_E, M_LOG2E, M_LN2, M_SQRT2 };

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const double result = values1[i] + values2[j];

            printf("%lf + %lf = %f\n", values1[i], values2[j], result);
        }
    }

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const double result = values1[i] - values2[j];

            printf("%lf - %lf = %lf\n", values1[i], values2[j], result);
        }
    }

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const double result = values1[i] * values2[j];

            printf("%lf * %lf = %lf\n", values1[i], values2[j], result);
        }
    }

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const double result = values1[i] / values2[j];

            printf("%lf / %lf = %f\n", values1[i], values2[j], result);
        }
    }

    for (int i = 0; i < _countof(values1); i++)
    {
        for (int j = 0; j < _countof(values2); j++)
        {
            const bool result = values1[i] == values2[j];

            printf("%lf == %lf = %s\n", values1[i], values2[j], result ? "TRUE" : "FALSE");
        }
    }
}

float convert_int_to_float(const int input)
{
    return (float)input;
}

double convert_int_to_double(const int input)
{
    return (double)input;
}

int convert_float_to_int(const float input)
{
    return (int)input;
}

int convert_double_to_int(const double input)
{
    return (int)input;
}

void basic_conversions_examples()
{
    printf("Converting 17.5 (float) to int is %d\n", convert_float_to_int(17.5f));
    printf("Converting 345.1 (float) to int is %d\n", convert_float_to_int(345.1f));
    printf("Converting -123.3 (double) to int is %d\n", convert_double_to_int(-123.3));
    printf("Converting 999.9 (double) to int is %d\n", convert_double_to_int(999.9));

    printf("\n----------\n");

    printf("Converting 7 to float is %f\n", convert_int_to_float(7));
    printf("Converting -15 to float is %f\n", convert_int_to_float(-15));
    printf("Converting 19 to double is %lf\n", convert_int_to_double(19));
    printf("Converting -201 to float is %lf\n", convert_int_to_double(-201));
}

void basic_operations_examples()
{
    basic_float_operations_examples();

    printf("\n----------\n");

    basic_double_operations_examples();

    printf("\n----------\n");

    basic_conversions_examples();

    printf("\n----------\n");
}

void basic_functions_examples()
{
    printf("fabs(+7.0)      = %f\n", fabs(+7.0));
    printf("fabs(-7.0)      = %f\n", fabs(-7.0));
    printf("fabs(-0.0)      = %f\n", fabs(-0.0));
    printf("fabs(-INFINITY) = %f\n", fabs(-INFINITY));

    printf("\n----------\n");

    printf("fmod(+17.1, +3.0)       = %.1lf\n", fmod(17.1, 3.0));
    printf("fmod(-19.1, +3.0)       = %.1lf\n", fmod(-19.1, 3.0));
    printf("fmod(+7.1, -4.0)        = %.1lf\n", fmod(7.1, -4.0));
    printf("fmod(-13.1, -2.0)       = %.1lf\n", fmod(-13.1, -2.0));
    printf("fmod(+0.0, 1.0)         = %.1lf\n", fmod(0.0, 1.0));
    printf("fmod(-0.0, 1.0)         = %.1lf\n", fmod(-0.0, 1.0));
    printf("fmod(+5.1, INFINITY)    = %.1lf\n", fmod(5.1, (double)INFINITY));

    printf("\n----------\n");
}

const char* classify_value(double x)
{
    switch (_fpclass(x))
    {
    case _FPCLASS_SNAN: return "Signaling NaN";
    case _FPCLASS_QNAN: return "Quiet NaN";
    case _FPCLASS_NINF: return "Negative Infinity";
    case _FPCLASS_NN:   return "Negative Normal";
    case _FPCLASS_ND:   return "Negative Denormal";
    case _FPCLASS_NZ:   return "Negative Zero";
    case _FPCLASS_PZ:   return "Positive Zero";
    case _FPCLASS_PD:   return "Positive Denormal";
    case _FPCLASS_PN:   return "Positive Normal";
    case _FPCLASS_PINF: return "Positive Infinity";
    }

    return "Unknown";
}

void classification_fpclass_example()
{
    const double zero = 0.0;

    printf("1.0/0.0   is %s\n", classify_value(1.0 / zero));
    printf("0.0/0.0   is %s\n", classify_value(0.0 / zero));
    printf("DBL_MIN/2 is %s\n", classify_value(DBL_MIN / 2));
    printf("-0.0      is %s\n", classify_value(-0.0));
    printf("1.0       is %s\n", classify_value(1.0));
}

void classification_isfinite_example()
{
    const double zero = 0.0;

    printf("_finite(NAN)                    = %d\n", _finite(NAN));
    printf("_finite(INFINITY)               = %d\n", _finite(INFINITY));
    printf("_finite(DBL_MIN/2.0)            = %d\n", _finite(DBL_MIN / 2.0));
    printf("_finite(0.0)                    = %d\n", _finite(0.0));
    printf("_finite(0.0 / 0.0)              = %d\n", _finite(0.0 / zero));
    printf("_finite(1.0)                    = %d\n", _finite(1.0));
    printf("_finite(exp(800.0))             = %d\n", _finite(exp(800.0)));
    printf("_finite(100.17)                 = %d\n", _finite(100.17));
    printf("_finite(INFINITY - INFINITY)    = %d\n", _finite(INFINITY - INFINITY));
}

void classification_isnan_example()
{
    const double zero = 0.0;

    printf("_isnan(NAN)                 = %d\n", _isnan(NAN));
    printf("_isnan(INFINITY)            = %d\n", _isnan(INFINITY));
    printf("_isnan(DBL_MIN/2.0)         = %d\n", _isnan(DBL_MIN / 2.0));
    printf("_isnan(0.0)                 = %d\n", _isnan(0.0));
    printf("_isnan(0.0 / 0.0)           = %d\n", _isnan(0.0 / zero));
    printf("_isnan(1.0)                 = %d\n", _isnan(1.0));
    printf("_isnan(exp(800.0))          = %d\n", _isnan(exp(800.0)));
    printf("_isnan(100.17)              = %d\n", _isnan(100.17));
    printf("_isnan(INFINITY - INFINITY) = %d\n", _isnan(INFINITY - INFINITY));
}

void classification_examples()
{
    classification_fpclass_example();

    printf("\n----------\n");

    classification_isfinite_example();

    printf("\n----------\n");
    
    classification_isnan_example();

    printf("\n----------\n");
}

void exponential_examples()
{
    printf("exp(0.007)      = %lf\n", exp(0.007));
    printf("exp(1.0)        = %lf\n", exp(1.0));
    printf("exp(17.0)       = %lf\n", exp(17.0));
    printf("exp(-0.0)       = %lf\n", exp(-0.0));
    printf("exp(-0.1)       = %lf\n", exp(-0.1));
    printf("exp(-INFINITY)  = %lf\n", exp((double)-INFINITY));

    printf("\n----------\n");

    printf("log(1.0)        = %lf\n", log(1.0));
    printf("log(137.0)      = %lf\n", log(137.0));
    printf("log(0.01)       = %lf\n", log(0.01));
    printf("log(INFINITY)   = %lf\n", log((double)INFINITY));

    printf("\n----------\n");
    
    printf("log10(0.001)    = %lf\n", log10(0.001));
    printf("log10(1000.0)   = %lf\n", log10(1000.0));
    printf("log10(137.0)    = %lf\n", log10(137.0));
    printf("log10(1.0)      = %lf\n", log10(1.0));
    printf("log10(INFINITY) = %lf\n", log10((double)INFINITY));

    printf("\n----------\n");
}

void power_examples()
{
    printf("pow(2.0, 11.0)      = %lf\n", pow(2.0, 11.0));
    printf("pow(2.0, 0.5)       = %lf\n", pow(2.0, 0.5));
    printf("pow(-17.0, -3.0)    = %lf\n", pow(-17.0, -3.0));
    printf("pow(INFINITY, 2.0)  = %lf\n", pow((double)INFINITY, 2.0));
    printf("pow(INFINITY, -1.0) = %lf\n", pow((double)INFINITY, -1.0));
    printf("pow(-1.0, NAN)      = %lf\n", pow(-1.0, (double)NAN));
    printf("pow(+1.0, NAN)      = %lf\n", pow(+1.0, (double)NAN));

    printf("\n----------\n");

    printf("sqrt(100.0) = %lf\n", sqrt(100.0));
    printf("sqrt(2.0)   = %lf\n", sqrt(2.0));
    printf("sqrt(5.0)   = %lf\n", sqrt(5.0));
    printf("sqrt(-0.0)  = %lf\n", sqrt(-0.0));
    printf("sqrt(-1.0)  = %lf\n", sqrt(-1.0));

    printf("\n----------\n");
}

void rounding_examples()
{
    printf("(int)(+12.7)        = %d\n", (int)(+12.7));
    printf("(int)(-12.7)        = %d\n", (int)(-12.7));
    printf("(int)(-0.0)         = %d\n", (int)(-0.0));
    printf("(int)(-INFINITY)    = %d\n", (int)(-INFINITY));
    printf("(int)(NAN)          = %d\n", (int)(NAN));

    printf("\n----------\n");

    printf("floor(+12.7)        = %+.1f\n", floor(12.7));
    printf("floor(-12.7)        = %+.1f\n", floor(-12.7));
    printf("floor(-0.0)         = %+.1f\n", floor(-0.0));
    printf("floor(-INFINITY)    = %+f\n",   floor(-INFINITY));
    printf("floor(NAN)          = %+f\n", floor(NAN));

    printf("\n----------\n");

    printf("ceil(+12.7)     = %+.1f\n", ceil(12.7));
    printf("ceil(-12.7)     = %+.1f\n", ceil(-12.7));
    printf("ceil(-0.0)      = %+.1f\n", ceil(-0.0));
    printf("ceil(-INFINITY) = %+f\n",   ceil(-INFINITY));
    printf("ceil(NAN)       = %+f\n", ceil(NAN));

    printf("\n----------\n");
}

void trigonometric_examples()
{
    printf("sin(PI / 2.0)           = %lf\n", sin(M_PI / 2.0));
    printf("sin(PI / 6.0)           = %lf\n", sin(M_PI / 6.0));
    printf("sin(-3.0 * PI / 4.0)    = %lf\n", sin(-3.0 * M_PI / 4.0));
    printf("sin(+0.0)               = %lf\n", sin(0.0));
    printf("sin(-0.0)               = %lf\n", sin(-0.0));
    printf("sin(INFINITY)           = %lf\n", sin((double)INFINITY));

    printf("\n----------\n");
    
    printf("cos(PI / 2.0)           = %lf\n", cos(M_PI / 2.0));
    printf("cos(PI / 3.0)           = %lf\n", cos(M_PI / 3.0));
    printf("cos(PI / 4.0)           = %lf\n", cos(M_PI / 4.0));
    printf("cos(-3.0 * PI / 4.0)    = %lf\n", cos(-3.0 * M_PI / 4.0));
    printf("cos(+0.0)               = %lf\n", cos(0.0));
    printf("cos(-0.0)               = %lf\n", cos(-0.0));
    printf("cos(INFINITY)           = %lf\n", cos((double)INFINITY));

    printf("\n----------\n");

    printf("tan(PI * 1.0 / 4.0) = %+f\n", tan(M_PI * 1.0 / 4.0));
    printf("tan(PI * 3.0 / 4.0) = %+f\n", tan(M_PI * 3.0 / 4.0));
    printf("tan(PI * 5.0 / 4.0) = %+f\n", tan(M_PI * 5.0 / 4.0));
    printf("tan(PI * 7.0 / 4.0) = %+f\n", tan(M_PI * 7.0 / 4.0));
    printf("tan(+0.0)       = %f\n", tan(0.0));
    printf("tan(-0.0)       = %f\n", tan(-0.0));
    printf("tan(INFINITY)   = %lf\n", tan((double)INFINITY));
    

    printf("\n----------\n");
    
    printf("asin(1.0)  = %+lf, 2.0 * asin(1.0)  = %+lf\n", asin(1.0), 2.0 * asin(1.0));
    printf("asin(-0.5) = %+lf, 6.0 * asin(-0.5) = %+f\n", asin(-0.5), 6 * asin(-0.5));
    printf("asin(0.0)  = %1lf, asin(-0.0)       = %lf\n", asin(+0.0), asin(-0.0));
    printf("asin(NAN)  = %1lf, asin(INFINITY)   = %lf\n", asin((double)NAN), asin((double)INFINITY));

    printf("\n----------\n");

    printf("acos(-1.0) = %lf\n", acos(-1.0));
    printf("acos(0.0)  = %lf, 2.0 * acos(0.0) = %lf\n", acos(0.0), 2.0 * acos(0.0));
    printf("acos(0.5)  = %lf, 3.0 * acos(0.5) = %lf\n", acos(0.5), 3.0 * acos(0.5));
    printf("acos(1.0)  = %lf\n", acos(1.0));

    printf("\n----------\n");
    
    printf("atan(1.0)       = %lf, 4.0 * atan(1.0)      = %lf\n", atan(1.0), 4 * atan(1.0));
    printf("atan(INFINITY)  = %lf, 2.0 * atan(INFINITY) = %lf\n", atan(INFINITY), 2 * atan(INFINITY));
    printf("atan(-0.0)      = %+lf, atan(+0.0)          = %+lf\n", atan(-0.0), atan(0.0));

    printf("\n----------\n");

    printf("atan2(0.0, 0.0) = %lf\tatan2(0.0, -0.0)=%f\n", atan2(0.0, 0.0), atan2(0, -0.0));
    printf("atan2(9.0, 0.0) = %lf\tatan2(9.0, -0.0)=%f\n", atan2(9.0, 0.0), atan2(9, -0.0));
}

int main(int argc, char** argv)
{
    basic_operations_examples();
    basic_functions_examples();
    classification_examples();
    exponential_examples();
    power_examples();
    rounding_examples();
    trigonometric_examples();

    return 0;
}
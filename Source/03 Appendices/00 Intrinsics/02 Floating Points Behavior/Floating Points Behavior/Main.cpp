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

#include <float.h>
#include <stdio.h>

// https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170

int main(int argc, char** argv)
{
    float a = FLT_MAX;
    float b = -1.1f;
    float lower = 0.0f;
    float upper = 0.0f;
    unsigned int control = 0;
    int err = 0;

    err = _controlfp_s(&control, _RC_DOWN, _MCW_RC);
    if (err)
    {
        printf("_controlfp_s(&control, _RC_DOWN, _MCW_RC) failed with control: %d\n", err);
    }  

    lower = a * b;

    err = _controlfp_s(&control, _RC_UP, _MCW_RC);
    if (err)
    {
        printf("_controlfp_s(&control, _RC_UP, _MCW_RC) failed with control: %d\n", err);
    }

    upper = a * b;

    err = _controlfp_s(&control, _CW_DEFAULT, _MCW_RC);
    if (err)
    {
        printf("_controlfp_s(&control, _CW_DEFAULT, _MCW_RC) failed with control: %d\n", err);
    }

    printf("Lower = %f\nUpper = %f\n", lower, upper);

    return 0;
}
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

#include <stdio.h>

struct xy
{
    float x, y;

    xy ()
    {
        x = 0.0f;
        y = 0.0f;
    }

    xy (const float a, const float b)
    {
        x = a;
        y = b;
    }

    xy operator- (const xy& rhs) const
    {
        return xy(x - rhs.x, y - rhs.y);
    }

    xy operator+ (const xy& rhs) const
    {
        return xy(x + rhs.x, y + rhs.y);
    }

    xy& operator += (const xy& value)
    {
        this->x += value.x;
        this->y += value.y;

        return *this;
    }

    xy& operator -= (const xy& value)
    {
        this->x -= value.x;
        this->y -= value.y;

        return *this;
    }

    xy operator+ (const float value)
    {
        return xy(x + value, y + value);
    }

    xy operator- (const float value)
    {
        return xy(x - value, y - value);
    }

    xy& operator += (const float value)
    {
        x += value;
        y += value;

        return *this;
    }

    xy& operator -= (const float value)
    {
        x -= value;
        y -= value;

        return *this;
    }
};

int main(int argc, char** argv)
{
    xy one = xy(1.0f, 1.0f);

    one = one + 1.0f;

    printf("x = %f y = %f\n", one.x, one.y);

    one += 3.0;

    printf("x = %f y = %f\n", one.x, one.y);

    one += xy(7.0f, 11.0f);

    printf("x = %f y = %f\n", one.x, one.y);

    return 0;
}
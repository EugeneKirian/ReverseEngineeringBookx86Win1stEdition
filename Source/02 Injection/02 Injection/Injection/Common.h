/*
Copyright (c) 2025 Eugene Kirian

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

struct UnknownStruct0x18
{
    float   Unk00;
    float   Unk01;
    float   Unk02;
    int     Unk03;
    float   Unk04;
    float   Unk05;
};

struct UnknownStruct
{
    int                 Unk00[5];
    UnknownStruct0x18   Unk01[6];
    float               Unk02[16];
    float               Unk03[16];
    float               Unk04[16];
};

struct Game
{
    int             Unk00;
    int             Unk01;
    void*           Assets;
    void*           Unk02;
    void*           Unk03;
    void*           Unk04;

    UnknownStruct   Unk05;
    UnknownStruct   Unk06;
    UnknownStruct   Unk07;
    
    unsigned char Unk08[256];
    unsigned char Unk09[256];
};

struct AssetContent
{
    int     Size;
    void*   State;
};
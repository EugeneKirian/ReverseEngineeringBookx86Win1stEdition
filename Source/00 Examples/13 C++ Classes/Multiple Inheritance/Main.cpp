/*
Copyright (c) 2024 - 2025 Eugene Kirian

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

#include "Button.h"
#include "Image.h"

#include <stdio.h>

void button_example()
{
    Button* button = new Button();

    button->SetText("Click Me!");
    button->SetX(button->GetX() + 20);
    button->SetY(button->GetY() + 50);

    printf("Button X: %d, Y: %d\n",
        button->GetX(), button->GetY());

    delete button;
}

void image_example()
{
    Image* image = new Image();

    image->SetImage("image.jpg");
    image->SetX(image->GetX() + 20);
    image->SetY(image->GetY() + 50);

    printf("Image X: %d, Y: %d\n",
        image->GetX(), image->GetY());

    delete image;
}

int main(int argc, char** argv)
{
    button_example();
    image_example();

    return 0;
}
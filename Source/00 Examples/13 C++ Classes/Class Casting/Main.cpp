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

#include "Button.h"
#include "Image.h"

#include <stdio.h>

void static_cast_up_example()
{
    Button* original = new Button();

    original->SetText("Button Text");

    Control* control = static_cast<Control*>(original);

    printf("Control type is %d\n", control->Type());

    Button* button = static_cast<Button*>(control);

    printf("Button text is %s\n", button->GetText());

    delete button;
}

void static_cast_down_example()
{
    Control* original = new Control();

    Button* button = static_cast<Button*>(original);

    printf("Control type is %d\n", button->Type());

    if (button->Type() == CONTROL_TYPE_BUTTON)
    {
        printf("Button text is %s\n", button->GetText());
    }
    else
    {
        printf("Control is not a button.\n");
    }

    delete button;
}

void valid_dynamic_cast_example()
{
    Image* original = new Image();

    original->SetImage("image.bmp");

    Control* control = dynamic_cast<Control*>(original);

    if (control != NULL)
    {
        printf("Control type is %d\n", control->Type());
    }

    delete control;
}

void invalid_dynamic_cast_example()
{
    Image* original = new Image();

    original->SetImage("image.bmp");

    Button* button = dynamic_cast<Button*>(original);

    if (button != NULL)
    {
        printf("Control type is %d\n", button->Type());
    }

    delete button;
}

int main(int argc, char** argv)
{
    static_cast_up_example();
    static_cast_down_example();
    valid_dynamic_cast_example();
    invalid_dynamic_cast_example();

    return 0;
}
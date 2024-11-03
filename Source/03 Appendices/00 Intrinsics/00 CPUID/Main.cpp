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

#include <intrin.h>
#include <stdio.h>
#include <string.h>

typedef enum PROCESSOR_VENDOR
{
    PROCESSOR_VENDOR_UNKNOWN    = 0,
    PROCESSOR_VENDOR_INTEL      = 1,
    PROCESSOR_VENDOR_AMD        = 2
} PROCESSOR_VENDOR;

PROCESSOR_VENDOR acquire_processor_vendor()
{
    int info[4] = {-1};

    __cpuid(info, 0);

    char vendor[0x20];
    memset(vendor, 0, sizeof(vendor));

    *reinterpret_cast<int*>(vendor + 0) = info[1];
    *reinterpret_cast<int*>(vendor + 4) = info[3];
    *reinterpret_cast<int*>(vendor + 8) = info[2];

    PROCESSOR_VENDOR result = PROCESSOR_VENDOR_UNKNOWN;

    if (strcmp(vendor, "GenuineIntel") == 0)
    {
        result = PROCESSOR_VENDOR_INTEL;
    }
    else if (strcmp(vendor, "AuthenticAMD") == 0)
    {
        result = PROCESSOR_VENDOR_AMD;
    }

    return result;
}

int main(int argc, char** argv)
{
    const PROCESSOR_VENDOR vendor = acquire_processor_vendor();

    printf("Processor is: ");

    switch (vendor)
    {
    case PROCESSOR_VENDOR_UNKNOWN:
        {
            printf("Unknown\n");

            break;
        }
    case PROCESSOR_VENDOR_INTEL:
        {
            printf("Intel\n");

            break;
        }
    case PROCESSOR_VENDOR_AMD:
        {
            printf("AMD\n");

            break;
        }
    }

	return 0;
}
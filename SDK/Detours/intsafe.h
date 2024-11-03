/******************************************************************
*                                                                 *
*  intsafe.h -- This module defines helper functions to prevent   *
*               integer overflow bugs.                            *
*                                                                 *
*  Copyright (c) Microsoft Corp.  All rights reserved.            *
*                                                                 *
******************************************************************/

#if _MSC_VER == 1400 // MSVC 2005

#ifndef _INTSAFE_H_INCLUDED_
#define _INTSAFE_H_INCLUDED_

#include <windows.h>

#define INTSAFE_E_ARITHMETIC_OVERFLOW   ((HRESULT)0x80070216L)

#define LONG_ERROR      (-1L)
#define ULONG_ERROR     0xffffffffUL

//
// ULONG addition
//

__inline
HRESULT
ULongAdd(
    ULONG ulAugend,
    ULONG ulAddend,
    ULONG* pulResult)
{
    HRESULT hr;

    if ((ulAugend + ulAddend) >= ulAugend)
    {
        *pulResult = (ulAugend + ulAddend);
        hr = S_OK;
    }
    else
    {
        *pulResult = ULONG_ERROR;
        hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
    
    return hr;
}

//
// DWORD addition
//
#define DWordAdd        ULongAdd

//
// DWORD multiplication
//
#define DWordMult       ULongMult

//
// ULONGLONG -> ULONG conversion
//

__inline
HRESULT
ULongLongToULong(
    ULONGLONG ullOperand,
    ULONG* pulResult)
{
    HRESULT hr;
    
    if (ullOperand <= ULONG_MAX)
    {
        *pulResult = (ULONG)ullOperand;
        hr = S_OK;
    }
    else
    {
        *pulResult = ULONG_ERROR;
        hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
    
    return hr;
}

//
// ULONG multiplication
//

__inline
HRESULT
ULongMult(
    ULONG ulMultiplicand,
    ULONG ulMultiplier,
    ULONG* pulResult)
{
    ULONGLONG ull64Result = UInt32x32To64(ulMultiplicand, ulMultiplier);
    
    return ULongLongToULong(ull64Result, pulResult);
}


#endif // _INTSAFE_H_INCLUDED_
#endif
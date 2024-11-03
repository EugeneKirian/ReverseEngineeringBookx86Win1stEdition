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

#include <windows.h>
#include <d3d9.h>
#include <dsound.h>

enum EXAMPLE_ENUM
{
    EXAMPLE_ENUM_ONE    = 1,
    EXAMPLE_ENUM_TWO    = 2,
    EXAMPLE_ENUM_THREE  = 3,
    EXAMPLE_ENUM_FOUR   = 4,
    EXAMPLE_ENUM_FIVE   = 5,
    EXAMPLE_ENUM_SIX    = 6
};

const int small_switch_example(EXAMPLE_ENUM input)
{
    switch (input)
    {
    case EXAMPLE_ENUM_ONE:
    case EXAMPLE_ENUM_TWO: return 20;
    case EXAMPLE_ENUM_FOUR: return 30;
    case EXAMPLE_ENUM_FIVE: return 50;
    default: return 70;
    }

    return -1;
}

const char* large_switch_example(HRESULT err)
{
    switch (err)
    {
    case D3D_OK:                                return "S_OK";
    case D3DERR_WRONGTEXTUREFORMAT:             return "D3DERR_WRONGTEXTUREFORMAT";
    case D3DERR_UNSUPPORTEDCOLOROPERATION:      return "D3DERR_UNSUPPORTEDCOLOROPERATION";
    case D3DERR_UNSUPPORTEDCOLORARG:            return "D3DERR_UNSUPPORTEDCOLORARG";
    case D3DERR_UNSUPPORTEDALPHAOPERATION:      return "D3DERR_UNSUPPORTEDALPHAOPERATION";
    case D3DERR_UNSUPPORTEDALPHAARG:            return "D3DERR_UNSUPPORTEDALPHAARG";
    case D3DERR_TOOMANYOPERATIONS:              return "D3DERR_TOOMANYOPERATIONS";
    case D3DERR_CONFLICTINGTEXTUREFILTER:       return "D3DERR_CONFLICTINGTEXTUREFILTER";
    case D3DERR_UNSUPPORTEDFACTORVALUE:         return "D3DERR_UNSUPPORTEDFACTORVALUE";
    case D3DERR_CONFLICTINGRENDERSTATE:         return "D3DERR_CONFLICTINGRENDERSTATE";
    case D3DERR_UNSUPPORTEDTEXTUREFILTER:       return "D3DERR_UNSUPPORTEDTEXTUREFILTER";
    case D3DERR_CONFLICTINGTEXTUREPALETTE:      return "D3DERR_CONFLICTINGTEXTUREPALETTE";
    case D3DERR_DRIVERINTERNALERROR:            return "D3DERR_DRIVERINTERNALERROR";
    case D3DERR_NOTFOUND:                       return "D3DERR_NOTFOUND";
    case D3DERR_MOREDATA:                       return "D3DERR_MOREDATA";
    case D3DERR_DEVICELOST:                     return "D3DERR_DEVICELOST";
    case D3DERR_DEVICENOTRESET:                 return "D3DERR_DEVICENOTRESET";
    case D3DERR_NOTAVAILABLE:                   return "D3DERR_NOTAVAILABLE";
    case D3DERR_OUTOFVIDEOMEMORY:               return "D3DERR_OUTOFVIDEOMEMORY";
    case D3DERR_INVALIDDEVICE:                  return "D3DERR_INVALIDDEVICE";
    case D3DERR_INVALIDCALL:                    return "D3DERR_INVALIDCALL";
    case D3DERR_DRIVERINVALIDCALL:              return "D3DERR_DRIVERINVALIDCALL";
    case D3DERR_WASSTILLDRAWING:                return "D3DERR_WASSTILLDRAWING";
    case D3DOK_NOAUTOGEN:                       return "D3DOK_NOAUTOGEN";
    case D3DERR_DEVICEREMOVED:                  return "D3DERR_DEVICEREMOVED";
    case S_NOT_RESIDENT:                        return "S_NOT_RESIDENT";
    case S_RESIDENT_IN_SHARED_MEMORY:           return "S_RESIDENT_IN_SHARED_MEMORY";
    case S_PRESENT_MODE_CHANGED:                return "S_PRESENT_MODE_CHANGED";
    case S_PRESENT_OCCLUDED:                    return "S_PRESENT_OCCLUDED";
    case D3DERR_DEVICEHUNG:                     return "D3DERR_DEVICEHUNG";
    case D3DERR_UNSUPPORTEDOVERLAY:             return "D3DERR_UNSUPPORTEDOVERLAY";
    case D3DERR_UNSUPPORTEDOVERLAYFORMAT:       return "D3DERR_UNSUPPORTEDOVERLAYFORMAT";
    case D3DERR_CANNOTPROTECTCONTENT:           return "D3DERR_CANNOTPROTECTCONTENT";
    case D3DERR_UNSUPPORTEDCRYPTO:              return "D3DERR_UNSUPPORTEDCRYPTO";
    case D3DERR_PRESENT_STATISTICS_DISJOINT:    return "D3DERR_PRESENT_STATISTICS_DISJOINT";
    case DS_NO_VIRTUALIZATION:                  return "DS_NO_VIRTUALIZATION";
    case DSERR_ALLOCATED:                       return "DSERR_ALLOCATED";
    case DSERR_CONTROLUNAVAIL:                  return "DSERR_CONTROLUNAVAIL";
    case DSERR_INVALIDPARAM:                    return "E_INVALIDARG";
    case DSERR_INVALIDCALL:                     return "DSERR_INVALIDCALL";
    case DSERR_GENERIC:                         return "E_FAIL";
    case DSERR_PRIOLEVELNEEDED:                 return "DSERR_PRIOLEVELNEEDED";
    case DSERR_OUTOFMEMORY:                     return "E_OUTOFMEMORY";
    case DSERR_BADFORMAT:                       return "DSERR_BADFORMAT";
    case DSERR_UNSUPPORTED:                     return "E_NOTIMPL";
    case DSERR_NODRIVER:                        return "DSERR_NODRIVER";
    case DSERR_ALREADYINITIALIZED:              return "DSERR_ALREADYINITIALIZED";
    case DSERR_NOAGGREGATION:                   return "DSERR_NOAGGREGATION";
    case DSERR_BUFFERLOST:                      return "DSERR_BUFFERLOST";
    case DSERR_OTHERAPPHASPRIO:                 return "DSERR_OTHERAPPHASPRIO";
    case DSERR_UNINITIALIZED:                   return "DSERR_UNINITIALIZED";
    case DSERR_NOINTERFACE:                     return "E_NOINTERFACE";
    case DSERR_ACCESSDENIED:                    return "E_ACCESSDENIED";
    case DSERR_BUFFERTOOSMALL:                  return "DSERR_BUFFERTOOSMALL";
    case DSERR_DS8_REQUIRED:                    return "DSERR_DS8_REQUIRED";
    case DSERR_SENDLOOP:                        return "DSERR_SENDLOOP";
    case DSERR_BADSENDBUFFERGUID:               return "DSERR_BADSENDBUFFERGUID";
    case DSERR_OBJECTNOTFOUND:                  return "DSERR_OBJECTNOTFOUND";
    case DSERR_FXUNAVAILABLE:                   return "DSERR_FXUNAVAILABLE";
    }

    return "<UNKNOWN>";
}

int main(int argc, char** argv)
{
    printf("%d\n", small_switch_example(EXAMPLE_ENUM_ONE));
    printf("%d\n", small_switch_example(EXAMPLE_ENUM_FOUR));
    printf("%s\n", large_switch_example(D3DERR_TOOMANYOPERATIONS));
    printf("%s\n", large_switch_example(DSERR_FXUNAVAILABLE));

    return 0;
}
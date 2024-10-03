#include <stdio.h>

#include "win32screencap.h"
#include "util.h"

void GetBitmapBuffer(HDC hdc, HBITMAP hBitmap, LPBYTE& bp, DWORD& size, WORD& bbp) {
    BITMAPINFO bmi;
    LPBITMAPINFO lpbmi =  &bmi;

    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);

    // Load information of bitmap to BITMAPINFO
    int r = GetDIBits(hdc, hBitmap, 0, 0, NULL, lpbmi, DIB_RGB_COLORS);
    if (!r) PrintError(r);

    LPBYTE oldp = bp;
    LPBYTE pixBuf = (LPBYTE) malloc(bmi.bmiHeader.biSizeImage);
    bp = pixBuf;
    if (oldp!=NULL) free(oldp);

    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biHeight = -abs(bmi.bmiHeader.biHeight); // Positive height means top down bitmap

    r = GetDIBits(hdc, hBitmap, 0, bmi.bmiHeader.biHeight, bp, lpbmi, DIB_RGB_COLORS);
    size = bmi.bmiHeader.biSizeImage;
    bbp = bmi.bmiHeader.biBitCount;
    if (!r) PrintError(r);
}

void CaptureScreen(LPBYTE& buffer, int& width, int& height, WORD& bbp) {
    DWORD size = 0;

    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0,SRCCOPY);
    hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));

    // Buffer is a LPBYTE - which is a pointer itself, so pass it by reference as normal
    GetBitmapBuffer(hScreenDC, hBitmap, buffer, size, bbp);

    ReleaseDC(NULL, hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
}

BOOL SetDpiAwareness() {
    printf("SetProcessDpiAwareness\n");
    PGNSI pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("user32.dll")),
                                         "SetProcessDpiAwarenessContext");
    if (pGNSI) {
        BOOL r = pGNSI(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
        if(r) return TRUE;
    }
    DWORD errorCode = GetLastError();
    PrintError(errorCode);
    return FALSE;
}

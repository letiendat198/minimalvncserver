#ifndef WIN32SCREENCAP_H
#define WIN32SCREENCAP_H

#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE ((DPI_AWARENESS_CONTEXT)-2)

#include <winsock.h> // winsock.h wiil include windows.h underneath
// Don't include winuser.h and stuffs as it will collide with windows.h

typedef BOOL (WINAPI *PGNSI)(DPI_AWARENESS_CONTEXT);

BOOL SetDpiAwareness();
void CaptureScreen(LPBYTE& buffer, int& width, int& height, WORD& bbp);

#endif //WIN32SCREENCAP_H

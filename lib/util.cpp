#include <stdio.h>

#include "util.h"

void PrintError(DWORD errorCode) {
    LPTSTR error = NULL;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL,
                            errorCode,
                            0,
                            (LPTSTR)&error,
                            0,
                            NULL);
    printf("%s", error);
    LocalFree(error);
    error = NULL;
}

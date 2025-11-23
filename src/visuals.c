#include "visuals.h"
#include <stdio.h> // For printf
#ifdef _WIN32
#include <windows.h> // For Win32 console API
#endif

void init_visuals() {
#ifdef _WIN32
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        return;
    }
    printf("%s", COLOR_RESET); // Reset colors to default to start clean
#endif
    // For POSIX systems, ANSI escape codes are usually supported by default.
    // No special initialization needed here.
}

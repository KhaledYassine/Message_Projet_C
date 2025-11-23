#ifndef VISUALS_H
#define VISUALS_H

// ANSI Color Codes
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLACK   "\x1b[30m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

// Background colors (optional, but good to have)
#define BG_BLACK   "\x1b[40m"
#define BG_RED     "\x1b[41m"
#define BG_GREEN   "\x1b[42m"
#define BG_YELLOW  "\x1b[43m"
#define BG_BLUE    "\x1b[44m"
#define BG_MAGENTA "\x1b[45m"
#define BG_CYAN    "\x1b[46m"
#define BG_WHITE   "\x1b[47m"

// Bold text (optional)
#define TEXT_BOLD     "\x1b[1m"
#define TEXT_ITALIC   "\x1b[3m"
#define TEXT_UNDERLINE "\x1b[4m"

// Function to initialize terminal for ANSI escape codes
void init_visuals();

#endif // VISUALS_H

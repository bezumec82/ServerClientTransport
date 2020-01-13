#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <stdio.h>
#include <string.h>


#ifdef LINUX_TERMINAL
    #define NORM     "\x1B[0m"  //normal
    #define RED      "\x1B[31m" //red
    #define GRN      "\x1B[32m" //green
    #define YEL      "\x1B[33m" //yellow
    #define BLU      "\x1B[34m" //blue
    #define MAG      "\x1B[35m" //magenta
    #define CYN      "\x1B[36m" //cyan
    #define WHT      "\x1B[37m" //white
    #define BOLD     "\e[1m"    //bold symbols
    #define ITAL     "\e[3m"    //italic text
    #define BLINK    "\e[5m"    //blink
#else // win, QT creator terminal, Eclipse terminal
    #define NORM     ""
    #define RED      ""
    #define GRN      ""
    #define YEL      ""
    #define BLU      ""
    #define MAG      ""
    #define CYN      ""
    #define WHT      ""
    #define BOLD     ""
    #define ITAL     ""
    #define BLINK    ""
#endif


#define __FILENAME__                                                        \
                    (strrchr(__FILE__, '/') ?                               \
                                            strrchr(__FILE__, '/') + 1 :    \
                                            __FILE__)
#define PRINTF(color, text, ...) \
            printf(color"%-20.20s %-20.20s #%-5i: " text "" NORM,           \
            __FILENAME__, __FUNCTION__, __LINE__ ,                          \
            ##__VA_ARGS__);

/* Additional line */
#define PRINTF_NL(color, text, ...)                                         \
                    printf(color"%49s " text "" NORM,"", ##__VA_ARGS__);

#define PRINT_ERR(text, ...)                                                \
        printf(RED"%-20.20s %-20.20s #%-5i: ERROR: " text "" NORM,          \
               __FILENAME__, __FUNCTION__, __LINE__,                        \
               ##__VA_ARGS__);

#define PRINT_WARN(text, ...)                                               \
        printf(YEL"%-20.20s %-20.20s #%-5i: WARNING: " text "" NORM,        \
               __FILENAME__, __FUNCTION__, __LINE__,                        \
               ##__VA_ARGS__);

#define FPRINTF                                                             \
        fprintf(stderr, RED"%-20.20s %-20.20s #%-5i: ERROR: '%s'.\r\n"      \
                    "%49s errno : %i.\r\n" NORM,                            \
                    __FILENAME__, __FUNCTION__, __LINE__,                   \
                    strerror(errno) , "", errno );

#define FPRINTF_EXT(text, ...)                                              \
        fprintf(stderr, RED"%-20.20s %-20.20s #%-5i: ERROR: '%s'.\r\n"      \
                    "%49s errno : %i.\r\n"                                  \
                    "%49s " text "" NORM,                                   \
                    __FILENAME__, __FUNCTION__, __LINE__, strerror(errno),  \
                    "", errno ,                                             \
                    "", ##__VA_ARGS__ );

#define PRINT_COND(condition, color, text, ... )                            \
        if (condition) { printf(color "" text "" NORM, ##__VA_ARGS__); }


#endif /* DEBUG_HPP */

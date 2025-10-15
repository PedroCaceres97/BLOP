#ifndef __BLOP_COLOURS__
#define __BLOP_COLOURS__

#include <blop/blop_basics.h>

/*
ANSI escape codes colours to work with console.
On linux they are enabled by default.
On windows you should enable virtual terminal.
*/

#ifndef BLOP_COLOUR_PREFIX
    #define BLOP_COLOUR_PREFIX BLOP_
#endif

#define SEQUENCE            "\x1b["
#define SEPARATOR           ";"
#define END_COLOR           "m"

#define HOME                "H"
#define ERASE               "2J"

#define RESET               "0"
#define BOLD                "1"
#define FAINT               "2"
#define ITALIC              "3"
#define UNDERLINE           "4"
#define BLINKING            "5"
#define INVERSE             "7"
#define HIDDEN              "8"
#define STRIKE              "9"

#define RESET_BOLD          "22"
#define RESET_FAINT         "22"
#define RESET_ITALIC        "23"
#define RESET_UNDERLINE     "24"
#define RESET_BLINKING      "25"
#define RESET_INVERSE       "27"
#define RESET_HIDDEN        "28"
#define RESET_STRIKE        "29"

#define ESC_FG_BLACK        "30"
#define ESC_FG_RED          "31"
#define ESC_FG_GREEN        "32"
#define ESC_FG_YELLOW       "33"
#define ESC_FG_BLUE         "34"
#define ESC_FG_MAGENTA      "35"
#define ESC_FG_CYAN         "36"
#define ESC_FG_WHITE        "37"
#define ESC_FG_DEFAULT      "39"
#define ESC_FG_RGB          "38;2;"

#define ESC_BG_BLACK        "40"
#define ESC_BG_RED          "41"
#define ESC_BG_GREEN        "42"
#define ESC_BG_YELLOW       "43"
#define ESC_BG_BLUE         "44"
#define ESC_BG_MAGENTA      "45"
#define ESC_BG_CYAN         "46"
#define ESC_BG_WHITE        "47"
#define ESC_BG_DEFAULT      "49"
#define ESC_BG_RGB          "48;2;"

#define RESET_COLOR                 ESC_SEQUENCE ESC_RESET  ESC_END_COLOR
#define CLEAR_SCREEN                ESC_SEQUENCE ESC_ERASE  ESC_SEQUENCE ESC_HOME

#define __both(fg, bg, text)        ESC_SEQUENCE fg         ESC_SEPARATOR bg      ESC_END_COLOR                     text RESET_COLOR
#define __only(color, text)         ESC_SEQUENCE color      ESC_END_COLOR                                           text RESET_COLOR

#define __both_bold(fg, bg, text)   ESC_SEQUENCE ESC_BOLD   ESC_SEPARATOR fg      ESC_SEPARATOR bg ESC_END_COLOR    text RESET_COLOR
#define __only_bold(color, text)    ESC_SEQUENCE ESC_BOLD   ESC_SEPARATOR color   ESC_END_COLOR                     text RESET_COLOR

#define __set_both(fg, bg)          ESC_SEQUENCE fg         ESC_SEPARATOR bg      ESC_END_COLOR
#define __set_only(color)           ESC_SEQUENCE color      ESC_END_COLOR                      

#define __set_both_bold(fg, bg)     ESC_SEQUENCE ESC_BOLD   ESC_SEPARATOR fg      ESC_SEPARATOR bg ESC_END_COLOR
#define __set_only_bold(color)      ESC_SEQUENCE ESC_BOLD   ESC_SEPARATOR color   ESC_END_COLOR                 

#define BLACK_TEXT(text)                       __only(ESC_FG_BLACK,                        text)         
#define RED_TEXT(text)                         __only(ESC_FG_RED,                          text)           
#define GREEN_TEXT(text)                       __only(ESC_FG_GREEN,                        text)         
#define YELLOW_TEXT(text)                      __only(ESC_FG_YELLOW,                       text)        
#define BLUE_TEXT(text)                        __only(ESC_FG_BLUE,                         text)          
#define MAGENTA_TEXT(text)                     __only(ESC_FG_MAGENTA,                      text)       
#define CYAN_TEXT(text)                        __only(ESC_FG_CYAN,                         text)          
#define WHITE_TEXT(text)                       __only(ESC_FG_WHITE,                        text)         

#define BLACK_GROUND(text)                     __only(ESC_BG_BLACK,                        text)       
#define RED_GROUND(text)                       __only(ESC_BG_RED,                          text)         
#define GREEN_GROUND(text)                     __only(ESC_BG_GREEN,                        text)       
#define YELLOW_GROUND(text)                    __only(ESC_BG_YELLOW,                       text)      
#define BLUE_GROUND(text)                      __only(ESC_BG_BLUE,                         text)        
#define MAGENTA_GROUND(text)                   __only(ESC_BG_MAGENTA,                      text)     
#define CYAN_GROUND(text)                      __only(ESC_BG_CYAN,                         text)        
#define WHITE_GROUND(text)                     __only(ESC_BG_WHITE,                        text)       

#define BOLD_BLACK_TEXT(text)                  __only_bold(ESC_FG_BLACK,                   text)  
#define BOLD_RED_TEXT(text)                    __only_bold(ESC_FG_RED,                     text)    
#define BOLD_GREEN_TEXT(text)                  __only_bold(ESC_FG_GREEN,                   text)  
#define BOLD_YELLOW_TEXT(text)                 __only_bold(ESC_FG_YELLOW,                  text) 
#define BOLD_BLUE_TEXT(text)                   __only_bold(ESC_FG_BLUE,                    text)   
#define BOLD_MAGENTA_TEXT(text)                __only_bold(ESC_FG_MAGENTA,                 text)
#define BOLD_CYAN_TEXT(text)                   __only_bold(ESC_FG_CYAN,                    text)   
#define BOLD_WHITE_TEXT(text)                  __only_bold(ESC_FG_WHITE,                   text)  

#define BLACK_TEXT_BLACK_GROUND(text)          __both(ESC_FG_BLACK,   ESC_BG_BLACK,        text)
#define BLACK_TEXT_RED_GROUND(text)            __both(ESC_FG_BLACK,   ESC_BG_RED,          text)
#define BLACK_TEXT_GREEN_GROUND(text)          __both(ESC_FG_BLACK,   ESC_BG_GREEN,        text)
#define BLACK_TEXT_YELLOW_GROUND(text)         __both(ESC_FG_BLACK,   ESC_BG_YELLOW,       text)
#define BLACK_TEXT_BLUE_GROUND(text)           __both(ESC_FG_BLACK,   ESC_BG_BLUE,         text)
#define BLACK_TEXT_MAGENTA_GROUND(text)        __both(ESC_FG_BLACK,   ESC_BG_MAGENTA,      text)
#define BLACK_TEXT_CYAN_GROUND(text)           __both(ESC_FG_BLACK,   ESC_BG_CYAN,         text)
#define BLACK_TEXT_WHITE_GROUND(text)          __both(ESC_FG_BLACK,   ESC_BG_WHITE,        text)

#define RED_TEXT_BLACK_GROUND(text)            __both(ESC_FG_RED,     ESC_BG_BLACK,        text)
#define RED_TEXT_RED_GROUND(text)              __both(ESC_FG_RED,     ESC_BG_RED,          text)
#define RED_TEXT_GREEN_GROUND(text)            __both(ESC_FG_RED,     ESC_BG_GREEN,        text)
#define RED_TEXT_YELLOW_GROUND(text)           __both(ESC_FG_RED,     ESC_BG_YELLOW,       text)
#define RED_TEXT_BLUE_GROUND(text)             __both(ESC_FG_RED,     ESC_BG_BLUE,         text)
#define RED_TEXT_MAGENTA_GROUND(text)          __both(ESC_FG_RED,     ESC_BG_MAGENTA,      text)
#define RED_TEXT_CYAN_GROUND(text)             __both(ESC_FG_RED,     ESC_BG_CYAN,         text)
#define RED_TEXT_WHITE_GROUND(text)            __both(ESC_FG_RED,     ESC_BG_WHITE,        text)

#define GREEN_TEXT_BLACK_GROUND(text)          __both(ESC_FG_GREEN,   ESC_BG_BLACK,        text)      
#define GREEN_TEXT_RED_GROUND(text)            __both(ESC_FG_GREEN,   ESC_BG_RED,          text)      
#define GREEN_TEXT_GREEN_GROUND(text)          __both(ESC_FG_GREEN,   ESC_BG_GREEN,        text)      
#define GREEN_TEXT_YELLOW_GROUND(text)         __both(ESC_FG_GREEN,   ESC_BG_YELLOW,       text)      
#define GREEN_TEXT_BLUE_GROUND(text)           __both(ESC_FG_GREEN,   ESC_BG_BLUE,         text)      
#define GREEN_TEXT_MAGENTA_GROUND(text)        __both(ESC_FG_GREEN,   ESC_BG_MAGENTA,      text)     
#define GREEN_TEXT_CYAN_GROUND(text)           __both(ESC_FG_GREEN,   ESC_BG_CYAN,         text)      
#define GREEN_TEXT_WHITE_GROUND(text)          __both(ESC_FG_GREEN,   ESC_BG_WHITE,        text)      

#define YELLOW_TEXT_BLACK_GROUND(text)         __both(ESC_FG_YELLOW,  ESC_BG_BLACK,        text) 
#define YELLOW_TEXT_RED_GROUND(text)           __both(ESC_FG_YELLOW,  ESC_BG_RED,          text) 
#define YELLOW_TEXT_GREEN_GROUND(text)         __both(ESC_FG_YELLOW,  ESC_BG_GREEN,        text) 
#define YELLOW_TEXT_YELLOW_GROUND(text)        __both(ESC_FG_YELLOW,  ESC_BG_YELLOW,       text)
#define YELLOW_TEXT_BLUE_GROUND(text)          __both(ESC_FG_YELLOW,  ESC_BG_BLUE,         text) 
#define YELLOW_TEXT_MAGENTA_GROUND(text)       __both(ESC_FG_YELLOW,  ESC_BG_MAGENTA,      text)
#define YELLOW_TEXT_CYAN_GROUND(text)          __both(ESC_FG_YELLOW,  ESC_BG_CYAN,         text) 
#define YELLOW_TEXT_WHITE_GROUND(text)         __both(ESC_FG_YELLOW,  ESC_BG_WHITE,        text) 

#define BLUE_TEXT_BLACK_GROUND(text)           __both(ESC_FG_BLUE,    ESC_BG_BLACK,        text)      
#define BLUE_TEXT_RED_GROUND(text)             __both(ESC_FG_BLUE,    ESC_BG_RED,          text)      
#define BLUE_TEXT_GREEN_GROUND(text)           __both(ESC_FG_BLUE,    ESC_BG_GREEN,        text)      
#define BLUE_TEXT_YELLOW_GROUND(text)          __both(ESC_FG_BLUE,    ESC_BG_YELLOW,       text)      
#define BLUE_TEXT_BLUE_GROUND(text)            __both(ESC_FG_BLUE,    ESC_BG_BLUE,         text)      
#define BLUE_TEXT_MAGENTA_GROUND(text)         __both(ESC_FG_BLUE,    ESC_BG_MAGENTA,      text)      
#define BLUE_TEXT_CYAN_GROUND(text)            __both(ESC_FG_BLUE,    ESC_BG_CYAN,         text)      
#define BLUE_TEXT_WHITE_GROUND(text)           __both(ESC_FG_BLUE,    ESC_BG_WHITE,        text)      

#define MAGENTA_TEXT_BLACK_GROUND(text)        __both(ESC_FG_MAGENTA, ESC_BG_BLACK,        text)     
#define MAGENTA_TEXT_RED_GROUND(text)          __both(ESC_FG_MAGENTA, ESC_BG_RED,          text)      
#define MAGENTA_TEXT_GREEN_GROUND(text)        __both(ESC_FG_MAGENTA, ESC_BG_GREEN,        text)     
#define MAGENTA_TEXT_YELLOW_GROUND(text)       __both(ESC_FG_MAGENTA, ESC_BG_YELLOW,       text)     
#define MAGENTA_TEXT_BLUE_GROUND(text)         __both(ESC_FG_MAGENTA, ESC_BG_BLUE,         text)      
#define MAGENTA_TEXT_MAGENTA_GROUND(text)      __both(ESC_FG_MAGENTA, ESC_BG_MAGENTA,      text)     
#define MAGENTA_TEXT_CYAN_GROUND(text)         __both(ESC_FG_MAGENTA, ESC_BG_CYAN,         text)      
#define MAGENTA_TEXT_WHITE_GROUND(text)        __both(ESC_FG_MAGENTA, ESC_BG_WHITE,        text)     

#define CYAN_TEXT_BLACK_GROUND(text)           __both(ESC_FG_CYAN,    ESC_BG_BLACK,        text)      
#define CYAN_TEXT_RED_GROUND(text)             __both(ESC_FG_CYAN,    ESC_BG_RED,          text)      
#define CYAN_TEXT_GREEN_GROUND(text)           __both(ESC_FG_CYAN,    ESC_BG_GREEN,        text)      
#define CYAN_TEXT_YELLOW_GROUND(text)          __both(ESC_FG_CYAN,    ESC_BG_YELLOW,       text)      
#define CYAN_TEXT_BLUE_GROUND(text)            __both(ESC_FG_CYAN,    ESC_BG_BLUE,         text)      
#define CYAN_TEXT_MAGENTA_GROUND(text)         __both(ESC_FG_CYAN,    ESC_BG_MAGENTA,      text)      
#define CYAN_TEXT_CYAN_GROUND(text)            __both(ESC_FG_CYAN,    ESC_BG_CYAN,         text)      
#define CYAN_TEXT_WHITE_GROUND(text)           __both(ESC_FG_CYAN,    ESC_BG_WHITE,        text)      

#define WHITE_TEXT_BLACK_GROUND(text)          __both(ESC_FG_WHITE,   ESC_BG_BLACK,        text)      
#define WHITE_TEXT_RED_GROUND(text)            __both(ESC_FG_WHITE,   ESC_BG_RED,          text)      
#define WHITE_TEXT_GREEN_GROUND(text)          __both(ESC_FG_WHITE,   ESC_BG_GREEN,        text)      
#define WHITE_TEXT_YELLOW_GROUND(text)         __both(ESC_FG_WHITE,   ESC_BG_YELLOW,       text)      
#define WHITE_TEXT_BLUE_GROUND(text)           __both(ESC_FG_WHITE,   ESC_BG_BLUE,         text)      
#define WHITE_TEXT_MAGENTA_GROUND(text)        __both(ESC_FG_WHITE,   ESC_BG_MAGENTA,      text)     
#define WHITE_TEXT_CYAN_GROUND(text)           __both(ESC_FG_WHITE,   ESC_BG_CYAN,         text)      
#define WHITE_TEXT_WHITE_GROUND(text)          __both(ESC_FG_WHITE,   ESC_BG_WHITE,        text)      

#define BOLD_BLACK_TEXT_BLACK_GROUND(text)     __both_bold(ESC_FG_BLACK,   ESC_BG_BLACK,   text)    
#define BOLD_BLACK_TEXT_RED_GROUND(text)       __both_bold(ESC_FG_BLACK,   ESC_BG_RED,     text)    
#define BOLD_BLACK_TEXT_GREEN_GROUND(text)     __both_bold(ESC_FG_BLACK,   ESC_BG_GREEN,   text)    
#define BOLD_BLACK_TEXT_YELLOW_GROUND(text)    __both_bold(ESC_FG_BLACK,   ESC_BG_YELLOW,  text)    
#define BOLD_BLACK_TEXT_BLUE_GROUND(text)      __both_bold(ESC_FG_BLACK,   ESC_BG_BLUE,    text)    
#define BOLD_BLACK_TEXT_MAGENTA_GROUND(text)   __both_bold(ESC_FG_BLACK,   ESC_BG_MAGENTA, text)    
#define BOLD_BLACK_TEXT_CYAN_GROUND(text)      __both_bold(ESC_FG_BLACK,   ESC_BG_CYAN,    text)    
#define BOLD_BLACK_TEXT_WHITE_GROUND(text)     __both_bold(ESC_FG_BLACK,   ESC_BG_WHITE,   text)    

#define BOLD_RED_TEXT_BLACK_GROUND(text)       __both_bold(ESC_FG_RED,     ESC_BG_BLACK,   text)
#define BOLD_RED_TEXT_RED_GROUND(text)         __both_bold(ESC_FG_RED,     ESC_BG_RED,     text) 
#define BOLD_RED_TEXT_GREEN_GROUND(text)       __both_bold(ESC_FG_RED,     ESC_BG_GREEN,   text) 
#define BOLD_RED_TEXT_YELLOW_GROUND(text)      __both_bold(ESC_FG_RED,     ESC_BG_YELLOW,  text) 
#define BOLD_RED_TEXT_BLUE_GROUND(text)        __both_bold(ESC_FG_RED,     ESC_BG_BLUE,    text) 
#define BOLD_RED_TEXT_MAGENTA_GROUND(text)     __both_bold(ESC_FG_RED,     ESC_BG_MAGENTA, text) 
#define BOLD_RED_TEXT_CYAN_GROUND(text)        __both_bold(ESC_FG_RED,     ESC_BG_CYAN,    text) 
#define BOLD_RED_TEXT_WHITE_GROUND(text)       __both_bold(ESC_FG_RED,     ESC_BG_WHITE,   text) 

#define BOLD_GREEN_TEXT_BLACK_GROUND(text)     __both_bold(ESC_FG_GREEN,   ESC_BG_BLACK,   text) 
#define BOLD_GREEN_TEXT_RED_GROUND(text)       __both_bold(ESC_FG_GREEN,   ESC_BG_RED,     text) 
#define BOLD_GREEN_TEXT_GREEN_GROUND(text)     __both_bold(ESC_FG_GREEN,   ESC_BG_GREEN,   text) 
#define BOLD_GREEN_TEXT_YELLOW_GROUND(text)    __both_bold(ESC_FG_GREEN,   ESC_BG_YELLOW,  text) 
#define BOLD_GREEN_TEXT_BLUE_GROUND(text)      __both_bold(ESC_FG_GREEN,   ESC_BG_BLUE,    text) 
#define BOLD_GREEN_TEXT_MAGENTA_GROUND(text)   __both_bold(ESC_FG_GREEN,   ESC_BG_MAGENTA, text) 
#define BOLD_GREEN_TEXT_CYAN_GROUND(text)      __both_bold(ESC_FG_GREEN,   ESC_BG_CYAN,    text) 
#define BOLD_GREEN_TEXT_WHITE_GROUND(text)     __both_bold(ESC_FG_GREEN,   ESC_BG_WHITE,   text) 

#define BOLD_YELLOW_TEXT_BLACK_GROUND(text)    __both_bold(ESC_FG_YELLOW,  ESC_BG_BLACK,   text) 
#define BOLD_YELLOW_TEXT_RED_GROUND(text)      __both_bold(ESC_FG_YELLOW,  ESC_BG_RED,     text) 
#define BOLD_YELLOW_TEXT_GREEN_GROUND(text)    __both_bold(ESC_FG_YELLOW,  ESC_BG_GREEN,   text) 
#define BOLD_YELLOW_TEXT_YELLOW_GROUND(text)   __both_bold(ESC_FG_YELLOW,  ESC_BG_YELLOW,  text) 
#define BOLD_YELLOW_TEXT_BLUE_GROUND(text)     __both_bold(ESC_FG_YELLOW,  ESC_BG_BLUE,    text) 
#define BOLD_YELLOW_TEXT_MAGENTA_GROUND(text)  __both_bold(ESC_FG_YELLOW,  ESC_BG_MAGENTA, text) 
#define BOLD_YELLOW_TEXT_CYAN_GROUND(text)     __both_bold(ESC_FG_YELLOW,  ESC_BG_CYAN,    text) 
#define BOLD_YELLOW_TEXT_WHITE_GROUND(text)    __both_bold(ESC_FG_YELLOW,  ESC_BG_WHITE,   text) 

#define BOLD_BLUE_TEXT_BLACK_GROUND(text)      __both_bold(ESC_FG_BLUE,    ESC_BG_BLACK,   text) 
#define BOLD_BLUE_TEXT_RED_GROUND(text)        __both_bold(ESC_FG_BLUE,    ESC_BG_RED,     text) 
#define BOLD_BLUE_TEXT_GREEN_GROUND(text)      __both_bold(ESC_FG_BLUE,    ESC_BG_GREEN,   text) 
#define BOLD_BLUE_TEXT_YELLOW_GROUND(text)     __both_bold(ESC_FG_BLUE,    ESC_BG_YELLOW,  text) 
#define BOLD_BLUE_TEXT_BLUE_GROUND(text)       __both_bold(ESC_FG_BLUE,    ESC_BG_BLUE,    text) 
#define BOLD_BLUE_TEXT_MAGENTA_GROUND(text)    __both_bold(ESC_FG_BLUE,    ESC_BG_MAGENTA, text) 
#define BOLD_BLUE_TEXT_CYAN_GROUND(text)       __both_bold(ESC_FG_BLUE,    ESC_BG_CYAN,    text) 
#define BOLD_BLUE_TEXT_WHITE_GROUND(text)      __both_bold(ESC_FG_BLUE,    ESC_BG_WHITE,   text) 

#define BOLD_MAGENTA_TEXT_BLACK_GROUND(text)   __both_bold(ESC_FG_MAGENTA, ESC_BG_BLACK,   text) 
#define BOLD_MAGENTA_TEXT_RED_GROUND(text)     __both_bold(ESC_FG_MAGENTA, ESC_BG_RED,     text) 
#define BOLD_MAGENTA_TEXT_GREEN_GROUND(text)   __both_bold(ESC_FG_MAGENTA, ESC_BG_GREEN,   text) 
#define BOLD_MAGENTA_TEXT_YELLOW_GROUND(text)  __both_bold(ESC_FG_MAGENTA, ESC_BG_YELLOW,  text) 
#define BOLD_MAGENTA_TEXT_BLUE_GROUND(text)    __both_bold(ESC_FG_MAGENTA, ESC_BG_BLUE,    text) 
#define BOLD_MAGENTA_TEXT_MAGENTA_GROUND(text) __both_bold(ESC_FG_MAGENTA, ESC_BG_MAGENTA, text) 
#define BOLD_MAGENTA_TEXT_CYAN_GROUND(text)    __both_bold(ESC_FG_MAGENTA, ESC_BG_CYAN,    text) 
#define BOLD_MAGENTA_TEXT_WHITE_GROUND(text)   __both_bold(ESC_FG_MAGENTA, ESC_BG_WHITE,   text) 

#define BOLD_CYAN_TEXT_BLACK_GROUND(text)      __both_bold(ESC_FG_CYAN,    ESC_BG_BLACK,   text) 
#define BOLD_CYAN_TEXT_RED_GROUND(text)        __both_bold(ESC_FG_CYAN,    ESC_BG_RED,     text) 
#define BOLD_CYAN_TEXT_GREEN_GROUND(text)      __both_bold(ESC_FG_CYAN,    ESC_BG_GREEN,   text) 
#define BOLD_CYAN_TEXT_YELLOW_GROUND(text)     __both_bold(ESC_FG_CYAN,    ESC_BG_YELLOW,  text) 
#define BOLD_CYAN_TEXT_BLUE_GROUND(text)       __both_bold(ESC_FG_CYAN,    ESC_BG_BLUE,    text) 
#define BOLD_CYAN_TEXT_MAGENTA_GROUND(text)    __both_bold(ESC_FG_CYAN,    ESC_BG_MAGENTA, text) 
#define BOLD_CYAN_TEXT_CYAN_GROUND(text)       __both_bold(ESC_FG_CYAN,    ESC_BG_CYAN,    text) 
#define BOLD_CYAN_TEXT_WHITE_GROUND(text)      __both_bold(ESC_FG_CYAN,    ESC_BG_WHITE,   text) 

#define BOLD_WHITE_TEXT_BLACK_GROUND(text)     __both_bold(ESC_FG_WHITE,   ESC_BG_BLACK,   text) 
#define BOLD_WHITE_TEXT_RED_GROUND(text)       __both_bold(ESC_FG_WHITE,   ESC_BG_RED,     text) 
#define BOLD_WHITE_TEXT_GREEN_GROUND(text)     __both_bold(ESC_FG_WHITE,   ESC_BG_GREEN,   text) 
#define BOLD_WHITE_TEXT_YELLOW_GROUND(text)    __both_bold(ESC_FG_WHITE,   ESC_BG_YELLOW,  text) 
#define BOLD_WHITE_TEXT_BLUE_GROUND(text)      __both_bold(ESC_FG_WHITE,   ESC_BG_BLUE,    text) 
#define BOLD_WHITE_TEXT_MAGENTA_GROUND(text)   __both_bold(ESC_FG_WHITE,   ESC_BG_MAGENTA, text) 
#define BOLD_WHITE_TEXT_CYAN_GROUND(text)      __both_bold(ESC_FG_WHITE,   ESC_BG_CYAN,    text) 
#define BOLD_WHITE_TEXT_WHITE_GROUND(text)     __both_bold(ESC_FG_WHITE,   ESC_BG_WHITE,   text) 

#define SET_BLACK_TEXT                       __set_only(ESC_FG_BLACK)         
#define SET_RED_TEXT                         __set_only(ESC_FG_RED)           
#define SET_GREEN_TEXT                       __set_only(ESC_FG_GREEN)         
#define SET_YELLOW_TEXT                      __set_only(ESC_FG_YELLOW)        
#define SET_BLUE_TEXT                        __set_only(ESC_FG_BLUE)          
#define SET_MAGENTA_TEXT                     __set_only(ESC_FG_MAGENTA)       
#define SET_CYAN_TEXT                        __set_only(ESC_FG_CYAN)          
#define SET_WHITE_TEXT                       __set_only(ESC_FG_WHITE)         

#define SET_BLACK_GROUND                     __set_only(ESC_BG_BLACK)       
#define SET_RED_GROUND                       __set_only(ESC_BG_RED)         
#define SET_GREEN_GROUND                     __set_only(ESC_BG_GREEN)       
#define SET_YELLOW_GROUND                    __set_only(ESC_BG_YELLOW)      
#define SET_BLUE_GROUND                      __set_only(ESC_BG_BLUE)        
#define SET_MAGENTA_GROUND                   __set_only(ESC_BG_MAGENTA)     
#define SET_CYAN_GROUND                      __set_only(ESC_BG_CYAN)        
#define SET_WHITE_GROUND                     __set_only(ESC_BG_WHITE)       

#define SET_BOLD_BLACK_TEXT                  __set_only_bold(ESC_FG_BLACK)  
#define SET_BOLD_RED_TEXT                    __set_only_bold(ESC_FG_RED)    
#define SET_BOLD_GREEN_TEXT                  __set_only_bold(ESC_FG_GREEN)  
#define SET_BOLD_YELLOW_TEXT                 __set_only_bold(ESC_FG_YELLOW) 
#define SET_BOLD_BLUE_TEXT                   __set_only_bold(ESC_FG_BLUE)   
#define SET_BOLD_MAGENTA_TEXT                __set_only_bold(ESC_FG_MAGENTA)
#define SET_BOLD_CYAN_TEXT                   __set_only_bold(ESC_FG_CYAN)   
#define SET_BOLD_WHITE_TEXT                  __set_only_bold(ESC_FG_WHITE)  

#define SET_BLACK_TEXT_BLACK_GROUND          __set_both(ESC_FG_BLACK,   ESC_BG_BLACK)
#define SET_BLACK_TEXT_RED_GROUND            __set_both(ESC_FG_BLACK,   ESC_BG_RED)
#define SET_BLACK_TEXT_GREEN_GROUND          __set_both(ESC_FG_BLACK,   ESC_BG_GREEN)
#define SET_BLACK_TEXT_YELLOW_GROUND         __set_both(ESC_FG_BLACK,   ESC_BG_YELLOW)
#define SET_BLACK_TEXT_BLUE_GROUND           __set_both(ESC_FG_BLACK,   ESC_BG_BLUE)
#define SET_BLACK_TEXT_MAGENTA_GROUND        __set_both(ESC_FG_BLACK,   ESC_BG_MAGENTA)
#define SET_BLACK_TEXT_CYAN_GROUND           __set_both(ESC_FG_BLACK,   ESC_BG_CYAN)
#define SET_BLACK_TEXT_WHITE_GROUND          __set_both(ESC_FG_BLACK,   ESC_BG_WHITE)

#define SET_RED_TEXT_BLACK_GROUND            __set_both(ESC_FG_RED,     ESC_BG_BLACK)
#define SET_RED_TEXT_RED_GROUND              __set_both(ESC_FG_RED,     ESC_BG_RED)
#define SET_RED_TEXT_GREEN_GROUND            __set_both(ESC_FG_RED,     ESC_BG_GREEN)
#define SET_RED_TEXT_YELLOW_GROUND           __set_both(ESC_FG_RED,     ESC_BG_YELLOW)
#define SET_RED_TEXT_BLUE_GROUND             __set_both(ESC_FG_RED,     ESC_BG_BLUE)
#define SET_RED_TEXT_MAGENTA_GROUND          __set_both(ESC_FG_RED,     ESC_BG_MAGENTA)
#define SET_RED_TEXT_CYAN_GROUND             __set_both(ESC_FG_RED,     ESC_BG_CYAN)
#define SET_RED_TEXT_WHITE_GROUND            __set_both(ESC_FG_RED,     ESC_BG_WHITE)

#define SET_GREEN_TEXT_BLACK_GROUND          __set_both(ESC_FG_GREEN,   ESC_BG_BLACK)      
#define SET_GREEN_TEXT_RED_GROUND            __set_both(ESC_FG_GREEN,   ESC_BG_RED)      
#define SET_GREEN_TEXT_GREEN_GROUND          __set_both(ESC_FG_GREEN,   ESC_BG_GREEN)      
#define SET_GREEN_TEXT_YELLOW_GROUND         __set_both(ESC_FG_GREEN,   ESC_BG_YELLOW)      
#define SET_GREEN_TEXT_BLUE_GROUND           __set_both(ESC_FG_GREEN,   ESC_BG_BLUE)      
#define SET_GREEN_TEXT_MAGENTA_GROUND        __set_both(ESC_FG_GREEN,   ESC_BG_MAGENTA)     
#define SET_GREEN_TEXT_CYAN_GROUND           __set_both(ESC_FG_GREEN,   ESC_BG_CYAN)      
#define SET_GREEN_TEXT_WHITE_GROUND          __set_both(ESC_FG_GREEN,   ESC_BG_WHITE)      

#define SET_YELLOW_TEXT_BLACK_GROUND         __set_both(ESC_FG_YELLOW,  ESC_BG_BLACK) 
#define SET_YELLOW_TEXT_RED_GROUND           __set_both(ESC_FG_YELLOW,  ESC_BG_RED) 
#define SET_YELLOW_TEXT_GREEN_GROUND         __set_both(ESC_FG_YELLOW,  ESC_BG_GREEN) 
#define SET_YELLOW_TEXT_YELLOW_GROUND        __set_both(ESC_FG_YELLOW,  ESC_BG_YELLOW)
#define SET_YELLOW_TEXT_BLUE_GROUND          __set_both(ESC_FG_YELLOW,  ESC_BG_BLUE) 
#define SET_YELLOW_TEXT_MAGENTA_GROUND       __set_both(ESC_FG_YELLOW,  ESC_BG_MAGENTA)
#define SET_YELLOW_TEXT_CYAN_GROUND          __set_both(ESC_FG_YELLOW,  ESC_BG_CYAN) 
#define SET_YELLOW_TEXT_WHITE_GROUND         __set_both(ESC_FG_YELLOW,  ESC_BG_WHITE) 

#define SET_BLUE_TEXT_BLACK_GROUND           __set_both(ESC_FG_BLUE,    ESC_BG_BLACK)      
#define SET_BLUE_TEXT_RED_GROUND             __set_both(ESC_FG_BLUE,    ESC_BG_RED)      
#define SET_BLUE_TEXT_GREEN_GROUND           __set_both(ESC_FG_BLUE,    ESC_BG_GREEN)      
#define SET_BLUE_TEXT_YELLOW_GROUND          __set_both(ESC_FG_BLUE,    ESC_BG_YELLOW)      
#define SET_BLUE_TEXT_BLUE_GROUND            __set_both(ESC_FG_BLUE,    ESC_BG_BLUE)      
#define SET_BLUE_TEXT_MAGENTA_GROUND         __set_both(ESC_FG_BLUE,    ESC_BG_MAGENTA)      
#define SET_BLUE_TEXT_CYAN_GROUND            __set_both(ESC_FG_BLUE,    ESC_BG_CYAN)      
#define SET_BLUE_TEXT_WHITE_GROUND           __set_both(ESC_FG_BLUE,    ESC_BG_WHITE)      

#define SET_MAGENTA_TEXT_BLACK_GROUND        __set_both(ESC_FG_MAGENTA, ESC_BG_BLACK)     
#define SET_MAGENTA_TEXT_RED_GROUND                __set_both(ESC_FG_MAGENTA, ESC_BG_RED)      
#define SET_MAGENTA_TEXT_GREEN_GROUND        __set_both(ESC_FG_MAGENTA, ESC_BG_GREEN)     
#define SET_MAGENTA_TEXT_YELLOW_GROUND       __set_both(ESC_FG_MAGENTA, ESC_BG_YELLOW)     
#define SET_MAGENTA_TEXT_BLUE_GROUND               __set_both(ESC_FG_MAGENTA, ESC_BG_BLUE)      
#define SET_MAGENTA_TEXT_MAGENTA_GROUND      __set_both(ESC_FG_MAGENTA, ESC_BG_MAGENTA)     
#define SET_MAGENTA_TEXT_CYAN_GROUND               __set_both(ESC_FG_MAGENTA, ESC_BG_CYAN)      
#define SET_MAGENTA_TEXT_WHITE_GROUND        __set_both(ESC_FG_MAGENTA, ESC_BG_WHITE)     

#define SET_CYAN_TEXT_BLACK_GROUND           __set_both(ESC_FG_CYAN,    ESC_BG_BLACK)      
#define SET_CYAN_TEXT_RED_GROUND             __set_both(ESC_FG_CYAN,    ESC_BG_RED)      
#define SET_CYAN_TEXT_GREEN_GROUND           __set_both(ESC_FG_CYAN,    ESC_BG_GREEN)      
#define SET_CYAN_TEXT_YELLOW_GROUND          __set_both(ESC_FG_CYAN,    ESC_BG_YELLOW)      
#define SET_CYAN_TEXT_BLUE_GROUND            __set_both(ESC_FG_CYAN,    ESC_BG_BLUE)      
#define SET_CYAN_TEXT_MAGENTA_GROUND         __set_both(ESC_FG_CYAN,    ESC_BG_MAGENTA)      
#define SET_CYAN_TEXT_CYAN_GROUND            __set_both(ESC_FG_CYAN,    ESC_BG_CYAN)      
#define SET_CYAN_TEXT_WHITE_GROUND           __set_both(ESC_FG_CYAN,    ESC_BG_WHITE)      

#define SET_WHITE_TEXT_BLACK_GROUND          __set_both(ESC_FG_WHITE,   ESC_BG_BLACK)      
#define SET_WHITE_TEXT_RED_GROUND            __set_both(ESC_FG_WHITE,   ESC_BG_RED)      
#define SET_WHITE_TEXT_GREEN_GROUND          __set_both(ESC_FG_WHITE,   ESC_BG_GREEN)      
#define SET_WHITE_TEXT_YELLOW_GROUND         __set_both(ESC_FG_WHITE,   ESC_BG_YELLOW)      
#define SET_WHITE_TEXT_BLUE_GROUND           __set_both(ESC_FG_WHITE,   ESC_BG_BLUE)      
#define SET_WHITE_TEXT_MAGENTA_GROUND        __set_both(ESC_FG_WHITE,   ESC_BG_MAGENTA)     
#define SET_WHITE_TEXT_CYAN_GROUND           __set_both(ESC_FG_WHITE,   ESC_BG_CYAN)      
#define SET_WHITE_TEXT_WHITE_GROUND          __set_both(ESC_FG_WHITE,   ESC_BG_WHITE)      

#define SET_BOLD_BLACK_TEXT_BLACK_GROUND     __set_both_bold(ESC_FG_BLACK,   ESC_BG_BLACK)    
#define SET_BOLD_BLACK_TEXT_RED_GROUND       __set_both_bold(ESC_FG_BLACK,   ESC_BG_RED)    
#define SET_BOLD_BLACK_TEXT_GREEN_GROUND     __set_both_bold(ESC_FG_BLACK,   ESC_BG_GREEN)    
#define SET_BOLD_BLACK_TEXT_YELLOW_GROUND    __set_both_bold(ESC_FG_BLACK,   ESC_BG_YELLOW)    
#define SET_BOLD_BLACK_TEXT_BLUE_GROUND      __set_both_bold(ESC_FG_BLACK,   ESC_BG_BLUE)    
#define SET_BOLD_BLACK_TEXT_MAGENTA_GROUND   __set_both_bold(ESC_FG_BLACK,   ESC_BG_MAGENTA)    
#define SET_BOLD_BLACK_TEXT_CYAN_GROUND      __set_both_bold(ESC_FG_BLACK,   ESC_BG_CYAN)    
#define SET_BOLD_BLACK_TEXT_WHITE_GROUND     __set_both_bold(ESC_FG_BLACK,   ESC_BG_WHITE)    

#define SET_BOLD_RED_TEXT_BLACK_GROUND       __set_both_bold(ESC_FG_RED,     ESC_BG_BLACK)
#define SET_BOLD_RED_TEXT_RED_GROUND         __set_both_bold(ESC_FG_RED,     ESC_BG_RED) 
#define SET_BOLD_RED_TEXT_GREEN_GROUND       __set_both_bold(ESC_FG_RED,     ESC_BG_GREEN) 
#define SET_BOLD_RED_TEXT_YELLOW_GROUND      __set_both_bold(ESC_FG_RED,     ESC_BG_YELLOW) 
#define SET_BOLD_RED_TEXT_BLUE_GROUND        __set_both_bold(ESC_FG_RED,     ESC_BG_BLUE) 
#define SET_BOLD_RED_TEXT_MAGENTA_GROUND     __set_both_bold(ESC_FG_RED,     ESC_BG_MAGENTA) 
#define SET_BOLD_RED_TEXT_CYAN_GROUND        __set_both_bold(ESC_FG_RED,     ESC_BG_CYAN) 
#define SET_BOLD_RED_TEXT_WHITE_GROUND       __set_both_bold(ESC_FG_RED,     ESC_BG_WHITE) 

#define SET_BOLD_GREEN_TEXT_BLACK_GROUND     __set_both_bold(ESC_FG_GREEN,   ESC_BG_BLACK) 
#define SET_BOLD_GREEN_TEXT_RED_GROUND       __set_both_bold(ESC_FG_GREEN,   ESC_BG_RED) 
#define SET_BOLD_GREEN_TEXT_GREEN_GROUND     __set_both_bold(ESC_FG_GREEN,   ESC_BG_GREEN) 
#define SET_BOLD_GREEN_TEXT_YELLOW_GROUND    __set_both_bold(ESC_FG_GREEN,   ESC_BG_YELLOW) 
#define SET_BOLD_GREEN_TEXT_BLUE_GROUND      __set_both_bold(ESC_FG_GREEN,   ESC_BG_BLUE) 
#define SET_BOLD_GREEN_TEXT_MAGENTA_GROUND   __set_both_bold(ESC_FG_GREEN,   ESC_BG_MAGENTA) 
#define SET_BOLD_GREEN_TEXT_CYAN_GROUND      __set_both_bold(ESC_FG_GREEN,   ESC_BG_CYAN) 
#define SET_BOLD_GREEN_TEXT_WHITE_GROUND     __set_both_bold(ESC_FG_GREEN,   ESC_BG_WHITE) 

#define SET_BOLD_YELLOW_TEXT_BLACK_GROUND    __set_both_bold(ESC_FG_YELLOW,  ESC_BG_BLACK) 
#define SET_BOLD_YELLOW_TEXT_RED_GROUND      __set_both_bold(ESC_FG_YELLOW,  ESC_BG_RED) 
#define SET_BOLD_YELLOW_TEXT_GREEN_GROUND    __set_both_bold(ESC_FG_YELLOW,  ESC_BG_GREEN) 
#define SET_BOLD_YELLOW_TEXT_YELLOW_GROUND   __set_both_bold(ESC_FG_YELLOW,  ESC_BG_YELLOW) 
#define SET_BOLD_YELLOW_TEXT_BLUE_GROUND     __set_both_bold(ESC_FG_YELLOW,  ESC_BG_BLUE) 
#define SET_BOLD_YELLOW_TEXT_MAGENTA_GROUND  __set_both_bold(ESC_FG_YELLOW,  ESC_BG_MAGENTA) 
#define SET_BOLD_YELLOW_TEXT_CYAN_GROUND     __set_both_bold(ESC_FG_YELLOW,  ESC_BG_CYAN) 
#define SET_BOLD_YELLOW_TEXT_WHITE_GROUND    __set_both_bold(ESC_FG_YELLOW,  ESC_BG_WHITE) 

#define SET_BOLD_BLUE_TEXT_BLACK_GROUND      __set_both_bold(ESC_FG_BLUE,    ESC_BG_BLACK) 
#define SET_BOLD_BLUE_TEXT_RED_GROUND        __set_both_bold(ESC_FG_BLUE,    ESC_BG_RED) 
#define SET_BOLD_BLUE_TEXT_GREEN_GROUND      __set_both_bold(ESC_FG_BLUE,    ESC_BG_GREEN) 
#define SET_BOLD_BLUE_TEXT_YELLOW_GROUND     __set_both_bold(ESC_FG_BLUE,    ESC_BG_YELLOW) 
#define SET_BOLD_BLUE_TEXT_BLUE_GROUND       __set_both_bold(ESC_FG_BLUE,    ESC_BG_BLUE) 
#define SET_BOLD_BLUE_TEXT_MAGENTA_GROUND    __set_both_bold(ESC_FG_BLUE,    ESC_BG_MAGENTA) 
#define SET_BOLD_BLUE_TEXT_CYAN_GROUND       __set_both_bold(ESC_FG_BLUE,    ESC_BG_CYAN) 
#define SET_BOLD_BLUE_TEXT_WHITE_GROUND      __set_both_bold(ESC_FG_BLUE,    ESC_BG_WHITE) 

#define SET_BOLD_MAGENTA_TEXT_BLACK_GROUND   __set_both_bold(ESC_FG_MAGENTA, ESC_BG_BLACK) 
#define SET_BOLD_MAGENTA_TEXT_RED_GROUND     __set_both_bold(ESC_FG_MAGENTA, ESC_BG_RED) 
#define SET_BOLD_MAGENTA_TEXT_GREEN_GROUND   __set_both_bold(ESC_FG_MAGENTA, ESC_BG_GREEN) 
#define SET_BOLD_MAGENTA_TEXT_YELLOW_GROUND  __set_both_bold(ESC_FG_MAGENTA, ESC_BG_YELLOW) 
#define SET_BOLD_MAGENTA_TEXT_BLUE_GROUND    __set_both_bold(ESC_FG_MAGENTA, ESC_BG_BLUE) 
#define SET_BOLD_MAGENTA_TEXT_MAGENTA_GROUND __set_both_bold(ESC_FG_MAGENTA, ESC_BG_MAGENTA) 
#define SET_BOLD_MAGENTA_TEXT_CYAN_GROUND    __set_both_bold(ESC_FG_MAGENTA, ESC_BG_CYAN) 
#define SET_BOLD_MAGENTA_TEXT_WHITE_GROUND   __set_both_bold(ESC_FG_MAGENTA, ESC_BG_WHITE) 

#define SET_BOLD_CYAN_TEXT_BLACK_GROUND      __set_both_bold(ESC_FG_CYAN,    ESC_BG_BLACK) 
#define SET_BOLD_CYAN_TEXT_RED_GROUND        __set_both_bold(ESC_FG_CYAN,    ESC_BG_RED) 
#define SET_BOLD_CYAN_TEXT_GREEN_GROUND      __set_both_bold(ESC_FG_CYAN,    ESC_BG_GREEN) 
#define SET_BOLD_CYAN_TEXT_YELLOW_GROUND     __set_both_bold(ESC_FG_CYAN,    ESC_BG_YELLOW) 
#define SET_BOLD_CYAN_TEXT_BLUE_GROUND       __set_both_bold(ESC_FG_CYAN,    ESC_BG_BLUE) 
#define SET_BOLD_CYAN_TEXT_MAGENTA_GROUND    __set_both_bold(ESC_FG_CYAN,    ESC_BG_MAGENTA) 
#define SET_BOLD_CYAN_TEXT_CYAN_GROUND       __set_both_bold(ESC_FG_CYAN,    ESC_BG_CYAN) 
#define SET_BOLD_CYAN_TEXT_WHITE_GROUND      __set_both_bold(ESC_FG_CYAN,    ESC_BG_WHITE) 

#define SET_BOLD_WHITE_TEXT_BLACK_GROUND     __set_both_bold(ESC_FG_WHITE,   ESC_BG_BLACK) 
#define SET_BOLD_WHITE_TEXT_RED_GROUND       __set_both_bold(ESC_FG_WHITE,   ESC_BG_RED) 
#define SET_BOLD_WHITE_TEXT_GREEN_GROUND     __set_both_bold(ESC_FG_WHITE,   ESC_BG_GREEN) 
#define SET_BOLD_WHITE_TEXT_YELLOW_GROUND    __set_both_bold(ESC_FG_WHITE,   ESC_BG_YELLOW) 
#define SET_BOLD_WHITE_TEXT_BLUE_GROUND      __set_both_bold(ESC_FG_WHITE,   ESC_BG_BLUE) 
#define SET_BOLD_WHITE_TEXT_MAGENTA_GROUND   __set_both_bold(ESC_FG_WHITE,   ESC_BG_MAGENTA) 
#define SET_BOLD_WHITE_TEXT_CYAN_GROUND      __set_both_bold(ESC_FG_WHITE,   ESC_BG_CYAN) 
#define SET_BOLD_WHITE_TEXT_WHITE_GROUND     __set_both_bold(ESC_FG_WHITE,   ESC_BG_WHITE) 

#endif // __BLOP_COLOURS__
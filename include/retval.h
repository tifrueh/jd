#ifndef ERROR_H
#define ERROR_H

#define ERROR_STR_BUFSIZE 256

enum return_value {
    SUCCESS = 0,
    ERROR = 1,
    NOT_FOUND = 2,
    OPTPARSE_ERROR = 3
};

extern char error_str[ERROR_STR_BUFSIZE];

#endif

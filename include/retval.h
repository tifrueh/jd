// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef RETVAL_H
#define RETVAL_H

#define ERROR_STR_BUFSIZE 256
#define CALLER_STR_BUFSIZE 16

#define E_NO_JD_ROOT \
    "No johnny.decimal path specified. If you haven't done so, you should use\n"\
    "jd-config(1) to set the config option \"jd_root\":\n"\
    "\n"\
    "  jd config --set jd_root <value>"

enum return_value {
    SUCCESS = 0,
    ERROR = 1,
    NOT_FOUND = 2,
    INPUT_ERROR = 3,
    CONFIG_ERROR = 4
};

extern char error_str[ERROR_STR_BUFSIZE];
extern char caller_str[CALLER_STR_BUFSIZE];

#endif

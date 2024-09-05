// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef ERROR_H
#define ERROR_H

#define ERROR_STR_BUFSIZE 256

#define E_NO_JD_PATH \
    "No johnny.decimal path specified. If you haven't done so, you should use\n"\
    "jd-config(1) to set the config option \"jd_path\":\n"\
    "\n"\
    "  jd config --set jd_path <value>"

enum return_value {
    SUCCESS = 0,
    ERROR = 1,
    NOT_FOUND = 2,
    OPTPARSE_ERROR = 3
};

extern char error_str[ERROR_STR_BUFSIZE];

#endif

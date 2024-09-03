// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef PATH_H
#define PATH_H

#define MAX_FNLEN 256
#define MAX_PATHLEN 4096

#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "retval.h"

struct jd_path {
    int area;
    int category;
    int id;
};

struct jd_path parse_jd_path(const char path_descriptor[]);
int get_fs_path(char* fs_path, int fs_path_bufsize, const struct jd_path jd_path, const char jd_root[]);

#endif

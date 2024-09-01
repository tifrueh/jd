#ifndef PATH_H
#define PATH_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct jd_path {
    int area;
    int category;
    int id;
};

struct jd_path parse_jd_path(const char path_descriptor[]);
int get_fs_path(const struct jd_path jd_path);

#endif

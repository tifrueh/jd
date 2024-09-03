// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef JDLS_H
#define JDLS_H

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "path.h"
#include "retval.h"

int jd_ls(int argc, char* argv[], const struct conf_data* configuration);

#endif

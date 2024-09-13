// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef JDFSLS_H
#define JDFSLS_H

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "path.h"
#include "retval.h"

int jdfs_ls(int argc, char* argv[], const struct conf_data* configuration);

#endif

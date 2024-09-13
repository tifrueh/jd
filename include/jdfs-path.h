// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef JDFSPATH_H
#define JDFSPATH_H

#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "path.h"
#include "retval.h"

int jdfs_path(int argc, char* argv[], const struct conf_data* configuration);

#endif

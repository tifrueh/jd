// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef JDCD_H
#define JDCD_H

#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "path.h"
#include "retval.h"

int jd_path(int argc, char* argv[], const struct conf_data* configuration);

#endif

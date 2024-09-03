// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef JDCONF_H
#define JDCONF_H

#include <stdio.h>
#include <getopt.h>

#include "configuration.h"
#include "optparse.h"
#include "retval.h"

int jd_config(int argc, char* argv[], const struct conf_data* configuration);

#endif

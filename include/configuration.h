// Copyright (C) 2024 Timo Früh
// See ../src/main.c for the full notice.

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "retval.h"

#define CONFIG_STUB "jdfs/jdfs.conf"
#define CONFIG_PATH_BUFSIZE 256
#define CONFIG_LINE_BUFSIZE 512
#define CONFIG_NAME_BUFSIZE 64
#define CONFIG_VALUE_BUFSIZE 256

struct conf_data {
    char* config_path;
    char* jdfs_root;
    char* show_hidden;
};

int get_conf_path(char* path_buffer, size_t path_bufsize);

int read_conf_data(char* buffer, size_t bufsize, const char* path, const char* name);
int write_conf_data(const char* name, const char* value, const char* path, const char* swappath);
int delete_conf_data(const char* name, const char* path, const char* swappath);
int print_conf_data(const char* path);

#endif

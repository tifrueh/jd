#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_STUB "jd/jd.conf"
#define MAX_CONFIG_PATH_SIZE 256

struct conf_data {
    char* jd_path;
};

int get_conf_path(char* path_buffer, size_t path_bufsize);

int read_conf_data(char* buffer, const char* path, const char* name);
int write_conf_data(const char* name, const char* value, const char* path);
int print_conf_data(const struct conf_data* configuration);


#endif

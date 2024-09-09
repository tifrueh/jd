// Copyright (C) 2024 Timo Früh
// See main.c for the full notice.

#include "configuration.h"

struct conf_pair {
    char* name;
    char* value;
};

int get_conf_path(char* path_buffer, size_t path_bufsize) {
    char* xdg_config_home = getenv("XDG_CONFIG_HOME");
    char* home = getenv("HOME");

    if (xdg_config_home == NULL) {
        return snprintf(path_buffer, path_bufsize, "%s/.config/%s", home, CONFIG_STUB);
    }

    return snprintf(path_buffer, path_bufsize, "%s/%s", xdg_config_home, CONFIG_STUB);

}

int parse_line(struct conf_pair* pair, size_t name_bufsize, size_t value_bufsize, const char* line) {
    char* equal_sign = strchr(line, '=');

    if (equal_sign == NULL) {
        return CONFIG_ERROR;
    }

    int name_size = equal_sign - line;

    if (name_size >= name_bufsize) {
        return CONFIG_ERROR;
    }

    snprintf(pair->name, name_size + 1, "%s", line);

    int value_size = &line[strlen(line)] - equal_sign;

    if (value_size >= value_bufsize) {
        return CONFIG_ERROR;
    }

    snprintf(pair->value, value_size - 1, "%s", equal_sign + 1);

    return SUCCESS;

}

int read_conf_data(char* buffer, size_t bufsize, const char* path, const char* name) {
    FILE* confptr = fopen(path, "r");

    if (confptr == NULL) {
        return CONFIG_ERROR;
    }

    int linenum = 1;
    char line_buffer[CONFIG_LINE_BUFSIZE];

    struct conf_pair pair = {
        calloc(CONFIG_NAME_BUFSIZE, sizeof(char)),
        calloc(CONFIG_VALUE_BUFSIZE, sizeof(char))
    };

    enum return_value return_value = NOT_FOUND;

    while (fgets(line_buffer, CONFIG_LINE_BUFSIZE, confptr)) {

        int err_code = parse_line(&pair, CONFIG_NAME_BUFSIZE, CONFIG_VALUE_BUFSIZE, line_buffer);

        if (err_code != SUCCESS) {
            snprintf(error_str, ERROR_STR_BUFSIZE, "%s:%i: malformed config line", path, linenum);
            return_value = err_code;
            goto exit_read;
        }

        if (strcmp(pair.name, name) == 0) {
            snprintf(buffer, bufsize, "%s", pair.value);
            return_value = SUCCESS;
        }

        linenum++;
    }

    exit_read:

    free(pair.name);
    free(pair.value);

    fclose(confptr);

    return return_value;
};

FILE* fopen_mkdir(const char* path, mode_t dir_mode, const char* file_mode) {
    char* pathdup = strdup(path);
    char* pathsep = strchr(pathdup + 1, '/');

    FILE* retval = NULL;

    while (pathsep != NULL) {
        *pathsep = '\0';

        if (mkdir(pathdup, dir_mode) && errno != EEXIST) {
            snprintf(error_str, ERROR_STR_BUFSIZE, "error creating directory %s: %s", pathdup, strerror(errno));
            goto exit_fopen_mkdir;
        }

        *pathsep = '/';
        pathsep = strchr(pathsep + 1, '/');
    }

    retval = fopen(path, file_mode);

    if (retval == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "error opening file %s: %s", path, strerror(errno));
        goto exit_fopen_mkdir;
    }

    exit_fopen_mkdir:
    free(pathdup);
    return retval;
}

int write_conf_data(const char* name, const char* value, const char* path, const char* swappath) {
    if (strlen(name) >= CONFIG_NAME_BUFSIZE) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "configuration name %s too long", name);
        return ERROR;
    }

    if (strlen(value) >= CONFIG_VALUE_BUFSIZE) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "configuration value %s too long", value);
        return ERROR;
    }

    FILE* confptr = fopen_mkdir(path, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, "r");
    FILE* swapptr = fopen_mkdir(swappath, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, "w");

    char new_conf_line[CONFIG_LINE_BUFSIZE];
    snprintf(new_conf_line, CONFIG_LINE_BUFSIZE, "%s=%s\n", name, value);

    char line_buffer[CONFIG_LINE_BUFSIZE];

    struct conf_pair pair = {
        calloc(CONFIG_NAME_BUFSIZE, sizeof(char)),
        calloc(CONFIG_VALUE_BUFSIZE, sizeof(char))
    };

    enum return_value retval = SUCCESS;

    if (confptr == NULL && swapptr != NULL) {
        fprintf(swapptr, "%s", new_conf_line);
        goto exit_write_rename;
    }

    if (swapptr == NULL) {
        retval = CONFIG_ERROR;
        goto exit_write;
    }

    int line = 0;
    int old_value_found = 0;

    while (fgets(line_buffer, CONFIG_LINE_BUFSIZE, confptr)) {
        int err_code = parse_line(&pair, CONFIG_NAME_BUFSIZE, CONFIG_VALUE_BUFSIZE, line_buffer);

        if (err_code != SUCCESS) {
            snprintf(error_str, ERROR_STR_BUFSIZE, "%s:%i: malformed config line", path, line);
            retval = err_code;
            goto exit_write;
        }

        if (strcmp(pair.name, name) == 0) {
            old_value_found = 1;
            fprintf(swapptr, "%s", new_conf_line);
        } else {
            fprintf(swapptr, "%s", line_buffer);
        }

        line++;
    }

    if (old_value_found == 0) {
        fprintf(swapptr, "%s", new_conf_line);
    }

    exit_write_rename:
    {}

    int rename_retval = rename(swappath, path);

    if (rename_retval != 0) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "error writing new config %s: %s", path, strerror(errno));
        retval = CONFIG_ERROR;
    }

    exit_write:

    free(pair.name);
    free(pair.value);

    if (swapptr != NULL) {
        fclose(swapptr);
    }

    if (confptr != NULL) {
        fclose(confptr);
    }

    return retval;
}

int delete_conf_data(const char* name, const char* path, const char* swappath) {
    FILE* confptr = fopen(path, "r");
    FILE* swapptr = fopen(swappath, "w");

    char line_buffer[CONFIG_LINE_BUFSIZE];

    struct conf_pair pair = {
        calloc(CONFIG_NAME_BUFSIZE, sizeof(char)),
        calloc(CONFIG_VALUE_BUFSIZE, sizeof(char))
    };

    enum return_value retval = NOT_FOUND;
    int line = 0;

    if (confptr == NULL || swapptr == NULL) {
        goto exit_delete;
    }

    while (fgets(line_buffer, CONFIG_LINE_BUFSIZE, confptr)) {
        int err_code = parse_line(&pair, CONFIG_NAME_BUFSIZE, CONFIG_VALUE_BUFSIZE, line_buffer);

        if (err_code != SUCCESS) {
            snprintf(error_str, ERROR_STR_BUFSIZE, "%s:%i: malformed config line", path, line);
            retval = err_code;
            goto exit_delete;
        }

        if (strcmp(pair.name, name) != 0) {
            fprintf(swapptr, "%s", line_buffer);
        } else {
            retval = SUCCESS;
        }

        line++;
    }

    int rename_retval = rename(swappath, path);

    if (rename_retval != 0) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "error writing new config %s: %s", path, strerror(errno));
        retval = CONFIG_ERROR;
    }

    exit_delete:

    free(pair.name);
    free(pair.value);

    if (swapptr != NULL) {
        fclose(swapptr);
    }

    if (confptr != NULL) {
        fclose(confptr);
    }

    return retval;

};

int print_conf_data(const char* path) {
    FILE* confptr = fopen(path, "r");

    if (confptr == NULL) {
        return CONFIG_ERROR;
    }

    char line_buffer[CONFIG_LINE_BUFSIZE];

    struct conf_pair pair = {
        calloc(CONFIG_NAME_BUFSIZE, sizeof(char)),
        calloc(CONFIG_VALUE_BUFSIZE, sizeof(char))
    };

    while (fgets(line_buffer, CONFIG_LINE_BUFSIZE, confptr)) {
        int err_code = parse_line(&pair, CONFIG_NAME_BUFSIZE, CONFIG_VALUE_BUFSIZE, line_buffer);

        if (err_code != SUCCESS) {
            printf("malformed config line: %s\n", line_buffer);
            continue;
        }

        printf("%s=%s\n", pair.name, pair.value);
    }

    free(pair.name);
    free(pair.value);

    fclose(confptr);

    return SUCCESS;
}

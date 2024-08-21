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
        return ERROR;
    }

    int name_size = equal_sign - line;

    if (name_size >= name_bufsize) {
        return ERROR;
    }

    snprintf(pair->name, name_size + 1, "%s", line);

    int value_size = &line[strlen(line)] - equal_sign;

    if (value_size >= value_bufsize) {
        return ERROR;
    }

    snprintf(pair->value, value_size - 1, "%s", equal_sign + 1);

    return SUCCESS;

}

int read_conf_data(char* buffer, size_t bufsize, const char* path, const char* name) {
    FILE* confptr = fopen(path, "r");

    if (confptr == NULL) {
        return NOT_FOUND;
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
            snprintf(error_str, ERROR_STR_BUFSIZE, "%s:%i: malformed config line\n", path, linenum);
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

int write_conf_data(const char* name, const char* value, const char* path, const char* swappath) {

    if (strlen(name) >= CONFIG_NAME_BUFSIZE) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "configuration name %s too long", name);
        return ERROR;
    }

    if (strlen(value) >= CONFIG_VALUE_BUFSIZE) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "configuration value %s too long", value);
        return ERROR;
    }

    FILE* confptr = fopen(path, "r");
    FILE* swapptr = fopen(swappath, "w");

    char new_conf_line[CONFIG_LINE_BUFSIZE];
    snprintf(new_conf_line, CONFIG_LINE_BUFSIZE, "%s=%s\n", name, value);

    char line_buffer[CONFIG_LINE_BUFSIZE];

    struct conf_pair pair = {
        calloc(CONFIG_NAME_BUFSIZE, sizeof(char)),
        calloc(CONFIG_VALUE_BUFSIZE, sizeof(char))
    };

    enum return_value retval = SUCCESS;
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

    int rename_retval = rename(swappath, path);

    if (rename_retval != 0) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to write new config %s: %s", path, strerror(errno));
        retval = ERROR;
    }

    exit_write:

    free(pair.name);
    free(pair.value);

    fclose(swapptr);
    fclose(confptr);

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
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to write new config %s: %s", path, strerror(errno));
        retval = ERROR;
    }

    exit_delete:

    free(pair.name);
    free(pair.value);

    fclose(swapptr);
    fclose(confptr);

    return retval;

};

int print_conf_data(const char* path) {
    FILE* confptr = fopen(path, "r");

    if (confptr == NULL) {
        return NOT_FOUND;
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

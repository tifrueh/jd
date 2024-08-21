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

int write_conf_data(const char* name, const char* value, const char* path) {
    printf("writing %s = %s to %s\n", name, value, path);
    return SUCCESS;
}

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


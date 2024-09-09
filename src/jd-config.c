// Copyright (C) 2024 Timo Früh
// See main.c for the full notice.

#include "jd-config.h"

enum actions {
    CONFIG_HELP,
    CONFIG_GET,
    CONFIG_LIST,
    CONFIG_SET,
    CONFIG_UNSET,
    CONFIG_NULL
} action = CONFIG_LIST;

struct option longopts[] = {
    { "help", no_argument, NULL, 'h' },
    { "get", required_argument, NULL, 'g' },
    { "list", no_argument, NULL, 'l' },
    { "set", required_argument, NULL, 's' },
    { "unset", required_argument, NULL, 'u' },
    { NULL, 0, NULL, 0 }
};

void config_print_help(char argv_0[]) {

    char help_string[] =
        "Usage:\n"
        "\n"
        "  jd %1$s [ -h | --help ]\n"
        "  jd %1$s [ -g | --get ] option\n"
        "  jd %1$s [ -l | --list ]\n"
        "  jd %1$s [ -s | --set ] option value\n"
        "  jd %1$s [ -u | --unset ] option\n"
        "\n"
        "For more information, please consult the manual page.\n";

    printf(help_string, argv_0);
}

int config_set(char* name, char* value, char* path) {
    if (value == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "%s couldn't be set: empty value", name);
        return INPUT_ERROR;
    }

    char swappath[CONFIG_PATH_BUFSIZE+1];
    snprintf(swappath, CONFIG_PATH_BUFSIZE+1, "%s~", path);

    return write_conf_data(name, value, path, swappath);
};

int config_get(char* name, char* path) {

    char value_buffer[CONFIG_VALUE_BUFSIZE];

    int read_retval = read_conf_data(value_buffer, CONFIG_VALUE_BUFSIZE, path, name);

    if (read_retval != SUCCESS) {
        return read_retval;
    }

    printf("%s\n", value_buffer);

    return SUCCESS;

}

int config_unset(char* name, char* path) {
    char swappath[CONFIG_PATH_BUFSIZE+1];
    snprintf(swappath, CONFIG_PATH_BUFSIZE+1, "%s~", path);

    return delete_conf_data(name, path, swappath);
}

int config_optswitch(int optchar, char* optarg) {
    switch (optchar) {
        case 'h':
            action = CONFIG_HELP;
            return OPT_FINAL;

        case 'g':
            action = CONFIG_GET;
            return OPT_FINAL;

        case 'l':
            action = CONFIG_LIST;
            return OPT_FINAL;

        case 's':
            action = CONFIG_SET;
            return OPT_FINAL;

        case 'u':
            action = CONFIG_UNSET;
            return OPT_FINAL;

        case '?':
            return OPT_ERROR;

        default:
            action = CONFIG_LIST;
            return OPT_FINAL;
    }
}

int jd_config(int argc, char* argv[], const struct conf_data* configuration) {

    int optchar;

    while((optchar = getopt_long(argc, argv, "hg:ls:u:", longopts, NULL)) != -1) {
        enum optswitch_retval optswitch = config_optswitch(optchar, optarg);

        if (optswitch == OPT_FINAL) {
            break;
        } else if (optswitch == OPT_ERROR) {
            return INPUT_ERROR;
        }
    }

    switch (action) {
        case CONFIG_HELP:
            config_print_help(argv[0]);
            return SUCCESS;

        case CONFIG_GET:
            return config_get(optarg, configuration->config_path);

        case CONFIG_LIST:
            return print_conf_data(configuration->config_path);

        case CONFIG_SET:
            return config_set(optarg, argv[optind], configuration->config_path);

        case CONFIG_UNSET:
            return config_unset(optarg, configuration->config_path);

        case CONFIG_NULL:
            return ERROR;

        default:
            return print_conf_data(configuration->config_path);
    }
}

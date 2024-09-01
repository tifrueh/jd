#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "jd-cd.h"
#include "jd-config.h"
#include "jd-ls.h"
#include "mesonconf.h"

void print_help(char argv_0[]) {

    char help_string[] =
        "Usage:\n"
        "  %1$s [ -h | --help ]\n"
        "  %1$s [ -v | --version]\n"
        "  %1$s <command> ...\n"
        "\n"
        "<command> must be one of the following:\n"
        "  cd        Change to a directory of the jd system\n"
        "  config    Configure the jd utility\n"
        "  ls        List directories of the jd system\n"
        "\n"
        "For more information, please consult the manual page.\n";

    printf(help_string, argv_0);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        print_help(argv[0]);
        return ERROR;
    }

    char* conf_path_buffer = calloc(CONFIG_PATH_BUFSIZE, sizeof(char));
    get_conf_path(conf_path_buffer, CONFIG_PATH_BUFSIZE);

    struct conf_data configuration = {
        calloc(CONFIG_PATH_BUFSIZE, sizeof(char)),
        calloc(CONFIG_VALUE_BUFSIZE, sizeof(char)),
        calloc(CONFIG_VALUE_BUFSIZE, sizeof(char))
    };

    snprintf(configuration.config_path, CONFIG_PATH_BUFSIZE, "%s", conf_path_buffer);

    free(conf_path_buffer);

    enum return_value return_value = ERROR;

    int jd_path_read = read_conf_data(configuration.jd_path, CONFIG_VALUE_BUFSIZE, configuration.config_path, "jd_path");

    if (jd_path_read != SUCCESS) {
        configuration.jd_path = NULL;
    };

    int show_hidden_read = read_conf_data(configuration.show_hidden, CONFIG_VALUE_BUFSIZE, configuration.config_path, "show_hidden");

    if (show_hidden_read != SUCCESS) {
        snprintf(configuration.show_hidden, CONFIG_VALUE_BUFSIZE, "%s", "true");
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 ) {
        print_help(argv[0]);
        return_value = SUCCESS;
        goto exit_jd;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("%s\n", TAG);
        return_value = SUCCESS;
        goto exit_jd;
    }

    if (strcmp(argv[1], "ls") == 0) {
        return_value = jd_ls(argc - 1, &argv[1], &configuration);
        goto exit_jd;
    }

    if (strcmp(argv[1], "cd") == 0) {
        return_value = jd_cd(argc - 1, &argv[1], &configuration);
        goto exit_jd;
    }

    if (strcmp(argv[1], "config") == 0) {
        return_value = jd_config(argc - 1, &argv[1], &configuration);
        goto exit_jd;
    }

    print_help(argv[0]);

    exit_jd:

    free(configuration.config_path);
    free(configuration.jd_path);

    if (return_value == ERROR && strcmp(error_str, "") != 0) {
        fprintf(stderr, "%s\n", error_str);
    }

    return return_value;

}

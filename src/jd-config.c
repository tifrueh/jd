#include "jd-config.h"

void config_print_help(char argv_0[]) {

    char help_string[] =
        "Usage:\n"
        "  jd %1$s [ -h | --help ]\n"
        "  jd %1$s [ -g | --get ] option\n"
        "  jd %1$s [ -l | --list ]\n"
        "  jd %1$s [ -s | --set ] option value\n"
        "  jd %1$s [ -u | --unset ] option\n"
        "\n"
        "For more information, please consult the manual page.\n";

    printf(help_string, argv_0);
}

int jd_config(int argc, char* argv[], const struct conf_data* configuration) {

    config_print_help(argv[0]);

    return SUCCESS;

}

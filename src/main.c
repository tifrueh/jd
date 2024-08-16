#include <stdio.h>
#include <string.h>

void print_help(char argv_0[]) {

    char help_string[] =
        "Usage:\n"
        "  %1$s [ -h | --help ]\n"
        "  %1$s [ -v | --version]\n"
        "  %1$s <command> ...\n"
        "\n"
        "<command> must be one of the following:\n"
        "  ls        List directories of the jd system\n"
        "  cd        Change to a directory of the jd system\n"
        "  config    Configure the jd utility\n"
        "\n"
        "For more information, please consult the manual page.\n";

    printf(help_string, argv_0);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 ) {
        print_help(argv[0]);
        return 0;

    } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("flag: version\n");
        return 0;

    } else if (strcmp(argv[1], "ls") == 0) {
        printf("subcommand: ls\n");
        return 0;

    } else if (strcmp(argv[1], "cd") == 0) {
        printf("subcommand: cd\n");
        return 0;

    } else if (strcmp(argv[1], "config") == 0) {
        printf("subcommand: config\n");
        return 0;

    } else {
        print_help(argv[0]);
        return 1;
    }

}

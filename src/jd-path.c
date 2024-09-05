// Copyright (C) 2024 Timo Früh
// See main.c for the full notice.

#include "jd-path.h"

void path_print_help(char argv_0[]) {

  char help_string[] =
      "Usage:\n"
      "  jd %1$s [ -h | --help ]\n"
      "  jd %1$s [ <jd_path_descriptor> ]\n"
      "\n"
      "<jd_path_descriptor> (if specified) must a valid and existing\n"
      "johnny.decimal area, category or id. To specify an area, use only the\n"
      "first digit of its categories, for example '2' for area 20-29. \n"
      "\n"
      "For more information, please consult the manual page.\n";

  printf(help_string, argv_0);
}

int jd_path(int argc, char* argv[], const struct conf_data* configuration) {

    if (configuration->jd_path == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "%s", E_NO_JD_PATH);
        return ERROR;
    }

    if (argc < 2) {
        path_print_help(argv[0]);
        return ERROR;
    }

    if (strcmp(argv[1], "-h") == 0) {
        path_print_help(argv[0]);
        return SUCCESS;
    }

    struct jd_path path = parse_jd_path(argv[1]);

    char* out_path = calloc(MAX_PATHLEN, sizeof(char));

    if (out_path == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to allocate memory for the path buffer: %s", strerror(errno));
    }

    int retval = get_fs_path(out_path, MAX_PATHLEN, path, configuration->jd_path);

    if (retval == SUCCESS) {
        printf("%s\n", out_path);
    }

    free(out_path);

    return retval;
}

// Copyright (C) 2024 Timo Früh
// See main.c for the full notice.

#include "jdfs-ls.h"

void ls_print_help(char argv_0[]) {

  char help_string[] =
      "Usage:\n"
      "  jdfs %1$s [ -h | --help ]\n"
      "  jdfs %1$s [ <jd_path_descriptor> ]\n"
      "\n"
      "<jd_path_descriptor> (if specified) must a valid and existing\n"
      "johnny.decimal area, category or id. To specify an area, use only the\n"
      "first digit of its categories, for example '2' for area 20-29.\n"
      "\n"
      "For more information, please consult the manual page.\n";

  printf(help_string, argv_0);
}

int print_direntry(struct dirent* direntry, int show_hidden) {
    if (direntry->d_name[0] == '.' && show_hidden == 0) {
        return SUCCESS;
    }

    if (direntry->d_type == DT_DIR) {
        return printf("%s/\n", direntry->d_name);
    }

    return printf("%s\n", direntry->d_name);
}

int list_dir(const char* directory_path, int show_hidden) {

    struct dirent** namelist;
    int scan = scandir(directory_path, &namelist, NULL, alphasort);

    if (scan == -1) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "error opening directory %s: %s", directory_path, strerror(errno));
        return ERROR;
    }

    for (int i = 0; i < scan; i++) {
        print_direntry(namelist[i], show_hidden);
        free(namelist[i]);
    }
    free(namelist);

    return SUCCESS;
}

int jdfs_ls(int argc, char* argv[], const struct conf_data* configuration) {

    snprintf(caller_str, CALLER_STR_BUFSIZE, "jdfs-%s", argv[0]);

    if (configuration->jdfs_root == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "%s", E_NO_JDFS_ROOT);
        return ERROR;
    }

    int show_hidden = (strcmp(configuration->show_hidden, "true") == 0) ? (1) : (0);

    if (argc < 2) {
        return list_dir(configuration->jdfs_root, show_hidden);
    }

    if (strcmp(argv[1], "-h") == 0) {
        ls_print_help(argv[0]);
        return SUCCESS;
    }

    struct jd_path path = parse_jd_path(argv[1]);

    char* out_path = calloc(MAX_PATHLEN, sizeof(char));

    if (out_path == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "error allocating memory for the path buffer: %s", strerror(errno));
    }

    int retval = get_fs_path(out_path, MAX_PATHLEN, path, configuration->jdfs_root);

    if (retval == SUCCESS) {
        retval = list_dir(out_path, show_hidden);
    }

    free(out_path);

    return retval;
}

// jdfs - navigation and manipulation tool for johnny.decimal systems
// Copyright (C) 2024 Timo Früh
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "jdfs-path.h"
#include "jdfs-config.h"
#include "jdfs-ls.h"
#include "mesonconf.h"

void print_help(char argv_0[]) {

    char help_string[] =
        "Usage:\n"
        "  %1$s [ -h | --help ]\n"
        "  %1$s [ -v | --version]\n"
        "  %1$s <command> ...\n"
        "\n"
        "<command> must be one of the following:\n"
        "  path      Display the path to a directory of the jd file system\n"
        "  config    Configure the jdfs utility\n"
        "  ls        List directories of the jd file system\n"
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

    char* jdfs_root_env = getenv("JDFS_ROOT");

    if (jdfs_root_env == NULL) {

        int jdfs_root_read = read_conf_data(configuration.jdfs_root, CONFIG_VALUE_BUFSIZE, configuration.config_path, "jdfs_root");

        if (jdfs_root_read != SUCCESS) {
            configuration.jdfs_root = NULL;
        };

    } else {
        snprintf(configuration.jdfs_root, CONFIG_VALUE_BUFSIZE, "%s", jdfs_root_env);
    }

    int show_hidden_read = read_conf_data(configuration.show_hidden, CONFIG_VALUE_BUFSIZE, configuration.config_path, "show_hidden");

    if (show_hidden_read != SUCCESS) {
        snprintf(configuration.show_hidden, CONFIG_VALUE_BUFSIZE, "%s", "false");
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 ) {
        print_help(argv[0]);
        return_value = SUCCESS;
        goto exit_jdfs;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("%s\n", TAG);
        return_value = SUCCESS;
        goto exit_jdfs;
    }

    if (strcmp(argv[1], "ls") == 0) {
        return_value = jdfs_ls(argc - 1, &argv[1], &configuration);
        goto exit_jdfs;
    }

    if (strcmp(argv[1], "path") == 0) {
        return_value = jdfs_path(argc - 1, &argv[1], &configuration);
        goto exit_jdfs;
    }

    if (strcmp(argv[1], "config") == 0) {
        return_value = jdfs_config(argc - 1, &argv[1], &configuration);
        goto exit_jdfs;
    }

    print_help(argv[0]);

    exit_jdfs:

    free(configuration.config_path);
    free(configuration.jdfs_root);
    free(configuration.show_hidden);

    if (return_value != SUCCESS && strcmp(error_str, "") != 0) {
        fprintf(stderr, "%s: %s\n", caller_str, error_str);
    }

    return return_value;

}

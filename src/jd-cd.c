#include "jd-cd.h"

void cd_print_help(char argv_0[]) {

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

int jd_cd(int argc, char* argv[]) {

    if (argc < 2) {
        cd_print_help(argv[0]);
        return ERROR;
    }

    if (strcmp(argv[1], "-h") == 0) {
        cd_print_help(argv[0]);
        return SUCCESS;
    }

    struct jd_path path = parse_jd_path(argv[1]);

    printf("jd path components\n------------------\n\narea: %i\ncategory: %i\nid: %i\n", path.area, path.category, path.id);

    return SUCCESS;
}

#include "jd-ls.h"

void ls_print_help(char argv_0[]) {

  char help_string[] =
      "Usage:\n"
      "  jd %1$s [ -h | --help ]\n"
      "  jd %1$s [ <jd_path_descriptor> ]\n"
      "\n"
      "<jd_path_descriptor> (if specified) must a valid and existing\n"
      "johnny.decimal area, category or id. To specify an area, use only the\n"
      "first digit of its categories, for example '2' for area 20-29.\n"
      "\n"
      "For more information, please consult the manual page.\n";

  printf(help_string, argv_0);
}

int jd_ls(int argc, char* argv[]) {

    ls_print_help(argv[0]);

    return SUCCESS;
}

#include "path.h"

struct jd_path parse_jd_path(const char path_descriptor[]) {
    struct jd_path retval = {-1, -1, -1};

    int descriptor_length = strlen(path_descriptor);

    if (descriptor_length < 1) {
        return retval;
    }

    if (descriptor_length >= 1 && isdigit(path_descriptor[0]) == 1) {
        retval.area = path_descriptor[0] - '0';
    }

    if (descriptor_length >= 2 && isdigit(path_descriptor[1]) == 1) {
        retval.category = path_descriptor[1] - '0';
    }

    if (descriptor_length >= 3 && path_descriptor[2] != '.') {
        return retval;
    }

    if (descriptor_length >= 4 && isdigit(path_descriptor[3]) == 1) {
        retval.id = atoi(path_descriptor + 3);
    }

    return retval;
};

#include "jd-config.h"

int jd_config(int argc, char* argv[], const struct conf_data* configuration) {

    print_conf_data(configuration);

    return SUCCESS;
}

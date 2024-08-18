#include "jd-config.h"

int jd_config(int argc, char* argv[], const struct conf_data* configuration) {

    return print_conf_data(configuration->config_path);

}

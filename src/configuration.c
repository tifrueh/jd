#include "configuration.h"

int get_conf_path(char* path_buffer, size_t path_bufsize) {
    char* xdg_config_home = getenv("XDG_CONFIG_HOME");
    char* home = getenv("HOME");

    if (xdg_config_home == NULL) {
        return snprintf(path_buffer, path_bufsize, "%s/.config/%s", home, CONFIG_STUB);
    }

    return snprintf(path_buffer, path_bufsize, "%s/%s", xdg_config_home, CONFIG_STUB);

}

int read_conf_data(char* buffer, const char* path, const char* name) {
    printf("reading %s from %s\n", name, path);
    return SUCCESS;
};

int write_conf_data(const char* name, const char* value, const char* path) {
    printf("writing %s = %s to %s\n", name, value, path);
    return SUCCESS;
}

int print_conf_data(const struct conf_data* configuration) {
    printf("jd_path = %s\n", configuration->jd_path);
    return SUCCESS;
}


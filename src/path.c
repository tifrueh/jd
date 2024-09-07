// Copyright (C) 2024 Timo Früh
// See main.c for the full notice.

#include "path.h"

struct jd_path parse_jd_path(const char path_descriptor[]) {
    struct jd_path retval = {-1, -1, -1};

    int descriptor_length = strlen(path_descriptor);

    if (descriptor_length < 1) {
        return retval;
    }

    if (descriptor_length >= 1 && isdigit(path_descriptor[0]) != 0) {
        retval.area = path_descriptor[0] - '0';
    }

    if (descriptor_length >= 2 && isdigit(path_descriptor[1]) != 0) {
        retval.category = path_descriptor[1] - '0';
    }

    if (descriptor_length >= 3 && path_descriptor[2] != '.') {
        return retval;
    }

    if (descriptor_length >= 4 && isdigit(path_descriptor[3]) != 0) {
        retval.id = atoi(path_descriptor + 3);
    }

    return retval;
};

int get_fs_path(char* fs_path, int fs_path_bufsize, const struct jd_path jd_path, const char jd_root[]) {

    if (jd_root == NULL || strcmp(jd_root, "") == 0) {

        snprintf(error_str, ERROR_STR_BUFSIZE, "%s", E_NO_JD_ROOT);
        return ERROR;
    }

    if (jd_path.area == -1) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "invalid area");
        return ERROR;
    }

    DIR* dir = opendir(jd_root);

    enum return_value retval = NOT_FOUND;
    char area_name[MAX_FNLEN] = "";

    if (dir == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to open directory %s: %s", jd_root, strerror(errno));
        return ERROR;
    }

    struct dirent* dp;
    while ((dp = readdir(dir)) != NULL) {

        if (strlen(dp->d_name) > 0 && dp->d_name[0] == jd_path.area + '0') {
            snprintf(area_name, MAX_FNLEN, "%s", dp->d_name);
            snprintf(fs_path, fs_path_bufsize, "%s/%s", jd_root, area_name);
            retval = SUCCESS;
            break;
        }

    }

    if (retval != SUCCESS) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to find area %i", jd_path.area);
        goto exit_get_fs_path;
    }

    if (jd_path.category == -1) {
        goto exit_get_fs_path;
    }

    closedir(dir);
    dir = opendir(fs_path);
    retval = NOT_FOUND;
    char category_name[MAX_FNLEN] = "";

    if (dir == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to open directory %s: %s", jd_root, strerror(errno));
        retval = ERROR;
        goto exit_get_fs_path;
    }

    while ((dp = readdir(dir)) != NULL) {

        if (strlen(dp->d_name) > 1 && dp->d_name[1] == jd_path.category + '0') {
            snprintf(category_name, MAX_FNLEN, "%s", dp->d_name);
            snprintf(fs_path, fs_path_bufsize, "%s/%s/%s", jd_root, area_name, category_name);
            retval = SUCCESS;
            break;
        }
    }

    if (retval != SUCCESS) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to find category %i%i", jd_path.area, jd_path.category);
        goto exit_get_fs_path;
    }

    if (jd_path.id == -1) {
        goto exit_get_fs_path;
    }

    closedir(dir);
    dir = opendir(fs_path);
    retval = NOT_FOUND;
    char id_name[MAX_FNLEN] = "";

    if (dir == NULL) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to open directory %s: %s", jd_root, strerror(errno));
        retval = ERROR;
        goto exit_get_fs_path;
    }

    while ((dp = readdir(dir)) != NULL) {
        snprintf(id_name, MAX_FNLEN, "%s", dp->d_name);
        char* dot = strchr(id_name, '.');
        char* space = strchr(id_name, ' ');

        if (dot == NULL || space == NULL) {
            continue;
        }

        if ((space - dot) > 3) {
            continue;
        }

        *space = '\0';

        char id[3] = "";

        snprintf(id, 3, "%s", dot + 1);

        *space = ' ';

        if (atoi(id) == jd_path.id) {
            snprintf(fs_path, fs_path_bufsize, "%s/%s/%s/%s", jd_root, area_name, category_name, id_name);
            retval = SUCCESS;
            break;
        }

    }

    if (retval != SUCCESS) {
        snprintf(error_str, ERROR_STR_BUFSIZE, "unable to find id %i%i.%.2i", jd_path.area, jd_path.category, jd_path.id);
        goto exit_get_fs_path;
    }

    exit_get_fs_path:
    closedir(dir);
    return retval;
};

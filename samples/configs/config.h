#ifndef CONFIG_H
#define CONFIG_H

struct LogConfig {
    char* err_path;
    char* info_path;
};

struct GlobalConfig {
    struct LogConfig logConfig;
};

struct GlobalConfig gGlobalConfig;

void initConfig(int argv, char **args);

#endif
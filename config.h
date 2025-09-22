#ifndef CONFIG_H
#define CONFIG_H

#define STANDART_TEMP_DIR   QStandardPaths::writableLocation(QStandardPaths::TempLocation)
#define CONFIG_FILE_NAME    "/time_widget_configuration.json"

#include <QStandardPaths>
#include <QString>

bool is_file_exists(const QString& file_path);
bool create_config_file(const QString& file_path);

struct ConfigData
{
    ConfigData(bool is_move, bool is_hide, bool is_run, int x, int y);

    bool is_move, is_hide, is_run;
    int  x, y;
};

struct Config
{
    static  void                write(ConfigData config_data);
    static  ConfigData          read();
};

#endif // CONFIG_H
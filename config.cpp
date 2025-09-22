#include "config.h"

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

#include <iostream>
#include <cstdlib>

bool is_file_exists(const QString& file_path)
{
    return QFileInfo::exists(file_path);
}

bool create_config_file(const QString& file_path)
{
    QFile file(file_path);
    
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) 
    {
        Config::write(ConfigData({true, false, false, 0, 0}));
        file.close();
        
        return true;
    }

    return false;
}

ConfigData::ConfigData(bool is_move, bool is_hide, bool is_run, int x, int y)
: is_move(is_move), is_hide(is_hide), is_run(is_run), x(x), y(y) {}
 
void Config::write(ConfigData config_data)
{
    QJsonObject obj;
    
    obj["is_move"]  = config_data.is_move;
    obj["is_hide"]  = config_data.is_hide;
    obj["is_run"]   = config_data.is_run;
    obj["x"]        = config_data.x;
    obj["y"]        = config_data.y;

    QJsonDocument doc(obj);
    QFile file(STANDART_TEMP_DIR + static_cast<QString>(CONFIG_FILE_NAME));

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) 
    {
        file.write(doc.toJson());
        file.close();
    }
}

ConfigData Config::read()
{
    QFile file(STANDART_TEMP_DIR + static_cast<QString>(CONFIG_FILE_NAME));

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        QByteArray      data = file.readAll();
        QJsonDocument   doc  = QJsonDocument::fromJson(data);

        if (doc.isObject()) 
        {
            QJsonObject obj  = doc.object();

            bool    is_move  = obj["is_move"].toBool();
            bool    is_hide  = obj["is_hide"].toBool();
            bool    is_run   = obj["is_run"].toBool();
            int     x        = obj["x"].toInt();
            int     y        = obj["y"].toInt();

            return ConfigData({is_move, is_hide, is_run, x, y});
        }

        file.close();
    }

    return ConfigData({true, false, false, 0, 0});
}
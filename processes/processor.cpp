#include "processor.h"

#include <sys/sysinfo.h>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <QDir>

Processor::Processor(QObject* parent)
: QObject(parent) {}
    
QString Processor::cpu()
{
    static qint64 previousTotal = 0;
    static qint64 previousIdle  = 0;
    
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "N/A";
    
    QTextStream in(&file);
    QString line = in.readLine();
    file.close();
    
    if (line.startsWith("cpu ")) 
    {
        QStringList values = line.split(' ', Qt::SkipEmptyParts);
        if (values.size() >= 5) 
        {
            qint64 user = values[1].toLongLong();
            qint64 nice = values[2].toLongLong();
            qint64 system = values[3].toLongLong();
            qint64 idle = values[4].toLongLong();
            
            qint64 total = user + nice + system + idle;
            qint64 totalDiff = total - previousTotal;
            qint64 idleDiff = idle - previousIdle;
            
            previousTotal = total;
            previousIdle = idle;
            
            if (totalDiff > 0) 
            {
                int usage = 100 * (totalDiff - idleDiff) / totalDiff;
                return QString("%1%").arg(usage);
            }
        }
    }
    
    return "N/A";
}

QString Processor::gpu()
{
    QStringList gpuPaths = 
    {
        "/sys/class/drm/card0/device/gpu_busy_percent",
        "/sys/class/drm/card1/device/gpu_busy_percent",
        "/sys/kernel/debug/dri/0/amdgpu_pm_info",
        "/sys/kernel/debug/dri/1/amdgpu_pm_info"
    };
    
    for (const QString &path : gpuPaths) 
    {
        QFile file(path);
        if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) 
        {
            QTextStream in(&file);
            QString content = in.readAll().trimmed();
            file.close();
            
            if (path.contains("amdgpu_pm_info")) 
            {
                QStringList lines = content.split('\n');
                for (const QString &line : lines) 
                {
                    if (line.contains("GPU Load") || line.contains("load")) 
                    {
                        QStringList parts = line.split(':');
                        if (parts.size() == 2) 
                        {
                            QString percent = parts[1].trimmed().remove('%');
                            bool ok;
                            int usage = percent.toInt(&ok);

                            if (ok) return QString("%1%").arg(usage);
                        }
                    }
                }
            } 
            else if (!content.isEmpty()) 
            {
                bool ok;
                int usage = content.toInt(&ok);

                if (ok) return QString("%1%").arg(usage);
            }
        }
    }
    
    QProcess nvidiaProcess;
    nvidiaProcess.start("which", QStringList() << "nvidia-smi");

    if (nvidiaProcess.waitForFinished(1000)) 
    {
        QString whichOutput = nvidiaProcess.readAllStandardOutput().trimmed();
        if (!whichOutput.isEmpty()) 
        {
            QProcess smiProcess;
            smiProcess.start("nvidia-smi", QStringList() << "--query-gpu=utilization.gpu" 
                                                        << "--format=csv,noheader,nounits");
            if (smiProcess.waitForFinished(1000)) 
            {
                QString output = smiProcess.readAllStandardOutput().trimmed();
                if (!output.isEmpty())
                {
                    bool ok;
                    int usage = output.toInt(&ok);

                    if (ok) return QString("%1%").arg(usage);
                }
            }
        }
    }
    
    return "N/A";
}

QString Processor::rom()
{
    QProcess process;

    process.start("df", QStringList() << "-h" << "/");
    if (process.waitForFinished(1000) && process.exitCode() == 0) 
    {
        QString output = process.readAllStandardOutput();
        QStringList lines = output.split('\n');

        if (lines.size() >= 2) 
        {
            QStringList parts = lines[1].split(' ', Qt::SkipEmptyParts);

            if (parts.size() >= 5) 
            {
                QString used = parts[2];
                QString total = parts[1];
                QString percent = parts[4];

                return QString("%1/%2 (%3)").arg(used, total, percent);
            }
        }
    }

    return "N/A";
}

QString Processor::ram()
{
    QFile file("/proc/meminfo");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        QTextStream in(&file);
        qint64 totalMem = 0;
        qint64 freeMem = 0;
        qint64 availableMem = 0;
        qint64 buffers = 0;
        qint64 cached = 0;
        
        while (!in.atEnd()) 
        {
            QString line = in.readLine();
            QStringList parts = line.split(':', Qt::SkipEmptyParts);

            if (parts.size() == 2) 
            {
                QString key = parts[0].trimmed();
                QString value = parts[1].trimmed().split(' ', Qt::SkipEmptyParts)[0];
                
                if      (key == "MemTotal")     totalMem = value.toLongLong();
                else if (key == "MemFree")      freeMem = value.toLongLong();
                else if (key == "MemAvailable") availableMem = value.toLongLong();
                else if (key == "Buffers")      buffers = value.toLongLong();
                else if (key == "Cached")       cached = value.toLongLong();
            }
        }

        file.close();
        
        if (totalMem > 0)
        {
            qint64 usedMem;

            if (availableMem > 0)   usedMem = totalMem - availableMem;
            else                    usedMem = totalMem - freeMem - buffers - cached;
            
            double usedPercent = (static_cast<double>(usedMem) / totalMem) * 100.0;
            double totalGB = totalMem / 1024.0 / 1024.0;
            double usedGB = usedMem / 1024.0 / 1024.0;
            
            return QString("%1G/%2G (%3%)")
                    .arg(QString::number(usedGB, 'f', 1))
                    .arg(QString::number(totalGB, 'f', 1))
                    .arg(QString::number(usedPercent, 'f', 1));
        }
    }
    
    QProcess process;

    process.start("free", QStringList() << "-b");
    if (process.waitForFinished(1000) && process.exitCode() == 0) 
    {
        QString output = process.readAllStandardOutput();
        QStringList lines = output.split('\n');

        if (lines.size() >= 2) 
        {
            QStringList parts = lines[1].split(' ', Qt::SkipEmptyParts);

            if (parts.size() >= 7)
            {
                qint64 totalMem = parts[1].toLongLong();
                qint64 usedMem = parts[2].toLongLong();
                
                double usedPercent = (static_cast<double>(usedMem) / totalMem) * 100.0;
                double totalGB = totalMem / 1024.0 / 1024.0 / 1024.0;
                double usedGB = usedMem / 1024.0 / 1024.0 / 1024.0;
                
                return QString("%1G/%2G (%3%)")
                        .arg(QString::number(usedGB, 'f', 1))
                        .arg(QString::number(totalGB, 'f', 1))
                        .arg(QString::number(usedPercent, 'f', 1));
            }
        }
    }
    
    struct sysinfo info;

    if (sysinfo(&info) == 0) 
    {
        qint64 totalMem = info.totalram * info.mem_unit;
        qint64 freeMem = info.freeram * info.mem_unit;
        qint64 usedMem = totalMem - freeMem;
        
        double usedPercent = (static_cast<double>(usedMem) / totalMem) * 100.0;
        double totalGB = totalMem / 1024.0 / 1024.0 / 1024.0;
        double usedGB = usedMem / 1024.0 / 1024.0 / 1024.0;
        
        return QString("%1G/%2G (%3%)")
                .arg(QString::number(usedGB, 'f', 1))
                .arg(QString::number(totalGB, 'f', 1))
                .arg(QString::number(usedPercent, 'f', 1));
    }
    
    return "N/A";
}
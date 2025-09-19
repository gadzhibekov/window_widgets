#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <QString>

class Processor : public QObject
{
    Q_OBJECT

public:
    explicit Processor(QObject* parent);

    Q_INVOKABLE QString cpu();
    Q_INVOKABLE QString gpu();
    Q_INVOKABLE QString rom();
    Q_INVOKABLE QString ram();

};

#endif // PROCESSOR_H
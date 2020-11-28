#ifndef SENSORHELPER_H
#define SENSORHELPER_H

#include <QObject>

// 传感器类，用来帮助解析和生成传感器传输的数据
class SensorHelper : public QObject
{
    Q_OBJECT
public:
    explicit SensorHelper(QObject *parent = nullptr);

signals:

};

#endif // SENSORHELPER_H

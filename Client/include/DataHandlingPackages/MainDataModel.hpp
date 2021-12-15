#ifndef MAINDATAMODEL_H
#define MAINDATAMODEL_H

#include <QObject>

class MainDataModel : public QObject
{
    Q_OBJECT
public:
    explicit MainDataModel(QObject *parent = nullptr);

signals:

};

#endif // MAINDATAMODEL_H

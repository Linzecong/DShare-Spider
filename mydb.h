#ifndef MYDB_H
#define MYDB_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlquery.h>
#include <QtSql/QSqlError>
#include <QTimer>

class MyDB:public QObject{
    Q_OBJECT
public:
    QTimer timer;
    QSqlDatabase* db;
    MyDB();
    ~MyDB(){}
    void reconnect();
    int min;
};




#endif // MYDB_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkRequest>
#include<QEventLoop>
#include<QByteArray>
#include<QTimer>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlquery.h>
#include <QtSql/QSqlError>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


public:
    void getUrl();
    void getTitle();
    void getPhoto();
    void getContent();

    void upload();

public:
    QString Data;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    QTimer Timer;

    QStringList Uploaded;

    QStringList URLList;
    QStringList TitleList;
    QStringList PhotoList;
    QStringList ContentList;
    QStringList TimeList;
    QStringList SourceList;

    QSqlDatabase* db;



private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

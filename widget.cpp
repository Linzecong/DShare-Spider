#include "widget.h"
#include "ui_widget.h"

#include<QString>
#include<QtNetwork/QNetworkAccessManager>
#include<QTextEdit>
#include<QMessageBox>
#include<QFile>
#include<QDateTime>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlquery.h>
#include <QtSql/QSqlError>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    manager = new QNetworkAccessManager();


    connect(&Timer,&QTimer::timeout,this,&Widget::getUrl);
    Timer.start(60*60*1000);


    getUrl();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::getUrl()
{
    URLList.clear();
    TitleList.clear();
    PhotoList.clear();
    ContentList.clear();
    TimeList.clear();
    SourceList.clear();
    ui->LastList->clear();

   // reply = manager->get(QNetworkRequest(QUrl("http://health.sina.com.cn/hc/ct/")));
    reply = manager->get(QNetworkRequest(QUrl("http://health.sina.com.cn/hc/sh/")));
  //  reply = manager->get(QNetworkRequest(QUrl("http://health.sina.com.cn/hc/ys/")));

    QByteArray responseData;
    QEventLoop eventLoop;
    connect(manager,SIGNAL(finished(QNetworkReply *)),&eventLoop,SLOT(quit()));
    eventLoop.exec();			// block until finish
    responseData = reply->readAll();


    Data=QString(responseData);

    QStringList temp1=Data.split("<h2><a href=\"");
    for(int i=1;i<temp1.length();i++)
        temp1[i]=temp1[i].left(temp1[i].indexOf("\" target=\"_blank\">"));//截取网页地址

    for(int i=1;i<temp1.length();i++){
        bool have=false;
        for(int j=0;j<Uploaded.length();j++){
            if(Uploaded[j]==temp1[i]){
                have=true;
                break;
                }

        }

        if(!have){
            URLList.append(temp1[i]);
            ui->LastList->addItem(temp1[i]);
            Uploaded.append(temp1[i]);
            ui->UploadList->addItem(temp1[i]);

        }
    }



    getContent();

}

void Widget::getTitle()
{

    QStringList temp2=Data.split("<h2><a href=\"");
    for(int i=1;i<temp2.length();i++)
        temp2[i]=temp2[i].mid(temp2[i].indexOf("\"_blank\">")+9,temp2[i].indexOf("</a></h2>")-9-temp2[i].indexOf("\"_blank\">"));//截取标题

    for(int i=1;i<temp2.length();i++)
    TitleList.append(temp2[i]);

}

void Widget::getPhoto()
{
    for(int i=0;i<URLList.length();i++){

        QString tempData=ContentList[i];

        if(tempData.indexOf("src=\"")>0)
        PhotoList.append(tempData.mid(tempData.indexOf("src=\"")+5,tempData.indexOf("\" alt=")-5-tempData.indexOf("src=\"")));
        else
        PhotoList.append("http://119.29.15.43/projectimage/icon.png");
    }

}

void Widget::getContent()
{

    for(int i=0;i<URLList.length();i++){


        reply = manager->get(QNetworkRequest(QUrl(URLList[i])));
        QByteArray responseData;
        QEventLoop eventLoop;
        connect(manager,SIGNAL(finished(QNetworkReply *)),&eventLoop,SLOT(quit()));
        eventLoop.exec();			// block until finish
        responseData = reply->readAll();


        QString tempData=QString(responseData);
        QString temp;

        if(tempData.indexOf("<!-- 微博列表 -->")>0)
           temp=tempData.mid(tempData.indexOf("\"blk_content\">")+14,tempData.indexOf("<!-- 微博列表 -->")-14-tempData.indexOf("\"blk_content\">"));
        else
            temp=tempData.mid(tempData.indexOf("\"blk_content\">")+14,tempData.indexOf("<!-- health_web")-14-tempData.indexOf("\"blk_content\">"));

        temp.replace("<script type=\"text/javascript\">","");
       temp.replace("window.sina_survey_config = {","");
       temp.replace("surveyCss: true,","");
       temp.replace("//调查问答样式true, false, http （不使用默认样式配置false或者不传此参数）","");
       temp.replace("resultCss: true,","");
       temp.replace("source: 'vote'","");
       temp.replace("//通过来源设置图片宽高 sina(手浪), vote(默认)","");
       temp.replace("//调查结果样式true, false, http （不使用默认样式配置false或者不传此参数）","");
       temp.replace("}","");
       temp.replace("</script>","");
        qDebug()<<temp;
        ContentList.append(temp);

        QString timedata=tempData.mid(tempData.indexOf("published at ")+13,tempData.indexOf(" ] -->")-13-tempData.indexOf("published at "));
        TimeList.append(timedata);

        QStringList sourcedata=tempData.split("<!--");
        SourceList.append(sourcedata[2].left(sourcedata[2].indexOf("-->")));

    }

    getPhoto();

    getTitle();

    upload();
}



void Widget::upload()
{
    for(int i=0;i<URLList.length();i++){
    QSqlQuery query;
    query.prepare("insert into news (title,coverphoto,content,source,posttime,likecount,dislikecount) values(?,?,?,?,?,?,?)");
    query.addBindValue(TitleList[i]);
    query.addBindValue(PhotoList[i]);
    query.addBindValue(ContentList[i]);
    query.addBindValue(SourceList[i]);
    query.addBindValue(TimeList[i]);
    query.addBindValue(qrand()%13);
    query.addBindValue(qrand()%3);
    query.exec();
    }

}

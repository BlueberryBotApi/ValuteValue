#include "currency.h"
#include "networkmanagerdemowidget.h"

Currency::Currency(QObject  *parent)
    : QObject {parent}
{
    this->codesOfCurrensy = QMap <QString,QString>();
    this->networkManager = new QNetworkAccessManager();
    connect( this->networkManager, SIGNAL(finished(QNetworkReply*)),SLOT( onFinished(QNetworkReply*)) );

}
Currency::~Currency()
{
    delete networkManager;
}

QMap<QString,QString> Currency::onFinished(QNetworkReply *reply) {
    ///this->isFinished = false;

    if( reply->error() != QNetworkReply::NoError )
    {
        qDebug()<< reply->errorString() ;
        reply->deleteLater();
    }

    QByteArray dataXml = (reply->readAll());///////////////////меняем кодировку
    QTextCodec *codec_1251 = QTextCodec::codecForName("cp-1251");
    QString string = codec_1251->toUnicode(dataXml);

    int i=0,j=0;
    QString name;
    QString buf;
    while(i!=-1)
    {
        buf="";
        i = string.indexOf("Name",j);
        if(i!=-1)
        {
            i+=5;
            while(string[i]!='<')
            {
                buf+=string[i];
                i++;
            }
            j=i;
            name = buf;
            buf="";
            i = string.indexOf("ParentCode",j);
            i+=11;
            while(string[i]!=' ')
            {
                buf+=string[i];
                i++;
            }
            this->codesOfCurrensy[name]=buf;
            j=i;
        }
    }

    reply->deleteLater();
   // this->isFinished = true;
    emit QMapIsReady(codesOfCurrensy);
    return codesOfCurrensy;
}

void Currency::takeXSDScheme()
{
    networkManager->get(QNetworkRequest(this->link));
}

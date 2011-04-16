

#include "qdownloader.h"
#include <QDebug>
#include <QTemporaryFile>

QDownloader::QDownloader(QNetworkAccessManager* _manager, QUrl _url, QFile* _file) {
    manager = _manager;
    url = _url;
    file = _file;
}

void QDownloader::downloadComplete(){
    file->close();
    emit finished();
}

void QDownloader::dataReady(){
    qDebug() << "Data ready";
    file->write(reply->readAll());
    
}

void QDownloader::slotError(QNetworkReply::NetworkError error){
    qDebug() << "Error " << error;
}

void QDownloader::beginDownload(){
    
    qDebug() << file->fileName();
    QNetworkRequest request;
    request.setUrl(url);
   
    if ( file->open(QIODevice::WriteOnly) ) {
        qDebug() << file->fileName();
        reply = manager->get( request );
    
        connect(reply, SIGNAL(readyRead()), this, SLOT(dataReady()));
        connect(reply, SIGNAL(finished()), this, SLOT(downloadComplete()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
        
    } else {
        qDebug() << "error opening file";
    }
    /*QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgressSlot(qint64, qint64)));
     */
}

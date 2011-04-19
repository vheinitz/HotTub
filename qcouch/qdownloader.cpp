

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
    file->write(reply->readAll());    
}


void QDownloader::beginDownload(){
    
    QNetworkRequest request;
    request.setUrl(url);
   
    if ( file->open(QIODevice::WriteOnly) ) {
        reply = manager->get( request );
    
        connect(reply, SIGNAL(readyRead()), this, SLOT(dataReady()));
        connect(reply, SIGNAL(finished()), this, SLOT(downloadComplete()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(downloadError(QNetworkReply::NetworkError)));
    }
}

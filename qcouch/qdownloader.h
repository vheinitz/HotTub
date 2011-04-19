

#ifndef __QDOWNLOADMANAGER_H
#define __QDOWNLOADMANAGER_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class QDownloader : public QObject {
    Q_OBJECT
    
public:
    QDownloader(QNetworkAccessManager* manager, QUrl url, QFile* file);
    
    void beginDownload();
private slots:
    void dataReady();
    void downloadComplete();
signals:
    void finished();
    void downloadError(QNetworkReply::NetworkError);

    
private:
    QNetworkAccessManager* manager;
    QNetworkReply* reply;
    QFile *file;
    QUrl url;
};




#endif

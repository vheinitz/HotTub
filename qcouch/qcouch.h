

#ifndef __QCOUCH_H
#define __QCOUCH_H

#include "document.h"
#include <QNetworkAccessManager>
#include <QStringList>
#include <QSignalMapper>
#include <QFile>

#include "qjson/parser.h"
#include "qjson/serializer.h"


enum Method {
    GET,
    PUT,
    HEAD,
    DELETE
};



class TimeoutException : public std::exception {
    virtual const char* what() const throw() {
        return "Timeout while communicating with couch";
    }
};

class CouchException : public std::exception {
public:
    CouchException();
    CouchException(QString message);
    virtual const char* what() const throw();
    virtual ~CouchException() throw();

private:
    QString message;
    
    
};

class DocumentNotFoundException : public CouchException {
public:
    DocumentNotFoundException();
};


class QCouch : public QObject{
    Q_OBJECT 
    
public:
    QCouch();
    
    void connect(QString host, int port);
    
    QString getHost();
    void createDatabase(QString name);
    void deleteDatabase(QString name);
    
    QList<QString> listDatabases();
    QList<QVariant> listViews(QString database);
    
    QList<QVariant> getView(QString database, QString design, QString view, QVariant startkey=QVariant(), QVariant endkey=QVariant(), bool descending=false, int limit=25, bool includeDocs=false, QString startKeyDocId="");
    
    Document getDocument(QString database, QString id, QString revision="");
    
    Document createDocument(QString database, QString id, QVariant var);
    QString updateDocument(QString database, QString id, QString revision, QVariant var); 
    bool deleteDocument(QString database, QString id, QString revision);
    
    QString putAttachment(QString database, QString id, QString revision, QString name, QIODevice* source); 
    QString getAttachment(QString database, QString id, QString name, QFile* file);
    void removeAttachment(QString database, QString id, QString revision, QString name);
    
    QString getUUID();
    
private slots:
    void connectFinished();
    void error(QString);
    void uploadProgressSlot(qint64, qint64);
    void uploadCompleteSlot();
    void downloadProgressSlot(qint64, qint64);
    void downloadCompleteSlot();
signals:
    void connected();
    void downloadProgress(qint64, qint64);
    void uploadProgress(qint64, qint64);
    void uploadComplete();
    void downloadComplete();
    
private:
    void checkErrors(QVariant response);
    
    QNetworkReply* doRequest(Method, QString, const QByteArray);
    QNetworkReply* doGet(QString url);
    QNetworkReply* doPut(QString url, const QByteArray);
    QNetworkReply* doDelete(QString url);
    
    
    QString host;
    int port;
    
    QNetworkAccessManager *manager;
    QJson::Parser parser;
    QJson::Serializer serializer;
    
};


#endif

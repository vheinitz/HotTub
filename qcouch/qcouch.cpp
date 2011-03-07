

#include "qcouch.h"
#include "document.h"


#include <QString>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <QEventLoop>

#include "qjson/parser.h"
#include "qjson/serializer.h"


using namespace QJson;
using namespace std;


CouchException::CouchException(QString str) {
    message = str;
}

CouchException::~CouchException() throw() {
}

const char* CouchException::what() const throw() {
    return message.toStdString().c_str();
}


QCouch::QCouch(){
    
}

void QCouch::connect(QString _host, int _port){   

    host = _host;
    port = _port;
    
    manager = new QNetworkAccessManager(this);
    
    QUrl url = QUrl(host);
    url.setPort(port);
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(connectFinished()));
    
}

QString QCouch::getHost(){
    return host;
}


bool QCouch::hasErrors(QVariant var){
    QVariantMap map = var.toMap();
    
    if ( map.find("error") != map.end() ) {
        return true;
    } else {
        return false;
    }
}


void QCouch::connectFinished(){
    emit connected();
}

void QCouch::error(QString str) {
    Q_UNUSED(str);
}


QNetworkReply* QCouch::doRequest(Method method, QString _url, const QByteArray bytes){
    QUrl url = QUrl();
    url.setEncodedUrl(QString(host + _url).toUtf8());
    url.setPort(port);
    
    QTimer timeout;
    QEventLoop loop;
    
    timeout.setSingleShot(true);
    QNetworkRequest request;
    request.setUrl(url);
        
    QNetworkReply *reply; 
    
    switch (method) {
        case GET:
            reply = manager->get(request);
            break;
        case PUT:
            reply = manager->put(request, bytes); 
            break;
        case DELETE:
            reply = manager->deleteResource(request);
            break;
        default:
            reply = manager->get(request);
            break;
    }
    
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(&timeout, SIGNAL(timeout()), &loop, SLOT(quit()));
    
    timeout.start(5000);
    
    loop.exec();
    
    if ( timeout.isActive() ) {
        return reply;
    } else {
        throw new TimeoutException;
    }
}

QNetworkReply* QCouch::doPut(QString url, const QByteArray bytes){
    return doRequest(PUT, url, bytes);
}

QNetworkReply* QCouch::doGet(QString url){
    const QByteArray emptyBytes;
    return doRequest(GET, url, emptyBytes);
}

QNetworkReply* QCouch::doDelete(QString url){
    const QByteArray emptyBytes;
    return doRequest(DELETE, url, emptyBytes);
}


void QCouch::createDatabase(QString name){
    const QByteArray emptyBytes;
    QNetworkReply *reply = doPut("/" + name, emptyBytes);
    
    QVariant var = parser.parse(reply->readAll());
    QVariantMap map = var.toMap();
    
    if ( map.find("error") != map.end() ) {
        throw CouchException(map["reason"].toString());
    }
}

void QCouch::deleteDatabase(QString name){
    doDelete("/" + name);
}


QList<QString> QCouch::listDatabases(){
    QNetworkReply *reply = doGet("/_all_dbs");
    QVariant var = parser.parse(reply->readAll());
    
    return var.toStringList();
    
}

QList<QVariant> QCouch::listViews(QString database){
    QString params = "startkey=\"_design/\"&endkey=\"_design0\"&include_docs=true";
    QString encodedParams = QUrl::toPercentEncoding(params, "=&", "/");
    
    QNetworkReply *reply = doGet("/" + database + "/_all_docs?" + encodedParams);
    QVariant var = parser.parse(reply->readAll());
    QVariantMap map = var.toMap();
    
    return map["rows"].toList();
}



QList<QVariant> QCouch::getView(QString database, QString design, QString view, QString startkey, QString endkey, int limit){
  
    QString params;

    if ( startkey.length() > 0 ) {
        params.append("startkey=\"");
        params.append(startkey);
        params.append("\"");
    }
    if ( endkey.length() > 0 ){ 
        if ( params.length() > 0 ) {
            params.append("&");
        }
        params.append("endkey=\"");
        params.append(endkey );
        params.append("\"");
    }
    
    if ( params.length() > 0 ){
        params.append("&");
    }
    params.append("limit=");
    params.append(QString::number(limit));
    
    
    qDebug() << params;
    QString encodedParams = QUrl::toPercentEncoding(params, "=&", "/");
    
    QNetworkReply *reply = doGet("/" + database + "/" + design + "/_view/" + view + "?" + encodedParams);
    
    QByteArray bytes = reply->readAll();
    QVariant var = parser.parse(bytes);
    QVariantMap map = var.toMap();
    
    QList<QVariant> results;
    
    QList<QVariant> rows = map["rows"].toList();
    foreach(QVariant row, rows){
        results.append(row);
    }
    
    return results;
    
}


Document QCouch::getDocument(QString database, QString id, QString revision){
    QString url = "/" + database + "/" + id;
    if (revision.length() > 0 ){
        url.append("?rev="+revision);
    }
    
    QNetworkReply *reply = doGet(url);
    QByteArray results = reply->readAll();
    qDebug() << "Results from call to getDocument " << results;
    QVariant var = parser.parse(results);
    
    return Document(database, var);
}


Document QCouch::createDocument(QString database, QString id, QVariant var){
    const QByteArray bytes = serializer.serialize(var);
    QNetworkReply * reply = doPut("/" + database + "/" + id, bytes);
    
    QVariant ret = parser.parse(reply->readAll());
    QVariantMap map = ret.toMap();
    
    Document doc;
    doc.setId(map["id"].toString());
    doc.setRevision(map["rev"].toString());
    
    return doc;
}

bool QCouch::updateDocument(QString database, QString id, QString revision, QVariant var){
    const QByteArray bytes = serializer.serialize(var);
    QNetworkReply *reply = doPut("/" + database + "/" + id + "?rev=" + revision, bytes);
    
    QVariant ret = reply->readAll();
    return hasErrors(ret);
}


bool QCouch::deleteDocument(QString database, QString id, QString revision){
    QNetworkReply *reply = doDelete("/" + database + "/" + id + "?rev=" + revision);

    QVariant var = parser.parse(reply->readAll());
    return hasErrors(var);
    
}

void QCouch::uploadCompleteSlot(){
    emit uploadComplete();
}


void QCouch::uploadProgressSlot(qint64 sent, qint64 total){
    emit uploadProgress(sent, total);
}

void QCouch::downloadProgressSlot(qint64 received, qint64 total){
    emit downloadProgress(received, total);
}

void QCouch::downloadCompleteSlot(){
    emit downloadComplete();
}

void QCouch::getAttachment(QString database, QString id, QString name){
    QNetworkRequest request;
    QUrl url;
    
    url.setUrl(host + "/" + database + "/" + id + "/" + name);
    url.setPort(port);
    
    request.setUrl(url);
    QNetworkReply *reply = manager->get( request );
    
    QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgressSlot(qint64, qint64)));
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(downloadCompleteSlot()));
}

void QCouch::putAttachment(QString database, QString id, QString revision, QString name, QIODevice* source) {
    QNetworkRequest request;
    QUrl url;
    
    QEventLoop loop;
    
    url.setUrl(host + "/" + database + "/" + id + "/" + name + "?rev=" + revision);
    url.setPort(port);
    request.setUrl(url);
    
    QNetworkReply *reply = manager->put( request, source );
    
    QObject::connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgressSlot(qint64, qint64)));
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(uploadCompleteSlot()));
    
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    
    loop.exec();
}



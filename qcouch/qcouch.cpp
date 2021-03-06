/*
 This file is part of Hot Tub, a project to help interactions with Apache CouchDB
 Copyright 2011 Matt Jennings <jennmat@gmail.com>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3.0 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "qcouch.h"
#include "document.h"
#include "qdownloader.h"


#include <QString>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QTemporaryFile>
#include <QDir>

#include "qjson/parser.h"
#include "qjson/serializer.h"


using namespace QJson;
using namespace std;


CouchException::CouchException() { }

CouchException::CouchException(QString str) {
    message = str;
}

CouchException::~CouchException() throw() {
}

const char* CouchException::what() const throw() {
    return message.toStdString().c_str();
}

DocumentNotFoundException::DocumentNotFoundException(){}


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


void QCouch::checkErrors(QVariant var){
    QVariantMap map = var.toMap();
    
    if ( map.find("error") != map.end() ) {
        throw CouchException(map["reason"].toString());
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



QList<QVariant> QCouch::getView(QString database, QString design, QString view, QVariant startkey, QVariant endkey, bool descending, int limit, bool includeDocs, QString startKeyDocId){
    QString params;

    if ( startkey.isValid() ) {
        params.append("startkey=");
        const QByteArray startKeyBytes = serializer.serialize(startkey);
        QString encodedStartKey = QUrl::toPercentEncoding(startKeyBytes);
        params.append(encodedStartKey);
    }
    if ( endkey.isValid() ){ 
        if ( params.length() > 0 ) {
            params.append("&");
        }
        params.append("endkey=");
        const QByteArray endKeyBytes = serializer.serialize(endkey);
        QString encodedEndKey = QUrl::toPercentEncoding(endKeyBytes);
        params.append(encodedEndKey);
    }
    
    if ( params.length() > 0 ){
        params.append("&");
    }
    params.append("limit=");
    params.append(QString::number(limit));

    if ( descending ) {
        params.append("&descending=true");
    }
    
    if ( startKeyDocId.length() > 0 ){
        params.append("&startkey_docid=");
        params.append(startKeyDocId);
    }

    if ( includeDocs ){
        params.append("&include_docs=true");
    }
    
    QList<QVariant> results;
    
    QNetworkReply *reply = doGet("/" + database + "/" + design + "/_view/" + view + "?" + params);
    
    QByteArray bytes = reply->readAll();
    QVariant var = parser.parse(bytes);
    QVariantMap map = var.toMap();
    
    
    QList<QVariant> rows = map["rows"].toList();
    foreach(QVariant row, rows){
        results.append(row);
    }

    return results;
}


Document QCouch::getDocument(QString database, QString id, QString revision){
    QString encodedId = QUrl::toPercentEncoding(id);
    QString url = "/" + database + "/" + encodedId;
    if (revision.length() > 0 ){
        url.append("?rev="+revision);
    }
    
    QNetworkReply *reply = doGet(url);
    if ( reply->error() == QNetworkReply::ContentNotFoundError ) {
        throw DocumentNotFoundException();
    }
    QByteArray results = reply->readAll();
    QVariant var = parser.parse(results);
    
    return Document(database, var);
}


Document QCouch::createDocument(QString database, QString id, QVariant var){
    const QByteArray bytes = serializer.serialize(var);
    QString encodedId = QUrl::toPercentEncoding(id);

    QNetworkReply * reply = doPut("/" + database + "/" + encodedId, bytes);
    
    QVariant ret = parser.parse(reply->readAll());
    QVariantMap map = ret.toMap();

    checkErrors(ret);
    
    Document doc;
    doc.setId(map["id"].toString());
    doc.setRevision(map["rev"].toString());

    
    return doc;
}

QString QCouch::updateDocument(QString database, QString id, QString revision, QVariant var){
    QString encodedId = QUrl::toPercentEncoding(id);
    QString url = "/" + database + "/" + encodedId;
    if ( revision.length() > 0 ) {
	QVariantMap map = var.toMap();
	map["_rev"] = QVariant(revision);
	var = QVariant(map);
    }
    const QByteArray bytes = serializer.serialize(var);
    QNetworkReply *reply = doPut(url, bytes);
    QVariant ret = parser.parse(reply->readAll());
    checkErrors(ret);

    QVariantMap map = ret.toMap();
    return map["rev"].toString();

}


bool QCouch::deleteDocument(QString database, QString id, QString revision){
    QString encodedId = QUrl::toPercentEncoding(id);
    QNetworkReply *reply = doDelete("/" + database + "/" + encodedId + "?rev=" + revision);

    QVariant var = parser.parse(reply->readAll());
    checkErrors(var);
    return true;
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

QString QCouch::getAttachment(QString database, QString id, QString name, QFile *file){
    QUrl url;
    QString encodedId = QUrl::toPercentEncoding(id);
    url.setUrl(host + "/" + database + "/" + encodedId + "/" + name);
    url.setPort(port);
    
    QEventLoop loop;
    QDownloader* downloader = new QDownloader(manager, url, file );
    QObject::connect(downloader, SIGNAL(finished()), &loop, SLOT(quit()));
    downloader->beginDownload();
    
    loop.exec();
    return file->fileName();
    
}

/*
  Returns the new revision
 */
QString QCouch::putAttachment(QString database, QString id, QString revision, QString name, QIODevice* source) {
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
    
    QVariant ret = parser.parse(reply->readAll());
    checkErrors(ret);
    
    QVariantMap map = ret.toMap();
    return map["rev"].toString();
    
}

void QCouch::removeAttachment(QString database, QString id, QString revision, QString name) {
    QString encodedId = QUrl::toPercentEncoding(id);
    QNetworkReply *reply = doDelete("/" + database + "/" + encodedId + "/" + name + "?rev=" + revision);
    
    QVariant var = parser.parse(reply->readAll());
    checkErrors(var);
}



QString QCouch::getUUID(){
    QNetworkReply *reply = doGet("/_uuids");
    
    QVariant var = parser.parse(reply->readAll());
    QVariantMap map = var.toMap();
    QList<QVariant> uuids = map["uuids"].toList();
    
    return uuids[0].toString();
}


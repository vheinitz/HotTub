

#include "document.h"
#include <QDebug>


Document::Document(){
}

Document::Document(QString _database, QVariant& var){
    map = var.toMap();
    id = map["_id"].toString();
    revision = map["_rev"].toString();
    database = _database;
}

QString Document::getId(){
    return id;
}

QString Document::getKey(){
    return key;
}

QString Document::getRevision(){
    return revision;
}

QVariantMap Document::getMap(){
    return map;
}

QList<QString> Document::getAttachmentIds(){
    QList<QString> results;
    if ( map.contains("_attachments") ){ 
        QVariant var = map["_attachments"];
        QVariantMap attachments = var.toMap();
        return attachments.keys();
    }
    return results;
}

void Document::setId(QString _id){
    id = _id;
}

void Document::setRevision(QString _revision){
    revision = _revision;
}

QString Document::getSourceDatabase(){
    return database;

}
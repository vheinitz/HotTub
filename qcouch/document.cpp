

#include "document.h"


Document::Document(){
}

Document::Document(QVariant& var){
    map = var.toMap();
    id = map["_id"].toString();
    revision = map["_rev"].toString();
    
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
    return results;
}

void Document::setId(QString _id){
    id = _id;
}

void Document::setRevision(QString _revision){
    revision = _revision;
}
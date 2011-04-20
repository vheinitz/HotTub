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

#include "document.h"
#include <QDebug>


Document::Document(){
}

Document::Document(QString _database, QVariant& var){
    map = var.toMap();
    id = map["_id"].toString();
    revision = map["_rev"].toString();
    if ( map.find("_attachments") != map.end() ) {
        attachments = map["_attachments"];
    }
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

QVariant Document::getAttachments(){
    return attachments;
}

void Document::setAttachments(QVariant _attachments){
    attachments = _attachments;
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
	map["_rev"] = revision;
}

QString Document::getSourceDatabase(){
    return database;

}

void Document::setValue(QString key, QVariant value){
    map[key] = value;
}

void Document::setSourceDatabase(QString _database){
    database = _database;

}

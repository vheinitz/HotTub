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

#ifndef __DOCUMENT_H
#define __DOCUMENT_H

#include <QString>
#include <QVariant>


class Document {
public:
    Document();
    Document(QString database, QVariant &);
    
    QString getId();
    QString getKey();
    QString getRevision();
    QVariant getAttachments();
    QString getSourceDatabase();
    QVariantMap getMap();
    
    QList<QString> getAttachmentIds();
   
    void setId(QString);
    void setRevision(QString);
    void setValue(QString key, QVariant value);
    void setAttachments(QVariant);
    void setSourceDatabase(QString);
private:
    QString id;
    QString key;
    QString revision;
    QString database;
    QVariantMap map;
    QVariant attachments;
    
};




#endif

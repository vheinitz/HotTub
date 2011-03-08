

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
    QString getSourceDatabase();
    QVariantMap getMap();
    
    QList<QString> getAttachmentIds();
   
    void setId(QString);
    void setRevision(QString);
    void setValue(QString key, QVariant value);
    
private:
    QString id;
    QString key;
    QString revision;
    QString database;
    QVariantMap map;
    
};




#endif

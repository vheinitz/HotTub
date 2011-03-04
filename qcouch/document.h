

#ifndef __DOCUMENT_H
#define __DOCUMENT_H

#include <QString>
#include <QVariant>


class Document {
public:
    Document();
    Document(QVariant &);
    
    QString getId();
    QString getKey();
    QString getRevision();
    QVariantMap getMap();
    
    QList<QString> getAttachmentIds();
   
    void setId(QString);
    void setRevision(QString);
    
private:
    QString id;
    QString key;
    QString revision;
    QVariantMap map;
    
};




#endif

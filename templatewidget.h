

#ifndef DRAGLABEL_H
#define DRAGLABEL_H

#include <Qt>
#include <QtGui>
#include "editor.h"
#include "couchdb/CouchDB.hpp"

using namespace CouchDB;

class TemplateWidget : public QWidget
{
    Q_OBJECT
	
public:
    TemplateWidget(Editor* editor, QWidget *parent);
    
    void setField(const QString &);
    void loadDocument(Document doc);
    
    
    void beginEditing();
    void stopEditing();
    void setCursor(Qt::CursorShape);   
    int getLeftAlignmentHint();
    int getTopAlignmentHint();
    void setLabel(const QString&);
    void paintEvent(QPaintEvent *event);
    bool allowResizeWidth();
    bool allowResizeHeight();
    void resizeEvent(QResizeEvent *event);
    QSize sizeHint();
    
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    
signals:
    void remove(TemplateWidget *);
    
private slots:
    void updateLabel(const QString& text);
    void showChangeEditorMenu();
    
    void changeEditorDate();
    void changeEditorTextarea();
    void changeEditorText();
    void changeEditorComboBox();
    
    void deleteWidget();
private:
    Editor *editor;
    QLabel *label;
    QLineEdit *labelEdit;
    QHBoxLayout *layout;
    QStackedLayout *stack;
    QToolBar *toolbar;
    QMenu *changeEditorMenu;
    bool isEditing;
    
    static const int MARGIN=5;
};



#endif

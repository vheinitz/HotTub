

#ifndef DRAGLABEL_H
#define DRAGLABEL_H

#include <Qt>
#include <QtGui>
#include "editor.h"

class TemplateWidget : public QWidget
{
    Q_OBJECT
	
public:
    TemplateWidget(Editor* editor, QWidget *parent);
     
    void beginEditing();
    void stopEditing();
    void setCursor(Qt::CursorShape);   
    int getLeftAlignmentHint();
    int getTopAlignmentHint();
    void setLabel(const QString&);
    void paintEvent(QPaintEvent *event);
    bool allowResizeWidth();
    bool allowResizeHeight();
    QSize sizeHint();
private slots:
    void updateLabel(const QString& text);
    
private:
    Editor *editor;
    QLabel *label;
    QLineEdit *labelEdit;
    QHBoxLayout *layout;
    QStackedLayout *stack;
    
    static const int MARGIN=50;
};



#endif

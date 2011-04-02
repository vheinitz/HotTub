

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
    
    void setField(const QString &);
    QString getField();
    void loadDocument(Document doc);
    void saveChanges(Document& doc);
    void reset();
    bool hasChanges();
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
    
    QString getEditorType();
    QVariant saveConfiguration();
    void loadConfiguration(QVariant&);
    
signals:
    void remove(TemplateWidget *);
    
private slots:
    void updateLabel(const QString& text);
    void showChangeEditorMenu();
    
    void changeEditorDate();
    void changeEditorTextarea();
    void changeEditorText();
    void changeEditorComboBox();
    void changeEditorList();
    void changeEditorGrid();
    
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
    QString field;
    static const int MARGIN=5;
};



#endif

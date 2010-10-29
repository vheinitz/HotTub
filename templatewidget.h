

 #ifndef DRAGLABEL_H
 #define DRAGLABEL_H

 #include <Qt>
 #include <QtGui>

 class TemplateWidget : public QWidget
 {
	
 public:
     TemplateWidget(QWidget *parent);
     
     void beginEditing();
     void stopEditing();
     void setCursor(Qt::CursorShape);   
     virtual void setActiveCursor(Qt::CursorShape) = 0;
     virtual int getLeftAlignmentHint() = 0;
     virtual int getTopAlignmentHint() = 0;
     virtual bool allowResizeWidth() = 0;
     virtual bool allowResizeHeight() = 0;

 protected:
     virtual void enterEditMode() = 0;
     virtual void exitEditMode() = 0;
     
     void setLabel(const QString&);
     void addWidget(QWidget *widget);
     void setLayout(QLayout *layout);
     
 private:
     
     QLabel *label;
     QHBoxLayout *layout;
 };



 #endif



 #ifndef DRAGLABEL_H
 #define DRAGLABEL_H

 #include <Qt>
 #include <QtGui>

 class TemplateWidget : public QWidget
 {
	
 public:
     TemplateWidget(QWidget *parent);
     
     virtual void enterEditMode() = 0;
     virtual void exitEditMode() = 0;
     
     void setCursor(Qt::CursorShape);   
     virtual void setActiveCursor(Qt::CursorShape) = 0;
 protected:
     void setLabel(const QString&);
     void addWidget(QWidget *widget);
     void setLayout(QLayout *layout);
     
 private:
     
     QLabel *label;
     QHBoxLayout *layout;
 };

 #endif

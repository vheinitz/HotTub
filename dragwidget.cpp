
 #include <QtGui>
 #include <QPen>
 #include <QPropertyAnimation>
 #include <iostream>

 #include "textedit.h"
 #include "dragwidget.h"


using namespace std;


 DragWidget::DragWidget(QWidget *parent)
     : QWidget(parent)
 {
     QFile dictionaryFile(":/dictionary/words.txt");
     dictionaryFile.open(QIODevice::ReadOnly);
     QTextStream inputStream(&dictionaryFile);

     int x = 5;
     int y = 5;

     while (!inputStream.atEnd()) {
         QString word;
         inputStream >> word;
         if (!word.isEmpty()) {
             TextEdit *edit = new TextEdit(word, this);
			 //wordLabel->setCursor(Qt::OpenHandCursor);
             edit->enterEditMode();
             edit->move(x, y);
             edit->show();
             edit->setAttribute(Qt::WA_DeleteOnClose);
             x += edit->width() + 2;
             if (x >= 195) {
                 x = 5;
                 y += edit->height() + 2;
             }
         }
     }

     QPalette newPalette = palette();
     newPalette.setColor(QPalette::Window, Qt::white);
     setPalette(newPalette);

     setAcceptDrops(true);
     setMinimumSize(800, qMax(600, y));
     setWindowTitle(tr("Draggable Text"));

	 cellWidth = 150;
	 cellHeight = 40;

	 cols = width()/cellWidth;
	 rows = height()/cellHeight;
	 int i=0;
	 int j=0;
	 grid = (QWidget ***)malloc(rows * sizeof(QWidget **));
	 for(i=0;i<rows;i++){
		grid[i] = (QWidget **)malloc(cols * sizeof(QWidget *));
		for(j=0; j<cols; j++){
			grid[i][j] = NULL;
		}
	 }

	 activeDragging = false;

	 activeCol = 0;
	 activeRow = 0;

	 timer = new QTimer(this);
	 connect(timer, SIGNAL(timeout()), this, SLOT(activeCellChanged()));
	 
 }


 void DragWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

	int i;
	QPainter painter(this);
	QPen pen;
	pen.setStyle(Qt::DotLine);
	pen.setColor(Qt::lightGray);
	painter.setPen(pen);

	for(i=0; i<width(); i+=cellWidth){
		painter.drawLine(i, 0, i, height());
	}

	for(i=0; i<height(); i+=cellHeight){
		painter.drawLine(0, i, width(), i);
	}
 }

void showMessage(QString message){
	QMessageBox msgBox;
	msgBox.setText(message);
	msgBox.exec();
}

 void DragWidget::mouseReleaseEvent(QMouseEvent *event)
 {
     if ( activeWidget != NULL ) {
     	activeWidget->setCursor(Qt::OpenHandCursor);
	 }
		 return;
	 activeCellChanged();

     QPoint position = event->pos();
	 int col = position.x()/cellWidth;
	 int row = position.y()/cellHeight;

     activeWidget->move(col * cellWidth, row * cellHeight);

	 grid[row][col] = activeWidget;

	 if ( activeWidget == grid[originRow][originCol] ) {
		grid[originRow][originCol] = NULL;
	 }

     activeDragging = false;
	 activeWidget = NULL;
	 timer->stop();
	 update();
 }

 void DragWidget::mousePressEvent(QMouseEvent *event)
 {
	 cout << "Mouse pressed " << endl;

     QWidget *child = childAt(event->pos());
	 if ( child == NULL ) return;

	 QWidget *parent = child->parentWidget();
	 while ( parent != this ) {
		child = parent;
		parent = parent->parentWidget();
	 }

	 TemplateWidget *templWidget = static_cast<TemplateWidget*>(child);
	 if ( templWidget ) {
	 	templWidget->setCursor(Qt::ClosedHandCursor);
	 }

	 int col = event->pos().x() / cellWidth;
	 int row = event->pos().y() / cellHeight;

	 originCol = col;
	 originRow = row;		 

	 offsetX = event->pos().x() - child->x();
	 offsetY = event->pos().y() - child->y();

	 activeWidget = templWidget;
	 activeWidget->raise();
	 activeDragging = true;
 }

void DragWidget::activeCellChanged(){

	if ( activeWidget == NULL ) return;

	int row = activeRow;
	int m = activeWidget->height() / cellHeight + 1;
	cout << activeWidget->height() << " " << cellHeight << " " << m << endl;
	int dir = 1;
	if ( lastActiveRow < activeRow ) {
		dir = -1;
	}
 	QAnimationGroup* animationGroup = new QParallelAnimationGroup();
	while ( grid[row][activeCol] != NULL && ( row != originRow || activeCol != originCol) ){ 
		QWidget* widget = grid[row][activeCol];
		QPropertyAnimation* animation = new QPropertyAnimation(widget, "geometry");
  		animation->setDuration(150);
 		animation->setEasingCurve(QEasingCurve::InOutQuart);
 		animation->setEndValue(QRect(widget->x(),widget->y()+(cellHeight*dir*m),widget->width(),widget->height()));

		animationGroup->addAnimation(animation);

		row = row + dir;
	}	


	if ( row != activeRow ){ 
 		animationGroup->start(QAbstractAnimation::DeleteWhenStopped);		
	
		while ( row != activeRow ) {
			grid[row][activeCol] = grid[row-(dir*m)][activeCol];
			row = row - dir;
		}
		grid[row][activeCol]=NULL;
	}

}

void DragWidget::mouseMoveEvent( QMouseEvent *event ) {

	if ( activeDragging ) {
		activeWidget->move(event->pos().x()-offsetX, event->pos().y()-offsetY);
	}

	lastActiveCol = activeCol;
	lastActiveRow = activeRow;
	QPoint pos = event->pos();
 	activeCol = pos.x() / cellWidth;
 	activeRow = pos.y() / cellHeight;


 	if ( (lastActiveCol != activeCol && originCol != activeCol) || (lastActiveRow != activeRow && originRow != activeRow) ) {
		if ( grid[activeRow][activeCol] != NULL ){ 
			timer->start(150);
		}
 	}

	update();

}



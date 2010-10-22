
 #include <QtGui>
 #include <QPen>
 #include <QPropertyAnimation>
 #include <iostream>

 #include "draglabel.h"
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
             DragLabel *wordLabel = new DragLabel(word, this);
             wordLabel->move(x, y);
             wordLabel->show();
             wordLabel->setAttribute(Qt::WA_DeleteOnClose);
             x += wordLabel->width() + 2;
             if (x >= 195) {
                 x = 5;
                 y += wordLabel->height() + 2;
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

 void DragWidget::dragEnterEvent(QDragEnterEvent *event)
 {
     if (event->mimeData()->hasText()) {
         if (children().contains(event->source())) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else {
         event->ignore();
     }
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

 void DragWidget::dropEvent(QDropEvent *event)
 {
	 activeCellChanged();

     QPoint position = event->pos();
	 int col = position.x()/cellWidth;
	 int row = position.y()/cellHeight;

     if ( event->mimeData()->hasText()) {
         const QMimeData *mime = event->mimeData();
         QStringList pieces = mime->text().split(QRegExp("\\s+"),
                              QString::SkipEmptyParts);
         QPoint hotSpot;

         QList<QByteArray> hotSpotPos = mime->data("application/x-hotspot").split(' ');
         if (hotSpotPos.size() == 2) {
             hotSpot.setX(hotSpotPos[0].toInt());
             hotSpot.setY(hotSpotPos[1].toInt());
			 if ( activeWidget == grid[originRow][originCol] ) {
			 	grid[originRow][originCol] = NULL;
			 }
         }

         foreach (QString piece, pieces) {
             DragLabel *newLabel = new DragLabel(piece, this);
			 position = position - hotSpot;
	

			 position.setX(col * cellWidth );
			 position.setY(row * cellHeight );
             newLabel->move(position);
             newLabel->show();
             newLabel->setAttribute(Qt::WA_DeleteOnClose);

             position += QPoint(newLabel->width(), 0);

			 grid[row][col] = newLabel;

         }

         if (event->source() == this) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else {
         event->ignore();
     }
     foreach (QObject *child, children()) {
         if (child->inherits("QWidget")) {
             QWidget *widget = static_cast<QWidget *>(child);
             if (!widget->isVisible())
                 widget->deleteLater();
         }
     }

     activeDragging = false;
	 activeWidget = NULL;
	 timer->stop();
	 update();
 }

 void DragWidget::mousePressEvent(QMouseEvent *event)
 {

     DragLabel *child = static_cast<DragLabel*>(childAt(event->pos()));
     if (!child)
         return;

     QPoint hotSpot = event->pos() - child->pos();

	 int col = event->pos().x() / cellWidth;
	 int row = event->pos().y() / cellHeight;

	 originCol = col;
	 originRow = row;		 

     QMimeData *mimeData = new QMimeData;
     mimeData->setText(child->text());
     mimeData->setData("application/x-hotspot",
                       QByteArray::number(hotSpot.x())
                       + " " + QByteArray::number(hotSpot.y()));

     QPixmap pixmap(child->size());
     child->render(&pixmap);

     QDrag *drag = new QDrag(this);
     drag->setMimeData(mimeData);
     drag->setPixmap(pixmap);
     drag->setHotSpot(hotSpot);

	 activeWidget = child;

     Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

     if (dropAction == Qt::MoveAction)
         child->close();

	 activeDragging = true;

 }

void DragWidget::activeCellChanged(){
	
	int row = activeRow;
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
 		animation->setEndValue(QRect(widget->x(),widget->y()+(cellHeight*dir),widget->width(),widget->height()));

		animationGroup->addAnimation(animation);

		row = row + dir;
	}	


	if ( row != activeRow ){ 
		cout << "Starting animation" << endl;
 		animationGroup->start(QAbstractAnimation::DeleteWhenStopped);		
	
		while ( row != activeRow ) {
			grid[row][activeCol] = grid[row-dir][activeCol];
			row = row - dir;
		}
		grid[row][activeCol]=NULL;
	}

}

void DragWidget::dragMoveEvent( QDragMoveEvent *event ) {
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



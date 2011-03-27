

#ifndef __GRIDCONFIG_H
#define __GRIDCONFIG_H

#include <QtGui>
#include <QStringList>

class GridConfig : public QDialog {
	Q_OBJECT
public:
	GridConfig();
	void addColumn(QString);
private slots:
	void addClicked();
	void addAllClicked();
	void removeClicked();
	void removeAllClicked();
	void addDoubleClicked(QListWidgetItem * item);
	void removeDoubleClicked(QListWidgetItem * item);

	void okClicked();
	void cancelClicked();
private:
	QListWidget *allColumnsListWidget;
	QListWidget *currentColumnsListWidget;

	void addSelected();
	void removeSelected();

};

#endif

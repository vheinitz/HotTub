
#ifndef __LISTCONFIG_H
#define __LISTCONFIG_H

#include <QtGui>

class ViewConfig : public QDialog {
	Q_OBJECT
public:
	ViewConfig();
	void addColumn(QString text);
	void addCurrentColumn(QString text);
	QStringList getCurrentColumns();
private slots:
	void okClicked();
	void cancelClicked();
	void addDoubleClicked(QListWidgetItem * item);
	void removeDoubleClicked(QListWidgetItem * item);
	void addClicked();
	void addAllClicked();
	void removeClicked();
	void removeAllClicked();

private:
	void addSelected();
	void removeSelected();

	QListWidget *allColumnsListWidget;
	QListWidget *currentColumnsListWidget;
};


#endif

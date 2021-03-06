#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QTreeWidgetItem>
#include <exception>
#include <functional>
#include <sstream>
#include <iostream>
#include <list>
#include <map>
#include <assert.h>

#include "ui_umc_window.h"
#include "bencoding.h"

class UMCWindow : public QWidget
{
	Q_OBJECT

public:
	UMCWindow(QWidget *parent = Q_NULLPTR);
	~UMCWindow();
	BElement* PaserBElement(const QString& path);
	void CreateTreeWidgetItem(BElement* data_ptr, QTreeWidgetItem* root);
	void CreateTreeWidget(const QString& path);

public slots:
	void OpenFile();

private:
	Ui::umc_window ui;
};

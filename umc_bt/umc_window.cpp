#include "umc_window.h"

UMCWindow::UMCWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.btnOpen, &QPushButton::clicked, this, &UMCWindow::OpenFile);

	ui.treeWidget->setHeaderHidden(true);
}

UMCWindow::~UMCWindow()
{
}



BElement* UMCWindow::PaserBElement(const QString& path)
{
	BElement* root_data = NULL;
	BElement::Parse(path.toStdString().c_str(), &root_data);

	//解析失败，直接断言
	assert(root_data != NULL);

	return root_data;
}

void  UMCWindow::CreateTreeWidgetItem(BElement* data_ptr, QTreeWidgetItem* root)
{
	switch (data_ptr->m_eType)
	{
	case eElementType::e_elem_int:
	{
		BElementInt* int_ptr = (BElementInt*)data_ptr;
		root->addChild(new QTreeWidgetItem(QStringList(QString::number(int_ptr->m_n))));
		break;
	}
	case eElementType::e_elem_string:
	{
		BElementString* string_ptr = (BElementString*)data_ptr;
		root->addChild(new QTreeWidgetItem(QStringList(QString(string_ptr->m_str))));
		
		break;
	}
	case eElementType::e_elem_list:
	{
		std::list<BElement*> m_list = ((BElementList*)data_ptr)->m_list;
		std::list<BElement*>::iterator iter;
		QTreeWidgetItem* array = new QTreeWidgetItem();
		array->setText(0, "array");
		root->addChild(array);

		for (iter = m_list.begin(); iter != m_list.end(); iter++)
		{
			CreateTreeWidgetItem(*iter, array);
		}
		break;
	}
	case eElementType::e_elem_dict:
	{
		std::map<BElementString*, BElement*> m_map = ((BElementDict*)data_ptr)->m_dict;
		std::map<BElementString*, BElement*>::iterator iter;

		for (iter = m_map.begin(); iter != m_map.end(); iter++)
		{
			QTreeWidgetItem* subItem= new QTreeWidgetItem();

			subItem->setText(0, QString(iter->first->m_str));
			CreateTreeWidgetItem(iter->second, subItem);
			root->addChild(subItem);
		}
		break;
	}
	};

	return;
}

void UMCWindow::CreateTreeWidget(const QString& path)
{
	BElement* root_data = PaserBElement(path);

	ui.treeWidget->clear();

	QTreeWidgetItem* root = new QTreeWidgetItem();

	root->setText(0, "object");
	CreateTreeWidgetItem(root_data, root);

	ui.treeWidget->addTopLevelItem(root);

	delete root_data;
}

void UMCWindow::OpenFile()
{
	QFileDialog* dialog = new QFileDialog(this);
	dialog->setWindowTitle(QStringLiteral("请选择种子"));

	if (dialog->exec()) {
		QStringList paths = dialog->selectedFiles();

		if (paths.size() > 0) {
			QString filePath = paths[0];

			ui.editPath->setText(filePath);
			CreateTreeWidget(filePath);
		}
	}
}

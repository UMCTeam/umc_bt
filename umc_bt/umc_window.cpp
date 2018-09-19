#include "umc_window.h"

UMCWindow::UMCWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.btnOpen, &QPushButton::clicked, this, &UMCWindow::OpenFile);
	
}

UMCWindow::~UMCWindow()
{
}

BElement* UMCWindow::PaserBElement(const QString& path)
{
	ui.treeWidget->clear();

	QFileInfo fileInfo(path);
	BElement* metaInfo = NULL;

	BElement::Parse(path.toStdString().c_str(), &metaInfo);

	//解析失败，直接断言
	assert(metaInfo == NULL);

	std::function<void (const QString& key, const BElement*, QTreeWidgetItem*)> CreateMetaTree;
	CreateMetaTree = [=, &CreateMetaTree](const QString& key, const BElement* element, QTreeWidgetItem* root) -> void {

		switch (element->m_eType)
		{
		case eElementType::e_elem_int:
		{
			root->setText(0, key);
			break;
		}
		case eElementType::e_elem_string:
		{
			BElementString* b_string = (BElementString*)element;
			root->setText(0, QString::fromUtf8(b_string->m_str));
			break;
		}
		case eElementType::e_elem_list:
		{
			std::list<BElement*> m_list = ((BElementList*)element)->m_list;
			std::list<BElement*>::iterator iter;
			int index = 0;

			for (iter = m_list.begin(); iter != m_list.end(); iter++)
			{
				QTreeWidgetItem* subItem = new QTreeWidgetItem(root);
				CreateMetaTree(QString(index), *iter, subItem);
				
				index++;
			}
			break;
		}
		case eElementType::e_elem_dict:
		{
			std::map<BElementString*, BElement*> m_map = ((BElementDict*)element)->m_dict;
			std::map<BElementString*, BElement*>::iterator iter;

			for (iter = m_map.begin(); iter != m_map.end(); iter++)
			{
				const QString key = QString::fromUtf8(iter->first->m_str);
				QTreeWidgetItem* subItem = new QTreeWidgetItem(root, QStringList(key));

				CreateMetaTree(key, iter->second, subItem);
			}
			break;
		}
		};

		return;
	};


	QTreeWidgetItem* item = new QTreeWidgetItem();
	ui.treeWidget->addTopLevelItem(item);

	//开始解析
	CreateMetaTree(fileInfo.fileName(), metaInfo, item);

	return metaInfo;
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
			PaserBElement(filePath);
		}
	}
}

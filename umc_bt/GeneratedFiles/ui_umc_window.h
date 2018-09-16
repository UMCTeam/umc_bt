/********************************************************************************
** Form generated from reading UI file 'umc_window.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UMC_WINDOW_H
#define UI_UMC_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_umc_window
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnOpen;
    QLineEdit *editPath;
    QTreeWidget *treeWidget;

    void setupUi(QWidget *umc_window)
    {
        if (umc_window->objectName().isEmpty())
            umc_window->setObjectName(QStringLiteral("umc_window"));
        umc_window->setWindowModality(Qt::ApplicationModal);
        umc_window->setEnabled(true);
        umc_window->resize(400, 296);
        verticalLayout = new QVBoxLayout(umc_window);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(umc_window);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnOpen = new QPushButton(frame);
        btnOpen->setObjectName(QStringLiteral("btnOpen"));

        horizontalLayout->addWidget(btnOpen);

        editPath = new QLineEdit(frame);
        editPath->setObjectName(QStringLiteral("editPath"));

        horizontalLayout->addWidget(editPath);


        verticalLayout->addWidget(frame);

        treeWidget = new QTreeWidget(umc_window);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));

        verticalLayout->addWidget(treeWidget);


        retranslateUi(umc_window);

        QMetaObject::connectSlotsByName(umc_window);
    } // setupUi

    void retranslateUi(QWidget *umc_window)
    {
        umc_window->setWindowTitle(QApplication::translate("umc_window", "umc_window", Q_NULLPTR));
        btnOpen->setText(QApplication::translate("umc_window", "\346\211\223\345\274\200\347\247\215\345\255\220", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class umc_window: public Ui_umc_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UMC_WINDOW_H

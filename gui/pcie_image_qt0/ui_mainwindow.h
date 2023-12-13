/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QFrame *line;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QComboBox *comboBox_c2h;
    QPushButton *pushButton_c2h_start;
    QLineEdit *lineEdit_c2h;
    QLabel *label_4;
    QComboBox *comboBox_h2c;
    QPushButton *pushButton_h2c_start;
    QLineEdit *lineEdit_h2c;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_6;
    QLineEdit *lineEdit_ureg_addr;
    QLabel *label_7;
    QLineEdit *lineEdit_ureg_data;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pushButton_ureg_get;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButton_ureg_set;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEdit_reso;
    QLabel *label_8;
    QPushButton *pushButton_sel_dir;
    QPushButton *pushButton_play;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *horizontalSpacer_10;
    QLineEdit *lineEdit_satu;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *verticalSpacer_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1297, 688);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);


        horizontalLayout_2->addLayout(horizontalLayout);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_2->addWidget(line);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer_3 = new QSpacerItem(17, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 0, 2, 2);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 4, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 2);

        comboBox_c2h = new QComboBox(centralWidget);
        comboBox_c2h->setObjectName(QStringLiteral("comboBox_c2h"));

        gridLayout->addWidget(comboBox_c2h, 2, 2, 1, 1);

        pushButton_c2h_start = new QPushButton(centralWidget);
        pushButton_c2h_start->setObjectName(QStringLiteral("pushButton_c2h_start"));

        gridLayout->addWidget(pushButton_c2h_start, 2, 3, 1, 1);

        lineEdit_c2h = new QLineEdit(centralWidget);
        lineEdit_c2h->setObjectName(QStringLiteral("lineEdit_c2h"));

        gridLayout->addWidget(lineEdit_c2h, 2, 4, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 2);

        comboBox_h2c = new QComboBox(centralWidget);
        comboBox_h2c->setObjectName(QStringLiteral("comboBox_h2c"));

        gridLayout->addWidget(comboBox_h2c, 3, 2, 1, 1);

        pushButton_h2c_start = new QPushButton(centralWidget);
        pushButton_h2c_start->setObjectName(QStringLiteral("pushButton_h2c_start"));

        gridLayout->addWidget(pushButton_h2c_start, 3, 3, 1, 1);

        lineEdit_h2c = new QLineEdit(centralWidget);
        lineEdit_h2c->setObjectName(QStringLiteral("lineEdit_h2c"));

        gridLayout->addWidget(lineEdit_h2c, 3, 4, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 0, 0, 1, 2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 0, 3, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_6, 0, 4, 1, 1);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 1, 0, 1, 1);

        lineEdit_ureg_addr = new QLineEdit(centralWidget);
        lineEdit_ureg_addr->setObjectName(QStringLiteral("lineEdit_ureg_addr"));

        gridLayout_2->addWidget(lineEdit_ureg_addr, 1, 1, 1, 2);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_2->addWidget(label_7, 1, 3, 1, 1);

        lineEdit_ureg_data = new QLineEdit(centralWidget);
        lineEdit_ureg_data->setObjectName(QStringLiteral("lineEdit_ureg_data"));

        gridLayout_2->addWidget(lineEdit_ureg_data, 1, 4, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 2, 0, 1, 1);

        pushButton_ureg_get = new QPushButton(centralWidget);
        pushButton_ureg_get->setObjectName(QStringLiteral("pushButton_ureg_get"));

        gridLayout_2->addWidget(pushButton_ureg_get, 2, 1, 1, 2);

        horizontalSpacer_8 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_8, 2, 3, 1, 1);

        pushButton_ureg_set = new QPushButton(centralWidget);
        pushButton_ureg_set->setObjectName(QStringLiteral("pushButton_ureg_set"));

        gridLayout_2->addWidget(pushButton_ureg_set, 2, 4, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        lineEdit_reso = new QLineEdit(centralWidget);
        lineEdit_reso->setObjectName(QStringLiteral("lineEdit_reso"));
        lineEdit_reso->setReadOnly(true);

        gridLayout_3->addWidget(lineEdit_reso, 2, 0, 1, 2);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_3->addWidget(label_8, 0, 0, 1, 1);

        pushButton_sel_dir = new QPushButton(centralWidget);
        pushButton_sel_dir->setObjectName(QStringLiteral("pushButton_sel_dir"));

        gridLayout_3->addWidget(pushButton_sel_dir, 1, 0, 1, 2);

        pushButton_play = new QPushButton(centralWidget);
        pushButton_play->setObjectName(QStringLiteral("pushButton_play"));

        gridLayout_3->addWidget(pushButton_play, 1, 4, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_9, 0, 3, 1, 2);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_10, 1, 2, 1, 2);

        lineEdit_satu = new QLineEdit(centralWidget);
        lineEdit_satu->setObjectName(QStringLiteral("lineEdit_satu"));

        gridLayout_3->addWidget(lineEdit_satu, 2, 4, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_11, 2, 2, 1, 2);


        verticalLayout->addLayout(gridLayout_3);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 258, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalLayout_2->setStretch(0, 4);
        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(2, 1);
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(comboBox_c2h, SIGNAL(currentIndexChanged(int)), MainWindow, SLOT(combo_box_c2h_slot(int)));
        QObject::connect(comboBox_h2c, SIGNAL(currentIndexChanged(int)), MainWindow, SLOT(combo_box_h2c_slot(int)));
        QObject::connect(pushButton_c2h_start, SIGNAL(clicked()), MainWindow, SLOT(push_button_c2h_start_slot()));
        QObject::connect(pushButton_h2c_start, SIGNAL(clicked()), MainWindow, SLOT(push_button_h2c_start_slot()));
        QObject::connect(pushButton_ureg_get, SIGNAL(clicked()), MainWindow, SLOT(push_button_ureg_get_slot()));
        QObject::connect(pushButton_ureg_set, SIGNAL(clicked()), MainWindow, SLOT(push_button_ureg_set_slot()));
        QObject::connect(pushButton_sel_dir, SIGNAL(clicked()), MainWindow, SLOT(push_button_sel_dir_slot()));
        QObject::connect(pushButton_play, SIGNAL(clicked()), MainWindow, SLOT(push_button_play_slot()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "pcie_image_qt", Q_NULLPTR));
        label->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Transfer Test", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "C2H", Q_NULLPTR));
        comboBox_c2h->clear();
        comboBox_c2h->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "8K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "32K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "64K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "128K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "256K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "512K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "1M", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4M", Q_NULLPTR)
         << QApplication::translate("MainWindow", "8M", Q_NULLPTR)
        );
        pushButton_c2h_start->setText(QApplication::translate("MainWindow", "START", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "H2C", Q_NULLPTR));
        comboBox_h2c->clear();
        comboBox_h2c->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "8K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "32K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "64K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "128K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "256K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "512K", Q_NULLPTR)
         << QApplication::translate("MainWindow", "1M", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4M", Q_NULLPTR)
         << QApplication::translate("MainWindow", "8M", Q_NULLPTR)
        );
        pushButton_h2c_start->setText(QApplication::translate("MainWindow", "START", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "User Reg", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "ADDR", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "DATA", Q_NULLPTR));
        pushButton_ureg_get->setText(QApplication::translate("MainWindow", "GET", Q_NULLPTR));
        pushButton_ureg_set->setText(QApplication::translate("MainWindow", "SET", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Display", Q_NULLPTR));
        pushButton_sel_dir->setText(QApplication::translate("MainWindow", "Select Pic", Q_NULLPTR));
        pushButton_play->setText(QApplication::translate("MainWindow", "PLAY", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

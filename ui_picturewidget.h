/********************************************************************************
** Form generated from reading UI file 'picturewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PICTUREWIDGET_H
#define UI_PICTUREWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "picturebutton.h"
#include "pictureview.h"

QT_BEGIN_NAMESPACE

class Ui_PictureWidget
{
public:
    QPushButton *btnL;
    QPushButton *btnR;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    pictureView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    pictureButton *btnPic0;
    pictureButton *btnPic1;
    pictureButton *btnPic2;
    pictureButton *btnPic3;
    pictureButton *btnPic4;
    pictureButton *btnPic5;
    pictureButton *btnPic6;
    pictureButton *btnPic7;
    pictureButton *btnPic8;
    pictureButton *btnPic9;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *PictureWidget)
    {
        if (PictureWidget->objectName().isEmpty())
            PictureWidget->setObjectName(QString::fromUtf8("PictureWidget"));
        PictureWidget->resize(780, 240);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PictureWidget->sizePolicy().hasHeightForWidth());
        PictureWidget->setSizePolicy(sizePolicy);
        btnL = new QPushButton(PictureWidget);
        btnL->setObjectName(QString::fromUtf8("btnL"));
        btnL->setGeometry(QRect(30, 90, 25, 25));
        btnR = new QPushButton(PictureWidget);
        btnR->setObjectName(QString::fromUtf8("btnR"));
        btnR->setGeometry(QRect(720, 90, 25, 25));
        layoutWidget = new QWidget(PictureWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(9, 9, 761, 225));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        graphicsView = new pictureView(layoutWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnPic0 = new pictureButton(layoutWidget);
        btnPic0->setObjectName(QString::fromUtf8("btnPic0"));

        horizontalLayout->addWidget(btnPic0);

        btnPic1 = new pictureButton(layoutWidget);
        btnPic1->setObjectName(QString::fromUtf8("btnPic1"));

        horizontalLayout->addWidget(btnPic1);

        btnPic2 = new pictureButton(layoutWidget);
        btnPic2->setObjectName(QString::fromUtf8("btnPic2"));

        horizontalLayout->addWidget(btnPic2);

        btnPic3 = new pictureButton(layoutWidget);
        btnPic3->setObjectName(QString::fromUtf8("btnPic3"));

        horizontalLayout->addWidget(btnPic3);

        btnPic4 = new pictureButton(layoutWidget);
        btnPic4->setObjectName(QString::fromUtf8("btnPic4"));

        horizontalLayout->addWidget(btnPic4);

        btnPic5 = new pictureButton(layoutWidget);
        btnPic5->setObjectName(QString::fromUtf8("btnPic5"));

        horizontalLayout->addWidget(btnPic5);

        btnPic6 = new pictureButton(layoutWidget);
        btnPic6->setObjectName(QString::fromUtf8("btnPic6"));

        horizontalLayout->addWidget(btnPic6);

        btnPic7 = new pictureButton(layoutWidget);
        btnPic7->setObjectName(QString::fromUtf8("btnPic7"));

        horizontalLayout->addWidget(btnPic7);

        btnPic8 = new pictureButton(layoutWidget);
        btnPic8->setObjectName(QString::fromUtf8("btnPic8"));

        horizontalLayout->addWidget(btnPic8);

        btnPic9 = new pictureButton(layoutWidget);
        btnPic9->setObjectName(QString::fromUtf8("btnPic9"));

        horizontalLayout->addWidget(btnPic9);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        layoutWidget->raise();
        btnR->raise();
        btnL->raise();

        retranslateUi(PictureWidget);

        QMetaObject::connectSlotsByName(PictureWidget);
    } // setupUi

    void retranslateUi(QWidget *PictureWidget)
    {
        PictureWidget->setWindowTitle(QCoreApplication::translate("PictureWidget", "PictureWidget", nullptr));
        btnL->setText(QString());
        btnR->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PictureWidget: public Ui_PictureWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PICTUREWIDGET_H

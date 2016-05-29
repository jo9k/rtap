/********************************************************************************
** Form generated from reading UI file 'myMainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYMAINWIDGET_H
#define UI_MYMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form_myMainWidget
{
public:
    QLabel *label_6;
    QLineEdit *lineEdit_ANGLE;
    QLineEdit *lineEdit_CFAC;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *Form_myMainWidget)
    {
        if (Form_myMainWidget->objectName().isEmpty())
            Form_myMainWidget->setObjectName(QStringLiteral("Form_myMainWidget"));
        Form_myMainWidget->resize(1140, 1116);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Form_myMainWidget->sizePolicy().hasHeightForWidth());
        Form_myMainWidget->setSizePolicy(sizePolicy);
        Form_myMainWidget->setMinimumSize(QSize(1140, 1116));
        label_6 = new QLabel(Form_myMainWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 10, 1121, 34));
        QFont font;
        font.setPointSize(22);
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignCenter);
        lineEdit_ANGLE = new QLineEdit(Form_myMainWidget);
        lineEdit_ANGLE->setObjectName(QStringLiteral("lineEdit_ANGLE"));
        lineEdit_ANGLE->setGeometry(QRect(510, 80, 113, 20));
        lineEdit_CFAC = new QLineEdit(Form_myMainWidget);
        lineEdit_CFAC->setObjectName(QStringLiteral("lineEdit_CFAC"));
        lineEdit_CFAC->setGeometry(QRect(510, 120, 113, 20));
        label = new QLabel(Form_myMainWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(380, 80, 121, 20));
        label_2 = new QLabel(Form_myMainWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(380, 120, 91, 20));

        retranslateUi(Form_myMainWidget);
        QObject::connect(lineEdit_ANGLE, SIGNAL(editingFinished()), Form_myMainWidget, SLOT(onANGLE()));
        QObject::connect(lineEdit_CFAC, SIGNAL(editingFinished()), Form_myMainWidget, SLOT(onCFAC()));

        QMetaObject::connectSlotsByName(Form_myMainWidget);
    } // setupUi

    void retranslateUi(QWidget *Form_myMainWidget)
    {
        Form_myMainWidget->setWindowTitle(QApplication::translate("Form_myMainWidget", "Form", 0));
        label_6->setText(QApplication::translate("Form_myMainWidget", "MY CENTRAL WIDGET", 0));
        label->setText(QApplication::translate("Form_myMainWidget", "Paramater ANGLE", 0));
        label_2->setText(QApplication::translate("Form_myMainWidget", "Parameter CFAC", 0));
    } // retranslateUi

};

namespace Ui {
    class Form_myMainWidget: public Ui_Form_myMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYMAINWIDGET_H

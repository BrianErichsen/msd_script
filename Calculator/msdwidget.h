#ifndef MSDWIDGET_H
#define MSDWIDGET_H

////////////////////////////////////////////////////////////////////////
// Author: Brian Erichsen Fagundes
// Date: 04/11/2024
// MSD - UofU - Spring semester
// CS 6015 Software Engineering
//
// expression-calculator widget for desktop applications
////////////////////////////////////////////////////////////////////////

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include "../Expression_Calculator/parse.h"
#include "../Expression_Calculator/env.h"
#include "../Expression_Calculator/val.h"

class msdwidget : public QWidget
{
    Q_OBJECT;
    QGridLayout* mainLayout;


    QLabel* expr_label;
    QTextEdit* expr_edit;
    QGroupBox* modeGroup;
    QVBoxLayout* modeLayout;
    QPushButton* file_btn;
    QRadioButton* interp_btn;
    QRadioButton* pretty_print_btn;
    QPushButton* submit_btn;
    QPushButton* reset_btn;
    QLabel* result_label;
    QTextEdit* result_edit;

public:
    explicit msdwidget(QWidget *parent = nullptr);
    void parse();
    void refresh();
    void loadFile();
};

#endif // MSDWIDGET_H

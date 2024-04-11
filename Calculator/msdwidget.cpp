#include "msdwidget.h"

msdwidget::msdwidget(QWidget *parent)
    : QWidget{parent}
{
    //creates main layour organized as a grid
    mainLayout = new QGridLayout;

    expr_label = new QLabel("Expression: ");
    expr_edit = new QTextEdit;
    //creates group of widgets to be added as the 3 different modes from the expr calculator
    modeGroup = new QGroupBox;
    modeLayout = new QVBoxLayout;//using vertical box in this case
    //creates all 3 buttons
    file_btn = new QPushButton("Input From File");
    interp_btn = new QRadioButton("Interp");
    pretty_print_btn = new QRadioButton("Pretty-Print");

    //addds proper widgets to mode layout
    modeLayout->addWidget(interp_btn);
    modeLayout->addWidget(pretty_print_btn);
    modeGroup->setLayout(modeLayout);
    submit_btn = new QPushButton("Submit");

    //adds to main layout proper widgets
    mainLayout->addWidget(expr_label, 0, 0);
    mainLayout->addWidget(expr_edit, 0, 1);
    mainLayout->addWidget(modeGroup, 1, 1);
    mainLayout->addWidget(file_btn, 2, 1);
    mainLayout->addWidget(submit_btn, 3, 1);

    //creates reset and result widgets and adds them to main layout
    result_label = new QLabel("Result: ");
    result_edit = new QTextEdit;
    reset_btn = new QPushButton("Refresh");

    mainLayout->addWidget(result_label, 4, 0);
    mainLayout->addWidget(result_edit, 4, 1);
    mainLayout->addWidget(reset_btn, 5, 1);
    //makes layout main one
    setLayout(mainLayout);

    //connects click from user signal to trigger methods to happen
    QObject::connect(submit_btn, &QPushButton::clicked, this, &msdwidget::parse);
    QObject::connect(reset_btn, &QPushButton::clicked, this, &msdwidget::refresh);
    QObject::connect(file_btn, &QPushButton::clicked, this, &msdwidget::loadFile);
    setWindowTitle("MSDScript");
}
//parses input from user input to compute proper result from expression
void msdwidget::parse() {
    //creates a expr ptr based on expr_edit input from user
    PTR(Expr) expr = parse_string(expr_edit->toPlainText().toUtf8().constData());
    std::string result;//creates space on the stack for variable result
    //checks which mode is chosen from the user interp or pretty string
    if (interp_btn->isChecked()) {
        result = expr->interp(Env::empty)->to_string();
    } else {
        result = expr->to_pretty_string();
    }
    result_edit->setText(QString::fromStdString(result));
}

//loads a chosen file if file exist
void msdwidget::loadFile() {
    QString filename = QFileDialog::getOpenFileName(this);
    QFile file(filename);
    if (!file.exists()) {
        qCritical() << "File Not Found!";
    }
    if (!file.open(QIODevice::OpenModeFlag::ReadOnly)) {
        qCritical() << "Could not open the file!";
        qCritical() << file.errorString();
        return;
    }
    QTextStream stream(&file);
    QString data = stream.readAll();
    expr_edit->setText(data);
    file.close();
}

//sets all fields to nothing
void msdwidget::refresh() {
    expr_edit->clear();
    result_edit->clear();
    //creates instace of data that hold all radio type buttons and iterates over data bank
    QList<QRadioButton*> buttons = modeGroup->findChildren<QRadioButton*>();
    for (QRadioButton* button : buttons) {
        button->setAutoExclusive(false);
        button->setChecked(false);
        button->setAutoExclusive(true);
    }
}

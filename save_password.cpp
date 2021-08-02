#include "save_password.h"
#include "ui_save_password.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QMessageBox>

save_password::save_password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::save_password)
{
    ui->setupUi(this);
}

save_password::~save_password()
{
    delete ui;
}

void save_password::on_buttonBox_accepted()
{
    QString description = ui->Description->text();
    if(description==""){
        QMessageBox::information(this,tr("Save"),tr("Enter description"));
    }
    else
    {
        QSqlQuery InsertQuery;
        InsertQuery.prepare("INSERT INTO SavedPasswords (Password, Description) VALUES ('"+MainWindow::passwordShuffle+"', '"+description+"')");
        if(InsertQuery.exec()){
            QMessageBox::information(this,tr("Save"),tr("Your password is saved to database."));
        }
        close();
    }
}

void save_password::on_buttonBox_rejected()
{

}


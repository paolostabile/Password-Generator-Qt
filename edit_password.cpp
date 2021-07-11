#include "edit_password.h"
#include "ui_edit_password.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

edit_password::edit_password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_password)
{
    ui->setupUi(this);
}

edit_password::~edit_password()
{
    delete ui;
}

void edit_password::on_buttonBox_replacepassword(QString str){
    ui->OldPassword->setText(str);
}

void edit_password::on_buttonBox_accepted()
{
    QString oldpass = ui->OldPassword->text();
    QString newpass = ui->NewPassword->text();
    QSqlQuery EditQuery;
    EditQuery.prepare("UPDATE SavedPasswords SET Password='"+newpass+"' WHERE Password='"+oldpass+"'");
    if(EditQuery.exec())
        while(EditQuery.next()){
            ui->OldPassword->setText(EditQuery.value(0).toString());
        }
    QMessageBox::information(this,tr("Edit"),tr("Your password is updated!"));
}

void edit_password::on_buttonBox_rejected()
{

}


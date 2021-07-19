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
    QString description = ui->Description->text();
    QSqlQuery EditQuery;
    if(newpass=="" & description!="")
    {
        EditQuery.prepare("UPDATE SavedPasswords SET Description='"+description+"' WHERE Password='"+oldpass+"'");
        if(EditQuery.exec())
            while(EditQuery.next())
            {
                ui->OldPassword->setText(EditQuery.value(0).toString());
            }
        QMessageBox::information(this,tr("Edit"),tr("Your description is updated!"));
        close();
    }
    else
    {
        if(newpass=="")
        {
            QMessageBox::information(this,tr("Edit"),tr("You didn't update your password!"));
            return;
        }
        if(description=="")
        {
            EditQuery.prepare("UPDATE SavedPasswords SET Password='"+newpass+"' WHERE Password='"+oldpass+"'");
            if(EditQuery.exec())
                while(EditQuery.next())
                {
                    ui->OldPassword->setText(EditQuery.value(0).toString());
                }
            QMessageBox::information(this,tr("Edit"),tr("Your password is updated!"));
            close();
        }
        else
        {
            EditQuery.prepare("UPDATE SavedPasswords SET Password='"+newpass+"', Description='"+description+"' WHERE Password='"+oldpass+"'");
            if(EditQuery.exec())
                while(EditQuery.next())
                {
                    ui->OldPassword->setText(EditQuery.value(0).toString());
                }
            QMessageBox::information(this,tr("Edit"),tr("Your password and description are updated!"));
            close();
        }
    }
}

void edit_password::on_buttonBox_rejected()
{

}


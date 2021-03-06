#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edit_password.h"
#include "save_password.h"
#include <QtSql>
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>

int MainWindow::passwordLength;
QString MainWindow::passwordShuffle;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ::MainWindow::passwordLength = ui->UpperCase_spinBox->value() +
            ui->LowerCase_spinBox->value() +
            ui->Numbers_spinBox->value() +
            ui->Symbols_spinBox->value();
    ::MainWindow::passwordShuffle = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void generatePassword(QSpinBox& spinBox, const std::string& collection, std::string& password)
{
    for (int i = 0; i < spinBox.value(); ++i)
        password += collection[rand() % collection.length()];
}

void MainWindow::checkPasswordStrength(QString& color, QString& strength)
{
    if (ui->LowerCase_checkBox->isChecked() && ui->UpperCase_checkBox->isChecked() && ui->Numbers_checkBox->isChecked() && ui->Symbols_checkBox->isChecked() && (passwordShuffle.length() >= 8))
    {
        color = "green";
        strength = "Strong";
    }
    else if ((ui->LowerCase_checkBox->isChecked() || ui->UpperCase_checkBox->isChecked()) && ui->Symbols_checkBox->isChecked() && (passwordShuffle.length() >= 6))
    {
        color = "blue";
        strength = "Moderate";
    }
    else
    {
        color = "red";
        strength = "Weak";
    }
}

void MainWindow::on_GeneratePassword_clicked()
{
    std::string password = "";
    generatePassword(*ui->UpperCase_spinBox, char_collection_upper, password);
    generatePassword(*ui->LowerCase_spinBox, char_collection_lower, password);
    generatePassword(*ui->Numbers_spinBox, numbers, password);
    generatePassword(*ui->Symbols_spinBox, symbols, password);
    random_shuffle(password.begin(), password.end());
    passwordShuffle = QString::fromStdString(password);
    ui->GeneratedPassword->setText(passwordShuffle);
    ui->GeneratedPassword_2->setText(passwordShuffle);
    QString color;
    QString strength;
    checkPasswordStrength(color, strength);
    ui->PasswordStrengthLabel->setStyleSheet("QLabel{color:" + color + ";}");
    ui->PasswordStrength->setStyleSheet("QLabel{color:" + color + ";}");
    ui->PasswordStrength->setText(strength + " password");
}

void MainWindow::on_CopyPassword_clicked()
{
    QClipboard *clip = QApplication::clipboard();
    clip->setText(passwordShuffle);
    QMessageBox::information(this,tr("Copy"),tr("Your password is copied to clipboard."));
}

void MainWindow::on_ClearPassword_clicked()
{
    ui->GeneratedPassword->clear();
    ui->GeneratedPassword_2->clear();
    ui->PasswordStrengthLabel->setStyleSheet(styleSheet());
    ui->PasswordStrength->setStyleSheet(styleSheet());
    ui->PasswordStrength->clear();
}

void MainWindow::on_OpenDatabase_clicked()
{
    QString FileName = QFileDialog::getOpenFileName(this,"Open Database","C://","*.sqlite");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(FileName);
    if(!db.open())
    {
        qDebug()<<"problem opening database";
    }
    ShowDatabaseTable();
}

void MainWindow::on_CreateDatabase_clicked()
{
    QString FileName = QFileDialog::getSaveFileName(NULL, "Create New Database","C://","*.sqlite");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(FileName);
    if(!db.open())
    {
        qDebug()<<"problem opening database";
    }
    QString query = "CREATE TABLE SavedPasswords(Password VARCHAR(40), Description VARCHAR(40));";
    QSqlQuery qry;
    if(!qry.exec(query))
    {
        qDebug()<<"error creating table";
    }
    qDebug()<<"end";
    ShowDatabaseTable();
}


void MainWindow::on_Save_clicked()
{
    if(ui->GeneratedPassword_2->text()!="")
    {
        save_password savePassword;
        savePassword.setModal(true);
        savePassword.exec();
        ShowDatabaseTable();
    }
    else
        QMessageBox::warning(this,tr("Error"),tr("You didn't generate password!"));
}

void MainWindow::on_Edit_clicked()
{
    if(ui->GeneratedPassword_2->text()!="")
    {
        QString oldPassword = ui->GeneratedPassword_2->text();
        edit_password editPassword;
        editPassword.setModal(true);
        editPassword.on_buttonBox_replacepassword(oldPassword);
        editPassword.exec();
        ShowDatabaseTable();
    }
    else
        QMessageBox::warning(this,tr("Error"),tr("You didn't select password!"));
}

void MainWindow::on_Delete_clicked()
{
    QString pass = ui->GeneratedPassword_2->text();
    if(pass!="")
    {
        QSqlQuery DeleteQuery;
        DeleteQuery.prepare("DELETE FROM SavedPasswords WHERE Password='"+pass+"'");
        if(DeleteQuery.exec()){
            QMessageBox::information(this,tr("Delete"),tr("Your password is deleted from database."));
            while(DeleteQuery.next())
                ui->GeneratedPassword_2->setText(DeleteQuery.value(0).toString());
        }
        ui->GeneratedPassword->clear();
        ui->GeneratedPassword_2->clear();
        ShowDatabaseTable();
    }
    else
        QMessageBox::warning(this,tr("Error"),tr("You didn't select password!"));
}

void MainWindow::ShowDatabaseTable()
{
    QSqlQueryModel*model = new QSqlQueryModel();
    QSqlQuery*qryShow = new QSqlQuery(db);
    qryShow->prepare("SELECT * FROM SavedPasswords");
    qryShow->exec();
    model->setQuery(*qryShow);
    ui->tableView->setModel(model);
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QString val = ui->tableView->model()->data(index).toString();
    QSqlQuery qryShow;
    qryShow.prepare("SELECT * FROM SavedPasswords WHERE Password='"+val+"' or Description='"+val+"'");
    if(qryShow.exec())
        while(qryShow.next()){
            ui->GeneratedPassword_2->setText(qryShow.value(0).toString());
        }
}

void MainWindow::on_UpperCase_checkBox_stateChanged(int arg1)
{
    ui->UpperCase_spinBox->setEnabled(arg1);
    ui->UpperCase_spinBox->setMinimum(arg1 ? 1 : 0);
    ui->UpperCase_spinBox->setValue(arg1 ? 2 : 0);
}

void MainWindow::on_Numbers_checkBox_stateChanged(int arg1)
{
    ui->Numbers_spinBox->setEnabled(arg1);
    ui->Numbers_spinBox->setMinimum(arg1 ? 1 : 0);
    ui->Numbers_spinBox->setValue(arg1 ? 2 : 0);
}

void MainWindow::on_Symbols_checkBox_stateChanged(int arg1)
{
    ui->Symbols_spinBox->setEnabled(arg1);
    ui->Symbols_spinBox->setMinimum(arg1 ? 1 : 0);
    ui->Symbols_spinBox->setValue(arg1 ? 2 : 0);
}

void MainWindow::spinBoxValueChanged()
{
    passwordLength = ui->UpperCase_spinBox->value()+ui->LowerCase_spinBox->value()+ui->Numbers_spinBox->value()+ui->Symbols_spinBox->value();
    ui->PasswordLength->setNum(passwordLength);
    ui->HorizontalSlider->setValue(passwordLength);
}

void MainWindow::on_UpperCase_spinBox_valueChanged()
{
    spinBoxValueChanged();
}

void MainWindow::on_LowerCase_spinBox_valueChanged()
{
    spinBoxValueChanged();
}

void MainWindow::on_Numbers_spinBox_valueChanged()
{
    spinBoxValueChanged();
}

void MainWindow::on_Symbols_spinBox_valueChanged()
{
    spinBoxValueChanged();
}

void MainWindow::on_ResetButton_clicked()
{
    ui->UpperCase_spinBox->setValue(2);
    ui->LowerCase_spinBox->setValue(2);
    ui->Numbers_spinBox->setValue(2);
    ui->Symbols_spinBox->setValue(2);
    ui->UpperCase_checkBox->setChecked(true);
    ui->LowerCase_checkBox->setChecked(true);
    ui->Numbers_checkBox->setChecked(true);
    ui->Symbols_checkBox->setChecked(true);
    ui->PasswordLength->setNum(8);
}

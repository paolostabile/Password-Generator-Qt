#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edit_password.h"
#include <QtSql>
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>
using namespace std;

int MainWindow::passwordLength;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ::MainWindow::passwordLength=ui->UpperCase_spinBox->value()+ui->LowerCase_spinBox->value()+ui->Numbers_spinBox->value()+ui->Symbols_spinBox->value();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_GeneratePassword_clicked()
{
    password = ""; passwordShuffle = "";
    srand(unsigned(time(0)));
    for (int i = 0; i < ui->UpperCase_spinBox->value(); ++i) {
        int randomInt = rand()%25;
        password += char_collection_upper[randomInt];
    }
    for (int i = 0; i < ui->LowerCase_spinBox->value(); ++i) {
        int randomInt = rand()%25;
        password += char_collection_lower[randomInt];
    }
    for (int i = 0; i < ui->Numbers_spinBox->value(); ++i) {
        int randomInt = rand()%10;
        password += numbers[randomInt];
    }
    for (int i = 0; i < ui->Symbols_spinBox->value(); ++i) {
        int randomInt = rand()%29;
        password += symbols[randomInt];
    }
    random_shuffle(password.begin(), password.end());
    passwordShuffle = QString::fromStdString(password);
    ui->GeneratedPassword->setText(passwordShuffle);
    ui->GeneratedPassword_2->setText(passwordShuffle);
    for (int i = 0;i < passwordShuffle.length(); ++i) {
        if(passwordShuffle[i].isUpper())
            hasUpper = true;
        if(passwordShuffle[i].isLower())
            hasLower = true;
        if(passwordShuffle[i].isDigit())
            hasDigit = true;
        if(!isalpha(password[i]) && !isdigit(password[i]))
            specialChar = true;
    }
    if (hasLower && hasUpper && hasDigit && specialChar && (passwordShuffle.length() >= 8))
    {
        ui->PasswordStrengthLabel->setStyleSheet("QLabel{color:green;}");
        ui->PasswordStrength->setStyleSheet("QLabel{color:green;}");
        ui->PasswordStrength->setText("Strong password");
    }
    else if ((hasLower || hasUpper) && specialChar && (passwordShuffle.length() >= 6))
    {
        ui->PasswordStrengthLabel->setStyleSheet("QLabel{color:blue;}");
        ui->PasswordStrength->setStyleSheet("QLabel{color:blue;}");
        ui->PasswordStrength->setText("Moderate password");
    }
    else
    {
        ui->PasswordStrengthLabel->setStyleSheet("QLabel{color:red;}");
        ui->PasswordStrength->setStyleSheet("QLabel{color:red;}");
        ui->PasswordStrength->setText("Weak password");
    }
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
    QString FileName = QFileDialog::getSaveFileName(NULL, "Create New File","C://","*.sqlite");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(FileName);
    if(!db.open())
    {
        qDebug()<<"problem opening database";
    }
    QString query = "CREATE TABLE SavedPasswords(Password VARCHAR(40));";
    QSqlQuery qry;
    if(!qry.exec(query))
    {
        qDebug()<<"error creating table";
    }
    qDebug()<<"end";
    ShowDatabaseTable();
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    QString val = ui->tableView->model()->data(index).toString();
    QSqlQuery qryShow;
    qryShow.prepare("SELECT * FROM SavedPasswords WHERE Password='"+val+"'");
    if(qryShow.exec())
        while(qryShow.next())
            ui->GeneratedPassword_2->setText(qryShow.value(0).toString());
}

void MainWindow::on_Save_clicked()
{
    QSqlQuery InsertQuery;
    InsertQuery.prepare("INSERT INTO SavedPasswords (Password) VALUES ('"+passwordShuffle+"')");
    if(InsertQuery.exec()){
        QMessageBox::information(this,tr("Save"),tr("Your password is saved to database."));
    }
    ShowDatabaseTable();
}

void MainWindow::on_Edit_clicked()
{
    QString oldPassword = ui->GeneratedPassword_2->text();
    edit_password editPassword;
    editPassword.setModal(true);
    editPassword.on_buttonBox_replacepassword(oldPassword);
    editPassword.exec();
    ui->GeneratedPassword->clear();
    ui->GeneratedPassword_2->clear();
    ShowDatabaseTable();
}

void MainWindow::on_Delete_clicked()
{
    QString pass=ui->GeneratedPassword_2->text();
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

void MainWindow::ShowDatabaseTable()
{
    QSqlQueryModel*model = new QSqlQueryModel();
    QSqlQuery*qryShow = new QSqlQuery(db);
    qryShow->prepare("SELECT * FROM SavedPasswords");
    qryShow->exec();
    model->setQuery(*qryShow);
    ui->tableView->setModel(model);
}

void MainWindow::on_UpperCase_checkBox_stateChanged(int arg1)
{

    if(ui->UpperCase_spinBox->isEnabled())
    {
        ui->UpperCase_spinBox->setEnabled(arg1);
        passwordLength -= ui->UpperCase_spinBox->value();
        ui->PasswordLength->setNum(passwordLength);
        ui->HorizontalSlider->setValue(passwordLength);
        ui->UpperCase_spinBox->setMinimum(0);
        ui->UpperCase_spinBox->setValue(0);
    }
    else
    {
        ui->UpperCase_spinBox->setEnabled(true);
        passwordLength += ui->UpperCase_spinBox->value();
        ui->PasswordLength->setNum(passwordLength);
        ui->HorizontalSlider->setValue(passwordLength);
        ui->UpperCase_spinBox->setMinimum(1);
        ui->UpperCase_spinBox->setValue(1);
    }
}

void MainWindow::on_LowerCase_checkBox_stateChanged(int arg1)
{
    if(ui->LowerCase_spinBox->isEnabled())
    {
        ui->LowerCase_spinBox->setEnabled(arg1);
        passwordLength -= ui->LowerCase_spinBox->value();
        ui->PasswordLength->setNum(passwordLength);
        ui->HorizontalSlider->setValue(passwordLength);
        ui->LowerCase_spinBox->setMinimum(0);
        ui->LowerCase_spinBox->setValue(0);
    }
    else
    {
        ui->LowerCase_spinBox->setEnabled(true);
        passwordLength += ui->LowerCase_spinBox->value();
        ui->PasswordLength->setNum(passwordLength);
        ui->HorizontalSlider->setValue(passwordLength);
        ui->LowerCase_spinBox->setMinimum(1);
        ui->LowerCase_spinBox->setValue(1);
    }
}

void MainWindow::on_Numbers_checkBox_stateChanged(int arg1)
{
    if(ui->Numbers_spinBox->isEnabled())
    {
        ui->Numbers_spinBox->setEnabled(arg1);
        passwordLength -= ui->Numbers_spinBox->value();
        ui->PasswordLength->setNum(passwordLength);
        ui->HorizontalSlider->setValue(passwordLength);
        ui->Numbers_spinBox->setMinimum(0);
        ui->Numbers_spinBox->setValue(0);
    }
    else
    {
        ui->Numbers_spinBox->setEnabled(true);
        passwordLength += ui->Numbers_spinBox->value();
        ui->PasswordLength->setNum(passwordLength);
        ui->HorizontalSlider->setValue(passwordLength);
        ui->Numbers_spinBox->setMinimum(1);
        ui->Numbers_spinBox->setValue(1);
    }
}

void MainWindow::on_Symbols_checkBox_stateChanged(int arg1)
{
    if(ui->Symbols_spinBox->isEnabled())
    {
        ui->Symbols_spinBox->setEnabled(arg1);
        passwordLength -= ui->Symbols_spinBox->value();
        ui->PasswordLength->setNum(passwordLength);
        ui->HorizontalSlider->setValue(passwordLength);
        ui->Symbols_spinBox->setMinimum(0);
        ui->Symbols_spinBox->setValue(0);
    }
    else
    {
        ui->Symbols_spinBox->setEnabled(true);
        passwordLength += ui->Symbols_spinBox->value();
        ui->PasswordLength->setNum(passwordLength);
        ui->HorizontalSlider->setValue(passwordLength);
        ui->Symbols_spinBox->setMinimum(1);
        ui->Symbols_spinBox->setValue(1);
    }
}

void MainWindow::on_UpperCase_spinBox_valueChanged()
{
    passwordLength = ui->UpperCase_spinBox->value()+ui->LowerCase_spinBox->value()+ui->Numbers_spinBox->value()+ui->Symbols_spinBox->value();
    ui->PasswordLength->setNum(passwordLength);
    ui->HorizontalSlider->setValue(passwordLength);
}

void MainWindow::on_LowerCase_spinBox_valueChanged()
{
    passwordLength = ui->UpperCase_spinBox->value()+ui->LowerCase_spinBox->value()+ui->Numbers_spinBox->value()+ui->Symbols_spinBox->value();
    ui->PasswordLength->setNum(passwordLength);
    ui->HorizontalSlider->setValue(passwordLength);
}

void MainWindow::on_Numbers_spinBox_valueChanged()
{
    passwordLength = ui->UpperCase_spinBox->value()+ui->LowerCase_spinBox->value()+ui->Numbers_spinBox->value()+ui->Symbols_spinBox->value();
    ui->PasswordLength->setNum(passwordLength);
    ui->HorizontalSlider->setValue(passwordLength);
}

void MainWindow::on_Symbols_spinBox_valueChanged()
{
    passwordLength = ui->UpperCase_spinBox->value()+ui->LowerCase_spinBox->value()+ui->Numbers_spinBox->value()+ui->Symbols_spinBox->value();
    ui->PasswordLength->setNum(passwordLength);
    ui->HorizontalSlider->setValue(passwordLength);
}

void MainWindow::on_ResetButton_clicked()
{
    ui->UpperCase_spinBox->setValue(1);
    ui->LowerCase_spinBox->setValue(1);
    ui->Numbers_spinBox->setValue(1);
    ui->Symbols_spinBox->setValue(1);
    ui->UpperCase_checkBox->setChecked(true);
    ui->LowerCase_checkBox->setChecked(true);
    ui->Numbers_checkBox->setChecked(true);
    ui->Symbols_checkBox->setChecked(true);
    ui->PasswordLength->setNum(4);
}

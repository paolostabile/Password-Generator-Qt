#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtSql>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string char_collection_upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string char_collection_lower = "abcdefghijklmnopqrstuvwxyz";
    std::string numbers = "0123456789";
    std::string symbols = "!#$%&'()*+,-./:;<=>?@[]^_{|}~";
    static QString passwordShuffle;
    static int passwordLength;
    QSqlDatabase db;
private slots:
    void on_GeneratePassword_clicked();
    void on_CopyPassword_clicked();
    void on_ClearPassword_clicked();
    void on_OpenDatabase_clicked();
    void on_CreateDatabase_clicked();
    void on_Save_clicked();
    void on_Edit_clicked();
    void on_Delete_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_UpperCase_checkBox_stateChanged(int arg1);
    void on_LowerCase_checkBox_stateChanged(int arg1);
    void on_Numbers_checkBox_stateChanged(int arg1);
    void on_Symbols_checkBox_stateChanged(int arg1);
    void on_UpperCase_spinBox_valueChanged();
    void on_LowerCase_spinBox_valueChanged();
    void on_Numbers_spinBox_valueChanged();
    void on_Symbols_spinBox_valueChanged();
    void on_ResetButton_clicked();
private:
    Ui::MainWindow *ui;
    void ShowDatabaseTable();
};
#endif // MAINWINDOW_H

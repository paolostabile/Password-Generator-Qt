#ifndef EDIT_PASSWORD_H
#define EDIT_PASSWORD_H
#include <QDialog>
namespace Ui {
class edit_password;
}
class edit_password : public QDialog
{
    Q_OBJECT
public:
    explicit edit_password(QWidget *parent = nullptr);
    ~edit_password();
    void on_buttonBox_replacepassword(QString str);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    Ui::edit_password *ui;
};
#endif // EDIT_PASSWORD_H

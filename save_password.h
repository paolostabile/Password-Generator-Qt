#ifndef SAVE_PASSWORD_H
#define SAVE_PASSWORD_H

#include <QDialog>

namespace Ui {
class save_password;
}

class save_password : public QDialog
{
    Q_OBJECT

public:
    explicit save_password(QWidget *parent = nullptr);
    ~save_password();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::save_password *ui;
};

#endif // SAVE_PASSWORD_H

#ifndef SETLEVEL_H
#define SETLEVEL_H

#include <QWidget>

namespace Ui {
class welcome;
}

class welcome: public QWidget
{
    Q_OBJECT

public:
    explicit welcome(QWidget *parent = 0);
    ~welcome();

private slots:
    void on_loadgame_clicked();

private:
    Ui::welcome *ui;
};

#endif // SETLEVEL_H

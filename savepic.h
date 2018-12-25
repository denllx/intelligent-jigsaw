#ifndef SAVEPIC_H
#define SAVEPIC_H

#include <QWidget>
#include <QLabel>
#include <QMovie>

namespace Ui {
class savepic;
}

class savepic : public QLabel
{
    Q_OBJECT

public:
    explicit savepic(QWidget *parent = 0);
    ~savepic();

signals:
    void savePicture(const QString& filename);

private:
    Ui::savepic *ui;
    QMovie* bg;
};

#endif // SAVEPIC_H

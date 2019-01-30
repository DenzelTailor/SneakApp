#ifndef DIALOG_DATA_H
#define DIALOG_DATA_H

#include "sneakeritem.h"

#include <QDialog>

namespace Ui {
class Dialog_Data;
}

class Dialog_Data : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Data(QWidget *parent = nullptr);
    ~Dialog_Data();

    SneakerItem getData() const { return m_sneaker; }
    void setData(const SneakerItem &sneaker);

private:
    Ui::Dialog_Data *ui;

    QImage m_image1;
    QImage m_image2;
    QString m_path;
    SneakerItem m_sneaker;

private slots:
    void on_buttonBox_accepted();
    void on_pushButton_BrowsePic1_clicked();
    void on_pushButton_BrowsePic2_clicked();
};

#endif // DIALOG_DATA_H

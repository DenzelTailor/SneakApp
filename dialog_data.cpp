#include "dialog_data.h"
#include "ui_dialog_data.h"

#include <QFileDialog>
#include <QStandardPaths>

Dialog_Data::Dialog_Data(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Data)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QDate today = QDate::currentDate();
    ui->dateEdit_Buydate->setDate(today);
    ui->dateEdit_Releasedate->setDate(today);

    m_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

Dialog_Data::~Dialog_Data()
{
    delete ui;
}

void Dialog_Data::setData(const QVariant &varData)
{
    QImage image1_box, image2_box;

    m_variant = varData;
    m_image1 = (varData.toList()[9]).value<QImage>();
    m_image2 = (varData.toList()[10]).value<QImage>();

    image1_box = m_image1.scaled(69,69, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    image2_box = m_image2.scaled(69,69, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->lineEdit_Brand->setText((varData.toList()[0]).toString());
    ui->lineEdit_Model->setText((varData.toList()[1]).toString());
    ui->lineEdit_CW->setText((varData.toList()[2]).toString());
    ui->lineEdit_Modelnr->setText((varData.toList()[3]).toString());
    ui->dateEdit_Releasedate->setDate((varData.toList()[4]).toDate());
    ui->dateEdit_Buydate->setDate((varData.toList()[5]).toDate());
    ui->doubleSpinBox_Price->setValue((varData.toList()[6]).toDouble());
    ui->lineEdit_Seller->setText((varData.toList()[7]).toString());
    ui->doubleSpinBox_Size->setValue((varData.toList()[8]).toDouble());
    ui->label_PreviewPic1->setPixmap(QPixmap::fromImage(image1_box));
    ui->label_PreviewPic2->setPixmap(QPixmap::fromImage(image2_box));
}

//---------------------------------------Slots---------------------------------------
void Dialog_Data::on_buttonBox_accepted()
{
    QVariant brand, model, cw, modelnr, releasedate, buydate, price, seller, shoesize, image1, image2;
    QList<QVariant> varList;

    brand = QVariant(ui->lineEdit_Brand->text());
    model = QVariant(ui->lineEdit_Model->text());
    cw = QVariant(ui->lineEdit_CW->text());
    modelnr = QVariant(ui->lineEdit_Modelnr->text());
    releasedate = QVariant(ui->dateEdit_Releasedate->date());
    buydate = QVariant(ui->dateEdit_Buydate->date());
    price = QVariant(ui->doubleSpinBox_Price->value());
    seller = QVariant(ui->lineEdit_Seller->text());
    shoesize = QVariant(ui->doubleSpinBox_Size->value());
    image1 = m_image1;
    image2 = m_image2;

    varList << brand << model << cw << modelnr << releasedate << buydate << price << seller << shoesize << image1 << image2;
    m_variant = QVariant(varList);

    m_image1 = QImage();
    m_image2 = QImage();
}

void Dialog_Data::on_pushButton_BrowsePic1_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Bilder (*.png *.jpg *.jpeg *.bmp)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Öffne Bilder"),
                                           m_path,
                                           tr("Bilder (*.png *.jpg *.jpeg *.bmp)"));
    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        QImage image_box = image.scaled(69,69, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_PreviewPic1->setPixmap(QPixmap::fromImage(image_box));

        m_image1 = image;
    }

    m_path = QFileInfo(fileName).path();
}

void Dialog_Data::on_pushButton_BrowsePic2_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Bilder (*.png *.jpg *.jpeg *.bmp)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Öffne Bilder"),
                                           m_path,
                                           tr("Bilder (*.png *.jpg *.jpeg *.bmp)"));
    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        QImage image_box = image.scaled(69,69, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_PreviewPic2->setPixmap(QPixmap::fromImage(image_box));

        m_image2 = image;
    }

    m_path = QFileInfo(fileName).path();
}

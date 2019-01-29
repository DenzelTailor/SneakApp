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

void Dialog_Data::setData(const SneakerItem &sneaker)
{
    QImage image1_box, image2_box;

    m_sneaker = sneaker;
    m_image1 = sneaker.getImage1();
    m_image2 = sneaker.getImage2();

    image1_box = m_image1.scaled(69,69, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    image2_box = m_image2.scaled(69,69, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->lineEdit_Brand->setText(sneaker.getBrand());
    ui->lineEdit_Model->setText(sneaker.getModel());
    ui->lineEdit_CW->setText(sneaker.getColorway());
    ui->lineEdit_Modelnr->setText(sneaker.getModelnr());
    ui->dateEdit_Releasedate->setDate(sneaker.getReleasedate());
    ui->dateEdit_Buydate->setDate(sneaker.getBuydate());
    ui->doubleSpinBox_Price->setValue(sneaker.getPrice());
    ui->lineEdit_Seller->setText(sneaker.getSeller());
    ui->doubleSpinBox_Size->setValue(sneaker.getSize());
    ui->label_PreviewPic1->setPixmap(QPixmap::fromImage(image1_box));
    ui->label_PreviewPic2->setPixmap(QPixmap::fromImage(image2_box));
}

//---------------------------------------Slots---------------------------------------
void Dialog_Data::on_buttonBox_accepted()
{
    QString brand;
    QString model;
    QString colorway;
    QString modelnr;
    QDate releasedate;
    QDate buydate;
    double price;
    QString seller;
    double size;
    QImage image1;
    QImage image2;

    brand = ui->lineEdit_Brand->text();
    model = ui->lineEdit_Model->text();
    colorway = ui->lineEdit_CW->text();
    modelnr = ui->lineEdit_Modelnr->text();
    releasedate = ui->dateEdit_Releasedate->date();
    buydate = ui->dateEdit_Buydate->date();
    price = ui->doubleSpinBox_Price->value();
    seller = ui->lineEdit_Seller->text();
    size = ui->doubleSpinBox_Size->value();
    image1 = m_image1;
    image2 = m_image2;

    m_sneaker = SneakerItem(brand, model, colorway, modelnr, releasedate, buydate, price, seller, size, image1, image2);

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

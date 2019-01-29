#include "mainwindow.h"

#include <QBrush>
#include <QCloseEvent>
#include <QDate>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsColorizeEffect>
#include <QGridLayout>
#include <QLabel>
#include <QLocale>
#include <QPainter>
#include <QPen>
#include <QProgressDialog>
#include <QScreen>
#include <QScrollBar>
#include <QSettings>
#include <QStandardPaths>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->action_BrandOrderAsc->setData("brandAsc");
    ui->action_BrandOrderDesc->setData("brandDesc");
    ui->action_BuyDateOrderAsc->setData("buyDateAsc");
    ui->action_BuyDateOrderDesc->setData("buyDateDesc");
    ui->action_PriceOrderAsc->setData("priceAsc");
    ui->action_PriceOrderDesc->setData("priceDesc");
    ui->action_ReleaseDateOrderAsc->setData("relDateAsc");
    ui->action_ReleaseDateOrderDesc->setData("relDateDesc");

    m_resizeTimer = new QTimer(this);
    m_resizeTimer->setSingleShot(true);
    connect(m_resizeTimer, SIGNAL(timeout()), this, SLOT(resize_timeout()));
    connect(ui->menu_ChangeOrder, SIGNAL(triggered(QAction *)), this, SLOT(action_SortSneakers_triggered(QAction *)));

    m_rubberband = nullptr;
    m_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    ui->scrollAreaWidgetContents->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->scrollAreaWidgetContents, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu_show(QPoint)));

    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::calcAvailSpace(const int leftMargin, const int rightMargin)
{
    return (this->geometry().width() - leftMargin - rightMargin - this->style()->pixelMetric(QStyle::PM_ScrollBarExtent));
}

bool MainWindow::brandLessThan(const SneakerItem &sneaker1, const SneakerItem &sneaker2)
{
    if (sneaker1.getBrand() == sneaker2.getBrand())
    {
        return sneaker1.getModel() < sneaker2.getModel();
    }
    else
    {
        return sneaker1.getBrand() < sneaker2.getBrand();
    }
}

bool MainWindow::buyDateLessThan(const SneakerItem &sneaker1, const SneakerItem &sneaker2)
{
    return sneaker1.getBuydate() < sneaker2.getBuydate();
}

bool MainWindow::priceLessThan(const SneakerItem &sneaker1, const SneakerItem &sneaker2)
{
    return sneaker1.getPrice() < sneaker2.getPrice();
}

bool MainWindow::relDateLessThan(const SneakerItem &sneaker1, const SneakerItem &sneaker2)
{
    return sneaker1.getReleasedate() < sneaker2.getReleasedate();
}

void MainWindow::addSneaker(const SneakerItem &sneaker)
{
    QImage imageClick;

    QString brand = sneaker.getBrand();
    QString model = sneaker.getModel();
    QImage image1 = sneaker.getImage1();

    m_sneakerList.append(sneaker);
    int sneakerIndex = m_sneakerList.size();

    if (image1.isNull())
    {
        QImage question = QImage(":/Images/question.png");
        imageClick = question.scaled(m_thumbSize, m_thumbSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        imageClick = image1.scaled(m_thumbSize, m_thumbSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QImage rounded(imageClick.width(), imageClick.height(), QImage::Format_ARGB32);
    rounded.fill(Qt::transparent);
    QBrush brush(imageClick);
    QPainter painter(&rounded);
    painter.setBrush(brush);
    painter.setPen(this->palette().color(QPalette::Background));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(0, 0, imageClick.width(), imageClick.height(), 2, 2);

    QClickLabel *clickLabel = new QClickLabel(this);
    clickLabel->setObjectName(QString("clickLabelObj %1").arg(m_sneakerList.size()));
    clickLabel->setMinimumSize(m_thumbSize,m_thumbSize);
    clickLabel->setMaximumSize(m_thumbSize,m_thumbSize);
    clickLabel->setPixmap(QPixmap::fromImage(rounded));
    clickLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    clickLabel->show();

    QLabel *brandLabel = new QLabel(brand, this);
    brandLabel->setObjectName(QString("brandLabelObj %1").arg(m_sneakerList.size()));
    QFont brandFont("Arial");
    brandFont.setPointSize(9);
    brandFont.setWeight(70);
    brandLabel->setFont(brandFont);
    QFontMetrics brandFm(brandFont);
    brandLabel->setMinimumSize(m_thumbSize, brandFm.height());
    brandLabel->setMaximumSize(brandLabel->minimumSize());
    brandLabel->show();

    QWidget *placeholderVert = new QWidget(this);
    placeholderVert->setMinimumSize(m_thumbSize,8);
    placeholderVert->setMaximumSize(placeholderVert->minimumSize());
    placeholderVert->show();

    QLabel *modelLabel = new QLabel(model, this);
    modelLabel->setObjectName(QString("modelLabelObj %1").arg(m_sneakerList.size()));
    QFont modelFont("Arial");
    modelFont.setPointSize(9);
    modelLabel->setFont(modelFont);
    QFontMetrics ModelFm(brandFont);
    modelLabel->setMinimumSize(m_thumbSize,ModelFm.height());
    modelLabel->setMaximumSize(modelLabel->minimumSize());
    modelLabel->show();

    if (m_column == m_maxColumns)
    {
        m_column = 0;
        m_row += 6;
    }

    ui->gridLayout_Scroll->addWidget(clickLabel,m_row,m_column);
    ui->gridLayout_Scroll->addWidget(placeholderVert, m_row+1, m_column);
    ui->gridLayout_Scroll->setRowMinimumHeight(m_row+1, 7);
    ui->gridLayout_Scroll->addWidget(brandLabel, m_row+2, m_column, Qt::AlignLeft);
    ui->gridLayout_Scroll->addWidget(placeholderVert, m_row+3, m_column);
    ui->gridLayout_Scroll->setRowMinimumHeight(m_row+3, 4);
    ui->gridLayout_Scroll->addWidget(modelLabel, m_row+4, m_column, Qt::AlignLeft);
    ui->gridLayout_Scroll->addWidget(placeholderVert, m_row+5, m_column);
    ui->gridLayout_Scroll->setRowMinimumHeight(m_row+5, 12);

    connect(clickLabel, &QClickLabel::clicked, [this, sneakerIndex]() {this->action_EditSneaker_triggered(sneakerIndex);});
    connect(clickLabel, &QClickLabel::clickedRight, [this, sneakerIndex]() {this->contextMenu_show(sneakerIndex);});

    if (m_column != m_maxColumns - 1)
    {
        QSpacerItem *spacerItemH = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Ignored);
        ui->gridLayout_Scroll->addItem(spacerItemH, m_row, m_column+1, -1, -1);
    }

    QSpacerItem *spacerItemV = new QSpacerItem(1,1, QSizePolicy::Ignored, QSizePolicy::Expanding);
    ui->gridLayout_Scroll->addItem(spacerItemV, m_row+6, m_column, -1, -1);

    m_column++;
}

void MainWindow::calcMaxColumns(int *calcSpace, const int availHSpace, const int minThumbSize, const int horiSpacing)
{
    while (*calcSpace <= availHSpace)
    {
        m_maxColumns++;
        *calcSpace = m_maxColumns * minThumbSize + (m_maxColumns - 1) * horiSpacing;
    }

    //eine Iteration zurueck
    *calcSpace = *calcSpace - (minThumbSize + horiSpacing);
    m_maxColumns--;
}

void MainWindow::calcThumbnailSize(int *calcSpace, const int availHSpace, const int maxThumbSize, const int horiSpacing)
{
    while (*calcSpace <= availHSpace)
    {
        m_thumbSize++;
        *calcSpace = m_maxColumns * m_thumbSize + (m_maxColumns - 1) * horiSpacing;
    }

    m_thumbSize--; //eine Iteration zurueck

    if (m_thumbSize > maxThumbSize)
    {
        m_thumbSize = maxThumbSize;
    }
}

void MainWindow::clearScrollGrid()
{
    QLayoutItem *item;

    while ((item = ui->gridLayout_Scroll->takeAt(0)))
    {
        if (item->widget())
        {
            delete item->widget();
        }
        delete item;
    }
}

void MainWindow::createDatabase()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/sneakers.sqlite");

    if (!db.open())
    {
        qDebug() << "error opening database";
    }

    QString query = "CREATE TABLE sneakertable ("
                    "ID integer,"
                    "Brand VARCHAR(100),"
                    "Model VARCHAR(100),"
                    "Colorway VARCHAR(100),"
                    "ModelNumber VARCHAR(100),"
                    "ReleaseDate DATETIME,"
                    "BuyingDate DATETIME,"
                    "Price double,"
                    "Size double,"
                    "Seller VARCHAR(100));";

    QSqlQuery qry;

    if (!qry.exec(query))
    {
        qDebug() << "error creating table";
    }

    updateDatabase();
    db.close();
}

void MainWindow::deleteItem(QList<int> deleteList)
{
    QList<SneakerItem> copyList = m_sneakerList;
    m_sneakerList.clear();

    for (int j = deleteList.size() - 1; j >= 0; j--)
    {
        copyList.removeAt(deleteList[j]);
    }

    clearScrollGrid();
    initScrollGrid();

    for (int i = 0; i < copyList.size(); i++)
    {
        addSneaker(copyList[i]);
    }
}

void MainWindow::editSneaker(const SneakerItem &sneaker, int index)
{
    QImage imageClick;

    QString brand = sneaker.getBrand();
    QString model = sneaker.getModel();
    QImage image1 = sneaker.getImage1();

    if (image1.isNull())
    {
        QImage question = QImage(":/Images/question.png");
        imageClick = question.scaled(m_thumbSize, m_thumbSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        imageClick = image1.scaled(m_thumbSize, m_thumbSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QImage rounded(imageClick.width(), imageClick.height(), QImage::Format_ARGB32);
    rounded.fill(Qt::transparent);
    QBrush brush(imageClick);
    QPainter painter(&rounded);
    painter.setBrush(brush);
    painter.setPen(this->palette().color(QPalette::Background));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(0, 0, imageClick.width(), imageClick.height(), 2, 2);

    m_sneakerList.replace(index-1, sneaker);

    QClickLabel *clickLabelOld = ui->scrollArea->findChild<QClickLabel *>(QString("clickLabelObj %1").arg(index));
    clickLabelOld->setPixmap(QPixmap::fromImage(rounded));

    QLabel *brandLabelOld = ui->scrollArea->findChild<QLabel *>(QString("brandLabelObj %1").arg(index));
    brandLabelOld->setText(brand);

    QLabel *modelLabelOld = ui->scrollArea->findChild<QLabel *>(QString("modelLabelObj %1").arg(index));
    modelLabelOld->setText(model);
}

void MainWindow::initScrollGrid()
{
    m_row = 0;
    m_column = 0;
    m_maxColumns = 1;

    ui->gridLayout->setContentsMargins(0,0,0,0);

    const int marginLeft = 40;
    const int marginRight = 40;
    const int marginTop = 40;
    const int marginBottom = 0;
    const int hSpacing = 24;
    const int vSpacing = 0;
    ui->gridLayout_Scroll->setContentsMargins(marginLeft,marginTop,marginRight,marginBottom);
    ui->gridLayout_Scroll->setHorizontalSpacing(hSpacing);
    ui->gridLayout_Scroll->setVerticalSpacing(vSpacing);

    const int thumbSize_min = 190;
    const int thumbSize_max = 220;
    m_thumbSize = thumbSize_min;

    //Fensterbreite minimal 3 Spalten
    int minWindowWidth = 3 * thumbSize_min + 2 * hSpacing + marginLeft + marginRight + this->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    this->setMinimumWidth(minWindowWidth);

    int layoutHSpace = 0;
    calcMaxColumns(&layoutHSpace, calcAvailSpace(marginLeft,marginRight), thumbSize_min, hSpacing);
    calcThumbnailSize(&layoutHSpace, calcAvailSpace(marginLeft,marginRight), thumbSize_max, hSpacing);
}

void MainWindow::loadSettings()
{
    QSettings windowConfig(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
    windowConfig.beginGroup("window");
    if (windowConfig.value("width").toInt() == 0 || windowConfig.value("height").toInt() == 0)
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();
        int windowHeight = int (screenGeometry.height()*0.75);
        int windowWidth = int (screenGeometry.width()*0.75);
        this->resize(windowWidth, windowHeight);
    }
    else
    {
        this->resize(windowConfig.value("width").toInt(), windowConfig.value("height").toInt());
        this->move(windowConfig.value("position").toPoint());
        if (windowConfig.value("maximized").toBool())
        {
            this->showMaximized();
        }
    }
    windowConfig.endGroup();

    m_sneakerList.clear();
    clearScrollGrid();
    initScrollGrid();

    qRegisterMetaTypeStreamOperators<SneakerItem>("SneakerItem");
    QSettings savedSneakers(QDir::currentPath() + "/sneakers.ini", QSettings::IniFormat);
    QList<SneakerItem> savedList = savedSneakers.value("sneakerlist").value<QList<SneakerItem>>();
    for (int i = 0; i < savedList.size(); i++)
    {
        addSneaker(savedList[i]);
    }

    createDatabase();
}

void MainWindow::saveSettings()
{
    QSettings windowConfig(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
    windowConfig.beginGroup("window");
    windowConfig.setValue("width", this->width());
    windowConfig.setValue("height", this->height());
    windowConfig.setValue("position", this->pos());
    windowConfig.setValue("maximized", this->isMaximized());
    windowConfig.endGroup();

    qRegisterMetaTypeStreamOperators<SneakerItem>("SneakerItem");
    QSettings savedSneakers(QDir::currentPath() + "/sneakers.ini", QSettings::IniFormat);
    savedSneakers.setValue("sneakerlist", QVariant::fromValue(m_sneakerList));
}

void MainWindow::updateDatabase()
{
    QSqlQuery qry;

    for (int i = 0; i < m_sneakerList.size(); i++)
    {
        qry.prepare("INSERT INTO sneakerTable ("
                    "ID,"
                    "Brand,"
                    "Model,"
                    "Colorway,"
                    "ModelNumber,"
                    "ReleaseDate,"
                    "BuyingDate,"
                    "Price,"
                    "Size,"
                    "Seller)"
                    "VALUES (?,?,?,?,?,?,?,?,?,?);");

        qry.addBindValue(i+1);
        qry.addBindValue(m_sneakerList[i].getBrand());
        qry.addBindValue(m_sneakerList[i].getModel());
        qry.addBindValue(m_sneakerList[i].getColorway());
        qry.addBindValue(m_sneakerList[i].getModelnr());
        qry.addBindValue(m_sneakerList[i].getReleasedate());
        qry.addBindValue(m_sneakerList[i].getBuydate());
        qry.addBindValue(m_sneakerList[i].getPrice());
        qry.addBindValue(m_sneakerList[i].getSize());
        qry.addBindValue(m_sneakerList[i].getSeller());

        if (!qry.exec())
        {
            qDebug() << "error adding values to db";
        }
    }
}

//---------------------------------------Events---------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QRect selectRec = QRect(m_mouseOrigin, (event->pos() + QPoint(0, ui->scrollArea->verticalScrollBar()->value() - this->menuBar()->height()))).normalized();
    m_rubberband->setGeometry(selectRec);

    QScrollBar *vertScrollBar = ui->scrollArea->verticalScrollBar();
    //vertScrollBar->setSingleStep(1);

    if (event->y() <= this->menuBar()->height())
    {
        vertScrollBar->setValue(vertScrollBar->value() - vertScrollBar->singleStep());
    }
    else if (event->y() >= height() - 1)
    {
        vertScrollBar->setValue(vertScrollBar->value() + vertScrollBar->singleStep());
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //Auswahlrechteck
    m_mouseOrigin = event->pos() + QPoint(0, ui->scrollArea->verticalScrollBar()->value() - this->menuBar()->height());

    if (!m_rubberband)
    {
        m_rubberband = new QRubberBand(QRubberBand::Rectangle, ui->scrollAreaWidgetContents);
    }

    m_rubberband->setGeometry(QRect(m_mouseOrigin, QSize()));
    m_rubberband->show();

    //Markierung von Widgets ggf. entfernen
    if (m_selectList.size() > 0)
    {
        QList<int>::iterator i;
        for (i = m_selectList.begin(); i != m_selectList.end(); i++)
        {
            QClickLabel *clicklabel = ui->scrollArea->findChild<QClickLabel *>(QString("clickLabelObj %1").arg(*i+1));
            clicklabel->setGraphicsEffect(nullptr);
        }

        m_selectList.clear();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_rubberband->hide();
    QRect selectRec = QRect(m_mouseOrigin, (event->pos() + QPoint(0, ui->scrollArea->verticalScrollBar()->value() - this->menuBar()->height()))).normalized();

    if (selectRec.width() > 1 && selectRec.height() > 1)
    {
        for (int i = 0; i < m_sneakerList.size(); i++)
        {
            QClickLabel *clicklabel = ui->scrollArea->findChild<QClickLabel *>(QString("clickLabelObj %1").arg(i+1));
            QPoint labelPos(clicklabel->pos());
            QRect labelRect(labelPos.x(), labelPos.y(), clicklabel->width(), clicklabel->height());
            QGraphicsColorizeEffect *selectColor = new QGraphicsColorizeEffect();
            selectColor->setColor(Qt::darkBlue);

            if (selectRec.intersects(labelRect))
            {
                m_selectList.append(i);
                clicklabel->setGraphicsEffect(selectColor);
            }
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    m_resizeTimer->stop();
    m_resizeTimer->start(100);
}

//---------------------------------------Slots---------------------------------------
void MainWindow::action_EditSneaker_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    int index = action->data().toInt();

    Dialog_Data dialogedit;

    SneakerItem sneaker = m_sneakerList[index-1];

    dialogedit.setModal(true);
    dialogedit.setData(sneaker);

    if (dialogedit.exec()==QDialog::Accepted)
    {
        if (sneaker != dialogedit.getData())
        {
            sneaker = dialogedit.getData();
            editSneaker(sneaker, index);
        }
    }
}

void MainWindow::action_EditSneaker_triggered(int index)
{
    Dialog_Data dialogedit;

    SneakerItem sneaker = m_sneakerList[index-1];

    dialogedit.setModal(true);
    dialogedit.setData(sneaker);

    if (dialogedit.exec()==QDialog::Accepted)
    {
        if (sneaker != dialogedit.getData())
        {
            sneaker = dialogedit.getData();
            editSneaker(sneaker, index);
        }
    }
}

void MainWindow::action_SortSneakers_triggered(QAction *action)
{
    QString order = action->data().toString();

    if (order == "brandAsc")
    {
        std::sort(m_sneakerList.begin(), m_sneakerList.end(), brandLessThan);
    }
    else if (order == "brandDesc")
    {
        std::sort(m_sneakerList.begin(), m_sneakerList.end(), brandLessThan);
        std::reverse(m_sneakerList.begin(), m_sneakerList.end());
    }
    else if (order == "buyDateAsc")
    {
        std::sort(m_sneakerList.begin(), m_sneakerList.end(), buyDateLessThan);
    }
    else if (order == "buyDateDesc")
    {
        std::sort(m_sneakerList.begin(), m_sneakerList.end(), buyDateLessThan);
        std::reverse(m_sneakerList.begin(), m_sneakerList.end());
    }
    else if (order == "priceAsc")
    {
        std::sort(m_sneakerList.begin(), m_sneakerList.end(), priceLessThan);
    }
    else if (order == "priceDesc")
    {
        std::sort(m_sneakerList.begin(), m_sneakerList.end(), priceLessThan);
        std::reverse(m_sneakerList.begin(), m_sneakerList.end());
    }
    else if (order == "relDateAsc")
    {
        std::sort(m_sneakerList.begin(), m_sneakerList.end(), relDateLessThan);
    }
    else if (order == "relDateDesc")
    {
        std::sort(m_sneakerList.begin(), m_sneakerList.end(), relDateLessThan);
        std::reverse(m_sneakerList.begin(), m_sneakerList.end());
    }
    else
    {
        return;
    }

    QList<SneakerItem> copyList = m_sneakerList;
    m_sneakerList.clear();

    clearScrollGrid();
    initScrollGrid();

    for (int i = 0; i < copyList.size(); i++)
    {
        addSneaker(copyList[i]);
    }
}

void MainWindow::contextMenu_show(const QPoint &position)
{
    QPoint globalPosition = ui->scrollAreaWidgetContents->mapToGlobal(position);

    QMenu contextMenu;
    contextMenu.addAction("Hinzufügen...", this, SLOT(on_action_AddSneaker_triggered()));
    contextMenu.exec(globalPosition);
}

void MainWindow::contextMenu_show(int index)
{
    QPoint globalPosition = QCursor::pos();

    QVariant varIndex(index);

    QMenu contextMenu;
    contextMenu.addAction("Hinzufügen...", this, SLOT(on_action_AddSneaker_triggered()));
    contextMenu.addAction("Bearbeiten...", this, SLOT(action_EditSneaker_triggered()))->setData(varIndex);
    contextMenu.addAction("Entfernen", this, SLOT(deleteItem_triggered()))->setData(varIndex);
    contextMenu.exec(globalPosition);
}

void MainWindow::deleteItem_triggered()
{
    if (m_selectList.size() == 0)
    {
        QAction *action = qobject_cast<QAction *>(sender());
        int index = action->data().toInt();

        QList<SneakerItem> copyList = m_sneakerList;
        copyList.removeAt(index-1);
        m_sneakerList.clear();

        clearScrollGrid();
        initScrollGrid();

        for (int i = 0; i < copyList.size(); i++)
        {
            addSneaker(copyList[i]);
        }
    }
    else
    {
        deleteItem(m_selectList);
        m_selectList.clear();
    }
}

void MainWindow::on_action_AddSneaker_triggered()
{
    Dialog_Data dialogadd;
    dialogadd.setModal(true);

    if (dialogadd.exec()==QDialog::Accepted)
    {
        SneakerItem sneaker = dialogadd.getData();
        addSneaker(sneaker);
    }
}

void MainWindow::on_action_Import_triggered()
{
    QList<QStringList> sneakerDetails;

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Liste (*.csv *.txt)"));
    dialog.setViewMode(QFileDialog::Detail);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Öffne Liste"),
                                           m_path,
                                           tr("Liste (*.csv *.txt)"));
    if (!fileName.isEmpty())
    {
        QStringList rowsData;

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QString data = file.readAll();
            rowsData = data.split(QRegExp("\n|\r\n|\r"));
        }

        for (int i = 0; i < rowsData.size(); i++)
        {
            if(rowsData.at(i) != "")
            {
                sneakerDetails.append(rowsData.at(i).split(";"));
            }
        }
    }

    QProgressDialog progress("Importiere Sneaker...", "Abbrechen", 0, sneakerDetails.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.show();

    for (int i = 0; i < sneakerDetails.size(); i++)
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
        QLocale german(QLocale::German);

        progress.setValue(i);

        if(progress.wasCanceled())
        {
            break;
        }

        brand = sneakerDetails[i][0];
        model = sneakerDetails[i][1];
        colorway = sneakerDetails[i][2];
        size = sneakerDetails[i][3].toDouble();
        modelnr = sneakerDetails[i][4];
        releasedate = QDate::fromString(sneakerDetails[i][5],"dd.MM.yyyy");
        buydate = QDate::fromString(sneakerDetails[i][6],"dd.MM.yyyy");
        price = german.toDouble(sneakerDetails[i][7]);
        seller = sneakerDetails[i][8];

        SneakerItem sneaker(brand, model, colorway, modelnr, releasedate, buydate, price, seller, size, image1, image2);
        addSneaker(sneaker);
    }

    progress.setValue(sneakerDetails.size());
}
void MainWindow::on_action_Quit_triggered()
{
    saveSettings();
    this->close();
}

void MainWindow::on_action_Statistics_triggered()
{
    Dialog_Statistics dialogstats;

    dialogstats.setModal(true);
    dialogstats.setData(m_sneakerList);
    dialogstats.exec();
}

void MainWindow::resize_timeout()
{
    m_resizeTimer->stop();

    QList<SneakerItem> copyList = m_sneakerList;
    m_sneakerList.clear();

    clearScrollGrid();
    initScrollGrid();

    for (int i = 0; i < copyList.size(); i++)
    {
        addSneaker(copyList[i]);
    }
}


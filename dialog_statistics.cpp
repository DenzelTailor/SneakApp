#include "dialog_statistics.h"
#include "ui_dialog_statistics.h"

#include <QBoxLayout>
#include <QDate>
#include <QDateTimeAxis>
#include <QGraphicsScene>
#include <QHeaderView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QScatterSeries>
#include <QValueAxis>

Dialog_Statistics::Dialog_Statistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Statistics)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    m_tableWidget = new QTableWidget();
    m_pieChartView = new QChartView();
    m_priceChartView = new QChartView();

    QHBoxLayout *diaLayout = new QHBoxLayout();
    diaLayout->addWidget(m_pieChartView, 0);
    diaLayout->addWidget(m_priceChartView, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(diaLayout);
    mainLayout->addWidget(m_tableWidget, 1);
    this->setLayout(mainLayout);
}

Dialog_Statistics::~Dialog_Statistics()
{
    delete ui;
}

void Dialog_Statistics::setData(const QList<QVariant> &sneakerList)
{
    m_sneakerList = sneakerList;
    setTable();
    setBrandDiagram();
    setPriceTimeDiagram();
}

void Dialog_Statistics::setBrandDiagram()
{
    QList<QString> brandList;
    QMap<QString, int> brandCount;
    QPieSeries *series = new QPieSeries();
    QChart *chart = new QChart();

    for (int i = 0; i < m_sneakerList.size(); i++)
    {
        brandList.append(m_sneakerList[i].toList()[0].toString());
    }

    for (int i = 0; i < brandList.size(); i++)
    {
        brandCount[brandList[i]]++;
    }

    brandList.removeDuplicates();

    for (int i = 0; i < brandList.size(); i++)
    {
        series->append(brandList[i] + ": " + QString::number(brandCount[brandList[i]]), brandCount[brandList[i]]);
        series->slices().at(i)->setLabelVisible();
    }

    chart->addSeries(series);
    chart->setTitle("Markenaufteilung der Schuhe (" + QString::number(m_sneakerList.size()) + ")");
    chart->legend()->hide();

    m_pieChartView->setChart(chart);
    m_pieChartView->setRenderHint(QPainter::Antialiasing);
    m_pieChartView->setMaximumHeight(300);
    m_pieChartView->setMaximumWidth(500);
}

void Dialog_Statistics::setPriceTimeDiagram()
{
    QList<QDateTime> buyDateList;
    QList<double> priceList;
    QScatterSeries *series = new QScatterSeries();
    QChart *chart = new QChart();
    QDateTimeAxis *axisX = new QDateTimeAxis();
    QValueAxis *axisY = new QValueAxis();
    double priceSum = 0;

    for (int i = 0; i < m_sneakerList.size(); i++)
    {
        buyDateList.append(m_sneakerList[i].toList()[5].toDateTime());
        priceList.append(m_sneakerList[i].toList()[6].toDouble());
        priceSum += m_sneakerList[i].toList()[6].toDouble();
    }

    for (int i = 0; i < m_sneakerList.size(); i++)
    {
        series->append(buyDateList[i].toMSecsSinceEpoch(), priceList[i]);
    }

    axisX->setFormat("dd-MM-yyyy");
    axisY->setMin(0);
    axisY->setMax(*std::max_element(priceList.begin(), priceList.end()) + 50.0);

    chart->addSeries(series);
    chart->setTitle("Verlauf Preise gekaufter Schuhe (insg. " + QString::number(priceSum) + " €)");
    chart->setAxisY(axisY, series);
    chart->setAxisX(axisX, series);
    chart->legend()->hide();

    m_priceChartView->setChart(chart);
    m_priceChartView->setRenderHint(QPainter::Antialiasing);
    m_priceChartView->setMaximumHeight(300);
}

void Dialog_Statistics::setTable()
{
    // QList<QString> brandList, modelList, cwList, modelnrList, releasedateList, buydateList, sellerList, priceList, sizeList;
    // QList<double> priceList, sizeList;

    m_tableWidget->setRowCount(m_sneakerList.size());
    m_tableWidget->setColumnCount(9);
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << "Marke" << "Modell" << "Colorway" << "Modellnummer" << "Releasedatum"
                                             << "Kaufdatum" << "Kaufpreis [€]" << "Größe" << "Verkäufer");
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setSectionsClickable(true);
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(m_tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerSection_clicked(int)));

    for (int i = 0; i < m_tableWidget->columnCount(); i++)
    {
        m_sortOrders.append(Qt::DescendingOrder);
    }

    m_sortOrders[0] = Qt::AscendingOrder;

    for (int i = 0; i < m_sneakerList.size(); i++)
    {
        for (int j = 0; j <= 8; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setData(Qt::EditRole, m_sneakerList[i].toList()[j]);
            m_tableWidget->setItem(i, j, item);
        }
    }

    m_tableWidget->sortItems(0, m_sortOrders[0]);
}

//---------------------------------------Slots---------------------------------------
void Dialog_Statistics::headerSection_clicked(int index)
{
    if(m_sortOrders[index] == Qt::AscendingOrder)
    {
        for (int i = 0; i < m_sortOrders.size(); i++)
        {
            m_sortOrders[i] = Qt::DescendingOrder;
        }
    }
    else
    {
        for (int i = 0; i < m_sortOrders.size(); i++)
        {
            m_sortOrders[i] = Qt::DescendingOrder;
        }

        m_sortOrders[index] = Qt::AscendingOrder;
    }
    m_tableWidget->sortItems(index, m_sortOrders[index]);
}

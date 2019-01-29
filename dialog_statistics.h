#ifndef DIALOG_STATISTICS_H
#define DIALOG_STATISTICS_H

#include "sneakeritem.h"

#include <QDialog>
#include <QTableWidget>
#include <QtCharts/QChartView>

using namespace QtCharts;

namespace Ui {
class Dialog_Statistics;
}

class Dialog_Statistics : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Statistics(QWidget *parent = nullptr);
    ~Dialog_Statistics();

    void setData(const QList<SneakerItem> &sneakerList);

private:
    Ui::Dialog_Statistics *ui;

    QChartView *m_pieChartView;
    QChartView *m_priceChartView;
    QList<SneakerItem> m_sneakerList;
    QList<Qt::SortOrder> m_sortOrders;
    QTableWidget *m_tableWidget;

    void setBrandDiagram();
    void setPriceTimeDiagram();
    void setTable();

private slots:
    void headerSection_clicked(int index);
};

#endif // DIALOG_STATISTICS_H

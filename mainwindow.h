#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog_data.h"
#include "dialog_statistics.h"
#include "qclicklabel.h"
#include "sneakeritem.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QRubberBand>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int m_column;
    int m_maxColumns;
    int m_thumbSize;
    int m_row;
    QList<int> m_selectList;
    QList<SneakerItem> m_sneakerList;
    QPoint m_mouseOrigin;
    QRubberBand *m_rubberband;
    QString m_path;
    QTimer *m_resizeTimer;

    int calcAvailSpace(const int leftMargin, const int rightMargin);
    static bool brandLessThan(const SneakerItem &sneaker1, const SneakerItem &sneaker2);
    static bool buyDateLessThan(const SneakerItem &sneaker1, const SneakerItem &sneaker2);
    static bool priceLessThan(const SneakerItem &sneaker1, const SneakerItem &sneaker2);
    static bool relDateLessThan(const SneakerItem &sneaker1, const SneakerItem &sneaker2);
    void addSneaker(const SneakerItem &sneaker);
    void calcMaxColumns(int *calcSpace, const int availHSpace, const int minThumbSize, const int horiSpacing);
    void calcThumbnailSize(int *calcSpace, const int availHSpace, const int maxThumbSize, const int horiSpacing);
    void clearScrollGrid();
    void createDatabase();
    void deleteItem(QList<int> deleteList);
    void editSneaker(const SneakerItem &sneaker, int index);
    void initScrollGrid();
    void loadDatabase();
    void loadSettings();
    void saveDatabase();
    void saveSettings();
    void updateDatabase();

    void closeEvent(QCloseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent* event);

private slots:
    void action_EditSneaker_triggered();
    void action_EditSneaker_triggered(int index);
    void action_SortSneakers_triggered(QAction *action);
    void contextMenu_show(const QPoint &position);
    void contextMenu_show(int index);
    void deleteItem_triggered();
    void on_action_AddSneaker_triggered();
    void on_action_Import_triggered();
    void on_action_Quit_triggered();
    void on_action_Statistics_triggered();
    void resize_timeout();
};

#endif // MAINWINDOW_H

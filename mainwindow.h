#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog_data.h"
#include "dialog_statistics.h"
#include "qclicklabel.h"
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
    QList<QVariant> m_sneakerList;
    QPoint m_mouseOrigin;
    QRubberBand *m_rubberband;
    QString m_path;
    QTimer *m_resizeTimer;

    int calcAvailSpace(const int leftMargin, const int rightMargin);
    static bool brandLessThan(const QVariant &var1, const QVariant &var2);
    static bool buyDateLessThan(const QVariant &var1, const QVariant &var2);
    static bool priceLessThan(const QVariant &var1, const QVariant &var2);
    static bool relDateLessThan(const QVariant &var1, const QVariant &var2);
    void addSneaker(const QVariant &sneakerVar);
    void calcMaxColumns(int *calcSpace, const int availHSpace, const int minThumbSize, const int horiSpacing);
    void calcThumbnailSize(int *calcSpace, const int availHSpace, const int maxThumbSize, const int horiSpacing);
    void clearScrollGrid();
    void deleteItem(QList<int> deleteList);
    void editSneaker(const QVariant &sneakerVar, int index);
    void initScrollGrid();
    void loadSettings();
    void saveSettings();

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

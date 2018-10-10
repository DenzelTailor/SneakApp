#ifndef QCLICKLABEL_H
#define QCLICKLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class QClickLabel : public QLabel {
    Q_OBJECT

public:
    explicit QClickLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags = Qt::WindowFlags());
    ~QClickLabel();

signals:
    void clicked();
    void clickedRight();

protected:
    void mousePressEvent(QMouseEvent *event);

};

#endif // QCLICKLABEL_H

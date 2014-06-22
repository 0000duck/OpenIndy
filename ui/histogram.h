#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>

class Histogram : public QWidget
{
    Q_OBJECT
public:
    explicit Histogram(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent * event);

};

#endif // HISTOGRAM_H

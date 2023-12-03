#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

class DrawingWidget : public QWidget {
    Q_OBJECT

public:
    explicit DrawingWidget(QWidget* parent = nullptr);
    void clearDrawing();
    void setPenColor(const QColor& newColor);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    bool drawing; // Indica daca se deseneaza
    bool erasing; // Indica daca se sterge
    QImage image; // Imaginea pe care se deseneaza
    QPoint lastPoint; // Ultimul punct pentru desen
    QPen pen; // Stiloul folosit pentru desenare si stergere
    void drawLineTo(const QPoint& endPoint); // Traseaza o linie pana la punctul dat
    void setEraser(); // Pregateste stiloul pentru stergere
};

#endif // DRAWINGWIDGET_H

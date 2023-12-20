#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QWidget>
#include <QImage>
#include <QStack>
#include <QPoint>


class DrawingWidget : public QWidget {
    Q_OBJECT

public:
    explicit DrawingWidget(QWidget* parent = nullptr);
    void ClearDrawing();
    void SetPenColor(const QColor& newColor);
    void setText(const QString& text);
    void ToggleFillMode();
    QImage GetImage() const noexcept;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    bool m_isDrawing; // Indica daca se deseneaza
    bool m_isErasing; // Indica daca se sterge
    QImage m_image; // Imaginea pe care se deseneaza
    QPoint m_lastPoint; // Ultimul punct pentru desen
    QPen m_pen; // Stiloul folosit pentru desenare si stergere
    bool m_fillMode;//indica daca se umple
    QColor m_currentFillColor;
    QImage image;

private:
    void DrawLineTo(const QPoint& endPoint); // Traseaza o linie pana la punctul dat
    void SetEraser(); // Pregateste stiloul pentru stergere
    void SetCurrentFillColor(const QColor& color);
    void FloodFill(const QPoint& startpoint, const QColor& fillColor, const QColor& oldColor);//Functia de umplere
};

#endif // DRAWINGWIDGET_H
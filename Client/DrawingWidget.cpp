#include "DrawingWidget.h"

DrawingWidget::DrawingWidget(QWidget* parent) : QWidget(parent), m_isDrawing(false), m_isErasing(false)
{
    setAttribute(Qt::WA_StaticContents); // Continutul widgetului ramane static la redimensionare
    m_image = QImage(500, 500, QImage::Format_ARGB32); // Initializare imagine
    m_image.fill(Qt::white); // Umple imaginea cu alb
    m_pen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); // Initializare stilou
}

void DrawingWidget::mousePressEvent(QMouseEvent* event) 
{
    if (event->button() == Qt::LeftButton) 
    {
        m_lastPoint = event->pos();
        m_isDrawing = true;
        if (m_isErasing) 
        {
            // Daca se sterge, seteaza stiloul la modul de desenare
            m_pen.setColor(Qt::black);
            m_isErasing = false;
        }
    }
    else if (event->button() == Qt::RightButton) 
    {
        m_lastPoint = event->pos();
        m_isDrawing = true;
        SetEraser(); // Seteaza modul de stergere
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event) 
{
    if ((event->buttons() & (Qt::LeftButton | Qt::RightButton)) && m_isDrawing)
        DrawLineTo(event->pos());
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event) 
{
    if ((event->button() == Qt::LeftButton || event->button() == Qt::RightButton) && m_isDrawing) 
    {
        DrawLineTo(event->pos());
        m_isDrawing = false;
    }
}

void DrawingWidget::paintEvent(QPaintEvent* event) 
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_image, dirtyRect);
}

void DrawingWidget::DrawLineTo(const QPoint& endPoint)
{
    QPainter painter(&m_image);
    painter.setPen(m_pen);
    painter.drawLine(m_lastPoint, endPoint);
    m_lastPoint = endPoint;
    update(QRect(m_lastPoint, endPoint).normalized().adjusted(-1, -1, 1, 1));
}

void DrawingWidget::SetEraser() 
{
    m_pen.setColor(Qt::white); // Culoarea de fundal pentru stergere
    m_pen.setWidth(10); // Puteti ajusta grosimea pentru eraser
    m_isErasing = true;
}

void DrawingWidget::ClearDrawing() 
{
    m_image.fill(Qt::white); 
    update();
}


void DrawingWidget::SetPenColor(const QColor& newColor) 
{
    m_pen.setColor(newColor);
}

#include "DrawingWidget.h"

DrawingWidget::DrawingWidget(QWidget* parent) : QWidget(parent), drawing(false), erasing(false)
{
    setAttribute(Qt::WA_StaticContents); // Continutul widgetului ramane static la redimensionare
    image = QImage(500, 500, QImage::Format_ARGB32); // Initializare imagine
    image.fill(Qt::white); // Umple imaginea cu alb
    pen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); // Initializare stilou
}

void DrawingWidget::mousePressEvent(QMouseEvent* event) 
{
    if (event->button() == Qt::LeftButton) 
    {
        lastPoint = event->pos();
        drawing = true;
        if (erasing) 
        {
            // Daca se sterge, seteaza stiloul la modul de desenare
            pen.setColor(Qt::black);
            erasing = false;
        }
    }
    else if (event->button() == Qt::RightButton) 
    {
        lastPoint = event->pos();
        drawing = true;
        setEraser(); // Seteaza modul de stergere
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event) 
{
    if ((event->buttons() & (Qt::LeftButton | Qt::RightButton)) && drawing)
        drawLineTo(event->pos());
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event) 
{
    if ((event->button() == Qt::LeftButton || event->button() == Qt::RightButton) && drawing) 
    {
        drawLineTo(event->pos());
        drawing = false;
    }
}

void DrawingWidget::paintEvent(QPaintEvent* event) 
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void DrawingWidget::drawLineTo(const QPoint& endPoint)
{
    QPainter painter(&image);
    painter.setPen(pen);
    painter.drawLine(lastPoint, endPoint);
    lastPoint = endPoint;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-1, -1, 1, 1));
}

void DrawingWidget::setEraser() 
{
    pen.setColor(Qt::white); // Culoarea de fundal pentru stergere
    pen.setWidth(10); // Puteti ajusta grosimea pentru eraser
    erasing = true;
}

void DrawingWidget::clearDrawing() 
{
    image.fill(Qt::white); 
    update();
}


void DrawingWidget::setPenColor(const QColor& newColor) 
{
    pen.setColor(newColor);
}

#include "DrawingWidget.h"
#include <QQueue>

DrawingWidget::DrawingWidget(QWidget* parent) : QWidget(parent), m_isDrawing(false), m_isErasing(false)
{
    setAttribute(Qt::WA_StaticContents); // Continutul widgetului ramane static la redimensionare
    m_image = QImage(621, 491, QImage::Format_ARGB32); // Initializare imagine
    m_image.fill(Qt::white); // Umple imaginea cu alb
    m_pen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); // Initializare stilou
    m_fillMode = false;
}

void DrawingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_fillMode) 
    {
        saveCurrentState();
        QPoint fillStartPoint = event->pos();
        QColor oldColor = m_image.pixelColor(fillStartPoint);
        FloodFill(fillStartPoint, m_currentFillColor, oldColor);
    }
    else
    {
        if (event->button() == Qt::LeftButton)
        {
            saveCurrentState();
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
            saveCurrentState();
            m_lastPoint = event->pos();
            m_isDrawing = true;
            SetEraser(); // Seteaza modul de stergere
        }
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & (Qt::LeftButton | Qt::RightButton)) && m_isDrawing)
    {
        DrawLineTo(event->pos());
        update();
    }
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if ((event->button() == Qt::LeftButton || event->button() == Qt::RightButton) && m_isDrawing)
    {
        DrawLineTo(event->pos());
        m_isDrawing = false;
        update();
    }
}

void DrawingWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_image, dirtyRect);
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

void DrawingWidget::setText(const QString& text)
{
    // Qt generates an error if the function doesnt exist, the function is not needed for this project, it is called by mistake by Qt when the project is cleaned and rebuilt
    return;
}

void DrawingWidget::DrawLineTo(const QPoint& endPoint)
{
    QPainter painter(&m_image);
    painter.setPen(m_pen);

    // Calculate the line points between m_lastPoint and endPoint
    int dx = abs(endPoint.x() - m_lastPoint.x());
    int dy = -abs(endPoint.y() - m_lastPoint.y());
    int sx = m_lastPoint.x() < endPoint.x() ? 1 : -1;
    int sy = m_lastPoint.y() < endPoint.y() ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    while (true) {
        painter.drawPoint(m_lastPoint); /* set pixel */
        if (m_lastPoint == endPoint) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; m_lastPoint.setX(m_lastPoint.x() + sx); } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; m_lastPoint.setY(m_lastPoint.y() + sy); } /* e_xy+e_y < 0 */
    }

    update(QRect(m_lastPoint, endPoint).normalized().adjusted(-1, -1, 1, 1));
}

/*void DrawingWidget::FloodFill(const QPoint& startPoint, const QColor& fillColor, const QColor& oldColor) {
    if (!m_image.rect().contains(startPoint) || m_image.pixelColor(startPoint) != oldColor || fillColor == oldColor) {
        return;
    }

    // Create an auxiliary image to keep track of visited pixels.
    QImage visited(m_image.size(), QImage::Format_ARGB32_Premultiplied);
    visited.fill(Qt::black); // Black pixels are unvisited.

    QQueue<QPoint> queue;
    queue.enqueue(startPoint);
    visited.setPixelColor(startPoint, Qt::white); // Mark the start point as visited.

    while (!queue.isEmpty()) {
        if (queue.size() > 10000) { // Limit the queue size to prevent memory overflow.
            qDebug() << "Flood fill operation aborted: too many pixels to process.";
            return;
        }

        QPoint p = queue.dequeue();
        if (m_image.pixelColor(p) == oldColor) {
            m_image.setPixelColor(p, fillColor);

            QPoint points[4] = {
                {p.x() + 1, p.y()},
                {p.x() - 1, p.y()},
                {p.x(), p.y() + 1},
                {p.x(), p.y() - 1}
            };

            for (const QPoint& newP : points) {
                if (m_image.rect().contains(newP) && visited.pixelColor(newP) != Qt::white && m_image.pixelColor(newP) == oldColor) {
                    queue.enqueue(newP);
                    visited.setPixelColor(newP, Qt::white); // Mark this pixel as visited.
                }
            }
        }
    }

    // Trigger an update for the widget to repaint itself.
    update(m_image.rect());
}*/

void DrawingWidget::FloodFill(const QPoint&, const QColor& fillColor, const QColor&) {
    // Fill the entire image with the selected fill color.
    m_image.fill(fillColor);

    // Trigger an update for the widget to repaint itself with the new fill color.
    update();
}

void DrawingWidget::SetCurrentFillColor(const QColor& color)
{
    m_currentFillColor = color;
}

void DrawingWidget::ToggleFillMode()
{
    m_fillMode = !m_fillMode; // Inverseazã valoarea lui m_fillMode
}

QImage DrawingWidget::GetImage() const noexcept
{
    return m_image;
}

void DrawingWidget::SetImage(const QImage& image)
{
	m_image = image;
	update();
}

void DrawingWidget::saveCurrentState() 
{
    if (m_undoStack.size() > 10) 
    { // Limit the undo stack size to avoid excessive memory use.
        m_undoStack.removeFirst(); // Remove the oldest state to maintain the stack size.
    }
    m_undoStack.push_back(m_image.copy()); // Save the current state of the image.
}

void DrawingWidget::Undo() 
{
    if (!m_undoStack.isEmpty()) {
        m_image = m_undoStack.takeLast(); // Take the last saved state.
        update(); // Update the canvas to reflect the undone state.
    }
}

void DrawingWidget::SetPenWidth(int newWidth) 
{
    m_pen.setWidth(newWidth);
    update();
}
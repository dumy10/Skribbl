#include "DrawingWidget.h"
#include <QQueue>
#include <QSet>
#include <QPair>
#include <QApplication>

DrawingWidget::DrawingWidget(QWidget* parent) 
    : QWidget(parent), 
      m_isDrawing(false), 
      m_currentMode(DrawMode::Pen),
      m_antiAliasing(true),
      m_savedPenColor(Qt::black),
      m_savedPenWidth(3)
{
    setAttribute(Qt::WA_StaticContents);
    m_image = QImage(621, 491, QImage::Format_ARGB32);
    m_image.fill(Qt::white);
    m_pen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    m_currentFillColor = Qt::black;
    updateCursor();
}

void DrawingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    QPoint clickPoint = event->pos();
    
    // Bounds check
    if (clickPoint.x() < 0 || clickPoint.x() >= m_image.width() ||
        clickPoint.y() < 0 || clickPoint.y() >= m_image.height()) {
        return;
    }

    switch (m_currentMode) {
        case DrawMode::Fill: {
            saveCurrentState();
            QColor oldColor = m_image.pixelColor(clickPoint);
            if (oldColor != m_currentFillColor) {
                FloodFill(clickPoint, m_currentFillColor, oldColor);
            }
            break;
        }
        case DrawMode::ColorPicker: {
            QColor pickedColor = m_image.pixelColor(clickPoint);
            SetPenColor(pickedColor);
            SetCurrentFillColor(pickedColor);
            emit colorPicked(pickedColor);
            break;
        }
        case DrawMode::Line:
        case DrawMode::Rectangle:
        case DrawMode::Circle:
            saveCurrentState();
            m_shapeStartPoint = clickPoint;
            m_lastPoint = clickPoint;
            m_isDrawing = true;
            break;
        default: // Pen or Eraser
            saveCurrentState();
            m_lastPoint = clickPoint;
            m_isDrawing = true;
            break;
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton) || !m_isDrawing) {
        return;
    }

    QPoint currentPoint = event->pos();
    
    switch (m_currentMode) {
        case DrawMode::Line:
        case DrawMode::Rectangle:
        case DrawMode::Circle:
            // For shapes, just update the endpoint for preview
            m_lastPoint = currentPoint;
            update();
            break;
        default: // Pen or Eraser
            DrawLineTo(currentPoint);
            update();
            break;
    }
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton || !m_isDrawing) {
        return;
    }

    QPoint endPoint = event->pos();
    
    switch (m_currentMode) {
        case DrawMode::Line:
        case DrawMode::Rectangle:
        case DrawMode::Circle: {
            // Draw the final shape on the image
            QPainter painter(&m_image);
            painter.setRenderHint(QPainter::Antialiasing, m_antiAliasing);
            painter.setPen(m_pen);
            drawShape(painter, m_shapeStartPoint, endPoint);
            break;
        }
        default: // Pen or Eraser
            DrawLineTo(endPoint);
            break;
    }
    
    m_isDrawing = false;
    update();
}

void DrawingWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_image, dirtyRect);
    
    // Draw shape preview while drawing
    if (m_isDrawing && (m_currentMode == DrawMode::Line || 
                        m_currentMode == DrawMode::Rectangle || 
                        m_currentMode == DrawMode::Circle)) {
        painter.setRenderHint(QPainter::Antialiasing, m_antiAliasing);
        painter.setPen(m_pen);
        drawShape(painter, m_shapeStartPoint, m_lastPoint);
    }
}

void DrawingWidget::SetDrawMode(DrawMode mode)
{
    if (m_currentMode == mode) {
        return;
    }
    
    // Save pen state when switching from pen mode
    if (m_currentMode == DrawMode::Pen) {
        m_savedPenColor = m_pen.color();
        m_savedPenWidth = m_pen.width();
    }
    
    m_currentMode = mode;
    
    // Configure pen for the new mode
    switch (mode) {
        case DrawMode::Eraser:
            m_pen.setColor(Qt::white);
            m_pen.setWidth(qMax(10, m_savedPenWidth));
            break;
        case DrawMode::Pen:
        case DrawMode::Line:
        case DrawMode::Rectangle:
        case DrawMode::Circle:
            // Restore saved pen state
            m_pen.setColor(m_savedPenColor);
            m_pen.setWidth(m_savedPenWidth);
            break;
        default:
            break;
    }
    
    updateCursor();
}

void DrawingWidget::ClearDrawing()
{
    m_image.fill(Qt::white);
    update();
}

void DrawingWidget::ClearUndoHistory()
{
    m_undoStack.clear();
    m_redoStack.clear();
}

void DrawingWidget::SetPenColor(const QColor& newColor)
{
    m_pen.setColor(newColor);
    // Save the color so it persists when switching between tools
    if (m_currentMode != DrawMode::Eraser) {
        m_savedPenColor = newColor;
    }
}

void DrawingWidget::DrawLineTo(const QPoint& endPoint)
{
    QPainter painter(&m_image);
    painter.setRenderHint(QPainter::Antialiasing, m_antiAliasing);
    painter.setPen(m_pen);
    
    // Use QPainter's built-in line drawing for smoother results
    painter.drawLine(m_lastPoint, endPoint);
    
    int rad = (m_pen.width() / 2) + 2;
    update(QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, rad, rad));
    
    m_lastPoint = endPoint;
}

void DrawingWidget::FloodFill(const QPoint& startPoint, const QColor& fillColor, const QColor& oldColor) {
    // Bounds check
    if (startPoint.x() < 0 || startPoint.x() >= m_image.width() ||
        startPoint.y() < 0 || startPoint.y() >= m_image.height()) {
        return;
    }
    
    // Don't fill if colors are the same
    if (oldColor == fillColor) {
        return;
    }
    
    // Proper flood fill algorithm using BFS
    QQueue<QPoint> queue;
    QSet<QPair<int, int>> visited;
    
    queue.enqueue(startPoint);
    visited.insert(qMakePair(startPoint.x(), startPoint.y()));
    
    while (!queue.isEmpty()) {
        QPoint point = queue.dequeue();
        int x = point.x();
        int y = point.y();
        
        // Bounds check
        if (x < 0 || x >= m_image.width() || y < 0 || y >= m_image.height()) {
            continue;
        }
        
        // Check if this pixel is the old color
        if (m_image.pixelColor(x, y) != oldColor) {
            continue;
        }
        
        // Fill this pixel
        m_image.setPixelColor(x, y, fillColor);
        
        // Add neighbors to queue
        QPoint neighbors[] = {
            QPoint(x + 1, y),
            QPoint(x - 1, y),
            QPoint(x, y + 1),
            QPoint(x, y - 1)
        };
        
        for (const QPoint& neighbor : neighbors) {
            QPair<int, int> key = qMakePair(neighbor.x(), neighbor.y());
            if (!visited.contains(key)) {
                queue.enqueue(neighbor);
                visited.insert(key);
            }
        }
    }
    
    update();
}

void DrawingWidget::SetCurrentFillColor(const QColor& color)
{
    m_currentFillColor = color;
}

void DrawingWidget::ToggleFillMode()
{
    if (m_currentMode == DrawMode::Fill) {
        SetDrawMode(DrawMode::Pen);
    } else {
        SetDrawMode(DrawMode::Fill);
    }
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
    if (m_undoStack.size() >= 20)  { // Increased limit for better undo history
        m_undoStack.removeFirst(); // Remove the oldest state to maintain the stack size.
    }

    m_undoStack.push_back(m_image.copy()); // Save the current state of the image.
    m_redoStack.clear(); // Clear redo stack when new action is performed
}

void DrawingWidget::Undo() 
{
    if (!m_undoStack.isEmpty()) {
        m_redoStack.push_back(m_image.copy()); // Save current state to redo stack
        if (m_redoStack.size() > 20) {
            m_redoStack.removeFirst();
        }
        m_image = m_undoStack.takeLast(); // Take the last saved state
        update(); // Update the canvas to reflect the undone state
    }
}

void DrawingWidget::SetPenWidth(int newWidth) 
{
    m_pen.setWidth(newWidth);
    if (m_currentMode == DrawMode::Pen) {
        m_savedPenWidth = newWidth;
    }
    update();
}

void DrawingWidget::Redo()
{
    if (!m_redoStack.isEmpty()) {
        m_undoStack.push_back(m_image.copy()); // Save current state to undo stack
        if (m_undoStack.size() > 20) {
            m_undoStack.removeFirst();
        }
        m_image = m_redoStack.takeLast(); // Restore from redo stack
        update();
    }
}

void DrawingWidget::SetAntiAliasing(bool enabled)
{
    m_antiAliasing = enabled;
}

DrawingWidget::DrawMode DrawingWidget::GetDrawMode() const noexcept
{
    return m_currentMode;
}

bool DrawingWidget::GetAntiAliasing() const noexcept
{
    return m_antiAliasing;
}

QColor DrawingWidget::GetColorAtPoint(const QPoint& point) const
{
    if (point.x() >= 0 && point.x() < m_image.width() &&
        point.y() >= 0 && point.y() < m_image.height()) {
        return m_image.pixelColor(point);
    }
    return Qt::white;
}

void DrawingWidget::updateCursor()
{
    switch (m_currentMode) {
        case DrawMode::Eraser:
            setCursor(Qt::CrossCursor);
            break;
        case DrawMode::Fill:
            setCursor(Qt::PointingHandCursor);
            break;
        case DrawMode::ColorPicker:
            setCursor(Qt::CrossCursor);
            break;
        case DrawMode::Line:
        case DrawMode::Rectangle:
        case DrawMode::Circle:
            setCursor(Qt::CrossCursor);
            break;
        default:
            setCursor(Qt::CrossCursor);
            break;
    }
}

void DrawingWidget::drawShape(QPainter& painter, const QPoint& start, const QPoint& end)
{
    switch (m_currentMode) {
        case DrawMode::Line:
            painter.drawLine(start, end);
            break;
        case DrawMode::Rectangle:
            painter.drawRect(QRect(start, end).normalized());
            break;
        case DrawMode::Circle: {
            int radius = qSqrt(qPow(end.x() - start.x(), 2) + qPow(end.y() - start.y(), 2));
            painter.drawEllipse(start, radius, radius);
            break;
        }
        default:
            break;
    }
}

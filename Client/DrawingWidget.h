#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPen> 
#include <QImage>
#include <QStack>
#include <QPoint>


class DrawingWidget : public QWidget {
	Q_OBJECT

public:
	explicit DrawingWidget(QWidget* parent = nullptr);

	void ClearDrawing();
	void setText(const QString& text);
	void ToggleFillMode();
	QImage GetImage() const noexcept;
	void SetImage(const QImage& image);
	void Undo();
	void SetPenColor(const QColor& newColor);
	void SetCurrentFillColor(const QColor& color);
	void SetPenWidth(int newWidth);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	bool m_isDrawing;
	bool m_isErasing;
	QImage m_image;
	QPoint m_lastPoint;
	QPen m_pen;
	bool m_fillMode{ false };
	QColor m_currentFillColor;
	QVector<QImage> m_undoStack;

private:
	void DrawLineTo(const QPoint& endPoint);
	void SetEraser();
	void FloodFill(const QPoint& startpoint, const QColor& fillColor, const QColor& oldColor);
	void saveCurrentState();
};

#endif // DRAWINGWIDGET_H
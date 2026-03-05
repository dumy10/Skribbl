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
	enum class DrawMode {
		Pen,
		Eraser,
		Fill,
		Line,
		Rectangle,
		Circle,
		ColorPicker
	};

	explicit DrawingWidget(QWidget* parent = nullptr);

signals:
	void colorPicked(const QColor& color);

public:
	void ClearDrawing();
	void ToggleFillMode();
	QImage GetImage() const noexcept;
	void SetImage(const QImage& image);
	void Undo();
	void Redo();
	void SetPenColor(const QColor& newColor);
	void SetCurrentFillColor(const QColor& color);
	void SetPenWidth(int newWidth);
	void SetDrawMode(DrawMode mode);
	void SetAntiAliasing(bool enabled);
	DrawMode GetDrawMode() const noexcept;
	QColor GetColorAtPoint(const QPoint& point) const;

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	bool m_isDrawing;
	QImage m_image;
	QPoint m_lastPoint;
	QPoint m_shapeStartPoint;
	QPen m_pen;
	QColor m_currentFillColor;
	QVector<QImage> m_undoStack;
	QVector<QImage> m_redoStack;
	DrawMode m_currentMode;
	bool m_antiAliasing;
	
	// Eraser state preservation
	QColor m_savedPenColor;
	int m_savedPenWidth;

private:
	void DrawLineTo(const QPoint& endPoint);
	void FloodFill(const QPoint& startpoint, const QColor& fillColor, const QColor& oldColor);
	void saveCurrentState();
	void updateCursor();
	void drawShape(QPainter& painter, const QPoint& start, const QPoint& end);
};

#endif // DRAWINGWIDGET_H
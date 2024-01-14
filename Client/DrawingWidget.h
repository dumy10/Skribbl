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
	bool m_isDrawing; // Indica daca se deseneaza
	bool m_isErasing; // Indica daca se sterge
	QImage m_image; // Imaginea pe care se deseneaza
	QPoint m_lastPoint; // Ultimul punct pentru desen
	QPen m_pen; // Stiloul folosit pentru desenare si stergere
	bool m_fillMode{ false };//indica daca se umple
	QColor m_currentFillColor;
	QVector<QImage> m_undoStack;

private:
	void DrawLineTo(const QPoint& endPoint); // Traseaza o linie pana la punctul dat
	void SetEraser(); // Pregateste stiloul pentru stergere
	void FloodFill(const QPoint& startpoint, const QColor& fillColor, const QColor& oldColor);//Functia de umplere
	void saveCurrentState();
};

#endif // DRAWINGWIDGET_H
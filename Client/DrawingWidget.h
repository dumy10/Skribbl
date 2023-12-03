#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

class DrawingWidget : public QWidget {
    Q_OBJECT

public:
    explicit DrawingWidget(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    bool drawing;
    QImage image;
    QPoint lastPoint;
    void drawLineTo(const QPoint& endPoint);
};
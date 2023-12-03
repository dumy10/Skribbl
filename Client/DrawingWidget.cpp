#include "DrawingWidget.h"

// Constructorul pentru widget-ul de desen.
// Aici setam imaginea initiala si proprietatile widget-ului.
DrawingWidget::DrawingWidget(QWidget* parent) : QWidget(parent), drawing(false) {
    setAttribute(Qt::WA_StaticContents); // Spune ca widget-ul nu are nevoie de redesenare completa la resize.
    image = QImage(500, 500, QImage::Format_ARGB32); // Initializeaza imaginea pentru desen.
    image.fill(Qt::white); // Umple suprafata de desen cu alb.
}

// Evenimentul apasat mouse. Acesta incepe desenarea.
void DrawingWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos(); // Salveaza ultima pozitie a cursorului.
        drawing = true; // Seteaza flag-ul de desenare la true.
    }
}

// Evenimentul de miscare mouse. Acesta deseneaza pe canvas.
void DrawingWidget::mouseMoveEvent(QMouseEvent* event) {
    if ((event->buttons() & Qt::LeftButton) && drawing) {
        drawLineTo(event->pos()); // Deseneaza o linie pana la noua pozitie a cursorului.
    }
}

// Evenimentul eliberat mouse. Acesta incheie desenarea.
void DrawingWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && drawing) {
        drawLineTo(event->pos()); // Deseneaza ultima sectiune a liniei.
        drawing = false; // Seteaza flag-ul de desenare la false.
    }
}

// Evenimentul de vopsire. Acesta vopseste imaginea pe widget.
void DrawingWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QRect dirtyRect = event->rect(); // Rectangula care trebuie redesenata.
    painter.drawImage(dirtyRect, image, dirtyRect); // Deseneaza imaginea pe widget.
}

// Functia care deseneaza linia efectiva.
void DrawingWidget::drawLineTo(const QPoint& endPoint) {
    QPainter painter(&image); // Creeaza un painter pentru imagine.
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); // Seteaza stilul penelului.
    painter.drawLine(lastPoint, endPoint); // Deseneaza o linie intre ultimul punct si punctul curent.
    lastPoint = endPoint; // Actualizeaza ultimul punct.
    // Redeseneaza doar partea de widget care contine linia desenata.
    update(QRect(lastPoint, endPoint).normalized().adjusted(-1, -1, 1, 1));
}

#ifndef DRAWINGTOOLSUIMANAGER_H
#define DRAWINGTOOLSUIMANAGER_H

#include "DrawingWidget.h"
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QColor>
#include <QString>
#include <array>
#include <span>
#include <unordered_map>

/**
 * @brief Manages the drawing tools UI including color palette, tool selection, and tool state.
 * 
 * This class coordinates all drawing tool buttons, color selection, brush settings,
 * and keeps tool button states synchronized with the drawing widget.
 */
class DrawingToolsUIManager 
{
public:
    explicit DrawingToolsUIManager(DrawingWidget* drawingWidget, QWidget* parentWidget) noexcept;

    // Tool operations
    void UpdateToolButtonStates();
    void SetPenColor(const QColor& color);
    void SetBrushSize(int size);
    void ToggleFillMode();
    void SetDrawMode(DrawingWidget::DrawMode mode);
    void SetAntiAliasing(bool enabled);
    void ClearDrawing();
    void ClearUndoHistory();
    void Undo();
    void Redo();

    // Visibility management
    void SetDrawingUIVisibility(bool visible) noexcept;

    // Color operations
    void OnColorPicked(const QColor& color);

    // Getters
    [[nodiscard]] DrawingWidget* GetDrawingWidget() const noexcept { return m_drawingWidget; }
    
    // Color palette (public for use in Game connections)
    static const std::unordered_map<QString, QColor> COLOR_PALETTE;

private:
    void UpdateColorPreview(const QColor& color);
    QWidget* FindChild(const QString& name) const noexcept;

private:
    DrawingWidget* m_drawingWidget;
    QWidget* m_parentWidget;
};

#endif // DRAWINGTOOLSUIMANAGER_H

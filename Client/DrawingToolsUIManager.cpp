#include "DrawingToolsUIManager.h"
#include <QLabel>

namespace {
    // Widget names
    constexpr std::string_view WIDGET_CLEAR = "Clear";
    constexpr std::string_view WIDGET_BUCKET = "Bucket";
    constexpr std::string_view WIDGET_BRUSH_SIZE = "BrushSize";
    constexpr std::string_view WIDGET_UNDO = "Undo";
    constexpr std::string_view WIDGET_REDO = "Redo";
    constexpr std::string_view WIDGET_SETTINGS = "SettingsButton";
    constexpr std::string_view WIDGET_ANTI_ALIASING = "AntiAliasing";
    constexpr std::string_view WIDGET_COLOR_PREVIEW = "CurrentColorPreview";
    
    // Tool button names
    constexpr std::string_view TOOL_PEN = "PenTool";
    constexpr std::string_view TOOL_ERASER = "EraserTool";
    constexpr std::string_view TOOL_LINE = "LineTool";
    constexpr std::string_view TOOL_RECTANGLE = "RectangleTool";
    constexpr std::string_view TOOL_CIRCLE = "CircleTool";
    constexpr std::string_view TOOL_COLOR_PICKER = "ColorPicker";
    
    // Color button names
    constexpr std::string_view COLOR_GREEN = "GreenButton";
    constexpr std::string_view COLOR_RED = "RedButton";
    constexpr std::string_view COLOR_BLUE = "BlueButton";
    constexpr std::string_view COLOR_ORANGE = "OrangeButton";
    constexpr std::string_view COLOR_BROWN = "BrownButton";
    constexpr std::string_view COLOR_PURPLE = "PurpleButton";
    constexpr std::string_view COLOR_WHITE = "WhiteButton";
    constexpr std::string_view COLOR_BLACK = "BlackButton";
    constexpr std::string_view COLOR_GREY = "GreyButton";
    constexpr std::string_view COLOR_YELLOW = "YellowButton";
    constexpr std::string_view COLOR_PINK = "PinkButton";
    constexpr std::string_view COLOR_TURQUOISE = "TurquoiseButton";
}

const std::unordered_map<QString, QColor> DrawingToolsUIManager::COLOR_PALETTE = {
    {"green", Qt::green},
    {"red", Qt::red},
    {"blue", Qt::blue},
    {"orange", QColor(255, 165, 0)},
    {"brown", QColor(165, 42, 42)},
    {"purple", QColor(128, 0, 128)},
    {"black", Qt::black},
    {"white", Qt::white},
    {"yellow", Qt::yellow},
    {"grey", Qt::gray},
    {"turquoise", QColor(64, 224, 208)},
    {"pink", QColor(255, 192, 203)}
};

DrawingToolsUIManager::DrawingToolsUIManager(DrawingWidget* drawingWidget, QWidget* parentWidget) noexcept
    : m_drawingWidget(drawingWidget)
    , m_parentWidget(parentWidget)
{
}

void DrawingToolsUIManager::UpdateToolButtonStates()
{
    if (!m_drawingWidget) {
        return;
    }

    auto currentMode = m_drawingWidget->GetDrawMode();

    // Helper lambda to safely set button checked state
    auto setButtonChecked = [this](std::string_view buttonName, bool checked) {
        if (auto* button = m_parentWidget->findChild<QPushButton*>(QString::fromUtf8(buttonName.data(), buttonName.size()))) {
            button->setChecked(checked);
        }
    };

    // Reset all tool buttons
    setButtonChecked(TOOL_PEN, false);
    setButtonChecked(TOOL_ERASER, false);
    setButtonChecked(TOOL_LINE, false);
    setButtonChecked(TOOL_RECTANGLE, false);
    setButtonChecked(TOOL_CIRCLE, false);
    setButtonChecked(TOOL_COLOR_PICKER, false);
    setButtonChecked(WIDGET_BUCKET, false);

    // Highlight active tool
    switch (currentMode) {
        case DrawingWidget::DrawMode::Pen:
            setButtonChecked(TOOL_PEN, true);
            break;
        case DrawingWidget::DrawMode::Eraser:
            setButtonChecked(TOOL_ERASER, true);
            break;
        case DrawingWidget::DrawMode::Line:
            setButtonChecked(TOOL_LINE, true);
            break;
        case DrawingWidget::DrawMode::Rectangle:
            setButtonChecked(TOOL_RECTANGLE, true);
            break;
        case DrawingWidget::DrawMode::Circle:
            setButtonChecked(TOOL_CIRCLE, true);
            break;
        case DrawingWidget::DrawMode::ColorPicker:
            setButtonChecked(TOOL_COLOR_PICKER, true);
            break;
        case DrawingWidget::DrawMode::Fill:
            setButtonChecked(WIDGET_BUCKET, true);
            break;
    }
}

void DrawingToolsUIManager::SetPenColor(const QColor& color)
{
    if (!m_drawingWidget) {
        return;
    }

    m_drawingWidget->SetPenColor(color);
    m_drawingWidget->SetCurrentFillColor(color);

    // Switch to pen mode when selecting a color (unless already using a drawing tool)
    auto currentMode = m_drawingWidget->GetDrawMode();
    if (currentMode == DrawingWidget::DrawMode::Eraser ||
        currentMode == DrawingWidget::DrawMode::Fill ||
        currentMode == DrawingWidget::DrawMode::ColorPicker) {
        m_drawingWidget->SetDrawMode(DrawingWidget::DrawMode::Pen);
        UpdateToolButtonStates();
    }

    UpdateColorPreview(color);
}

void DrawingToolsUIManager::SetBrushSize(int size)
{
    if (m_drawingWidget) {
        m_drawingWidget->SetPenWidth(size);
    }
}

void DrawingToolsUIManager::ToggleFillMode()
{
    if (m_drawingWidget) {
        m_drawingWidget->ToggleFillMode();
        UpdateToolButtonStates();
    }
}

void DrawingToolsUIManager::SetDrawMode(DrawingWidget::DrawMode mode)
{
    if (m_drawingWidget) {
        m_drawingWidget->SetDrawMode(mode);
        UpdateToolButtonStates();
    }
}

void DrawingToolsUIManager::SetAntiAliasing(bool enabled)
{
    if (m_drawingWidget) {
        m_drawingWidget->SetAntiAliasing(enabled);
    }
}

void DrawingToolsUIManager::ClearDrawing()
{
    if (m_drawingWidget) {
        m_drawingWidget->ClearDrawing();
    }
}

void DrawingToolsUIManager::ClearUndoHistory()
{
    if (m_drawingWidget) {
        m_drawingWidget->ClearUndoHistory();
    }
}

void DrawingToolsUIManager::Undo()
{
    if (m_drawingWidget) {
        m_drawingWidget->Undo();
    }
}

void DrawingToolsUIManager::Redo()
{
    if (m_drawingWidget) {
        m_drawingWidget->Redo();
    }
}

void DrawingToolsUIManager::SetDrawingUIVisibility(bool visible) noexcept
{
    if (!m_parentWidget) {
        return;
    }

    // Primary controls and color buttons
    const std::array<std::string_view, 18> widgetNames = {
        WIDGET_CLEAR, COLOR_GREEN, COLOR_RED, COLOR_BLUE,
        COLOR_ORANGE, COLOR_BROWN, COLOR_PURPLE, COLOR_WHITE,
        COLOR_BLACK, COLOR_GREY, COLOR_YELLOW, COLOR_PINK,
        COLOR_TURQUOISE, WIDGET_SETTINGS, WIDGET_BUCKET,
        WIDGET_BRUSH_SIZE, WIDGET_UNDO, WIDGET_REDO
    };

    // Tool buttons and additional controls
    const std::array<std::string_view, 7> toolNames = {
        TOOL_LINE, TOOL_RECTANGLE, TOOL_CIRCLE,
        TOOL_PEN, TOOL_ERASER, TOOL_COLOR_PICKER,
        WIDGET_ANTI_ALIASING
    };

    // Set visibility for all widgets
    auto setVisibility = [this, visible](std::string_view widgetName) {
        if (auto* widget = m_parentWidget->findChild<QWidget*>(QString::fromUtf8(widgetName.data(), widgetName.size()))) {
            widget->setVisible(visible);
        }
    };

    for (const auto& name : widgetNames) {
        setVisibility(name);
    }

    for (const auto& name : toolNames) {
        setVisibility(name);
    }

    // Color preview label
    setVisibility(WIDGET_COLOR_PREVIEW);
}

void DrawingToolsUIManager::OnColorPicked(const QColor& color)
{
    // When color is picked, automatically switch back to pen mode
    SetPenColor(color);
}

void DrawingToolsUIManager::UpdateColorPreview(const QColor& color)
{
    if (auto* colorPreview = m_parentWidget->findChild<QLabel*>(QString::fromUtf8(WIDGET_COLOR_PREVIEW.data(), WIDGET_COLOR_PREVIEW.size()))) {
        colorPreview->setStyleSheet(
            QString("background-color: %1; border: 2px solid black;").arg(color.name())
        );
    }
}

QWidget* DrawingToolsUIManager::FindChild(const QString& name) const noexcept
{
    return m_parentWidget ? m_parentWidget->findChild<QWidget*>(name) : nullptr;
}

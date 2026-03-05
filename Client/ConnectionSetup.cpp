#include "ConnectionSetup.h"
#include "Game.h"

ConnectionSetup::ConnectionSetup(Ui::GameClass& ui, Game* game, NetworkWorker* networkWorker, GameTimerManager* timerManager, DrawingToolsUIManager* drawingToolsManager)
	: m_ui(ui)
	, m_game(game)
	, m_networkWorker(networkWorker)
	, m_timerManager(timerManager)
	, m_drawingToolsManager(drawingToolsManager)
{
}

void ConnectionSetup::SetupNetworkConnections()
{
	// Network worker signals
	QObject::connect(m_networkWorker, &NetworkWorker::RoomUpdateReceived, m_game, &Game::OnRoomUpdateReceived);
	QObject::connect(m_networkWorker, &NetworkWorker::MessageSent, m_game, &Game::OnMessageSent);

	// Timer signals
	QObject::connect(m_timerManager, &GameTimerManager::UpdateTimerTick, m_game, &Game::OnUpdateTimerTick);
}

void ConnectionSetup::SetupColorButtonConnections()
{
	// Color button mappings: UI button -> color palette key
	static const std::array<std::pair<QPushButton*, std::string_view>, 13> colorButtons = { {
		{m_ui.GreenButton, "green"},
		{m_ui.RedButton, "red"},
		{m_ui.BlueButton, "blue"},
		{m_ui.OrangeButton, "orange"},
		{m_ui.BrownButton, "brown"},
		{m_ui.PurpleButton, "purple"},
		{m_ui.WhiteButton, "white"},
		{m_ui.BlackButton, "black"},
		{m_ui.GreyButton, "grey"},
		{m_ui.YellowButton, "yellow"},
		{m_ui.PinkButton, "pink"},
		{m_ui.TurquoiseButton, "turquoise"}
	} };

	// Connect all color buttons using the mapping
	for (const auto& [button, colorKey] : colorButtons) {
		QObject::connect(button, &QPushButton::clicked, m_game, [this, colorKey]() {
			m_drawingToolsManager->SetPenColor(DrawingToolsUIManager::COLOR_PALETTE.at(QString::fromUtf8(colorKey.data(), colorKey.size()))); });
	}

	// Bucket tool (fill mode)
	QObject::connect(m_ui.Bucket, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->ToggleFillMode(); });
}

void ConnectionSetup::SetupToolButtonConnections()
{
	// Clear and undo/redo buttons
	QObject::connect(m_ui.Clear, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->ClearDrawing(); });
	QObject::connect(m_ui.Undo, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->Undo(); });
	QObject::connect(m_ui.Redo, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->Redo(); });

	// Brush size slider
	QObject::connect(m_ui.BrushSize, &QSlider::valueChanged, m_game, [this](int size) { m_drawingToolsManager->SetBrushSize(size); });

	// Drawing mode buttons
	QObject::connect(m_ui.LineTool, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->SetDrawMode(DrawingWidget::DrawMode::Line); });
	QObject::connect(m_ui.RectangleTool, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->SetDrawMode(DrawingWidget::DrawMode::Rectangle); });
	QObject::connect(m_ui.CircleTool, &QPushButton::clicked, m_game, [this]() {	m_drawingToolsManager->SetDrawMode(DrawingWidget::DrawMode::Circle); });
	QObject::connect(m_ui.PenTool, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->SetDrawMode(DrawingWidget::DrawMode::Pen); });
	QObject::connect(m_ui.EraserTool, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->SetDrawMode(DrawingWidget::DrawMode::Eraser); });
	QObject::connect(m_ui.ColorPicker, &QPushButton::clicked, m_game, [this]() { m_drawingToolsManager->SetDrawMode(DrawingWidget::DrawMode::ColorPicker); });

	// Anti-aliasing checkbox
	QObject::connect(m_ui.AntiAliasing, &QCheckBox::toggled, m_game, [this](bool enabled) {	m_drawingToolsManager->SetAntiAliasing(enabled); });

	// DrawingWidget color picker signal
	if (auto* drawingWidget = GetDrawingWidget()) {
		QObject::connect(drawingWidget, &DrawingWidget::colorPicked, m_game, [this](const QColor& color) { m_drawingToolsManager->OnColorPicked(color);	});
	}
}

void ConnectionSetup::SetupGameControlConnections()
{
	// Chat and game control buttons
	QObject::connect(m_ui.SendMesageButton, &QPushButton::clicked, m_game, &Game::OnSendButtonClicked);
	QObject::connect(m_ui.LeaveGame, &QPushButton::clicked, m_game, &Game::OnLeaveButtonClicked);
	QObject::connect(m_game, SIGNAL(PlayerQuit()), m_game, SLOT(OnPlayerQuit()));

	// Initialize default button states
	if (auto* drawingWidget = GetDrawingWidget()) {
		if (m_ui.PenTool) {
			m_ui.PenTool->setChecked(drawingWidget->GetDrawMode() == DrawingWidget::DrawMode::Pen);
		}
		if (m_ui.AntiAliasing) {
			m_ui.AntiAliasing->setChecked(drawingWidget->GetAntiAliasing());
		}
	}
}

void ConnectionSetup::SetupAllConnections()
{
	SetupNetworkConnections();
	SetupColorButtonConnections();
	SetupToolButtonConnections();
	SetupGameControlConnections();
}

DrawingWidget* ConnectionSetup::GetDrawingWidget() const noexcept
{
	return qobject_cast<DrawingWidget*>(m_ui.drawingArea);
}

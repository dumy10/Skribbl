#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	m_loginForm(nullptr),
	m_registerForm(nullptr),
	m_menu(nullptr),
	m_lobby(nullptr),
	m_game(nullptr)
{
	m_stackedWidget = new QStackedWidget(this);
	setCentralWidget(m_stackedWidget);

	// Set fixed size to prevent resizing
	setFixedSize(1070, 690);
	setWindowTitle("Skribbl Client");

	// Create and add ServerConnectForm
	m_serverConnectForm = new ServerConnectForm(this);
	m_stackedWidget->addWidget(m_serverConnectForm);

	// Connect ServerConnectForm signals
	connect(m_serverConnectForm, &ServerConnectForm::NavigateToLogin, this, &MainWindow::ShowLoginForm);

	// Show ServerConnectForm by default
	m_stackedWidget->setCurrentWidget(m_serverConnectForm);
}

MainWindow::~MainWindow()
{
	if (m_loginForm) {
		delete m_loginForm;
	}

	if (m_registerForm) {
		delete m_registerForm;
	}

	if (m_menu) {
		delete m_menu;
	}

	if (m_lobby) {
		delete m_lobby;
	}

	if (m_game) {
		delete m_game;
	}
}

void MainWindow::ShowLoginForm()
{
	if (!m_loginForm) {
		m_loginForm = new LoginForm(this);
		m_stackedWidget->addWidget(m_loginForm);

		connect(m_loginForm, &LoginForm::NavigateToRegister, this, &MainWindow::ShowRegisterForm);
		connect(m_loginForm, &LoginForm::NavigateToMenu, this, &MainWindow::ShowMenu);
	}
	
	m_stackedWidget->setCurrentWidget(m_loginForm);
}

void MainWindow::ShowRegisterForm()
{
	if (!m_registerForm) {
		m_registerForm = new RegisterForm(this);
		m_stackedWidget->addWidget(m_registerForm);

		connect(m_registerForm, &RegisterForm::NavigateToLogin, this, &MainWindow::ShowLoginForm);
	}
	
	m_stackedWidget->setCurrentWidget(m_registerForm);
}

void MainWindow::ShowMenu(const std::string& username)
{
	// Stop lobby timer if it exists
	if (m_lobby) {
		m_lobby->StopTimer();
	}
	
	// Stop game timer if it exists
	if (m_game) {
		m_game->StopTimer();
	}
	
	// Remove old menu if it exists
	if (m_menu) {
		m_stackedWidget->removeWidget(m_menu);
		delete m_menu;
		m_menu = nullptr;
	}

	m_menu = new Menu(username, this);
	m_stackedWidget->addWidget(m_menu);

	connect(m_menu, &Menu::NavigateToLobby, this, &MainWindow::ShowLobby);

	// Resize to default size
	setFixedSize(1070, 690);

	m_stackedWidget->setCurrentWidget(m_menu);
}

void MainWindow::ShowLobby(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID)
{
	// Stop game timer if it exists
	if (m_game) {
		m_game->StopTimer();
	}
	
	// Remove old lobby if it exists
	if (m_lobby) {
		m_stackedWidget->removeWidget(m_lobby);
		delete m_lobby;
		m_lobby = nullptr;
	}

	m_lobby = new Lobby(username, playerIndex, isOwner, roomID, this);
	m_stackedWidget->addWidget(m_lobby);

	connect(m_lobby, &Lobby::NavigateToMenu, this, [this, username]() {
		ShowMenu(username);
	});
	connect(m_lobby, &Lobby::NavigateToGame, this, &MainWindow::ShowGame);

	// Resize to default size
	setFixedSize(1070, 690);

	m_stackedWidget->setCurrentWidget(m_lobby);
}

void MainWindow::ShowGame(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID)
{
	// Stop lobby timer if it exists
	if (m_lobby) {
		m_lobby->StopTimer();
	}
	
	// Remove old game if it exists
	if (m_game) {
		m_stackedWidget->removeWidget(m_game);
		delete m_game;
		m_game = nullptr;
	}

	m_game = new Game(username, playerIndex, isOwner, roomID, this);
	m_stackedWidget->addWidget(m_game);

	connect(m_game, &Game::NavigateToMenu, this, [this, username]() {
		ShowMenu(username);
	});

	// Resize to game size
	setFixedSize(1080, 720);

	m_stackedWidget->setCurrentWidget(m_game);
}

void MainWindow::ReturnToServerConnect()
{
	m_stackedWidget->setCurrentWidget(m_serverConnectForm);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (m_lobby && m_stackedWidget->currentWidget() == m_lobby) {
		emit m_lobby->PlayerLeft();
	}

	if (m_game && m_stackedWidget->currentWidget() == m_game) {
		emit m_game->PlayerQuit();
	}

	QCoreApplication::processEvents();

	QMainWindow::closeEvent(event);
}

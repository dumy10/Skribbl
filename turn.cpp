import turn;
using namespace skribbl;

Turn::Turn(const Player& player):
	m_player{ player }
{
	// Empty
}
void Turn::setPlayerTurn(const Player& player)
{
	this->m_player = player;
}

const Player& Turn::getCurrentPlayer() const noexcept
{
	return this->m_player;
}
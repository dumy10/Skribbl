export module turn;

import player;


namespace skribbl
{
	export class Turn
	{
	public:
		Turn(const Player& player);
		void setPlayerTurn(const Player& player);
		const Player& getCurrentPlayer() const noexcept;

	private:
		Player m_player;
	};
}
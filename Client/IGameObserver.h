#ifndef IGAMEOBSERVER_H
#define IGAMEOBSERVER_H

#include <string>
#include <unordered_map>

/**
 * @brief Enum representing the current state of the game
 */
enum class EGameState
{
	Unknown,
	WaitingForPlayers,
	RoundStarting,
	Drawing,
	Guessing,
	RoundEnding,
	GameEnded
};

/**
 * @brief Observer interface for game events
 * 
 * Classes implementing this interface will be notified of various
 * game events such as state changes, round changes, player updates, etc.
 */
class IGameObserver
{
public:
	/**
	 * @brief Called when the game state changes
	 * @param state The new game state
	 */
	virtual void OnGameStateChanged(EGameState state) = 0;

	/**
	 * @brief Called when a new round starts
	 * @param roundNumber The current round number
	 * @param drawerName The name of the player who is drawing
	 */
	virtual void OnRoundChanged(int roundNumber, const std::string& drawerName) = 0;

	/**
	 * @brief Called when the current player makes a correct guess
	 */
	virtual void OnCorrectGuess() = 0;

	/**
	 * @brief Called when all players have guessed the word
	 */
	virtual void OnAllPlayersGuessed() = 0;

	/**
	 * @brief Called when the round timer updates
	 * @param timeLeft Time remaining in seconds
	 */
	virtual void OnTimerUpdate(int timeLeft) = 0;

	/**
	 * @brief Called when the timer reaches zero
	 */
	virtual void OnTimeEnd() = 0;

	/**
	 * @brief Called when chat messages are updated
	 * @param encodedChat The encoded chat string from server
	 */
	virtual void OnChatUpdated(const std::string& encodedChat) = 0;

	/**
	 * @brief Called when the drawing is updated
	 * @param drawingData The drawing image data
	 */
	virtual void OnDrawingUpdated(const std::string& drawingData) = 0;

	/**
	 * @brief Called when the player list or scores change
	 * @param playerCount Number of players in the room
	 * @param playerScores Map of player names to their scores
	 */
	virtual void OnPlayersUpdated(int playerCount, const std::unordered_map<std::string, std::string>& playerScores) = 0;

	/**
	 * @brief Called when the word is revealed (at round end or when player finishes drawing)
	 * @param word The word that was being drawn
	 */
	virtual void OnWordRevealed(const std::string& word) = 0;

	/**
	 * @brief Called when the game ends
	 * @param winner The name of the winning player
	 */
	virtual void OnGameEnded(const std::string& winner) = 0;

	virtual ~IGameObserver() = default;
};

#endif // IGAMEOBSERVER_H

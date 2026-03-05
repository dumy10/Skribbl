#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <string>

/**
 * @brief Manages the game state and provides queries about current state.
 * 
 * This class encapsulates all game state variables and provides
 * a clean interface for querying and updating game state.
 */
class GameStateManager 
{
public:
    explicit GameStateManager(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID) noexcept;

    // State queries
    [[nodiscard]] bool IsDrawing() const noexcept { return m_isDrawing; }
    [[nodiscard]] bool HasGuessedWord() const noexcept { return m_guessedWord; }
    [[nodiscard]] bool IsOwner() const noexcept { return m_isOwner; }
    [[nodiscard]] int GetPlayerIndex() const noexcept { return m_playerIndex; }
    [[nodiscard]] const std::string& GetUsername() const noexcept { return m_username; }
    [[nodiscard]] const std::string& GetRoomID() const noexcept { return m_roomID; }

    // State mutations
    void SetDrawing(bool isDrawing) noexcept { m_isDrawing = isDrawing; }
    void SetGuessedWord(bool guessed) noexcept { m_guessedWord = guessed; }
    void ResetGuessedWord() noexcept { m_guessedWord = false; }

private:
    std::string m_username;
    std::string m_roomID;
    int m_playerIndex;
    bool m_isOwner;
    bool m_isDrawing{false};
    bool m_guessedWord{false};
};

#endif // GAMESTATEMANAGER_H

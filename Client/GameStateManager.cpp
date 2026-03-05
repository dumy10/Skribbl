#include "GameStateManager.h"

GameStateManager::GameStateManager(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID) noexcept
    : m_username(username)
    , m_roomID(roomID)
    , m_playerIndex(playerIndex)
    , m_isOwner(isOwner)
    , m_isDrawing(false)
    , m_guessedWord(false)
{
}

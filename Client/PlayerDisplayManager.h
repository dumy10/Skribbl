#ifndef PLAYERDISPLAYMANAGER_H
#define PLAYERDISPLAYMANAGER_H

#include <QLabel>
#include <string>
#include <vector>
#include <unordered_map>
#include <array>

/**
 * @brief Manages the display of player information in the game UI.
 * 
 * This class handles updating player names, scores, and visibility
 * of player UI elements. It maintains a cache of player scores.
 */
class PlayerDisplayManager 
{
public:
    explicit PlayerDisplayManager(std::array<QLabel*, 4> nameLabels, std::array<QLabel*, 4> scoreLabels) noexcept;

    // Display operations
    void DisplayPlayer(const std::string& username, int index, const std::string& score) noexcept;
    void DisplayPlayerCount(int count) noexcept;
    void HidePlayers() noexcept;
    void UpdatePlayerList(const std::vector<std::string>& players,const std::unordered_map<std::string, std::string>& scores) noexcept;

    // Score cache access
    [[nodiscard]] const std::unordered_map<std::string, std::string>& GetPlayerScores() const noexcept { return m_playerScores; }

private:
    std::array<QLabel*, 4> m_nameLabels;
    std::array<QLabel*, 4> m_scoreLabels;
    std::unordered_map<std::string, std::string> m_playerScores;
};

#endif // PLAYERDISPLAYMANAGER_H

#include "PlayerDisplayManager.h"
#include "utils.h"

PlayerDisplayManager::PlayerDisplayManager(std::array<QLabel*, 4> nameLabels, std::array<QLabel*, 4> scoreLabels) noexcept
    : m_nameLabels(nameLabels)
    , m_scoreLabels(scoreLabels)
{
}

void PlayerDisplayManager::DisplayPlayer(const std::string& username, int index, const std::string& score) noexcept
{
    if (index < 1 || index > 4) {
        return;
    }

    const size_t arrayIndex = static_cast<size_t>(index - 1);
    Utils::ShowLabelWithText(m_nameLabels[arrayIndex], username);
    Utils::ShowLabelWithText(m_scoreLabels[arrayIndex], score);

    // Update score cache
    m_playerScores[username] = score;
}

void PlayerDisplayManager::DisplayPlayerCount(int count) noexcept
{
    // Convert raw pointers to span for SetWidgetVisibilityByCount
    std::array<QWidget*, 4> nameWidgets{};
    std::array<QWidget*, 4> scoreWidgets{};

    for (size_t i = 0; i < 4; i++) {
        nameWidgets[i] = m_nameLabels[i];
        scoreWidgets[i] = m_scoreLabels[i];
    }

    Utils::SetWidgetVisibilityByCount(nameWidgets, count);
    Utils::SetWidgetVisibilityByCount(scoreWidgets, count);
}

void PlayerDisplayManager::HidePlayers() noexcept
{
    for (auto* label : m_nameLabels) {
        label->hide();
    }
    for (auto* label : m_scoreLabels) {
        label->hide();
    }
}

void PlayerDisplayManager::UpdatePlayerList(const std::vector<std::string>& players, const std::unordered_map<std::string, std::string>& scores) noexcept
{
    if (players.empty()) {
        return;
    }

    DisplayPlayerCount(static_cast<int>(players.size()));

    for (size_t i = 0; i < players.size(); ++i) {
        const auto& playerName = players[i];
        auto it = scores.find(playerName);
        const std::string& score = (it != scores.end()) ? it->second : "0";
        DisplayPlayer(playerName, static_cast<int>(i + 1), score);
    }
}

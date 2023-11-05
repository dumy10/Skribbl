export module player;
import <string>;

namespace skribbl 
{
    export class Player
    {
    public:
        void setPlayerPoints(int points);
        int getPlayerPoints() const;
        const std::string_view getPlayerName() const noexcept;
        const std::string_view getPlayerPassword() const noexcept;
        Player() = default;
        Player(const std::string& name, const std::string& password, const std::string& email);
        Player(const Player& player);
        bool operator<(const Player& player);
        bool operator>(const Player& player);
    private:
        std::string m_name;
        std::string m_password;
        std::string m_email;
        int m_points;
    };
}


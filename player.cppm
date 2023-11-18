export module player;
import <string>;

namespace skribbl 
{
    export class Player
    {
    public:
        Player();
        Player(const std::string& name, const std::string& password, const std::string& email);

        const std::string_view getName() const noexcept;
        const std::string_view getPassword() const noexcept;
        const std::string_view getEmail() const noexcept;
        int getPoints() const;

        void setName(const std::string& name);
		void setPassword(const std::string& password);
        void setEmail(const std::string& email);
        void setPoints(int points);
        void addPoints(int points);


        bool operator<(const Player& player);
        bool operator>(const Player& player);
        bool operator==(const Player& player);
    private:
        std::string m_name;
        std::string m_password;
        std::string m_email;
        int m_points;
    };
}


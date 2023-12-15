export module player;
import <string>;

namespace skribbl 
{
    export class Player
    {
    public:
        Player();
        Player(int id, const std::string& name, const std::string& password, const std::string& email);

        const std::string getName() const noexcept;
        const std::string getPassword() const noexcept;
        const std::string getEmail() const noexcept;
        int getPoints() const;
        int getId() const;

        void setId(int id);
        void setName(const std::string& name);
		void setPassword(const std::string& password);
        void setEmail(const std::string& email);
        void setPoints(int points);
        void addPoints(int points);
        void substractPoints(int points);
        void makeAdmin();
        void removeAdmin();

        void setGuessedTheWord(bool guessed);

        bool operator<(const Player& player);
        bool operator>(const Player& player);
        bool operator==(const Player& player);
    private:
        int m_id;
        std::string m_name;
        std::string m_password;
        std::string m_email;
        int m_points;
        bool m_guessedTheWord{ false };
        bool is_admin;
    };
}


export module player;
import <string>;

namespace skribbl
{
	export class Player
	{
	public:
		Player() = default;
		Player(int id, const std::string& name, const std::string& password, const std::string& email);

		const std::string GetName() const noexcept;
		const std::string GetPassword() const noexcept;
		const std::string GetEmail() const noexcept;
		int GetPoints() const;
		int GetId() const;
		bool IsDrawing() const;
		bool HasGuessedTheWord() const;

		void SetId(int id);
		void SetName(const std::string& name);
		void SetPassword(const std::string& password);
		void SetEmail(const std::string& email);
		void SetPoints(int points);
		void AddPoints(int points);
		void SubstractPoints(int points);
		void SetDrawing(bool IsDrawing);
		void SetGuessedTheWord(bool guessed);

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
		bool m_isDrawing{ false };
	};
}


export module player;
import <string>;
import <sstream>;
import <vector>;

namespace skribbl
{
	export class Player
	{
	public:
		Player() = default;
		Player(int id, const std::string& name, const std::string& password, const std::string& email);

		[[nodiscard]] const int GetId() const noexcept;
		[[nodiscard]] const int GetPoints() const noexcept;

		[[nodiscard]] const std::string GetName() const noexcept;
		[[nodiscard]] const std::string GetPassword() const noexcept;
		[[nodiscard]] const std::string GetEmail() const noexcept;
		[[nodiscard]] const std::string Serialize() const noexcept;

		[[nodiscard]] const Player Deserialize(const std::string& serializedPlayer) const;

		void SetId(int id);
		void SetName(const std::string& name);
		void SetPassword(const std::string& password);
		void SetEmail(const std::string& email);

		void SetPoints(int points);
		void AddPoints(int points);

		[[nodiscard]] bool operator<(const Player& player);
		[[nodiscard]] bool operator>(const Player& player);
		[[nodiscard]] bool operator==(const Player& player);

	private:
		int m_id;
		int m_points;
		std::string m_name;
		std::string m_email;
		std::string m_password;
	};
}

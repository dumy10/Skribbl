#include "CppUnitTest.h"

import game;
import player;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace skribbl;

namespace SkribblTests
{
	// Helper: creates a simple Player
	static Player MakePlayer(int id, const std::string& name, int points = 0)
	{
		Player p(id, name, "pw", "salt", name + "@test.com");
		p.SetPoints(points);
		return p;
	}

	TEST_CLASS(GameTests)
	{
	public:

		// ── Construction ────────────────────────────────────────────────────

		TEST_METHOD(Constructor_InitialStatusIsWaiting)
		{
			Player p = MakePlayer(1, "Alice");
			Game g(1, p, "CODE01", 4, 1);
			Assert::AreEqual(static_cast<int>(GameStatus::WAITING), g.GetGameStatusAsInt());
		}

		TEST_METHOD(Constructor_IdIsSet)
		{
			Player p = MakePlayer(1, "Alice");
			Game g(99, p, "CODE01", 4, 1);
			Assert::AreEqual(99, g.GetId());
		}

		TEST_METHOD(Constructor_GameCodeIsSet)
		{
			Player p = MakePlayer(1, "Alice");
			Game g(1, p, "MYCODE", 4, 1);
			Assert::AreEqual(std::string("MYCODE"), g.GetGameCode());
		}

		TEST_METHOD(Constructor_MaxPlayersIsSet)
		{
			Player p = MakePlayer(1, "Alice");
			Game g(1, p, "CODE01", 6, 1);
			Assert::AreEqual(size_t(6), g.GetNumberOfMaxPlayers());
		}

		TEST_METHOD(Constructor_StarterPlayerPresent)
		{
			Player p = MakePlayer(1, "Alice");
			Game g(1, p, "CODE01", 4, 1);
			Assert::AreEqual(size_t(1), g.GetCurrentPlayerCount());
		}

		TEST_METHOD(Constructor_WelcomeMessageInChat)
		{
			Player p = MakePlayer(1, "Alice");
			Game g(1, p, "CODE01", 4, 1);
			Assert::IsTrue(g.GetChatLines().size() >= 1);
		}

		// ── AddPlayer / RemovePlayer ──────────────────────────────────────────

		TEST_METHOD(AddPlayer_IncreasesPlayerCount)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			Player p2 = MakePlayer(2, "Bob");
			g.AddPlayer(p2);
			Assert::AreEqual(size_t(2), g.GetCurrentPlayerCount());
		}

		TEST_METHOD(AddPlayer_MultiplePlayersAreTracked)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.AddPlayer(MakePlayer(3, "Charlie"));
			Assert::AreEqual(size_t(3), g.GetCurrentPlayerCount());
		}

		TEST_METHOD(RemovePlayer_DecreasesPlayerCount)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			Player p2 = MakePlayer(2, "Bob");
			g.AddPlayer(p2);
			g.RemovePlayer(p2);
			Assert::AreEqual(size_t(1), g.GetCurrentPlayerCount());
		}

		TEST_METHOD(RemovePlayer_NonExistentPlayer_DoesNotThrow)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			Player ghost = MakePlayer(99, "Ghost");
			g.RemovePlayer(ghost); // should silently do nothing
			Assert::AreEqual(size_t(1), g.GetCurrentPlayerCount());
		}

		// ── GetPlayerIndex ────────────────────────────────────────────────────

		TEST_METHOD(GetPlayerIndex_ExistingPlayer_ReturnsCorrectIndex)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			Assert::AreEqual(0, g.GetPlayerIndex("Alice"));
		}

		TEST_METHOD(GetPlayerIndex_SecondPlayer_ReturnsOne)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			Assert::AreEqual(1, g.GetPlayerIndex("Bob"));
		}

		TEST_METHOD(GetPlayerIndex_NonExistentPlayer_ReturnsMinusOne)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			Assert::AreEqual(-1, g.GetPlayerIndex("Nobody"));
		}

		// ── GetPlayerScore ────────────────────────────────────────────────────

		TEST_METHOD(GetPlayerScore_ExistingPlayer_ReturnsScore)
		{
			Player p1 = MakePlayer(1, "Alice", 80);
			Game g(1, p1, "CODE01", 4, 1);
			Assert::AreEqual(80, g.GetPlayerScore("Alice"));
		}

		TEST_METHOD(GetPlayerScore_NonExistentPlayer_ReturnsZero)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			Assert::AreEqual(0, g.GetPlayerScore("Nobody"));
		}

		// ── GetWinningPlayerName ──────────────────────────────────────────────

		TEST_METHOD(GetWinningPlayerName_SinglePlayer_ReturnsTheirName)
		{
			Player p1 = MakePlayer(1, "Alice", 50);
			Game g(1, p1, "CODE01", 4, 1);
			Assert::AreEqual(std::string("Alice"), g.GetWinningPlayerName());
		}

		TEST_METHOD(GetWinningPlayerName_ReturnsHighestScorer)
		{
			Player p1 = MakePlayer(1, "Alice", 30);
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob",     80));
			g.AddPlayer(MakePlayer(3, "Charlie", 50));
			Assert::AreEqual(std::string("Bob"), g.GetWinningPlayerName());
		}

		// ── GetAllPlayersGuessedWord ──────────────────────────────────────────

		TEST_METHOD(GetAllPlayersGuessedWord_SinglePlayer_ReturnsFalse)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			// Even if Alice "guessed", with only 1 player the method returns false.
			Assert::IsFalse(g.GetAllPlayersGuessedWord());
		}

		// ── SetGameCode / SetGameStatus ───────────────────────────────────────

		TEST_METHOD(SetGameCode_UpdatesCode)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "OLD", 4, 1);
			g.SetGameCode("NEW");
			Assert::AreEqual(std::string("NEW"), g.GetGameCode());
		}

		TEST_METHOD(SetGameStatus_UpdatesStatus)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.SetGameStatus(GameStatus::INPROGRESS);
			Assert::AreEqual(static_cast<int>(GameStatus::INPROGRESS), g.GetGameStatusAsInt());
		}

		TEST_METHOD(SetGameStatusFromInt_SetsCorrectly)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.SetGameStatusFromInt(3); // FINISHED = 3
			Assert::AreEqual(static_cast<int>(GameStatus::FINISHED), g.GetGameStatusAsInt());
		}

		TEST_METHOD(SetMaxPlayers_UpdatesMax)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.SetMaxPlayers(8);
			Assert::AreEqual(size_t(8), g.GetNumberOfMaxPlayers());
		}

		// ── Serialization ────────────────────────────────────────────────────

		TEST_METHOD(SerializePlayers_SinglePlayer_NoTrailingSemicolon)
		{
			Player p1 = MakePlayer(1, "Alice", 0);
			Game g(1, p1, "CODE01", 4, 1);
			std::string s = g.SerializePlayers();
			// Should not end with ';'
			Assert::IsFalse(s.empty());
			Assert::AreNotEqual(';', s.back());
		}

		TEST_METHOD(SerializePlayers_MultiplePlayers_SeparatedBySemicolon)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			std::string s = g.SerializePlayers();
			// Two players → exactly one semicolon between them
			Assert::IsTrue(s.find(';') != std::string::npos);
		}

		TEST_METHOD(SerializePlayerNames_SinglePlayer)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			Assert::AreEqual(std::string("Alice"), g.SerializePlayerNames());
		}

		TEST_METHOD(SerializePlayerNames_MultiplePlayers_CommaSeparated)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			std::string s = g.SerializePlayerNames();
			Assert::AreEqual(std::string("Alice,Bob"), s);
		}

		TEST_METHOD(SerializeGameChat_ContainsWelcomeMessage)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			std::string chat = g.SerializeGameChat();
			Assert::IsTrue(chat.find("Welcome") != std::string::npos);
		}

		// ── DeserializePlayers ────────────────────────────────────────────────

		TEST_METHOD(DeserializePlayers_RoundTrip)
		{
			Player p1 = MakePlayer(1, "Alice", 10);
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob", 20));

			std::string serialized = g.SerializePlayers();
			g.DeserializePlayers(serialized);

			Assert::AreEqual(size_t(2), g.GetPlayers().size());
			Assert::AreEqual(std::string("Alice"), g.GetPlayers()[0].GetName());
			Assert::AreEqual(std::string("Bob"),   g.GetPlayers()[1].GetName());
		}

		// ── DeserializeGameChat ───────────────────────────────────────────────

		TEST_METHOD(DeserializeGameChat_RoundTrip)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			// The game already has the welcome message; add a custom one.
			bool guessed = false;
			g.AddChatLineMessage("nice draw", "Bob", guessed);

			std::string serialized = g.SerializeGameChat();
			g.DeserializeGameChat(serialized);

			auto chat = g.GetChatLines();
			Assert::IsTrue(!chat.empty());
		}

		// ── StartGame / EndGame ───────────────────────────────────────────────

		TEST_METHOD(StartGame_SetsStatusToInProgress)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));

			bool started = g.StartGame({ "apple", "banana", "cherry", "date",
			                             "elderberry", "fig", "grape", "honeydew" });
			Assert::IsTrue(started);
			Assert::AreEqual(static_cast<int>(GameStatus::INPROGRESS), g.GetGameStatusAsInt());
		}

		TEST_METHOD(StartGame_ResetsAllPlayerPointsToZero)
		{
			Player p1 = MakePlayer(1, "Alice", 100);
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob", 200));

			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			for (const auto& player : g.GetPlayers()) {
				Assert::AreEqual(0, player.GetPoints());
			}
		}

		TEST_METHOD(EndGame_SetsStatusToFinished)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			bool ended = g.EndGame();
			Assert::IsTrue(ended);
			Assert::AreEqual(static_cast<int>(GameStatus::FINISHED), g.GetGameStatusAsInt());
		}

		// ── AddChatLineMessage ────────────────────────────────────────────────

		TEST_METHOD(AddChatLineMessage_WrongGuess_AppendsChatLine)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			bool guessed = false;
			g.AddChatLineMessage("wrongword", "Bob", guessed);

			Assert::IsFalse(guessed);
			auto chat = g.GetChatLines();
			bool found = false;
			for (const auto& line : chat) {
				if (line.find("Bob: wrongword") != std::string::npos) {
					found = true;
					break;
				}
			}
			Assert::IsTrue(found);
		}

		TEST_METHOD(AddChatLineMessage_CorrectGuess_SetsGuessedTrue)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));

			// Words set is sorted; "apple" will be the first round word.
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			bool guessed = false;
			g.AddChatLineMessage("apple", "Bob", guessed);

			Assert::IsTrue(guessed);
		}

		TEST_METHOD(AddChatLineMessage_CorrectGuess_CaseInsensitive)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			bool guessed = false;
			g.AddChatLineMessage("APPLE", "Bob", guessed);
			Assert::IsTrue(guessed);
		}

		TEST_METHOD(AddChatLineMessage_CorrectGuess_AddsPointsToGuesser)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			bool guessed = false;
			g.AddChatLineMessage("apple", "Bob", guessed);

			Assert::IsTrue(g.GetPlayerScore("Bob") > 0);
		}

		TEST_METHOD(AddChatLineMessage_CorrectGuess_AddsGuessedWordMessageToChat)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			bool guessed = false;
			g.AddChatLineMessage("apple", "Bob", guessed);

			auto chat = g.GetChatLines();
			bool found = false;
			for (const auto& line : chat) {
				if (line.find("Bob") != std::string::npos &&
				    line.find("guessed") != std::string::npos) {
					found = true;
					break;
				}
			}
			Assert::IsTrue(found);
		}

		TEST_METHOD(AddChatLineMessage_ReturnsTrue)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			bool guessed = false;
			Assert::IsTrue(g.AddChatLineMessage("hi", "Alice", guessed));
		}

		// ── IsLastRound ───────────────────────────────────────────────────────

		TEST_METHOD(IsLastRound_NotLastRound_ReturnsFalse)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });
			// Round 1 / player Alice is drawing; last round is round 4 / last player.
			Assert::IsFalse(g.IsLastRound());
		}

		// ── GetMaxNumberOfRoundsAllowed ───────────────────────────────────────

		TEST_METHOD(GetMaxNumberOfRoundsAllowed_ReturnsFour)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			Assert::AreEqual(4, g.GetMaxNumberOfRoundsAllowed());
		}

		// ── Move semantics ────────────────────────────────────────────────────

		TEST_METHOD(MoveConstructor_TransfersId)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g1(77, p1, "CODE01", 4, 1);
			Game g2(std::move(g1));
			Assert::AreEqual(77, g2.GetId());
		}

		TEST_METHOD(MoveAssignment_TransfersGameCode)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g1(1, p1, "ALPHA", 4, 1);

			Player p2 = MakePlayer(2, "Bob");
			Game g2(2, p2, "BETA", 4, 1);
			g2 = std::move(g1);

			Assert::AreEqual(std::string("ALPHA"), g2.GetGameCode());
		}

		// ── NextRound ─────────────────────────────────────────────────────────

		TEST_METHOD(NextRound_AdvancesDrawingPlayer)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			std::string firstDrawer = g.GetDrawingPlayer();
			g.NextRound();
			std::string secondDrawer = g.GetDrawingPlayer();

			Assert::AreNotEqual(firstDrawer, secondDrawer);
		}

		TEST_METHOD(NextRound_ReturnsTrue_WhenGameNotOver)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);
			g.AddPlayer(MakePlayer(2, "Bob"));
			g.StartGame({ "apple", "banana", "cherry", "date",
			              "elderberry", "fig", "grape", "honeydew" });

			Assert::IsTrue(g.NextRound());
		}

		// ── SetPlayers / GetPlayers ───────────────────────────────────────────

		TEST_METHOD(SetPlayers_ReplacesPlayerList)
		{
			Player p1 = MakePlayer(1, "Alice");
			Game g(1, p1, "CODE01", 4, 1);

			std::vector<Player> newPlayers = { MakePlayer(3, "Dave"), MakePlayer(4, "Eve") };
			g.SetPlayers(newPlayers);

			Assert::AreEqual(size_t(2), g.GetPlayers().size());
			Assert::AreEqual(std::string("Dave"), g.GetPlayers()[0].GetName());
		}
	};
}

#include "CppUnitTest.h"

import round;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace skribbl;

namespace SkribblTests
{
	TEST_CLASS(RoundTests)
	{
	public:

		// ── Construction ────────────────────────────────────────────────────

		TEST_METHOD(Constructor_SetsInitialRoundNumberToOne)
		{
			Round r("GAME1", { "apple", "banana", "cherry" }, 3);
			Assert::AreEqual(uint8_t(1), r.GetRoundNumber());
		}

		TEST_METHOD(Constructor_DrawingPlayerIsEmpty)
		{
			Round r("GAME1", { "apple" }, 2);
			Assert::AreEqual(std::string(""), r.GetDrawingPlayer());
		}

		TEST_METHOD(Constructor_CurrentWordIsEmpty)
		{
			Round r("GAME1", { "apple" }, 2);
			Assert::AreEqual(std::string(""), r.GetCurrentWord());
		}

		TEST_METHOD(Constructor_ImageDataIsEmpty)
		{
			Round r("GAME1", { "apple" }, 2);
			Assert::AreEqual(std::string(""), r.GetImageData());
		}

		TEST_METHOD(Constructor_TimesVectorHasCorrectSize)
		{
			Round r("GAME1", { "apple" }, 4);
			Assert::AreEqual(size_t(4), r.GetTimes().size());
		}

		TEST_METHOD(Constructor_AllTimesInitialisedToZero)
		{
			Round r("GAME1", { "apple" }, 3);
			for (int t : r.GetTimes()) {
				Assert::AreEqual(0, t);
			}
		}

		// ── StartRound ───────────────────────────────────────────────────────

		TEST_METHOD(StartRound_SetsDrawingPlayer)
		{
			Round r("GAME1", { "apple", "banana" }, 2);
			r.StartRound("Alice", 1);
			Assert::AreEqual(std::string("Alice"), r.GetDrawingPlayer());
		}

		TEST_METHOD(StartRound_SetsRoundNumber)
		{
			Round r("GAME1", { "apple", "banana" }, 2);
			r.StartRound("Alice", 2);
			Assert::AreEqual(uint8_t(2), r.GetRoundNumber());
		}

		TEST_METHOD(StartRound_SetsCurrentWordFromWordSet)
		{
			// The set is ordered alphabetically; first word will be "apple".
			Round r("GAME1", { "apple", "zebra" }, 2);
			r.StartRound("Alice", 1);
			Assert::AreEqual(std::string("apple"), r.GetCurrentWord());
		}

		TEST_METHOD(StartRound_ConsumesWordFromSet)
		{
			// After one StartRound the next call should pick the second word.
			Round r("GAME1", { "apple", "zebra" }, 2);
			r.StartRound("Alice", 1);
			r.StopRound();
			r.StartRound("Bob", 2);
			// "apple" was consumed; next in sorted order is "zebra"
			Assert::AreEqual(std::string("zebra"), r.GetCurrentWord());
		}

		TEST_METHOD(StartRound_ClearsPreviousGuesses)
		{
			Round r("GAME1", { "apple", "banana" }, 2);
			r.StartRound("Alice", 1);
			r.PlayerGuessedWord("Bob", 1, 30);
			Assert::AreEqual(size_t(1), r.GetGuessedPlayerNames().size());

			r.StopRound();
			r.StartRound("Bob", 2);
			Assert::AreEqual(size_t(0), r.GetGuessedPlayerNames().size());
		}

		TEST_METHOD(StartRound_ResetsTimesToZero)
		{
			Round r("GAME1", { "apple", "banana" }, 2);
			r.StartRound("Alice", 1);
			r.UpdateTimes(0, 45);
			r.StopRound();
			r.StartRound("Bob", 2);
			Assert::AreEqual(0, r.GetTimes()[0]);
		}

		TEST_METHOD(StartRound_ClearsImageData)
		{
			Round r("GAME1", { "apple", "banana" }, 2);
			r.StartRound("Alice", 1);
			r.SetImageData("somedata");
			r.StopRound();
			r.StartRound("Bob", 2);
			Assert::AreEqual(std::string(""), r.GetImageData());
		}

		TEST_METHOD(StartRound_ReturnsTrue)
		{
			Round r("GAME1", { "apple" }, 1);
			Assert::IsTrue(r.StartRound("Alice", 1));
		}

		// ── StopRound ────────────────────────────────────────────────────────

		TEST_METHOD(StopRound_ReturnsTrue)
		{
			Round r("GAME1", { "apple" }, 1);
			r.StartRound("Alice", 1);
			Assert::IsTrue(r.StopRound());
		}

		TEST_METHOD(StopRound_ClearsImageData)
		{
			Round r("GAME1", { "apple" }, 1);
			r.StartRound("Alice", 1);
			r.SetImageData("drawing");
			r.StopRound();
			Assert::AreEqual(std::string(""), r.GetImageData());
		}

		// ── SetImageData / GetImageData ───────────────────────────────────────

		TEST_METHOD(SetImageData_StoresData)
		{
			Round r("GAME1", { "apple" }, 1);
			r.SetImageData("base64encodeddata");
			Assert::AreEqual(std::string("base64encodeddata"), r.GetImageData());
		}

		// ── PlayerGuessedWord ─────────────────────────────────────────────────

		TEST_METHOD(PlayerGuessedWord_AddsPlayerToGuessedSet)
		{
			Round r("GAME1", { "apple" }, 3);
			r.StartRound("Alice", 1);
			r.PlayerGuessedWord("Bob", 1, 40);
			Assert::IsTrue(r.GetGuessedPlayerNames().count("Bob") == 1);
		}

		TEST_METHOD(PlayerGuessedWord_UpdatesTimeForPlayer)
		{
			Round r("GAME1", { "apple" }, 3);
			r.StartRound("Alice", 1);
			r.PlayerGuessedWord("Bob", 1, 40);
			Assert::AreEqual(40, r.GetTimes()[1]);
		}

		TEST_METHOD(PlayerGuessedWord_MultipleGuessers)
		{
			Round r("GAME1", { "apple" }, 4);
			r.StartRound("Alice", 1);
			r.PlayerGuessedWord("Bob",     1, 50);
			r.PlayerGuessedWord("Charlie", 2, 30);
			Assert::AreEqual(size_t(2), r.GetGuessedPlayerNames().size());
		}

		TEST_METHOD(PlayerGuessedWord_DuplicatePlayer_CountsOnce)
		{
			Round r("GAME1", { "apple" }, 3);
			r.StartRound("Alice", 1);
			r.PlayerGuessedWord("Bob", 1, 50);
			r.PlayerGuessedWord("Bob", 1, 40); // Same player guesses again
			Assert::AreEqual(size_t(1), r.GetGuessedPlayerNames().size());
		}

		// ── ClearAllPlayersGuessed ────────────────────────────────────────────

		TEST_METHOD(ClearAllPlayersGuessed_EmptiesSet)
		{
			Round r("GAME1", { "apple" }, 3);
			r.StartRound("Alice", 1);
			r.PlayerGuessedWord("Bob", 1, 30);
			r.PlayerGuessedWord("Eve", 2, 20);
			r.ClearAllPlayersGuessed();
			Assert::AreEqual(size_t(0), r.GetGuessedPlayerNames().size());
		}

		// ── UpdateTimes ───────────────────────────────────────────────────────

		TEST_METHOD(UpdateTimes_ValidIndex_SetsValue)
		{
			Round r("GAME1", { "apple" }, 3);
			r.UpdateTimes(0, 55);
			Assert::AreEqual(55, r.GetTimes()[0]);
		}

		TEST_METHOD(UpdateTimes_NegativeIndex_DoesNotThrow)
		{
			Round r("GAME1", { "apple" }, 3);
			// Should silently return; no crash.
			r.UpdateTimes(-1, 55);
		}

		TEST_METHOD(UpdateTimes_IndexEqualToSize_DoesNotThrow)
		{
			Round r("GAME1", { "apple" }, 3);
			r.UpdateTimes(3, 55); // index == size (out of bounds)
		}

		TEST_METHOD(UpdateTimes_IndexBeyondSize_DoesNotThrow)
		{
			Round r("GAME1", { "apple" }, 3);
			r.UpdateTimes(100, 55);
		}

		// ── CalculatePoints ───────────────────────────────────────────────────

		TEST_METHOD(CalculatePoints_TimeAtThirty_ReturnsOneHundred)
		{
			Round r("GAME1", { "apple" }, 1);
			Assert::AreEqual(100, r.CalculatePoints(30));
		}

		TEST_METHOD(CalculatePoints_TimeAboveThirty_ReturnsOneHundred)
		{
			Round r("GAME1", { "apple" }, 1);
			Assert::AreEqual(100, r.CalculatePoints(60));
			Assert::AreEqual(100, r.CalculatePoints(45));
		}

		TEST_METHOD(CalculatePoints_TimeZero_ReturnsOneHundred)
		{
			// ((60 - 0) * 100) / 60 = 100
			Round r("GAME1", { "apple" }, 1);
			Assert::AreEqual(100, r.CalculatePoints(0));
		}

		TEST_METHOD(CalculatePoints_TimeTwentyNine_UsesFormula)
		{
			// time < 30: ((60 - 29) * 100) / 60 = 3100 / 60 = 51
			Round r("GAME1", { "apple" }, 1);
			Assert::AreEqual(51, r.CalculatePoints(29));
		}

		TEST_METHOD(CalculatePoints_TimeOne_UsesFormula)
		{
			// ((60 - 1) * 100) / 60 = 5900 / 60 = 98
			Round r("GAME1", { "apple" }, 1);
			Assert::AreEqual(98, r.CalculatePoints(1));
		}

		// ── Move semantics ────────────────────────────────────────────────────

		TEST_METHOD(MoveConstructor_TransfersState)
		{
			Round r1("GAME1", { "apple", "banana" }, 2);
			r1.StartRound("Alice", 1);
			r1.SetImageData("img");
			r1.PlayerGuessedWord("Bob", 1, 30);

			Round r2(std::move(r1));

			Assert::AreEqual(uint8_t(1),              r2.GetRoundNumber());
			Assert::AreEqual(std::string("Alice"),    r2.GetDrawingPlayer());
			Assert::AreEqual(std::string("apple"),    r2.GetCurrentWord());
			Assert::AreEqual(std::string("img"),      r2.GetImageData());
			Assert::IsTrue(r2.GetGuessedPlayerNames().count("Bob") == 1);
		}

		TEST_METHOD(MoveAssignment_TransfersState)
		{
			Round r1("GAME1", { "apple", "banana" }, 2);
			r1.StartRound("Alice", 2);
			r1.SetImageData("imgdata");

			Round r2("GAME2", { "cherry" }, 1);
			r2 = std::move(r1);

			Assert::AreEqual(uint8_t(2),                r2.GetRoundNumber());
			Assert::AreEqual(std::string("Alice"),      r2.GetDrawingPlayer());
			Assert::AreEqual(std::string("imgdata"),    r2.GetImageData());
		}
	};
}

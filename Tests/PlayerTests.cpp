#include "CppUnitTest.h"

import player;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace skribbl;

namespace SkribblTests
{
	TEST_CLASS(PlayerTests)
	{
	public:

		// ── Construction ────────────────────────────────────────────────────

		TEST_METHOD(DefaultConstructor_DoesNotThrow)
		{
			Player p;
			// Just verify it can be created without throwing.
		}

		TEST_METHOD(ParameterisedConstructor_SetsAllFields)
		{
			Player p(42, "Alice", "hash123", "salt456", "alice@example.com");

			Assert::AreEqual(42,                         p.GetId());
			Assert::AreEqual(std::string("Alice"),       p.GetName());
			Assert::AreEqual(std::string("hash123"),     p.GetPassword());
			Assert::AreEqual(std::string("salt456"),     p.GetSalt());
			Assert::AreEqual(std::string("alice@example.com"), p.GetEmail());
			Assert::AreEqual(0,                          p.GetPoints());
		}

		// ── Setters / Getters ────────────────────────────────────────────────

		TEST_METHOD(SetId_UpdatesId)
		{
			Player p;
			p.SetId(7);
			Assert::AreEqual(7, p.GetId());
		}

		TEST_METHOD(SetName_UpdatesName)
		{
			Player p;
			p.SetName("Bob");
			Assert::AreEqual(std::string("Bob"), p.GetName());
		}

		TEST_METHOD(SetPassword_UpdatesPassword)
		{
			Player p;
			p.SetPassword("secret");
			Assert::AreEqual(std::string("secret"), p.GetPassword());
		}

		TEST_METHOD(SetSalt_UpdatesSalt)
		{
			Player p;
			p.SetSalt("salty");
			Assert::AreEqual(std::string("salty"), p.GetSalt());
		}

		TEST_METHOD(SetEmail_UpdatesEmail)
		{
			Player p;
			p.SetEmail("x@y.com");
			Assert::AreEqual(std::string("x@y.com"), p.GetEmail());
		}

		TEST_METHOD(SetPoints_OverwritesPoints)
		{
			Player p;
			p.SetPoints(99);
			Assert::AreEqual(99, p.GetPoints());
		}

		// ── Points arithmetic ────────────────────────────────────────────────

		TEST_METHOD(AddPoints_PositiveValue_IncreasesPoints)
		{
			Player p;
			p.SetPoints(10);
			p.AddPoints(5);
			Assert::AreEqual(15, p.GetPoints());
		}

		TEST_METHOD(AddPoints_NegativeValue_DecreasesPoints)
		{
			Player p;
			p.SetPoints(10);
			p.AddPoints(-4);
			Assert::AreEqual(6, p.GetPoints());
		}

		TEST_METHOD(AddPoints_Zero_DoesNotChange)
		{
			Player p;
			p.SetPoints(20);
			p.AddPoints(0);
			Assert::AreEqual(20, p.GetPoints());
		}

		TEST_METHOD(AddPoints_MultipleCallsAccumulate)
		{
			Player p;
			p.AddPoints(10);
			p.AddPoints(20);
			p.AddPoints(5);
			Assert::AreEqual(35, p.GetPoints());
		}

		// ── Serialization / Deserialization ─────────────────────────────────

		TEST_METHOD(Serialize_ProducesCorrectFormat)
		{
			Player p(1, "Alice", "hash", "salt", "a@b.com");
			p.SetPoints(50);
			std::string serialized = p.Serialize();
			Assert::AreEqual(std::string("1,Alice,hash,salt,a@b.com,50"), serialized);
		}

		TEST_METHOD(Serialize_DefaultPointsIsZero)
		{
			Player p(2, "Bob", "h", "s", "b@c.com");
			std::string serialized = p.Serialize();
			// Points should be 0 in the serialized form
			Assert::IsTrue(serialized.find(",0") != std::string::npos);
		}

		TEST_METHOD(Deserialize_RoundTrip_PreservesAllFields)
		{
			Player original(5, "Charlie", "pw", "sl", "c@d.com");
			original.SetPoints(75);

			std::string serialized = original.Serialize();
			Player restored = Player{}.Deserialize(serialized);

			Assert::AreEqual(original.GetId(),       restored.GetId());
			Assert::AreEqual(original.GetName(),     restored.GetName());
			Assert::AreEqual(original.GetPassword(), restored.GetPassword());
			Assert::AreEqual(original.GetSalt(),     restored.GetSalt());
			Assert::AreEqual(original.GetEmail(),    restored.GetEmail());
			Assert::AreEqual(original.GetPoints(),   restored.GetPoints());
		}

		TEST_METHOD(Deserialize_InvalidFormat_ThrowsRuntimeError)
		{
			auto action = [] { Player{}.Deserialize("1,Alice,hash,salt"); }; // Only 4 tokens
			Assert::ExpectException<std::runtime_error>(action);
		}

		TEST_METHOD(Deserialize_EmptyString_ThrowsRuntimeError)
		{
			auto action = [] { Player{}.Deserialize(""); };
			Assert::ExpectException<std::runtime_error>(action);
		}

		TEST_METHOD(Deserialize_ExtraTokens_ThrowsRuntimeError)
		{
			auto action = [] { Player{}.Deserialize("1,Alice,hash,salt,a@b.com,0,extra"); };
			Assert::ExpectException<std::runtime_error>(action);
		}

		// ── Comparison operators ─────────────────────────────────────────────

		TEST_METHOD(OperatorLessThan_ReturnsTrueWhenLessPoints)
		{
			Player low, high;
			low.SetPoints(10);
			high.SetPoints(20);
			Assert::IsTrue(low < high);
		}

		TEST_METHOD(OperatorLessThan_ReturnsFalseWhenEqualPoints)
		{
			Player a, b;
			a.SetPoints(10);
			b.SetPoints(10);
			Assert::IsFalse(a < b);
		}

		TEST_METHOD(OperatorLessThan_ReturnsFalseWhenMorePoints)
		{
			Player high, low;
			high.SetPoints(30);
			low.SetPoints(10);
			Assert::IsFalse(high < low);
		}

		TEST_METHOD(OperatorGreaterThan_ReturnsTrueWhenMorePoints)
		{
			Player high, low;
			high.SetPoints(30);
			low.SetPoints(10);
			Assert::IsTrue(high > low);
		}

		TEST_METHOD(OperatorGreaterThan_ReturnsFalseWhenEqualPoints)
		{
			Player a, b;
			a.SetPoints(15);
			b.SetPoints(15);
			Assert::IsFalse(a > b);
		}

		TEST_METHOD(OperatorGreaterThan_ReturnsFalseWhenLessPoints)
		{
			Player low, high;
			low.SetPoints(5);
			high.SetPoints(20);
			Assert::IsFalse(low > high);
		}

		TEST_METHOD(OperatorEqual_ReturnsTrueWhenSameName)
		{
			Player a, b;
			a.SetName("Dave");
			b.SetName("Dave");
			a.SetPoints(10);
			b.SetPoints(999); // Different points; equality is name-based
			Assert::IsTrue(a == b);
		}

		TEST_METHOD(OperatorEqual_ReturnsFalseWhenDifferentName)
		{
			Player a, b;
			a.SetName("Dave");
			b.SetName("Eve");
			Assert::IsFalse(a == b);
		}
	};
}

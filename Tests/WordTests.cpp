#include "CppUnitTest.h"

import word;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace skribbl;

namespace SkribblTests
{
	TEST_CLASS(WordTests)
	{
	public:

		// ── Construction ────────────────────────────────────────────────────

		TEST_METHOD(DefaultConstructor_DoesNotThrow)
		{
			Word w;
			// Verify default construction succeeds.
		}

		TEST_METHOD(ParameterisedConstructor_SetsFields)
		{
			Word w(3, "banana");
			Assert::AreEqual(3,                        w.GetId());
			Assert::AreEqual(std::string("banana"),    w.GetWord());
		}

		// ── Setters / Getters ────────────────────────────────────────────────

		TEST_METHOD(SetId_UpdatesId)
		{
			Word w;
			w.SetId(99);
			Assert::AreEqual(99, w.GetId());
		}

		TEST_METHOD(SetWord_UpdatesWord)
		{
			Word w;
			w.SetWord("elephant");
			Assert::AreEqual(std::string("elephant"), w.GetWord());
		}

		TEST_METHOD(SetId_Zero_IsAccepted)
		{
			Word w;
			w.SetId(0);
			Assert::AreEqual(0, w.GetId());
		}

		TEST_METHOD(SetWord_EmptyString_IsAccepted)
		{
			Word w;
			w.SetWord("");
			Assert::AreEqual(std::string(""), w.GetWord());
		}

		// ── Serialization ────────────────────────────────────────────────────

		TEST_METHOD(Serialize_ProducesIdCommaWord)
		{
			Word w(7, "giraffe");
			Assert::AreEqual(std::string("7,giraffe"), w.Serialize());
		}

		TEST_METHOD(Serialize_IdZeroWord)
		{
			Word w(0, "cat");
			Assert::AreEqual(std::string("0,cat"), w.Serialize());
		}

		TEST_METHOD(Serialize_EmptyWord)
		{
			Word w(1, "");
			Assert::AreEqual(std::string("1,"), w.Serialize());
		}

		TEST_METHOD(Serialize_LargeId)
		{
			Word w(100000, "hippo");
			Assert::AreEqual(std::string("100000,hippo"), w.Serialize());
		}

		TEST_METHOD(Serialize_WordWithSpaces)
		{
			Word w(2, "blue whale");
			Assert::AreEqual(std::string("2,blue whale"), w.Serialize());
		}
	};
}

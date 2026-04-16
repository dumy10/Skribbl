#include "CppUnitTest.h"

import utils;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SkribblTests
{
	TEST_CLASS(UtilsTests)
	{
	public:

		// ── split ────────────────────────────────────────────────────────────

		TEST_METHOD(Split_BasicCase_ReturnsTwoParts)
		{
			auto result = split("hello,world", ",");
			Assert::AreEqual(size_t(2), result.size());
			Assert::AreEqual(std::string("hello"), result[0]);
			Assert::AreEqual(std::string("world"), result[1]);
		}

		TEST_METHOD(Split_MultipleDelimiters_ReturnsAllParts)
		{
			auto result = split("a,b,c,d", ",");
			Assert::AreEqual(size_t(4), result.size());
			Assert::AreEqual(std::string("a"), result[0]);
			Assert::AreEqual(std::string("d"), result[3]);
		}

		TEST_METHOD(Split_EmptyString_ReturnsEmptyVector)
		{
			auto result = split("", ",");
			Assert::AreEqual(size_t(0), result.size());
		}

		TEST_METHOD(Split_DelimiterNotFound_ReturnsSingleElement)
		{
			auto result = split("hello", ",");
			Assert::AreEqual(size_t(1), result.size());
			Assert::AreEqual(std::string("hello"), result[0]);
		}

		TEST_METHOD(Split_DelimiterAtEnd_TrailingPartOmitted)
		{
			// The implementation only adds a trailing part if startIndex != str.size(),
			// so a trailing delimiter with nothing after it produces no extra element.
			auto result = split("a,b,", ",");
			Assert::AreEqual(size_t(2), result.size());
			Assert::AreEqual(std::string("a"), result[0]);
			Assert::AreEqual(std::string("b"), result[1]);
		}

		TEST_METHOD(Split_MultiCharDelimiter_SplitsCorrectly)
		{
			auto result = split("one::two::three", "::");
			Assert::AreEqual(size_t(3), result.size());
			Assert::AreEqual(std::string("one"),   result[0]);
			Assert::AreEqual(std::string("two"),   result[1]);
			Assert::AreEqual(std::string("three"), result[2]);
		}

		TEST_METHOD(Split_OnlyDelimiter_ReturnsEmptyVector)
		{
			auto result = split(",", ",");
			// startIndex ends up equal to str.size(), so nothing is appended
			Assert::AreEqual(size_t(0), result.size());
		}

		TEST_METHOD(Split_AdjacentDelimiters_ProducesEmptyToken)
		{
			auto result = split("a,,b", ",");
			Assert::AreEqual(size_t(3), result.size());
			Assert::AreEqual(std::string("a"), result[0]);
			Assert::AreEqual(std::string(""),  result[1]);
			Assert::AreEqual(std::string("b"), result[2]);
		}

		// ── parseUrlArgs ─────────────────────────────────────────────────────

		TEST_METHOD(ParseUrlArgs_EmptyString_ReturnsEmptyMap)
		{
			auto result = parseUrlArgs("");
			Assert::AreEqual(size_t(0), result.size());
		}

		TEST_METHOD(ParseUrlArgs_SingleParam_ParsesCorrectly)
		{
			auto result = parseUrlArgs("key=value");
			Assert::AreEqual(size_t(1), result.size());
			Assert::AreEqual(std::string("value"), result.at("key"));
		}

		TEST_METHOD(ParseUrlArgs_MultipleParams_ParsesAll)
		{
			auto result = parseUrlArgs("a=1&b=2&c=3");
			Assert::AreEqual(size_t(3), result.size());
			Assert::AreEqual(std::string("1"), result.at("a"));
			Assert::AreEqual(std::string("2"), result.at("b"));
			Assert::AreEqual(std::string("3"), result.at("c"));
		}

		TEST_METHOD(ParseUrlArgs_PairWithNoEquals_IsIgnored)
		{
			auto result = parseUrlArgs("noequals");
			Assert::AreEqual(size_t(0), result.size());
		}

		TEST_METHOD(ParseUrlArgs_PercentEncodedValue_IsDecoded)
		{
			auto result = parseUrlArgs("name=Hello%20World");
			Assert::AreEqual(std::string("Hello World"), result.at("name"));
		}

		TEST_METHOD(ParseUrlArgs_PlusSignInValue_IsDecodedAsSpace)
		{
			auto result = parseUrlArgs("msg=hello+world");
			Assert::AreEqual(std::string("hello world"), result.at("msg"));
		}

		TEST_METHOD(ParseUrlArgs_EmptyValue_IsAllowed)
		{
			auto result = parseUrlArgs("key=");
			Assert::AreEqual(size_t(1), result.size());
			Assert::AreEqual(std::string(""), result.at("key"));
		}

		TEST_METHOD(ParseUrlArgs_EmptyKeyWithValue_IsAllowed)
		{
			auto result = parseUrlArgs("=value");
			Assert::AreEqual(size_t(1), result.size());
			Assert::AreEqual(std::string("value"), result.at(""));
		}

		// ── urlDecode ────────────────────────────────────────────────────────

		TEST_METHOD(UrlDecode_NoEncoding_ReturnsSameString)
		{
			Assert::AreEqual(std::string("hello"), urlDecode("hello"));
		}

		TEST_METHOD(UrlDecode_PercentEncodedSpace_ReturnsSpace)
		{
			Assert::AreEqual(std::string("hello world"), urlDecode("hello%20world"));
		}

		TEST_METHOD(UrlDecode_PlusSign_ReturnsSpace)
		{
			Assert::AreEqual(std::string("hello world"), urlDecode("hello+world"));
		}

		TEST_METHOD(UrlDecode_MixedEncodings_DecodesAll)
		{
			Assert::AreEqual(std::string("a b=c"), urlDecode("a+b%3Dc"));
		}

		TEST_METHOD(UrlDecode_EmptyString_ReturnsEmptyString)
		{
			Assert::AreEqual(std::string(""), urlDecode(""));
		}

		TEST_METHOD(UrlDecode_PercentAtEnd_IsPassedThrough)
		{
			// Incomplete % sequence at end of string: '%' with fewer than 2 remaining chars
			// Implementation passes '%' through unchanged.
			std::string result = urlDecode("hello%");
			Assert::AreEqual(std::string("hello%"), result);
		}

		TEST_METHOD(UrlDecode_PercentWithOneChar_IsPassedThrough)
		{
			std::string result = urlDecode("hello%2");
			// Only 1 char after '%'; not enough for a valid hex pair
			Assert::AreEqual(std::string("hello%2"), result);
		}

		TEST_METHOD(UrlDecode_PercentWithInvalidHex_PassesThroughPercent)
		{
			// "zz" is not valid hex; the implementation passes '%' through unchanged.
			std::string result = urlDecode("hello%zz");
			Assert::AreEqual(std::string("hello%zz"), result);
		}

		TEST_METHOD(UrlDecode_PercentEncodedSlash_IsDecoded)
		{
			Assert::AreEqual(std::string("a/b"), urlDecode("a%2Fb"));
		}

		TEST_METHOD(UrlDecode_UpperCaseHex_IsDecoded)
		{
			Assert::AreEqual(std::string("a/b"), urlDecode("a%2Fb"));
		}

		TEST_METHOD(UrlDecode_LowerCaseHex_IsDecoded)
		{
			Assert::AreEqual(std::string("a/b"), urlDecode("a%2fb"));
		}

		TEST_METHOD(UrlDecode_MultiplePercentSequences_DecodesAll)
		{
			// "Hello%20World%21" → "Hello World!"
			Assert::AreEqual(std::string("Hello World!"), urlDecode("Hello%20World%21"));
		}
	};
}

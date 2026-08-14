#include <string>

#include <bandit/bandit.h>
#include "format.h"

using namespace bandit;
using namespace coda;
using namespace snowhouse;

using std::invalid_argument;

namespace
{
    void assert_invalid_format(const std::string &value)
    {
        AssertThrows(invalid_argument, format(value));
    }
}

go_bandit([]() {
    describe("format parser contract", []() {
        it("accepts the documented width-before-format grammar", []() {
            format f("{0,12:f2}", 123.1234123);
            Assert::That(f.str(), Equals("      123.12"));
        });

        it("preserves the legacy width-after-format compatibility form", []() {
            format f("{0:f2,12}", 123.1234123);
            Assert::That(f.str(), Equals("      123.12"));
        });

        it("rejects trailing characters in index and width fields", []() {
            assert_invalid_format("{0junk}");
            assert_invalid_format("{0,12junk}");
            assert_invalid_format("{0:f2,12junk}");
        });

        it("rejects whitespace and plus signs in numeric tokens", []() {
            assert_invalid_format("{ 0}");
            assert_invalid_format("{+0}");
            assert_invalid_format("{0, 12}");
            assert_invalid_format("{0,+12}");

            format precision_space("{0:f 2}");
            AssertThrows(invalid_argument, precision_space.args(123.123));

            format precision_plus("{0:f+2}");
            AssertThrows(invalid_argument, precision_plus.args(123.123));
        });

        it("rejects ambiguous or repeated separators", []() {
            assert_invalid_format("{0,12:f2,8}");
            assert_invalid_format("{0::f2}");
            assert_invalid_format("{0,12,8}");
        });

        it("rejects trailing characters in numeric precision", []() {
            format f("{0:f2junk}");
            AssertThrows(invalid_argument, f.args(123.123));
        });

        it("rejects negative indexes and precision", []() {
            assert_invalid_format("{-1}");

            format f("{0:f-1}");
            AssertThrows(invalid_argument, f.args(123.123));
        });

        it("rejects widths that cannot be represented safely", []() {
            assert_invalid_format("{0,128}");
            assert_invalid_format("{0,-129}");
        });
    });
});

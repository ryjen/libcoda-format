#include <bandit/bandit.h>
#include <coda/format/format.h>

using namespace bandit;
using namespace snowhouse;

using coda::format;

go_bandit([]() {
    describe("public header compatibility", []() {
        it("supports the canonical coda include path", []() {
            format value("{0}", "canonical");
            Assert::That(value.str(), Equals("canonical"));
        });
    });
});

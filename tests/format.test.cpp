/*!
 * implementation of the format class
 * @copyright ryan jennings (arg3.com), 2012 under LGPL
 *
 * requires the Igloo C++ Unit Testing Library
 * http://igloo-testing.org
 */

#include <bandit/bandit.h>
#include "format.h"

using namespace bandit;

using namespace arg3;

class OStreamClass
{
    friend ostream &operator<<(ostream &out, const OStreamClass &obj);
private:
    int data;
    string name;
public:
    OStreamClass(const string &name, int data) : data(data), name(name)
    {

    }
};

ostream &operator<<(ostream &out, const OStreamClass &obj)
{
    out << obj.name << ":" << obj.data;
    return out;
}

go_bandit([]()
{

    describe("a formatter", []()
    {

        it("should know the number of argument specifiers", []()
        {
            format f("this is a {0}");

            Assert::That(f.specifiers(), Equals(1));

            format f2("{0} is a {1}, {2} eh?");

            Assert::That(f2.specifiers(), Equals(3));

        });

        it("should be able to pass arguments in the constructor", []()
        {
            // construct and replace with arguments
            format f("{1} is a {0}, {2} eh?", "test", "this", "cool");

            // all specifiers replaced
            Assert::That(f.specifiers(), Equals(0));

            Assert::That(f.str(), Equals("this is a test, cool eh?"));
        });

        it("should be copyable", []()
        {
            format f1("{0} is a {1}, {2} eh?");

            format f2(f1);

            Assert::That(f2.specifiers(), Equals(3));

            f2 << "this";

            format f3(f2);

            Assert::That(f2.specifiers(), Equals(2));

            Assert::That(f2.str(), Equals("this is a {1}, {2} eh?"));
        });

        it("should be movable", []()
        {
            format f1("{0} is a {1}, {2} eh?");

            format f2(std::move(f1));

            Assert::That(f2.specifiers(), Equals(3));

            f2 << "this";

            format f3(std::move(f2));

            Assert::That(f2.specifiers(), Equals(0));

            Assert::That(f3.str(), Equals("this is a {1}, {2} eh?"));
        });

        it("can be copy assigned", []()
        {

            format f1("{0} is a {1}, {2} eh?");

            format f2 = f1;

            Assert::That(f2.specifiers(), Equals(3));

            f2 << "this";

            format f3("test");

            f3 = f2;

            Assert::That(f2.specifiers(), Equals(2));

            Assert::That(f2.str(), Equals("this is a {1}, {2} eh?"));

        });

        it("can be move assigned", []()
        {

            format f1("{0} is a {1}, {2} eh?");

            format f2 = std::move(f1);

            Assert::That(f2.specifiers(), Equals(3));

            f2 << "this";

            format f3("test");

            f3 = std::move(f2);

            Assert::That(f2.specifiers(), Equals(0));

            Assert::That(f3.str(), Equals("this is a {1}, {2} eh?"));

        });

        it("can handle no conversion", []()
        {
            format f("this has no specifiers");

            Assert::That(f.str(), Equals("this has no specifiers"));
        });

        it("can add arguments", []()
        {

            format f("{0} is a {1}, {2} eh?");

            f.arg("this");

            Assert::That(f.specifiers(), Equals(2));

            Assert::That(f.str(), Equals("this is a {1}, {2} eh?"));

            f.arg("test");

            Assert::That(f.specifiers(), Equals(1));

            Assert::That(f.str(), Equals("this is a test, {2} eh?"));

            f.arg("cool");

            Assert::That(f.specifiers(), Equals(0));

            Assert::That(f.str(), Equals("this is a test, cool eh?"));

        });

        it("can add multiple arguments", []()
        {
            format f("{0} is a {1}, {2} eh?");

            f.arg("this", "test", "cool");

            Assert::That(f.specifiers(), Equals(0));

            Assert::That(f.str(), Equals("this is a test, cool eh?"));
        });

        it("can cast to a string", []()
        {
            string str = format("cast to a {0}", "string");

            Assert::That(str, Equals("cast to a string"));
        });

        it("can left shift arguments", []()
        {

            format f("{0} {1}");

            f << "test" << "one";

            Assert::That(f.str(), Equals("test one"));

            format f2 = (format("something {0}") << "else");

            Assert::That(f2.str(), Equals("something else"));
        });

        it("throws exceptions", []()
        {
            AssertThrows(invalid_argument, format("{0} {2}"));

            AssertThrows(invalid_argument, format("{0} {2"));

            AssertThrows(invalid_argument, (format("{0}") << "test" << "two"));

            AssertThrows(invalid_argument, format("{0:} {1,asdfasdf}"))
        });

        it("can arrange arguments", []()
        {

            format f("{2} and {1} and {0}", 1, 2, 3);

            Assert::That(f.str(), Equals("3 and 2 and 1"));
        });

        it("can format floating point numbers", []()
        {

            format f("{0:f2}", 1243.4533889798);

            Assert::That(f.str(), Equals("1243.45"));

            f.reset("{0:Fasdfasdf}");

            AssertThrows(invalid_argument, f.arg("1123.123123"));

        });

        it("can format scientific numbers", []()
        {

            format f("{0:e5}", 3.1415926534);

            Assert::That(f.str(), Equals("3.14159e+00"));

            f.reset("{0:E}");

            f.arg(1.0e-10);

            Assert::That(f.str(), Equals("1.000000000E-10"));

            f.reset("{0:Fasdfasdf}");

            AssertThrows(invalid_argument, f.arg("123.123-10"));

        });

        it("can format hex numbers", []()
        {
            format f("{0:X}", 10);

            Assert::That(f.str(), Equals("0A"));
        });

        it("can format octal numbers", []()
        {
            format f("{0:O}", 10);

            Assert::That(f.str(), Equals("12"));
        });

        it("can reset its arguments", []()
        {
            format f("{0:f}", 123.56789);

            Assert::That(f.str(), Equals("123.567890000"));

            f.reset();

            Assert::That(f.specifiers(), Equals(1));

            f.arg("test");

            Assert::That(f.specifiers(), Equals(0));

            Assert::That(f.str(), Equals("test"));

            f.reset("{0}, {1}!");

            f.arg("Hello", "World");

            Assert::That(f.str(), Equals("Hello, World!"));
        });

        it("can handle custom argument types", []()
        {

            OStreamClass test("custom", 42);

            format f("{0}", test);

            Assert::That(f.str(), Equals("custom:42"));

        });

        it("can print to a stream", []()
        {

            stringstream buf;

            format f("{0} is so {1}!", "format", "cool");

            f.print(buf);

            Assert::That(buf.str(), Equals("format is so cool!"));
        });

        it("can left shift to a stream", []()
        {

            format f("{0} is so {1}!", "format", "cool");

            stringstream buf;

            buf << f;

            Assert::That(buf.str(), Equals("format is so cool!"));
        });

        it("can escape specifier tags", []()
        {

            format f("{0} is {{0}}", "this");

            Assert::That(f.str(), Equals("this is {0}"));

            f.reset("{{{{0}}}}");

            Assert::That(f.specifiers(), Equals(0));

            Assert::That(f.str(), Equals("{{0}}"));

            f.reset("{0}}}");
            f.arg("yup");

            Assert::That(f.str(), Equals("yup}"));

            f.reset("{{0}");

            Assert::That(f.str(), Equals("{0}"));

        });

        it("can format argument width", []()
        {
            format f("{0,-12} one", "test");

            Assert::That(f.str(), Equals("test         one"));

            f.reset("{0,12} one");

            f << "test";

            Assert::That(f.str(), Equals("        test one"));

            f.reset("{0:f2,12}");

            f << 123.1234123;

            Assert::That(f.str(), Equals("      123.12"));
        });

        it("can add a new line", []()
        {
            format f("{0:n}", "hello");

            Assert::That(f.str(), Equals("hello\n"));
        });

    });


});


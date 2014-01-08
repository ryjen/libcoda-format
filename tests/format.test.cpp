/*!
 * implementation of the format class
 * @copyright ryan jennings (arg3.com), 2012 under LGPL
 *
 * requires the Igloo C++ Unit Testing Library
 * http://igloo-testing.org
 */

#include <igloo/igloo.h>
#include "format.h"

using namespace igloo;

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

Context(FormatTest)
{

    Spec(knownNumberOfSpecifiers)
    {
        format f("this is a {0}");

        Assert::That(f.specifiers(), Equals(1));

        format f2("{0} is a {1}, {2} eh?");

        Assert::That(f2.specifiers(), Equals(3));

    }

    Spec(multiArgConstructor)
    {
        // construct and replace with arguments
        format f("{1} is a {0}, {2} eh?", "test", "this", "cool");

        // all specifiers replaced
        Assert::That(f.specifiers(), Equals(0));

        Assert::That(f.str(), Equals("this is a test, cool eh?"));
    }

    Spec(CopyConstructor)
    {
        format f1("{0} is a {1}, {2} eh?");

        format f2(f1);

        Assert::That(f2.specifiers(), Equals(3));

        f2 << "this";

        format f3(f2);

        Assert::That(f2.specifiers(), Equals(2));

        Assert::That(f2.str(), Equals("this is a {1}, {2} eh?"));
    }

    Spec(AssignmentOperator)
    {

        format f1("{0} is a {1}, {2} eh?");

        format f2 = f1;

        Assert::That(f2.specifiers(), Equals(3));

        f2 << "this";

        format f3 = f2;

        Assert::That(f2.specifiers(), Equals(2));

        Assert::That(f2.str(), Equals("this is a {1}, {2} eh?"));

    }

    Spec(handleNoConversion)
    {
        format f("this has no specifiers");

        Assert::That(f.str(), Equals("this has no specifiers"));
    }

    Spec(addingArguments)
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

    }

    Spec(addingMultipleArguments)
    {
        format f("{0} is a {1}, {2} eh?");

        f.arg("this", "test", "cool");

        Assert::That(f.specifiers(), Equals(0));

        Assert::That(f.str(), Equals("this is a test, cool eh?"));
    }

    Spec(stringCastOperator)
    {
        string str = format("cast to a {0}", "string");

        Assert::That(str, Equals("cast to a string"));
    }

    Spec(leftShiftOperator)
    {

        format f("{0} {1}");

        f << "test" << "one";

        Assert::That(f.str(), Equals("test one"));

        format f2 = (format("something {0}") << "else");

        Assert::That(f2.str(), Equals("something else"));
    }

    Spec(hasExceptions)
    {
        AssertThrows(invalid_argument, format("{0} {2}"));

        AssertThrows(invalid_argument, (format("{0}") << "test" << "two"));
    }

    Spec(argumentsFollowSpecifierOrder)
    {

        format f("{2} and {1} and {0}", 1, 2, 3);

        Assert::That(f.str(), Equals("3 and 2 and 1"));
    }

    Spec(floatingPointSpecialization)
    {

        format f("{0:f2}", 1243.4533889798);

        Assert::That(f.str(), Equals("1243.45"));

    }

    Spec(hexidecimalSpecialization)
    {
        format f("{0:X}", 10);

        Assert::That(f.str(), Equals("0A"));
    }

    Spec(canResetFormat)
    {
        format f("{0:f}", 123.56789);

        Assert::That(f.str(), Equals("123.56789"));

        f.reset();

        Assert::That(f.specifiers(), Equals(1));

        f.arg("test");

        Assert::That(f.specifiers(), Equals(0));

        Assert::That(f.str(), Equals("test"));

        f.reset("{0}, {1}!");

        f.arg("Hello", "World");

        Assert::That(f.str(), Equals("Hello, World!"));
    }

    Spec(customArgumentTypes)
    {

        OStreamClass test("custom", 42);

        format f("{0}", test);

        Assert::That(f.str(), Equals("custom:42"));

    }

    Spec(printingToSuppliedStream)
    {

        stringstream buf;

        format f("{0} is so {1}!", "format", "cool");

        f.print(buf);

        Assert::That(buf.str(), Equals("format is so cool!"));
    }

    Spec(leftShiftingToStream)
    {

        format f("{0} is so {1}!", "format", "cool");

        stringstream buf;

        buf << f;

        Assert::That(buf.str(), Equals("format is so cool!"));
    }

    Spec(canEscapeSpecifierTags)
    {

        format f("{0} is {{0}}", "this");

        Assert::That(f.str(), Equals("this is {0}"));

        f.reset("{{{{0}}}}");

        Assert::That(f.specifiers(), Equals(0));

        Assert::That(f.str(), Equals("{{0}}"));

    }

    Spec(canFormatWidth)
    {
        format f("{0,-12} one", "test");

        Assert::That(f.str(), Equals("test         one"));

        f.reset("{0,12} one");

        f << "test";

        Assert::That(f.str(), Equals("        test one"));
    }
};

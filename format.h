/*!
 * implementation of the format class
 * @copyright ryan jennings (arg3.com), 2012 under LGPL
 */

#ifndef ARG3_FORMAT_H
#define ARG3_FORMAT_H

#include <string>
#include <list>
#include <sstream>

using namespace std;

namespace arg3
{

    /*!
     * class to handle printf style formating using a format string containing specifiers that
     * get replaced with argument values
     */
    class format
    {
    public:

        // template methods

        /*!
         * adds an argument for the next specifier
         * singlular form of the varaidic override
         * @throws invalid_argument if there is no specifier for the argument
         */
        template<typename T>
        format &arg(const T &value) throw (invalid_argument)
        {

            // check if there isn't a specifier
            if (currentSpecifier_ == specifiers_.end())
            {
                throw invalid_argument("no specifier for argument");
            }

            specifier &arg = *currentSpecifier_++; // get specifier and advance

            // get the argument value as a string
            ostringstream buf;

            begin_manip(buf, arg); // set stream flags for arg
            buf << value; // append value
            end_manip(buf, arg); // cleanup stream from arg

            arg.replacement = buf.str();

            return *this;
        }

        /*!
         * adds a list of arguments to replace specifiers
         * @throws invalid_argument if there is no specifier for an argument
         */
        template<typename T, typename... Args>
        format &arg(const T &value, const Args &... args) throw (invalid_argument)
        {
            arg(value); // add argument
            arg(args...); // add remaining arguments (recursive)
            return *this;
        }

        // template constructors

        /*!
         * constructor to create a specifiers from a format string and add arguments
         * @throws invalid_argument if there isn't a specifier for an argument
         */
        template<typename T, typename... Args>
        format(const string &str, const T &value, const Args &... args) throw (invalid_argument) : format(str)
        {
            arg(value); // add argument
            arg(args...); // add remaining arguments
        }

        /*!
         * single form of the variadic template constructor
         */
        template<typename T>
        format(const string &str, const T &value) throw (invalid_argument) : format(str)
        {
            arg(value); // add argument
        }

        // constructors

        /*!
         * copy constructor
         */
        format(const format &other);

        format(format &&other);

        /*!
         * default constructor needs a format string
         */
        format(const string &str);

        virtual ~format();


        // operators

        /*!
         * assigns a format to this instance
         */
        format &operator=(const format &rhs);

        format &operator=(format &&rhs);

        /*!
         * converts the format with the given args and returns the string
         * @throws invalid_argument if there was a formatting error
         * @see str
         */
        operator string() throw (invalid_argument);

        /*!
         * adds an argument to the format
         * @throws invalid_argument if there is no specifier for the value
         */
        template<typename T>
        format &operator<< (const T &value) throw (invalid_argument)
        {

            return arg(value);
        }

        // methods

        /*!
         * converts the format with the given args and returns the string
         */
        string str();

        /*!
         * @return the number of specifiers in the format
         */
        size_t specifiers() const;

        /*!
         * reset using the format string
         */
        void reset(const string &value) throw (invalid_argument);

        /*!
         * resets the specifiers to build a new string
         */
        void reset() throw (invalid_argument);

        void print(ostream &out);
    private:
        // private constants
        static const char s_open_tag = '{';
        static const char s_close_tag = '}';

        // struct for a single specifier in the format
        typedef struct
        {
            string::size_type prev; // the prev position in format string
            string::size_type next; // the next position in format string
            size_t index; // the argument index
            string format; // the format
            char type;     // the specifier
            int8_t width;  // width of the replacement
            string replacement; // the replacement value
        } specifier;

        typedef list<specifier> SpecifierList; // for sorting


        // private methods

        /*!
         * creates the specifier list from the format string
         * @throws invalid_argument if the format string is invalid
         */
        void initialize() throw (invalid_argument);
        void add_specifier(string::size_type start, string::size_type end) throw (invalid_argument);
        void begin_manip(ostream &out, const specifier &arg) const throw (invalid_argument);
        void end_manip(ostream &out, const specifier &arg);
        void unescape(ostream &buf, string::size_type start, string::size_type end);

        // private member variables
        string value_; // the format
        SpecifierList specifiers_; // the list of specifiers in the format
        SpecifierList::iterator currentSpecifier_; // the current specifier

        friend ostream &operator<<(ostream &out, format &f);
    };

    ostream &operator<<(ostream &out, format &f);

}

#endif


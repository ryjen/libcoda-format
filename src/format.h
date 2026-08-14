/*!
 * implementation of the format class
 * @copyright ryan jennings (coda.life), 2012 under LGPL
 */

#ifndef CODA_FORMAT_H
#define CODA_FORMAT_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace coda
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
        template <typename T>
        format &args(const T &value)
        {
            // check if there isn't a specifier
            if (currentSpecifier_ == specifiers_.end()) {
                throw std::invalid_argument("no specifier for argument");
            }

            specifier &arg = *currentSpecifier_++;  // get specifier and advance

            // get the argument value as a string
            std::ostringstream buf;

            begin_manip(buf, arg);  // set stream flags for arg
            buf << value;           // append value
            end_manip(buf, arg);    // cleanup stream from arg

            arg.replacement = buf.str();

            return *this;
        }

        /*!
         * adds a list of arguments to replace specifiers
         * @throws invalid_argument if there is no specifier for an argument
         */
        template <typename T, typename... Args>
        format &args(const T &value, const Args &... argv)
        {
            args(value);    // add argument
            args(argv...);  // add remaining arguments (recursive)
            return *this;
        }

        // template constructors

        /*!
         * constructor to create a specifiers from a format string and add arguments
         * @throws invalid_argument if there isn't a specifier for an argument
         */
        template <typename T, typename... Args>
        format(const std::string &str, const T &value, const Args &... argv) : format(str)
        {
            args(value);    // add argument
            args(argv...);  // add remaining arguments
        }

        /*!
         * single form of the variadic template constructor
         */
        template <typename T>
        format(const std::string &str, const T &value) : format(str)
        {
            args(value);  // add argument
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
        format(const std::string &str);

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
        operator std::string();

        /*!
         * adds an argument to the format
         * @throws invalid_argument if there is no specifier for the value
         */
        template <typename T>
        format &operator<<(const T &value)
        {
            return args(value);
        }

        // methods

        /*!
         * converts the format with the given args and returns the string
         */
        std::string str();

        /*!
         * @return the number of specifiers in the format
         */
        std::size_t specifiers() const;

        /*!
         * reset using the format string
         */
        void reset(const std::string &value);

        /*!
         * resets the specifiers to build a new string
         */
        void reset();

        void print(std::ostream &out);

       private:
        // private constants
        static const char s_open_tag = '{';
        static const char s_close_tag = '}';

        // struct for a single specifier in the format
        typedef struct {
            std::string::size_type prev;  // the prev position in format string
            std::string::size_type next;  // the next position in format string
            std::size_t index;            // the argument index
            std::string format;           // the format
            char type;                    // the specifier
            std::int8_t width;            // width of the replacement
            std::string replacement;      // the replacement value
        } specifier;

        typedef std::list<specifier> SpecifierList;  // for sorting

        // private methods

        /*!
         * creates the specifier list from the format string
         * @throws invalid_argument if the format string is invalid
         */
        void initialize();
        void add_specifier(std::string::size_type start, std::string::size_type end);
        void begin_manip(std::ostream &out, const specifier &arg) const;
        void end_manip(std::ostream &out, const specifier &arg);
        void unescape(std::ostream &buf, std::string::size_type start, std::string::size_type end);

        // private member variables
        std::string value_;                         // the format
        SpecifierList specifiers_;                  // the list of specifiers in the format
        SpecifierList::iterator currentSpecifier_;  // the current specifier

        friend std::ostream &operator<<(std::ostream &out, format &f);
    };

    std::ostream &operator<<(std::ostream &out, format &f);
}

#endif

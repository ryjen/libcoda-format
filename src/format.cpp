/*!
 * implementation of the format class
 * @copyright ryan jennings (coda.life), 2012 under LGPL
 */

#include "format.h"

#include <cstdlib>
#include <iomanip>
#include <iterator>
#include <utility>

namespace coda
{
    format::format(const std::string &str) : value_(str), specifiers_(), currentSpecifier_(specifiers_.begin())
    {
        initialize();
    }

    format::~format()
    {
        // specifiers_.clear();
    }

    format::format(const format &other) : value_(other.value_), specifiers_(), currentSpecifier_(specifiers_.begin())
    {
        // copy specifiers
        for (auto s : other.specifiers_) {
            specifiers_.push_back(s);
        }

        // set current position to begining
        currentSpecifier_ = specifiers_.begin();

        // advance current position according to the other position
        std::advance(currentSpecifier_,
                     std::distance(other.specifiers_.begin(), SpecifierList::const_iterator(other.currentSpecifier_)));
    }

    format::format(format &&other)
        : value_(std::move(other.value_)),
          specifiers_(std::move(other.specifiers_)),
          currentSpecifier_(std::move(other.currentSpecifier_))
    {
        other.specifiers_.clear();
        other.currentSpecifier_ = other.specifiers_.begin();
    }

    format &format::operator=(const format &rhs)
    {
        value_ = rhs.value_;  // copy the format

        specifiers_.clear();  // clear any specifiers already set

        // copy other specifiers
        for (auto s : rhs.specifiers_) {
            specifiers_.push_back(s);
        }

        // set current position to begining
        currentSpecifier_ = specifiers_.begin();
        // advance current position according to the other position
        std::advance(currentSpecifier_,
                     std::distance(rhs.specifiers_.begin(), SpecifierList::const_iterator(rhs.currentSpecifier_)));

        return *this;
    }

    format &format::operator=(format &&rhs)
    {
        value_ = std::move(rhs.value_);  // copy the format

        specifiers_ = std::move(rhs.specifiers_);

        // set current position to begining
        currentSpecifier_ = std::move(rhs.currentSpecifier_);

        rhs.specifiers_.clear();
        rhs.currentSpecifier_ = rhs.specifiers_.begin();

        return *this;
    }

    /*!
     * returns the number of specifiers in the format string
     */
    std::size_t format::specifiers() const
    {
        // the size of the specifier list minus any specifier arguments already added
        return specifiers_.size() -
               std::distance(specifiers_.begin(), SpecifierList::const_iterator(currentSpecifier_));
    }

    /*!
     * adds a specifier to the list
     * @throws invalid_argument if specifier does not contain an index
     */
    void format::add_specifier(std::string::size_type start, std::string::size_type end)
    {
        // get the string inside the delimiters
        std::string temp = value_.substr(start, end - start);

        // the specifier to create
        specifier spec;
        spec.index = 0;
        spec.prev = start - 1;  // exclude start tag
        spec.next = end + 1;    // exclude end tag
        spec.width = 0;
        spec.type = '\0';

        try {
            // look for {0,10:f2}
            auto divider = temp.find(':');

            if (divider != std::string::npos) {
                std::string format = temp.substr(divider + 1);

                spec.type = format[0];

                auto comma = temp.find(',', divider);

                if (comma != std::string::npos) {
                    spec.format = format.substr(1, comma);

                    spec.width = std::stoi(temp.substr(comma + 1));

                } else {
                    spec.format = format.substr(1, format.length() - 1);
                }

                temp = temp.substr(0, divider);
            } else {
                // look for {0,-10}
                divider = temp.find(',');

                if (divider != std::string::npos) {
                    spec.width = std::stoi(temp.substr(divider + 1));

                    temp = temp.substr(0, divider);
                }
            }

            spec.index = std::stoi(temp);

        } catch (...) {
            throw std::invalid_argument("invalid specifier format");
        }

        specifiers_.push_back(spec);
    }

    void format::initialize()
    {
        auto len = value_.length();

        // find each open tag
        for (std::size_t pos = 0; pos < len; pos++) {
            if (value_[pos] != s_open_tag) {
                continue;
            }

            if (++pos >= len) break;

            // check if its an escape tag, ie  {{
            if (pos < len && value_[pos] == s_open_tag) {
                continue;
            }

            // get the closing tag
            auto end = value_.find(s_close_tag, pos);

            if (end == std::string::npos) {
                throw std::invalid_argument("no specifier closing tag");
            }

            // add the specifier
            add_specifier(pos, end);
        }

        // short circuit if no specifiers found
        if (specifiers_.size() == 0) {
            return;
        }

        // sort specifiers based on index
        specifiers_.sort([&](const specifier &first, const specifier &second) { return first.index < second.index; });

        // set the current position (note this is *after* sorting)
        currentSpecifier_ = specifiers_.begin();

        std::size_t index = 0;

        // check if specifier indexes follow an incremental order
        for (auto spec : specifiers_) {
            if (spec.index != index++) {
                throw std::invalid_argument("specifier index not ordered");
            }
        }
    }

    void format::begin_manip(std::ostream &out, const specifier &arg) const
    {
        if (arg.width != 0) {
            out << std::setw(std::abs(arg.width));
            if (arg.width < 0) {
                out << std::left;
            }
        }

        // short circuit if specifier has no format string
        if (arg.type == '\0') {
            return;
        }

        switch (arg.type) {
            // printf styles
            // TODO: define formats
            case 'E':
                out << std::uppercase;
            case 'e':
                if (!arg.format.empty()) {
                    try {
                        int p = std::stoi(arg.format);
                        out << std::setprecision(p);
                    } catch (...) {
                        throw std::invalid_argument("invalid precision format for argument");
                    }
                } else {
                    out << std::setprecision(9);
                }
                out << std::scientific;
                break;
            case 'F':
            case 'f':
                if (!arg.format.empty()) {
                    try {
                        int p = std::stoi(arg.format);
                        out << std::setprecision(p);
                    } catch (...) {
                        throw std::invalid_argument("invalid precision format for argument");
                    }
                } else {
                    out << std::setprecision(9);
                }
                out << std::fixed;
                break;

            case 'X':
                out << std::uppercase;
            case 'x':
                out << std::hex << std::setfill('0');
                if (arg.width == 0) {
                    out << std::setw(2);
                }
                break;
            case 'O':
            case 'o':
                out << std::oct;
                break;
        }
    }

    void format::end_manip(std::ostream &out, const specifier &arg)
    {
        // cleanup any stream manipulation

        switch (arg.type) {
            case 'x':
            case 'X':
                out << std::dec;
                break;
            case 'n':
                out << std::endl;
                break;
        }
    }

    void format::reset()
    {
        specifiers_.clear();
        initialize();
    }

    void format::reset(const std::string &value)
    {
        value_ = value;
        reset();
    }

    std::string format::str()
    {
        std::ostringstream buf;

        print(buf);

        return buf.str();
    }

    void format::unescape(std::ostream &buf, std::string::size_type start, std::string::size_type end)
    {
        for (auto i = start; i < end; ++i) {
            char tag = value_[i];

            if (tag != s_open_tag && tag != s_close_tag) {
                buf << tag;
                continue;
            }

            if (i + 1 < end && value_[i + 1] == tag) {
                i++;
            }

            buf << tag;
        }
    }

    void format::print(std::ostream &buf)
    {
        // short circuit if no specifiers
        if (specifiers_.size() == 0) {
            unescape(buf, 0, value_.length());
            return;
        }
        // sort based on position
        specifiers_.sort([&](const specifier &first, const specifier &second) { return first.prev < second.prev; });

        std::size_t last = 0;

        // loop through all added arguments
        for (auto spec = specifiers_.begin(); spec != currentSpecifier_; ++spec) {
            if (spec->prev != 0) {
                // add filler between specifiers
                unescape(buf, last, spec->prev);
            }

            // append replacement
            buf << spec->replacement;

            last = spec->next;
        }

        // add ending
        if (last < value_.length()) {
            unescape(buf, last, value_.length());
        }
    }

    format::operator std::string()
    {
        return str();
    }

    std::ostream &operator<<(std::ostream &out, format &f)
    {
        f.print(out);
        return out;
    }
}

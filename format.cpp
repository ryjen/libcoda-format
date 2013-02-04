/*!
 * implementation of the format class
 * @copyright ryan jennings (arg3.com), 2012 under LGPL
 */

#include <iomanip>
#include "format.h"

namespace arg3 {

format::format(const string &str) : mFormat(str), mSpecifiers(), mCurrent(mSpecifiers.begin()) {
    initialize();
}

format::~format()
{
    //mSpecifiers.clear();
}

format::format(const format &other) : mFormat(other.mFormat), mSpecifiers(), mCurrent(mSpecifiers.begin()) {

    // copy specifiers
    for(auto s : other.mSpecifiers) {
        mSpecifiers.push_back(s);
    }

    // set current position to begining
    mCurrent = mSpecifiers.begin();
    // advance current position according to the other position
    advance(mCurrent, distance(other.mSpecifiers.begin(), SpecifierList::const_iterator(other.mCurrent)));
}

format& format::operator=(const format &rhs) {
    if(this != &rhs) {

        mFormat = rhs.mFormat; // copy the format

        mSpecifiers.clear(); // clear any specifiers already set

        // copy other specifiers
        for(auto s : rhs.mSpecifiers) {
            mSpecifiers.push_back(s);
        }

        // set current position to begining
        mCurrent = mSpecifiers.begin();
        // advance current position according to the other position
        advance(mCurrent, distance(rhs.mSpecifiers.begin(), SpecifierList::const_iterator(rhs.mCurrent)));
    }
    return *this;
}

/*!
 * returns the number of specifiers in the format string
 */
size_t format::specifiers() const {
    // the size of the specifier list minus any specifier arguments already added
    return mSpecifiers.size() - distance(mSpecifiers.begin(), SpecifierList::const_iterator(mCurrent));
}

/*!
 * adds a specifier to the list
 * @throws invalid_argument if specifier does not contain an index
 */
void format::add_specifier(string::size_type start, string::size_type end) throw (invalid_argument) {

    // get the string inside the delimiters
    string temp = mFormat.substr(start, end-start);

    // the specifier to create
    specifier spec;
    spec.index = 0;
    spec.prev = start-1; // exclude start tag
    spec.next = end+1; // exclude end tag
    spec.width = 0;
    spec.type = '\0';

    try {

        // look for {0,10:f2}
        auto divider = temp.find(':');

        if(divider != string::npos)
        {
            string format = temp.substr(divider+1);

            spec.type = format[0];

            spec.format = format.substr(1, format.length()-1);

            temp = temp.substr(0, divider);
        }

        // look for {0,-10}
        divider = temp.find(',');

        if(divider != string::npos) {
            spec.width = stoi(temp.substr(divider+1));

            temp = temp.substr(0, divider);
        }

        spec.index = stoi(temp);

    }  catch( ... ) {
        throw invalid_argument("invalid specifier format");
    }

    mSpecifiers.push_back( spec );

}


void format::initialize() throw (invalid_argument) {

    auto len = mFormat.length();

    // find each open tag
    for(auto pos = 0; pos < len; pos++) {

        if(mFormat[pos] != open_tag) {
            continue;
        }

        if(++pos >= len) break;

        // check if its an escape tag, ie  {{
        if(pos < len && mFormat[pos] == open_tag) {
            continue;
        }

        // get the closing tag
        auto end = mFormat.find(close_tag, pos);

        if(end == string::npos) {
            throw invalid_argument("no specifier closing tag");
        }

        if(end+1 < len && mFormat[end+1] == close_tag) {
            continue;
        }

        // add the specifier
        add_specifier(pos, end);
    }

    // short circuit if no specifiers found
    if(mSpecifiers.size() == 0) {
        return;
    }

    // sort specifiers based on index
    mSpecifiers.sort([&](const specifier &first, const specifier &second)
    {
        return first.index < second.index;
    });

    // set the current position (note this is *after* sorting)
    mCurrent = mSpecifiers.begin();

    size_t index = 0;

    // check if specifier indexes follow an incremental order
    for(auto spec : mSpecifiers) {
        if(spec.index != index++) {
            throw invalid_argument("specifier index not ordered");
        }
    }
}

void format::begin_manip(ostream &out, const specifier &arg) const throw (invalid_argument) {

    if(arg.width != 0) {
        out << setw(abs(arg.width));
        if(arg.width < 0) {
            out << left;
        }
    }

    // short circuit if specifier has no format string
    if(arg.type == '\0' ) {
        return;
    }

    switch(arg.type) {
        // printf styles
        // TODO: define formats
    case 'E':
        out << uppercase;
    case 'e':
        out << scientific;
    case 'f':
        if(!arg.format.empty()) {
            try {
                int p = stoi(arg.format);
                out << fixed << setprecision(p);
            } catch(...) {
                throw invalid_argument("invalid precision format for argument");
            }
        } else {
            out << setprecision(9);
        }
        break;

    case 'X':
        out << uppercase;
    case 'x':
        out << hex << setfill('0');
        if(arg.width == 0) {
            out << setw(2);
        }
        break;
    case 'o':
        out << oct;
        break;
    }
}

void format::end_manip(ostream &out, const specifier &arg) {
    // cleanup any stream manipulation

    switch(arg.type)
    {
    case 'x':
    case 'X':
        out << dec;
        break;
    case 'n':
        out << endl;
        break;
    }
}

void format::reset() throw (invalid_argument) {
    mSpecifiers.clear();
    initialize();
}

void format::reset(const string &value) throw (invalid_argument)
{
    mFormat = value;
    reset();
}

string format::str() throw (invalid_argument) {

    ostringstream buf;

    print(buf);

    return buf.str();
}

void format::unescape(ostream &buf, string::size_type start, string::size_type end)
{

    for(auto i = start; i < end; ++i) {

        char tag = mFormat[i];

        if(tag != open_tag && tag != close_tag) {
            buf << tag;
            continue;
        }

        if(i+1 < end && mFormat[i+1] == tag) {
            i++;
        }

        buf << tag;
    }
}

void format::print(ostream &buf) {

    // short circuit if no specifiers
    if(mSpecifiers.size() == 0)
    {
        unescape(buf, 0, mFormat.length());
        return;
    }
    // sort based on position
    mSpecifiers.sort([&](const specifier& first, const specifier &second)
    {
        return first.prev < second.prev;
    });

    size_t last = 0;

    // loop through all added arguments
    for(auto spec = mSpecifiers.begin(); spec != mCurrent; ++spec ) {
        if(spec->prev != 0) {
            // add filler between specifiers
            unescape(buf, last, spec->prev);
        }

        // append replacement
        buf << spec->replacement;

        last = spec->next;
    }

    // add ending
    if(last < mFormat.length()) {
        unescape(buf, last, mFormat.length());
    }
}

format::operator string() throw (invalid_argument) {
    return str();
}


ostream &operator<<(ostream &out, format &f) {
    f.print(out);
    return out;
}

}

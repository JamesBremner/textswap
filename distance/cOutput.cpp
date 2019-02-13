#include <string>
#include <sstream>
#include <vector>

#include "cOutput.h"
#include "cFileText.h"

cNGram::cNGram( cSentence c )
    : myElem(
{
    c
} )
{

}
cNGram::cNGram( cOutput& o, int pos, int len )
{
    for( int k = pos; k < pos+len; k++ )
        myElem.push_back( o.myElement[k] );
}
    std::string cNGram::Text()
    {
        std::stringstream ss;
        ss << "'";
        for( auto c : myElem )
            ss << c.TextID();
        ss << "'";
        return ss.str();
    }
    bool cNGram::operator==(const cNGram& rhs) const
    {
        if( size() != rhs.size() )
            return false;
        for( int k = 0; k < size(); k++ )
            if( myElem[k] != rhs.myElem[k] )
                return false;
        return true;
    }

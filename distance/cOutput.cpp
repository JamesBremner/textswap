#include <string>
#include <sstream>
#include <vector>
#include <random>       // std::default_random_engine
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

    /** Calculate distance between two outputs */
float Distance3(    cOutput& o1,
                    cOutput& o2,
                    const sWeight& W
               )
{
    //cout << "\n Distance calculation " << o1.TextElements() << " V " << o2.TextElements() << "\n";

    int total_move_count = 0;
    int presence_count    = 0;

    // Find matching ngrams of length 2 or greater in two outputs
    o1.Match( o2 );

    // loop over ngrams in output 1
    for( auto& g : o1.myGram )
    {
        // locate ngrams in both outputs
        int i1 = o1.Where( g );
        int i2 = o2.Where( g );

        if( i2 >= 0 )
        {
            // ngram is present in both outputs, possibly moved

            int m = abs( i1 - i2 );
            //cout << g.Text() << " moved " << m << "\n";
            total_move_count += m;
        }
        else
        {
           // cout << g.Text() << " present in 1st output only\n";
            presence_count++;
        }
    }

    // loop over ngrams in 2nd output
    for( auto& g :o2.myGram )
    {
        if( o1.Where( g ) == -1 )
        {
            presence_count++;
            //cout << g.Text() << " present in 2nd output only\n";
        }
    }

    float t = total_move_count * W.move + presence_count * W.presence;
    //cout << "Distance score " << t << "\n";
    return t;
}

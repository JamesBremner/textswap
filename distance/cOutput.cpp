#include <string>
#include <sstream>
#include <vector>
#include <random>       // std::default_random_engine
#include <algorithm>

#include "cOutput.h"
#include "cFileText.h"

using namespace std;

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


void cOutput::Parse( const string& s )
{
    myElement.clear();
    myGram.clear();
    for( auto c : s )
    {
        myElement.push_back( c );
        myGram.push_back( cNGram( c ) );
    }
}
string cOutput::Text()
{
    stringstream ss;
    for( auto& g : myGram )
    {
        ss << g.Text() << " ";
    }
    return ss.str();
}

string cOutput::TextElements()
{
    stringstream ss;
    for( auto& e : myElement )
        ss << e.TextID();
    return ss.str();
}

int cOutput::find( cSentence& c )
{
    int i = -1;
    for( auto e : myElement )
    {
        i++;
        if( e == c )
            return i;

    }
    return -1;
}

void cOutput::Match(
    cOutput& o2 )
{
    vector< cNGram > ret;

    for( int k = 0; k < size()-1; k++ )
    {
        for( int len = size()-k; len > 1; len-- )
        {
            cNGram test_sequence( *this, k, len );

            //cout << "Trying: " << test_sequence.Text() << "\n";

            if( o2.Match( test_sequence ) != -1 )
            {
                //cout << "Found sequence " << test_sequence.Text() << "\n";

                ret.push_back( test_sequence );

                // skip past seccessfully found sequence
                k += len-1;

                break;
            }
        }
    }
//    cout << "Sequences found ";
//    for( auto& s : ret )
//    {
//        cout << s.Text();
//    }
//    cout << "\n";

    Convert( ret );
    o2.Convert( ret );

//    cout << Text() << " V " << o2.Text() << "\n";

    return;
}

int cOutput::Match( cNGram& seq )
{
    // find location in this output of fist sentence in test sequence
    int it = find( seq[0] );

    // check fist sentence in test sequence is present
    if( it == -1 )
        return -1;

    // check that, at least, this output is long enough to possibly include entire test sequence
    if( size() - it < seq.size() )
        return -1;

    for( int k = 1; k < seq.size(); k++ )
    {
        //cout << "comparing " << seq.myElem[k].TextID()[0] << " V " << myElement[ it+k ].TextID()[0] << "\n";
        //if( seq.myElem[k] != myElement[ it+k ] )
        if( seq[k] != myElement[ it+k ] )
        {
            return -1;
        }
    }
    return it;
}


void cOutput::Convert( std::vector< cNGram >& vg )
{
    // loop over matching ngrams
    for( auto& g : vg  )
    {
        // find ngram in elements
        int i = Match( g );
        if( i == -1 )
            continue;

        // replace all elements with ngram
        for( int k = i; k < i+g.size(); k++ )
        {
            myGram[k] = g;
        }
    }

    // remove all but first refence to each ngram
    myGram.erase(
        unique( myGram.begin(), myGram.end() ),
        myGram.end() );
}

int cOutput::Where( cNGram& target )
{
    int i = -1;
    for( auto& g : myGram )
    {
        i++;
        if( g == target )
            return i;
    }
    return -1;
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

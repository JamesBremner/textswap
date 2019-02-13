#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/moment.hpp>

#include "cOutput.h"
#include "cFileText.h"

int cSentence::myLastID = 0;

using namespace std;



struct sWeight
{
    float move;
    float presence;
};

cFileText::cFileText()
{
    AddText();
}
void cFileText::AddSentence( const std::string& text )
{
    LastPara().push_back( new cSentence( text ) );
}
void cFileText::AddPara()
{
    LastSection().push_back( para_t() );
}
void cFileText::AddSection()
{
    LastText().push_back( section_t() );
}
void cFileText::AddText()
{
    // create para with no sentence
    para_t p;

    // create section with one paragraph
    section_t s;
    s.push_back( p );

    // create a text with one section
    text_t t;
    t.push_back( s );

    myText.push_back( t );

    cSentence::ResetID();
}
string cFileText::Text()
{
    stringstream ss;
    int text_count = 1;
    for( auto& t : myText )
    {
        ss << "\nText " << text_count++ << "\n";
        int section_count = 1;
        for( auto& s : t )
        {
            ss << "\nSection " << section_count++ << "\n";
            int para_count = 1;
            for( auto& p : s )
            {
                ss << "\nPara " << para_count++ << "\n";
                for( auto& sent : p )
                {
                    ss << "   " << sent->TextID()
                       << ": " << sent->TextText()
                       << "\n";
                }
            }
        }
    }
    return ss.str();
}

void cFileText::Shuffle()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine re(seed);
    for( auto & t : myText )
    {
        for( auto & s : t )
        {
            for( auto & p : s )
            {
                shuffle( p.begin(), p.end(), re );
            }
        }
    }
}
cOutput cFileText::Output( int text )
{
    string so;
    auto& t = myText[ text ];
    for( auto& s : t )
    {
        for( auto & p : s )
        {
            for( auto& sent : p  )
            {
                so += sent->TextID();
            }
        }
    }
    cOutput o;
    o.Parse( so );
    return o;
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

            if( o2.Match( test_sequence ) != -1 )
            {
                cout << "Found sequence " << test_sequence.Text() << "\n";

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

    return;
}

int cOutput::Match( cNGram& seq )
{
    int it = find( seq[0] );
    if( it == -1 )
        return -1;

    for( int k = 1; k < seq.size(); k++ )
    {
        if( seq[k] != myElement[ it+k ] )
        {
            return -1;
        }
    }
    return it;
}

/** Calculate distance between two outputs */
float Distance3(    cOutput& o1,
                    cOutput& o2,
                    const sWeight& W
               )
{
    cout << "\n Distance calculation " << o1.TextElements() << " V " << o2.TextElements() << "\n";

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
            cout << g.Text() << " moved " << m << "\n";
            total_move_count += m;
        }
        else
        {
            cout << g.Text() << " present in 1st output only\n";
            presence_count++;
        }
    }

    // loop over ngrams in 2nd output
    for( auto& g :o2.myGram )
    {
        if( o1.Where( g ) == -1 )
        {
            presence_count++;
            cout << g.Text() << " present in 2nd output only\n";
        }
    }

    float t = total_move_count * W.move + presence_count * W.presence;
    cout << "Distance score " << t << "\n";
    return t;
}

void cOutput::Convert( vector< cNGram >& vg )
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

    // display results
    cout << Text() << "\n";
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

int main()
{
    std::cout << "Distance Calculator built " << __DATE__ << " at " << __TIME__ << "\n";

    // specify input and distance parameters
    sWeight W;
    cout << "\nEnter move weight\n : ";
    cin >> W.move;
    cout << "\nEnter presence weight\n : ";
    cin >> W.presence;

    cFileText theFile;
    theFile.AddSentence( "test 1." );
    theFile.AddSentence( "test 2." );
    theFile.AddSentence(  "test 3." );
    theFile.AddSentence(  "test 4." );
    theFile.AddPara();
    theFile.AddSentence( "test 5." );
    theFile.AddSentence( "test 6." );
    theFile.AddSentence("test 7." );
    theFile.AddSentence(  "test 8." );

    theFile.AddText();
    theFile.AddSentence( "test 9." );
    theFile.AddSentence( "test 10." );
    theFile.AddSentence(  "test 11." );
    theFile.AddSentence( "test 12." );
    theFile.AddPara();
    theFile.AddSentence(  "test 13." );
    theFile.AddSentence(  "test 14." );
    theFile.AddSentence(  "test 15." );
    theFile.AddSentence(  "test 16." );
    cout << theFile.Text();
    theFile.Shuffle();
    cout << theFile.Text();
    cOutput o1 = theFile.Output( 0 );
    cOutput o2 = theFile.Output( 1 );
    float ds = Distance3( o1, o2, W );

    boost::accumulators::accumulator_set<float, boost::accumulators::stats<
    boost::accumulators::tag::mean, boost::accumulators::tag::min, boost::accumulators::tag::max > > acc;

    std::string output;
    while( 1 )
    {
        cout << "\nEnter outputs space delimited, e.g. 'abc cba xyz' ( <ctrl-c> to quit )  \n :";

        string line;
        std::getline (std::cin,line);
        vector< cOutput > vOutput;
        std::stringstream sst(line);
        std::string a;
        while( getline( sst, a, ' ' ) )
        {
            cOutput o;
            o.Parse( a );
            vOutput.push_back( o );
        }

        // loop over every pair of output
        for( int k = 0; k < vOutput.size(); k++ )
        {
            for( int k2 = k+1; k2 < vOutput.size(); k2++ )
            {
                // calculate distance between pair
                cOutput o1( vOutput[k] );
                cOutput o2( vOutput[k2] );
                float ds = Distance3( o1, o2, W );

                // accumulate statistics
                acc( ds );
            }
        }

        cout << "\nMin Distance:   " << boost::accumulators::min( acc ) << std::endl;
        cout << "Mean Distance:   " << boost::accumulators::mean( acc ) << std::endl;
        cout << "Max Distance:   " << boost::accumulators::max( acc ) << std::endl;
    }

    return 0;
}

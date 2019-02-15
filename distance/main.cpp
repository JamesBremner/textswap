#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <random>       // std::default_random_engine


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


void cFileText::AddSentence( const std::string& text )
{
    LastPara().push_back( cSentence( text ) );
}
void cFileText::AddPara()
{
    LastSection().push_back( cPara() );
}
void cFileText::AddSection()
{
    LastText().push_back( cSection() );
}
void cFileText::AddText()
{
    // create para with no sentence
    cPara p;

    // create section with one paragraph
    cSection s;
    s.push_back( p );

    // create a text with one section
    cText t;
    t.push_back( s );

    myText.push_back( t );

    cSentence::ResetID();
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
                so += sent.TextID();
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

int main()
{
    std::cout << "Distance Calculator built " << __DATE__ << " at " << __TIME__ << "\n";

    string filename;
    cout << "Read JSON text file\n : ";
    cin >> filename;
    if( filename.length() > 1 )
    {
        cFileText theFile( filename );
        ofstream of("a.log");
        of << theFile.Text();
        theFile.Difference();
        return 0;
    }

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
    //theFile.setSwapAllParas( eSwapOption::first_last_fixed );
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

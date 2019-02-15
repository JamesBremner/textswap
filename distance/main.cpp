#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


#include <random>       // std::default_random_engine


#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/moment.hpp>

#include "cOutput.h"
#include "cFileText.h"

using namespace std;

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

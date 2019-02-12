#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef char                elem_t;

class cNGram
{
public:
    vector< elem_t > myElem;
    cNGram()
    {

    }
    cNGram( elem_t c )
        : myElem(
    {
        c
    } )
    {

    }
    int size() const
    {
        return (int) myElem.size();
    }
    string Text()
    {
        stringstream ss;
        ss << "'";
        for( auto c : myElem )
            ss << c;
        ss << "'";
        return ss.str();
    }
    vector<elem_t>::iterator begin()
    {
        return myElem.begin();
    }
    vector<elem_t>::iterator end()
    {
        return myElem.end();
    }
    bool operator==(const cNGram& rhs) const
    {
        if( size() != rhs.size() )
            return false;
        for( int k = 0; k < size(); k++ )
            if( myElem[k] != rhs.myElem[k] )
                return false;
        return true;
    }
};

class cOutput
{
public:
    vector< elem_t > myElement;     // output as a vector of individual elements
    vector< cNGram >   myGram;      // output as a vector of matching n-grams

    /** Parse input string of characters representing elements */
    void Parse( const string& s );

    int size()
    {
        return (int) myElement.size();
    }
    vector< elem_t >::iterator begin()
    {
        return myElement.begin();
    }
    vector< elem_t >::iterator end()
    {
        return myElement.end();
    }
    /** find location of element */
    int find( elem_t& c );

    /** Display text of converted output */
    string Text();

    /** Find matching ngrams */
    void Match( cOutput& o );

    /** Find match for a ngram in unconverted output */
    int Match( cNGram& g );

    /** Index of a ngram in the converted output */
    int Where( cNGram& g );

    /** Convert from elements to matching ngrams of elements */
    void Convert( vector< cNGram >& g );

};
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

int cOutput::find( elem_t& c )
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
///    cout << "checking ";
////    for( auto c : target )
////        cout << c;
////    cout << "\n";
//
//    bool success;
//    for( int kv = 0; kv <= o.size()-target.size(); kv++ )
//    {
//        if( o.myElement[kv] != target.myElem[0] )
//            continue;
//        success = true;
//        for( int kt = 0; kt < target.size(); kt++ )
//        {
//            if( target.myElem[kt] != o.myElement[kv+kt] )
//            {
//                success = false;
//                break;
//            }
//        }
//    }
//    return success;
//}

/** Find sequences in two element lists

A sequence is two or more elements adjacent and in the same order in both outputs being compared

*/

void cOutput::Match(
    cOutput& o2 )
{
    vector< cNGram > ret;

    for( int k = 0; k < size()-1; k++ )
    {
        for( int len = size()-k; len > 1; len-- )
        {
            cNGram test_sequence;
            test_sequence.myElem = vector< char > ( begin()+k, begin()+k+len );

            //if( is_sequence_in( test_sequence, o2 ) )
            if( o2.Match( test_sequence ) != -1 )
            {
                // cout << "Found sequence " << test_sequence.Text() << "\n";

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
    int it = find( seq.myElem[0] );
    if( it == -1 )
        return -1;
    while( 1 )
    {
        bool success = true;
        for( int k = 1; k < seq.size(); k++ )
        {
            if( seq.myElem[k] != myElement[ it+k ] )
            {
                success = false;
                break;
            }
        }
        if( success )
            return it;
    }
    return -1;
}

float Distance3(    cOutput& o1,
                    cOutput& o2
               )
{
    int total_move_count = 0;

    // Find matching ngrams of length 2 or greater in two outputs
    o1.Match( o2 );

    cout << "\n Distance calculations:\n";

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
        }
    }

    // loop over ngrams in 2nd output
    for( auto& g :o2.myGram )
    {
        if( o1.Where( g ) == -1 )
        {
            cout << g.Text() << " present in 2nd output only\n";
        }
    }

    return total_move_count;
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
    int input_element_count;
    float deletion_weight;
    float element_count_weight;
    float presence_weight;
    float move_weight;

    std::string output;
    cOutput o1, o2;
    while( 1 )
    {
        cout << "\nEnter two outputs, e.g. 'abc cba' ( <ctrl-c> to quit )  \n :";

        cin >> output;
        o1.Parse( output );

        cin >> output;
        o2.Parse( output );

        cout << "\n";

        Distance3( o1, o2 );

    }

    return 0;
}

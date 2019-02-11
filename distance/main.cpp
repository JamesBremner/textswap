#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum class eAlgo
{
    input_output,
    output_output
};

bool is_sequence_in( vector<char>& target, vector<char>& v )
{
//    cout << "checking ";
//    for( auto c : target )
//        cout << c;
//    cout << "\n";

    for( int kv = 0; kv <= v.size()-target.size(); kv++ )
    {
        if( v[kv] != target[0] )
            continue;
        bool success = true;
        for( int kt = 0; kt < target.size(); kt++ )
        {
            if( target[kt] != v[kv+kt] )
            {
                success = false;
                break;
            }
            if( success )
                return true;
        }
    }
    return false;
}

vector< vector<char> > find_sequences(
    vector<char>& vo1,
    vector<char>& vo2 )
{
    vector< vector<char> > ret;
    vector<char> test_sequence;
    for( int k = 0; k < vo1.size()-1; k++ )
    {
        for( int len = vo1.size()-k; len > 1; len-- )
        {
            test_sequence.clear();
            for( int l = 0; l < len; l++ )
                test_sequence.push_back( vo1[k+l] );

            if( is_sequence_in( test_sequence, vo2 ) )
            {
////                cout << "Found sequence ";
////                for( auto c : test_sequence )
////                    cout << c;
////                cout << "\n";

                ret.push_back( test_sequence );

                // skip past seccessfully found sequence
                k += len-1;

                break;
            }
        }
    }
    cout << "Sequences found ";
    for( auto& s : ret ) {
        cout << "'";
        for( auto c : s ) {
            cout << c;
        }
        cout << "' ";
    }
    cout << "\n";

    return ret;
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

    eAlgo algo;
//    cout << "Enter distance calcutation required, 1 for input v output, 2 for output v output\n :";
//    int choice;
//    cin >> choice;
//    if( 0 >= choice || choice > 2 )
//    {
//        cout << "invalid choice\n";
//        return 1;
//    }
//    algo = ( eAlgo ) ( choice - 1 );
    algo = eAlgo::output_output;

    switch( algo )
    {

    case eAlgo::input_output:
        cout << "Enter input element count, e.g. 10   \n :";
        cin >> input_element_count;
        cout << "Enter deletion weight  ( suggest " << input_element_count / 2 << " )  \n :";
        cin >> deletion_weight;
        break;

    case eAlgo::output_output:
        cout << "Enter input element count weight\n : ";
        cin >> element_count_weight;
        cout << "Enter presence weight\n : ";
        cin >> presence_weight;
        cout << "Enter move weight\n : ";
        cin >> move_weight;
        break;
    }

    std::string output;
    std::vector<char> vout;
    std::vector<char> vout2;
    while( 1 )
    {
        cout << "\nenter first output, e.g. abc ( <ctrl-c> to quit )  \n :";
        cin >> output;
        cout << "\n";

        // parse output
        if( ! output.length() )
            return 0;
        for( auto c : output )
            vout.push_back( c );

        // re-initilaize distance calculation
        int deletion_count = 0;
        int move_count = 0;
        int element_count_delta = 0;
        int presence_delta = 0;

        switch( algo )
        {
        case eAlgo::input_output:
            // loop over original elements
            for( int k = 1; k <= input_element_count; k++ )
            {
                // find new position of element
                auto it = find (vout.begin(), vout.end(), k);
                if (it == vout.end())
                {

                    // element was deleted
                    deletion_count++;
                }
                else
                {
                    // how far was it moved from original position
                    int move = abs( (int)(it - vout.begin() ) - k + 1 );

                    if( move > 0 )
                        cout << k << " moved " << move << "\n";
                    move_count += move;
                }
            }

            // display results
            cout << deletion_count << " deletions "
                 <<  move_count << " moves\n"
                 << "\nTotal distance from input "
                 << move_count + deletion_weight * deletion_count
                 << "\n";

            break;

        case eAlgo::output_output:

            cout << "\nenter second output, e.g. abc ( <ctrl-c> to quit )  \n :";
            cin >> output;
            cout << "\n";

            // parse output
            if( ! output.length() )
                return 0;
            for( auto c : output )
                vout2.push_back( c );

            find_sequences( vout, vout2 );

            element_count_delta = abs( (int) ( vout.size() - vout2.size() ) );

            for( auto v1 : vout )
            {
                //std::cout << "loop " << v1 << "\n";
                auto it2 = find( vout2.begin(), vout2.end(), v1 );
                if( it2 == vout2.end() )
                {
                    cout << v1 << " not present in 2nd output\n";
                    presence_delta++;
                }
                else
                {
                    auto it1 = find( vout.begin(), vout.end(), v1 );
                    int move = abs( (int)(it1 - vout.begin() ) - (int)(it2 - vout2.begin() ) );
                    if( move )
                        std::cout << v1 << " moved " << move << "\n";
                    move_count += move;
                }
            }
            for( auto v2 : vout2 )
            {
                auto it = find( vout.begin(), vout.end(), v2 );
                if( it == vout.end() )
                {
                    cout << v2 << " not present in 1st output\n";
                    presence_delta++;
                }
            }
            std::cout << "\n\ncomparing '";
            for( auto c : vout )
                std::cout << c;
            cout << "' with '";
            for( auto c : vout2 )
                std::cout << c;
            cout << "'\n";
            float TotalDistance = element_count_delta * element_count_weight
                                  + presence_delta * presence_weight
                                  + move_count * move_weight;
            cout << "element_count_delta " << element_count_delta
                 << " presence delta " << presence_delta
                 << " move count " << move_count
                 << "\nTotal distance " << TotalDistance
                 << "\n";

            break;

        }

        // loop for next user input
    }

    return 0;
}

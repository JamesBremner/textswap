#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    // specify input and distance parameters
    int input_element_count;
    float deletion_weight;
    std::string output;
    cout << "Enter input element count, e.g. 10   \n :";
    cin >> input_element_count;
    cout << "Enter deletion weight  ( suggest " << input_element_count / 2 << " )  \n :";
    cin >> deletion_weight;

    while( 1 )
    {
        cout << "\nenter output, e.g. 123 ( <ctrl-c> to quit )  \n :";
        cin >> output;
        cout << "\n";

        // parse output
        if( ! output.length() )
            return 0;
        std::vector<int> vout;
        for( auto c : output )
            vout.push_back( (int)c - 48 );

        // re-initilaize distance calculation
        int deletion_count = 0;
        int move_count = 0;

        // loop over original elements
        for( int k = 1; k <= input_element_count; k++ )
        {
            // find new position of element
            auto it = find (vout.begin(), vout.end(), k);
            if (it == vout.end()) {

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

        // loop for next user input
    }

    return 0;
}

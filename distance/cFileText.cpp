#include <algorithm>
#include <chrono>       // std::chrono::system_clock
#include "cFileText.h"

void cFileText::setSwapAllParas( eSwapOption swap )
{
    for( auto & t : myText )
    {
        for( auto & s : t )
        {
            for( auto & p : s )
            {
                p.mySwap = swap;
            }
        }
    }
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
                p.Shuffle( re );
            }
        }
    }
}
void cPara::Shuffle( std::default_random_engine& re )
{
    std::vector< cSentence >::iterator start = mySentence.begin();
    std::vector< cSentence >::iterator stop = mySentence.end();
    switch( mySwap )
    {
    case eSwapOption::none:
        return;
    case eSwapOption::all:
        break;
    case eSwapOption::first_fixed:
        start++;
        break;
    case eSwapOption::last_fixed:
        stop--;
        break;
    case eSwapOption::first_last_fixed:
        start++;
        stop--;
        break;
    }
    shuffle( start, stop, re );
}

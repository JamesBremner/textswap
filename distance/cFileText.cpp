#include <algorithm>
#include <chrono>       // std::chrono::system_clock
#include <fstream>
#include <sstream>
#include <iostream>
#include "json.hpp"
#include "cFileText.h"

using namespace std;

cFileText::cFileText()
{
    AddText();
}
cFileText::cFileText( const std::string& filename )
{
    using json = nlohmann::json;
    std::ifstream i("tableau-2500-textes-apres-traitement.json");
    //std::ifstream i(filename);
    if( ! i.is_open() )
    {
        std::cout << filename << "\n";
        throw std::runtime_error("Cannot open text file\n" );
    }
    json j;
    i >> j;

    AddText();

    int textCount = j.size();
    for (auto& e : j)
    {
        for (auto& e2 : e)
        {
            int dbg2 = e2.size();
            if( e2.is_string())
            {
                string s = e2;
                int p = s.find("<p>");
                if( p != -1 )
                    s.erase(p,3);
                p = s.find("</p>");
                if( p != -1 )
                    s.erase(p,4);
                p = s.find("|");
                if( p != -1 )
                    s.erase(p,3);
                p = s.find("...");
                if( p != -1 )
                    s.erase(p,3);

                p = 0;
                while( 1 )
                {
                    int pdot = s.find(".",p);
                    int pq = s.find("?",p);
                    if( pdot == -1 && pq == -1 )
                        break;
                    int pnext = pdot;
                    if( pdot == -1 )
                        pnext = pq;
                    else if( pq > -1 && pq < pdot )
                        pnext = pq;
                    //std::cout <<"sentence: "<< s.substr(p,pnext-p+1) << "\n";
                    AddSentence( s.substr(p,pnext-p+1) );
                    p = pnext+1;
                }

                //std::cout << "paragraph: "<< s << "\n\n";
                AddPara();
            }
        }
        AddText();
    }

    cout << Text();
}

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
string cFileText::Text()
{
    stringstream ss;
    int text_count = 1;
    for( auto& t : myText )
    {
        if( ! t.size() )
            continue;
        ss << "\nText " << text_count++ << "\n";
        int section_count = 1;
        for( auto& s : t )
        {
            ss << "\nSection " << section_count++ << "\n";
            int para_count = 1;
            for( auto& p : s )
            {
                if( p.size() ) {
                ss << "\nPara " << para_count++ << "\n";
                for( auto& sent : p )
                {
                    ss << "   " << sent.TextID()
                       << ": " << sent.TextText()
                       << "\n";
                }
                }
            }
        }
    }
    return ss.str();
}

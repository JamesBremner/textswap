#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#include "cSwapTextFile.h"

using namespace std;

int cText::myLastIndex = 0;

void cSwapTextFile::Read( const std::string& filename )
{
    std::ifstream in( filename.c_str() );
    if( ! in.is_open() )
        throw std::runtime_error("Cannot open input file");
    std::stringstream sstr;
    sstr << in.rdbuf();
    myRawText = sstr.str();
}

void cSwapTextFile::Parse()
{
    Add( cText() );
    Add( cSection() );
    Add( cParagraph() );
    int p_now = 0;
    int p_sentence = 0;
    int p_para = 0;
    while( 1 )
    {
        p_sentence = myRawText.find(".",p_now);
        p_para = myRawText.find("\n",p_now);
        if( p_sentence != -1 )
        {
            Add( cSentence( myRawText.substr(p_now,p_sentence-p_now+1)));
            p_now = p_sentence+2;
            if( p_now > p_para )
            {

                // reached the end of a para

                CheckForTitle();

                CheckForSection();

                // start next para
                Add( cParagraph() );
            }
        }
        else
        {
            return;
        }
    }
}
void cSwapTextFile::CheckForTitle()
{
    // get last element
    cElement * lp = myText.back().mySection.back().myElement.back();

    // check for paragraph that is really a title
    if( ! lp->IsTitle() )
        return;

    dump_last_section_elements();

    // add the title
    Add( cTitle( lp->myElement[0]->myText ) );
    std::cout << lp->myElement[0]->myText << "\n";
    if( lp->myElement[0]->myText == "SELECTION.")
        int dbg = 0;

    // null the paragraph that was a title
    //lp->clear();
    myText.back().mySection.back().myElement.erase(
        myText.back().mySection.back().myElement.end()-2 );

    dump_last_section_elements();
}

void cSwapTextFile::dump_last_section_elements()
{
     auto& last_section_elements = myText.back().mySection.back().myElement;
    for( auto e : last_section_elements )
        std::cout << (int)e->myType << " ";
    std::cout << "\n";
}

void cSwapTextFile::CheckForSection()
{
    auto& last_section_elements = myText.back().mySection.back().myElement;
    int count = last_section_elements.size();
    if( count < 2 )
        return;
    for( auto e : last_section_elements )
        std::cout << (int)e->myType << " ";
    std::cout << "\n";
    if( last_section_elements[count-1]->myType == cElement::eType::title
            && last_section_elements[count-2]->myType == cElement::eType::title )
    {
        // Two titles together indicate the start of a new section
        cElement * t1 = last_section_elements[count-2];
        cElement * t2 = last_section_elements[count-1];
        last_section_elements.erase(last_section_elements.end()-1);
        last_section_elements.erase(last_section_elements.end()-1);
        Add( cSection() );
        Add( cTitle( t1->myText ) );
        Add( cTitle( t2->myText ) );
        delete t1;
        delete t2;


    }

}
int main()
{
    cSwapTextFile input;

    input.Read("test.txt");
    input.Parse();
    input.Dump();

//    input.Add( cText() );
//    input.Add( cSection() );
//    input.Add( cTitle( "Title I") );
//    input.Add( cParagraph() );
//    input.Add( cSentence("This is first sentence in text 1.") );
//    input.Add( cTitle( "Title II") );
//    input.Add( cParagraph() );
//    input.Add( cSentence("This is 2nd sentence in text 1.") );
//
//    input.Add( cText() );
//    input.Add( cSection() );
//    input.Add( cTitle( "Title I") );
//    input.Add( cParagraph() );
//    input.Add( cSentence("This is 1st sentence in text 2.") );
//    input.Add( cSection() );
//    input.Add( cTitle( "Title II") );
//    input.Add( cParagraph() );
//    input.Add( cSentence("This is 2nd sentence in text 2.") );
//
//    std::cout << "The input was:\n";
//    input.Dump();
//
//    input.Swap();
//
//    std::cout << "+++++++++++++++\n";
//    std::cout << "The output is:\n";
//    input.Dump();

    return 0;
}

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

using namespace std;

class cElement
{
public:
    enum class eType
    {
        sentence,
        paragraph,
        title,
        block
    } myType;
    std::vector< cElement* > myElement;
    std::string myText;

    cElement( eType type, const std::string& text )
        : myType( type )
        , myText( text )
    {

    }
    void Add( cElement* sentence )
    {
        switch( sentence->myType )
        {
        case eType::sentence:
            if( myType != eType::paragraph )
                throw std::runtime_error("Attempt to add sentence to non paragraph");
            break;
        }

        myElement.push_back( sentence );
    }
    virtual void Dump()
    {
    }
};
class cSentence : public cElement
{
public:
    cSentence( const std::string text )
        : cElement( cElement::eType::sentence, text )
    {

    }
    virtual void Dump()
    {
        std::cout << "\n" << std::left <<std::setw(20) << "SENTENCE: " << myText << "\n";
    }
};
class cTitle : public cElement
{
public:
    cTitle( const std::string text )
        : cElement( cElement::eType::sentence, text )
    {

    }
    virtual void Dump()
    {
        std::cout <<"\n"<< std::left <<std::setw(20) << "TITLE: " << myText;
    }
};
class cParagraph : public cElement
{
public:
    cParagraph()
        : cElement( cElement::eType::paragraph, "" )
    {

    }
    void Dump()
    {
        std::cout << "\nPARAGAPH:";
        for( auto e : myElement )
            e->Dump();
        std::cout << "END_PARAGRAPH\n";
    }
};

class cSection
{
public:
    std::vector< cElement* > myElement;
    void Add( const cParagraph& paragraph )
    {
        myElement.push_back( new cParagraph( paragraph ));
    }
    void Add( const cSentence& sentence )
    {
        myElement.back()->Add( new cSentence( sentence ) );
    }
    void Add( const cTitle& title )
    {
        myElement.push_back( new cTitle( title ) );
    }
    void Dump()
    {
        std::cout << "\nSECTION:";
        for( auto e : myElement )
            e->Dump();
        std::cout << "END_SECTION\n";
    }
};
class cText
{
public:
    std::vector< cSection > mySection;
    int myIndex;

    cText()
        : myIndex( ++myLastIndex )
    {
    }
    void Add( const cSection& section )
    {
        mySection.push_back( section );
    }
    void Add( const cParagraph& paragraph )
    {
        mySection.back().Add( paragraph );
    }
    void Add( const cSentence& sentence )
    {
        mySection.back().Add( sentence );
    }
    void Add( const cTitle& title )
    {
        mySection.back().Add( title );
    }
    void Dump()
    {
        std::cout << "TEXT "<< myIndex << ":\n";
        for( auto& s : mySection )
            s.Dump();
        std::cout << "END_TEXT\n\n";
    }
    void Swap()
    {
        if( mySection.size() > 1 )
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

            std::shuffle (mySection.begin(), mySection.end(), std::default_random_engine(seed));
        }
    }
private:
    static int myLastIndex;
};
int cText::myLastIndex = 0;

class cSwapTextFile
{
public:
    std::vector< cText > myText;
    void Add( const cText& text )
    {
        myText.push_back( text );
    }
    void Add( const cSection& section )
    {
        myText.back().Add( section );
    }
    void Add( const cParagraph& paragraph )
    {
        myText.back().Add( paragraph );
    }
    void Add( const cSentence& sentence )
    {
        myText.back().Add( sentence );
    }
    void Add( const cTitle& title )
    {
        myText.back().Add( title );
    }
    void Dump()
    {
        for( auto& t : myText )
        {
            t.Dump();
        }
    }
    void Swap()
    {
        for( auto& t : myText )
        {
            t.Swap();
        }
    }
};

int main()
{
    cSwapTextFile input;

    input.Add( cText() );
    input.Add( cSection() );
    input.Add( cTitle( "Title I") );
    input.Add( cParagraph() );
    input.Add( cSentence("This is first sentence in text 1.") );
    input.Add( cTitle( "Title II") );
    input.Add( cParagraph() );
    input.Add( cSentence("This is 2nd sentence in text 1.") );

    input.Add( cText() );
    input.Add( cSection() );
    input.Add( cTitle( "Title I") );
    input.Add( cParagraph() );
    input.Add( cSentence("This is 1st sentence in text 2.") );
    input.Add( cSection() );
    input.Add( cTitle( "Title II") );
    input.Add( cParagraph() );
    input.Add( cSentence("This is 2nd sentence in text 2.") );

    std::cout << "The input was:\n";
    input.Dump();

    input.Swap();

    std::cout << "+++++++++++++++\n";
    std::cout << "The output is:\n";
    input.Dump();

    return 0;
}

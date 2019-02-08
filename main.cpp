#include <iostream>
#include <sstream>
#include <fstream>
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
    void Add( cElement* e )
    {
        // check that it is possible to add new element type to this element type
        try
        {
            switch( e->myType )
            {
            case eType::sentence:
                if( myType != eType::paragraph )
                    throw 1;
                break;
            case eType::paragraph:
                if( myType != eType::block )
                    throw 1;
                break;
            case eType::title:
                if( myType != eType::block )
                    throw 1;
                break;
            case eType::block:
                throw 1;
                break;
            }
        }
        catch( int x )
        {
            std::stringstream ss;
            ss << "Invalid addition of " << e->TypeText() << " to " << TypeText();
            throw std::runtime_error( ss.str());
        }

        // add new element to this element
        myElement.push_back( e );

    }
    // remove all child elements
    void clear()
    {
        myElement.clear();
    }
    virtual void Dump() = 0;
    virtual bool IsTitle()
    {
        return false;
    }

    std::string TypeText()
    {
        std::string vt[] { "sentence", "paragraph", "title", "block" };
        return vt[(int)myType];
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
        std::cout << "\n" << std::left <<std::setw(20) << "SENTENCE: " << myText;
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
    /** True if paragraph is formatted as a title

    A paragraph is a title if it has one sentence with all uppercase letters
    */
    bool IsTitle()
    {
        if( myElement.size() != 1 )
            return false;
        for( auto c : myElement[0]->myText ) {
            if( islower( c ) )
                return false;
        }
        return true;
    }
    void Dump()
    {
        // Do not display empty paragraphs
        if( ! myElement.size() )
            return;

        std::cout << "\nPARAGAPH:";
        for( auto e : myElement )
            e->Dump();
        std::cout << "\nEND_PARAGRAPH ";
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
        if( ! myElement.size() )
            throw std::runtime_error("No paragraph for sentence");
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

    void Read( const std::string& filename )
    {
        std::ifstream in( filename.c_str() );
        if( ! in.is_open() )
            throw std::runtime_error("Cannot open input file");
        std::stringstream sstr;
        sstr << in.rdbuf();
        myRawText = sstr.str();
    }

    void Parse()
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
                if( p_now > p_para ) {

                    // reached the end of a para

                    CheckForTitle();

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


private:
    std::vector< cText > myText;
    std::string myRawText;

    /** Check if last paragraph is actually a title */
    void CheckForTitle()
    {
        // get last element
        cElement * lp = myText.back().mySection.back().myElement.back();

        // check for paragraph that is really a title
        if( ! lp->IsTitle() )
            return;

        // add the title
        Add( cTitle( lp->myElement[0]->myText ) );

        // null the paragraph that was a title
        lp->clear();

    }
};

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

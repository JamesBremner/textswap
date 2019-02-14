enum class eSwapOption
{
    all,
    none,
    first_fixed,
    last_fixed,
    first_last_fixed
};

class cOutput;

class cSentence
{
public:
    /** Construct from text structure ID */
    cSentence( char c )
        : myID(
    {
        c
    } )
    {
    }
    /** Construct from sentence text

    The sentence will be assigned the next text structure ID
    */
    cSentence( const std::string& text )
        : myText( text )
    {
        char cid = myLastID++ + 'a';
        myID = std::string( { cid } );
    }
    /** Get text structure ID */
    std::string  TextID()
    {
        return myID;
    }

    /** Get sentence text */
    std::string TextText()
    {
        return myText;
    }
    /** Restart the text structure id

    This should be called at the beginning of each text
    so that corresponding sentences in every structure will have the same id
    */
    static void ResetID()
    {
        myLastID = 0;
    }
    /** Sentences are equal if they occupy the same position in their text */
    bool operator==(const cSentence& rhs) const
    {
        return myID == rhs.myID;
    }
    bool operator!=(const cSentence& rhs) const
    {
        return myID != rhs.myID;
    }
private:
    std::string myID;       // letter identifying sentence position in text structure
    static int myLastID;    // last id used, constructor will increment and use for next
    std::string myText;     // text of the sentence
};



class cPara
{
public:
    eSwapOption mySwap;
    std::vector< cSentence > mySentence;
    void push_back( const cSentence& sent )
    {
        mySentence.push_back( sent );
    }
    std::vector< cSentence >::iterator begin()
    {
        return  mySentence.begin();
    }
    std::vector< cSentence >::iterator end()
    {
        return  mySentence.end();
    }
    int size()
    {
        return (int) mySentence.size();
    }
    void Shuffle( std::default_random_engine& re );

};
class cSection
{
public:
    eSwapOption mySwap;
    std::vector< cPara > myPara;
    cPara& back()
    {
        return myPara.back();
    }
    void push_back( const cPara& para )
    {
        myPara.push_back( para );
    }
    std::vector< cPara >::iterator begin()
    {
        return myPara.begin();
    }
    std::vector< cPara >::iterator end()
    {
        return myPara.end();
    }
};
class cText
{
public:
    eSwapOption mySwap;
    std::vector< cSection > mySection;
    cSection& back()
    {
        return mySection.back();
    }
    void push_back( const cSection& sect )
    {
        mySection.push_back( sect );
    }
    std::vector< cSection >::iterator begin()
    {
        return mySection.begin();
    }
    std::vector< cSection >::iterator end()
    {
        return mySection.end();
    }
    int size()
    {
        return mySection.size();
    }
};

class cFileText
{
public:
    std::vector< cText > myText;

    cFileText();
    cFileText( const std::string& filename );
    void AddText();
    void AddSection();
    void AddPara();
    void AddSentence( const std::string& e );

    std::string Text();

    void Shuffle();

    cOutput Output( int text );

    void setSwapAllParas( eSwapOption swap );


private:
    cText& LastText()
    {
        return myText.back();
    }
    cSection& LastSection()
    {
        return LastText().back();
    }
    cPara& LastPara()
    {
        return LastSection().back();
    }
};



class cSentence;
typedef std::vector< cSentence * > para_t;
typedef std::vector< para_t > section_t;
typedef std::vector< section_t > text_t;

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


class cFileText
{
public:
    std::vector< text_t > myText;

    cFileText();
    void AddText();
    void AddSection();
    void AddPara();
    void AddSentence( const std::string& e );

    std::string Text();

    void Shuffle();

    cOutput Output( int text );


private:
    text_t& LastText()
    {
        return myText.back();
    }
    section_t& LastSection()
    {
        return LastText().back();
    }
    para_t& LastPara()
    {
        return LastSection().back();
    }
};



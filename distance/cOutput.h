typedef cElement                elem_t;

class cOutput;

class cNGram
{
public:
    std::vector< elem_t > myElem;
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
    cNGram( cOutput& o , int pos, int len );
    int size() const
    {
        return (int) myElem.size();
    }
    std::string Text()
    {
        std::stringstream ss;
        ss << "'";
        for( auto c : myElem )
            ss << c.TextID();
        ss << "'";
        return ss.str();
    }
    std::vector<elem_t>::iterator begin()
    {
        return myElem.begin();
    }
    std::vector<elem_t>::iterator end()
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
    std::vector< elem_t > myElement;     // output as a vector of individual elements
    std::vector< cNGram >   myGram;      // output as a vector of matching n-grams

    /** Parse input string of characters representing elements */
    void Parse( const std::string& s );

    int size()
    {
        return (int) myElement.size();
    }
    std::vector< elem_t >::iterator begin()
    {
        return myElement.begin();
    }
    std::vector< elem_t >::iterator end()
    {
        return myElement.end();
    }
    /** find location of element */
    int find( elem_t& c );

    /** Display text of converted output */
    std::string Text();
    std::string TextElements();

    /** Find matching ngrams */
    void Match( cOutput& o );

    /** Find match for a ngram in unconverted output */
    int Match( cNGram& g );

    /** Index of a ngram in the converted output */
    int Where( cNGram& g );

    /** Convert from elements to matching ngrams of elements */
    void Convert( std::vector< cNGram >& g );

};


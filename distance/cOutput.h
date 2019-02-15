class cSentence;

class cOutput;

class cNGram
{
public:

    cNGram()
    {

    }
    cNGram( cSentence c );
    cNGram( cOutput& o, int pos, int len );

    int size() const
    {
        return (int) myElem.size();
    }
    std::string Text();

    std::vector<cSentence>::iterator begin()
    {
        return myElem.begin();
    }
    std::vector<cSentence>::iterator end()
    {
        return myElem.end();
    }
    bool operator==(const cNGram& rhs) const;

    cSentence& operator[] (int x)
    {
        return myElem[x];
    }
private:
    std::vector< cSentence > myElem;
};

class cOutput
{
public:
    std::vector< cSentence > myElement;     // output as a vector of individual elements
    std::vector< cNGram >   myGram;      // output as a vector of matching n-grams

    /** Parse input string of characters representing elements */
    void Parse( const std::string& s );

    int size()
    {
        return (int) myElement.size();
    }
    std::vector< cSentence >::iterator begin()
    {
        return myElement.begin();
    }
    std::vector< cSentence >::iterator end()
    {
        return myElement.end();
    }
    /** find location of element */
    int find( cSentence& c );

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
    void Convert( std::vector< cNGram >& vg );

};

/// The weight assigned to particular differences between texts.
struct sWeight
{
    float move;
    float presence;
};

/** Calculate distance between two outputs
    @param[in] o1 first output text
    @param[in] o2 second output text
    @return distance
*/
float Distance3(
    cOutput& o1,
    cOutput& o2,
    const sWeight& W
);

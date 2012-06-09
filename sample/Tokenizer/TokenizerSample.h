// Ref.: C++ Cookbook http://www.ryanstephens.com/cookbook.html
#ifndef TOKENIZERSAMPLE_H_INCLUDED
#define TOKENIZERSAMPLE_H_INCLUDED

#include <cntl/String.h>

class StringTokenizer
{
public:

    StringTokenizer( const cntl::String& s, const char* delim = NULL )
        : str_( s )
        , count_( -1 )
        , begin_( 0 )
        , end_( 0 )
        {
            if ( !delim )
                delim_ = " \f\n\r\t\v";  //default to whitespace
            else
                delim_ = delim;

            // Point to the first token
            begin_ = str_.find_first_not_of( delim_ );
            end_ = str_.find_first_of( delim_, begin_ );
        }

    size_t
    countTokens()
        {
            if ( count_ >= 0 ) // return if we've already counted
                return(count_);

            std::size_t n = 0;
            std::size_t i = 0;

            for (;;)
            {
                // advance to the first token
                if ( (i = str_.find_first_not_of(delim_, i)) == cntl::String::npos )
                    break;
                // advance to the next delimiter
                i = str_.find_first_of( delim_, i+1 );
                n++;
                if ( i == cntl::String::npos )
                    break;
            }
            return (count_ = n);
        }

    bool
    hasMoreTokens()
        {
            return( begin_ != end_ );
        }

    void
    nextToken( cntl::String& s )
        {
            if ( begin_ != cntl::String::npos && end_ != cntl::String::npos )
            {
                s = str_.substr( begin_, end_-begin_ );
                begin_ = str_.find_first_not_of( delim_, end_ );
                end_ = str_.find_first_of( delim_, begin_ );
            }
            else if ( begin_ != cntl::String::npos && end_ == cntl::String::npos )
            {
                s = str_.substr( begin_, str_.size()-begin_ );
                begin_ = str_.find_first_not_of( delim_, end_ );
            }
        }

private:

    StringTokenizer()
        {};

    cntl::String delim_;
    cntl::String str_;
    int count_;
    int begin_;
    int end_;
};

#endif

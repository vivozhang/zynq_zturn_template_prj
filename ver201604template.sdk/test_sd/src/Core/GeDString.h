///////////////////////////////////////////////////////////////////////////////
//                                                                     
// TITLE:       GE Dynamically Allocated String Class ( cSString )
//
// $Header: /Runtime/Core/Branches/CurrentProductVersion/lib/sysapi/GeDString.h 9     9/22/08 11:24a Ahmeda $Log: /Runtime/MarkVIe/CurrentProductVersion/Lib/sysapi/public/GenericTemplate.h $ 
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef GEDSTRING_H
#define GEDSTRING_H

// Include Files: 
//---------------
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
//#include <new.h>

// Constants:
//-----------
#ifndef NULL
#define NULL 0
#endif

#define DEFAULT_GEDSTRING_BLKSIZE 32
#define SIZE_TYPE int

// Types and Classes:
//-------------------                   

//=============================================================================
// String Class Definition
//=============================================================================

// String Class Definition
//-----------------------------------------------------------------------------
class cDString
{
private:
    // Member Data
    char *      mpString;                    // The string, NULL terminated
    SIZE_TYPE   mBufferSize;
    SIZE_TYPE   mChunkSize;
    unsigned    mNumAllocErrors;
    
    // Internal string class methods
    unsigned    BufferSizeRequired(const unsigned & NewStringSize);
    bool        IsAllocated() const; 
    char *      Allocate(const unsigned & BufferSize);
    char *      Reallocate(const unsigned & BufferSize);

protected:
    char * GetFreeChunk(unsigned int Size);
    
public:
// Test Methods
//    SIZE_TYPE   BufferSize() { return mBufferSize; }
//    SIZE_TYPE   ChunkSize() { return mChunkSize; }
/*
    class cIterator
    {
       private:
            cSLinkedListNode<LIST_ITEM> *mpCurr;
            
            // - Copy constructor and assignment are okay since we do jyst want
            //   to copy the pointer value mpCurr.
            //iterator(iterator & Iter);
            //iterator & operastor=(iterator & Iter);
        public:
            // Constructor / Destructor
            cIterator() { mpCurr = NULL; }
            cIterator(cSLinkedListNode<LIST_ITEM> * pNode) { mpCurr = pNode; }

            // Assignment Methods
            cIterator & operator=(const cIterator & Iter)  
                        { return mpCurr = Iter.mpCurr, *this; }
            cIterator & operator++()                 
                        { mpCurr = mpCurr->mpNext;  return *this; }     
            cIterator   operator++(int);    // Not defined
            cIterator & operator--();       // Not defined
            cIterator   operator--(int);    // Not defined

            // Equality Methods
            bool        operator==(const cIterator & Iter) const
                        { return (mpCurr == Iter.mpCurr); } 
            bool        operator!=(const cIterator & Iter) const
                        { return (mpCurr != Iter.mpCurr); } 

            // Dereferencing Methods
            LIST_ITEM * operator->()                 
                        { assert(mpCurr); return &mpCurr->mItem; }
            LIST_ITEM & operator*()                  
                        { assert(mpCurr); return mpCurr->mItem; }
            
            friend class cSLinkedList;
    };
*/    
    
    // Member Functions
public:
    static const int  npos = -1;

    // Constructor / Destructors
    //---------------------------------
    cDString(unsigned ChunkSize = DEFAULT_GEDSTRING_BLKSIZE);    // Constructor
//    explicit basic_string(const allocator_type& al);
    cDString(const cDString & String);                      // Copy Constructor 
//    basic_string(const basic_string& right, size_type roff,
//        size_type count = npos);
//    basic_string(const basic_string& right, size_type roff,
//        size_type count, const allocator_type& al);
//    basic_string(const value_type *ptr, size_type count);
//    basic_string(const value_type *ptr, size_type count,
//        const allocator_type& al);
    cDString(const char * pString);                         // WARNING this method may fail memory allocation

//    basic_string(const value_type *ptr,
//        const allocator_type& al);
//    basic_string(size_type count, value_type ch);
//    basic_string(size_type count, value_type ch,
//        const allocator_type& al);
//    template <class InIt>
//        basic_string(InIt first, InIt last);
//    template <class InIt>
//        basic_string(InIt first, InIt last,
//            const allocator_type& al);
    ~cDString();                         // Destructor

    // Operators
    //---------------------------------
    cDString & operator=(const cDString & String);
    cDString & operator=(const char *pString);
    cDString & operator=(char Char);
    cDString & operator+=(const cDString & String);         
    cDString & operator+=(const char *pString);
    cDString & operator+=(char Char);

    const char & operator[](SIZE_TYPE Offset) const;

    friend cDString operator+ (const cDString & Left,   const cDString & Right);
    friend cDString operator+ (const cDString & String, const char *pString);
    friend cDString operator+ (const char *pString,     const cDString & String);
    friend cDString operator+ (const cDString & String, const char Char);
    friend cDString operator+ (const char Char,         const cDString & String);

    friend bool     operator==(const cDString & Left,   const cDString & Right);
    friend bool     operator==(const char * pString,    const cDString & String);
    friend bool     operator==(const cDString & String, const char * pString);

    friend bool     operator!=(const cDString & Left,   const cDString & Right);
    friend bool     operator!=(const char * pString,    const cDString & String);
    friend bool     operator!=(const cDString & String, const char * pString);

    friend bool operator<(const cDString & Left,const cDString & Right);
//template<class Elem, class Tr, class Alloc>
//    bool operator<(const basic_string<Elem, Tr, Alloc>& left,const basic_string<Elem, Tr, Alloc>& right);
//template<class Elem, class Tr, class Alloc>
//    bool operator<(const basic_string<Elem, Tr, Alloc>& left,const Elem *right);
//template<class Elem, class Tr, class Alloc>
//    bool operator<(const Elem *left,const basic_string<Elem, Tr, Alloc>& right);

    friend bool operator>(const cDString & Left,const cDString & Right);
//template<class Elem, class Tr, class Alloc>
//    bool operator>(const basic_string<Elem, Tr, Alloc>& left,const basic_string<Elem, Tr, Alloc>& right);
//template<class Elem, class Tr, class Alloc>
//    bool operator>(const basic_string<Elem, Tr, Alloc>& left,const Elem *right);
//template<class Elem, class Tr, class Alloc>
//    bool operator>(const Elem *left,const basic_string<Elem, Tr, Alloc>& right);

    friend bool operator<=(const cDString & Left,const cDString & Right);
//template<class Elem, class Tr, class Alloc>
//    bool operator<=(const basic_string<Elem, Tr, Alloc>& left,const basic_string<Elem, Tr, Alloc>& right);
//template<class Elem, class Tr, class Alloc>
//    bool operator<=(const basic_string<Elem, Tr, Alloc>& left,const Elem *right);
//template<class Elem, class Tr, class Alloc>
//    bool operator<=(const Elem *left,const basic_string<Elem, Tr, Alloc>& right);

    friend bool operator>=(const cDString & Left,const cDString & Right);
//template<class Elem, class Tr, class Alloc>
//    bool operator>=(const basic_string<Elem, Tr, Alloc>& left,const basic_string<Elem, Tr, Alloc>& right);
//template<class Elem, class Tr, class Alloc>
//    bool operator>=(const basic_string<Elem, Tr, Alloc>& left,const Elem *right);
//template<class Elem, class Tr, class Alloc>
//    bool operator>=(const Elem *left,const basic_string<Elem, Tr, Alloc>& right);

//template<class Elem, class Tr, class Alloc>
//    void swap(basic_string<Elem, Tr, Alloc>& left,basic_string<Elem, Tr, Alloc>& right);
//template<class Elem, class Tr, class Alloc>
//    basic_ostream<Elem>& operator<<(basic_ostream<Elem>& ostr,const basic_string<Elem, Tr, Alloc>& str);
//template<class Elem, class Tr, class Alloc>
//    basic_istream<Elem>& operator>>(basic_istream<Elem>& istr,basic_string<Elem, Tr, Alloc>& str);
//template<class Elem, class Tr, class Alloc>
//    basic_istream<Elem, Tr>& getline(basic_istream<Elem, Tr>& istr,basic_string<Elem, Tr, Alloc>& str);
// template<class Elem, class Tr, class Alloc>
//    basic_istream<Elem, Tr>& getline(basic_istream<Elem, Tr>& istr,basic_string<Elem, Tr, Alloc>& str,Elem delim);

//    operator const char*() const;  // Implicit type conversion! - Do not implement

    // Iterator Access Methods
    //---------------------------------
//    iterator begin();
//    const_iterator begin() const;
//    iterator end();
//    const_iterator end() const;
//    reverse_iterator rbegin();
//    const_reverse_iterator rbegin() const;
//    reverse_iterator rend();
//    const_reverse_iterator rend() const;
    const char & at(SIZE_TYPE Offset) const;
//    reference at(size_type off);
//    const_reference operator[](size_type off) const;
//    reference operator[](size_type off);
//    void push_back(value_type ch);

    // Type Conversion Methods
    //---------------------------------
    const char *c_str() const;
    const char *data() const;

    // General Methods
    //---------------------------------
    SIZE_TYPE length() const;
    SIZE_TYPE size() const;
//    size_type max_size() const;
//    void resize(size_type newsize, value_type ch = value_type());
//    size_type capacity() const;
//    void reserve(size_type count = 0);
    bool empty() const;
//    basic_string& append(const basic_string& right);
//    basic_string& append(const basic_string& right,size_type roff, size_type count);
    cDString & append(const char *pString, SIZE_TYPE Count);
    cDString & append(const char *pString);
    cDString & append(char Char);
//    cDString & append(SIZE_TYPE Count, char Char);
//    template<class InIt>
//        basic_string& append(InIt first, InIt last);
//    basic_string& assign(const basic_string& right);
//    basic_string& assign(const basic_string& right,size_type roff, size_type count);
    cDString & assign(const char * pString, SIZE_TYPE Count);
    cDString & assign(const char * pString);
//    basic_string& assign(size_type count, value_type ch);
//    template<class InIt>
//        basic_string& assign(InIt first, InIt last);
//    basic_string& insert(size_type off,const basic_string& right);
//    basic_string& insert(size_type off,const basic_string& right, size_type roff,size_type count);
//    basic_string& insert(size_type off,const value_type *ptr, size_type count);
//    basic_string& insert(size_type off,const value_type *ptr);
//    basic_string& insert(size_type off,size_type count, value_type ch);
//    iterator insert(iterator where,value_type ch = value_type());
//    void insert(iterator where, size_type count, value_type ch);
//    template<class InIt>
//        void insert(iterator where,InIt first, InIt last);
    cDString & erase(SIZE_TYPE Offset = 0,SIZE_TYPE Count = npos);
//    iterator erase(iterator where);
//    iterator erase(iterator first, iterator last);
    void clear();
//    basic_string& replace(size_type off, size_type n0,const basic_string& right);
//    basic_string& replace(size_type off, size_type n0,const basic_string& right, size_type roff,size_type count);
//    basic_string& replace(size_type off, size_type n0,const value_type *ptr, size_type count);
//    basic_string& replace(size_type off, size_type n0,const value_type *ptr);
//    basic_string& replace(size_type off, size_type n0,size_type count, value_type ch);
//    basic_string& replace(iterator first, iterator last,const basic_string& right);
//    basic_string& replace(iterator first, iterator last,const value_type *ptr, size_type count);
//    basic_string& replace(iterator first, iterator last,const value_type *ptr);
//    basic_string& replace(iterator first, iterator last,size_type count, value_type ch);
    cDString & replace(char CharToReplace, char NewChar);
    void xmldecode();
//    template<class InIt>
//        basic_string& replace(iterator first, iterator last,InIt first2, InIt last2);
    SIZE_TYPE copy(char * pString, SIZE_TYPE Count,SIZE_TYPE Offset = 0) const;
//    void swap(basic_string& right);
    SIZE_TYPE find(const cDString & String,SIZE_TYPE Offset = 0) const;
//    size_type find(const value_type *ptr, size_type off,size_type count) const;
    SIZE_TYPE find(const char * pString,SIZE_TYPE Offset = 0) const;
//    size_type find(value_type ch, size_type off = 0) const;
//    size_type rfind(const basic_string& right,size_type off = npos) const;
//    size_type rfind(const value_type *ptr, size_type off,size_type count = npos) const;
//    size_type rfind(const value_type *ptr,size_type off = npos) const;
//    size_type rfind(value_type ch,size_type off = npos) const;
//    size_type find_first_of(const basic_string& right,size_type off = 0) const;
//    size_type find_first_of(const value_type *ptr,size_type off, size_type count) const;
//    size_type find_first_of(const value_type *ptr,size_type off = 0) const;
    int find_first_of(char Char, SIZE_TYPE Offset = 0) const;
//    size_type find_last_of(const basic_string& right,size_type off = npos) const;
//    size_type find_last_of(const value_type *ptr,size_type off, size_type count = npos) const;
//    size_type find_last_of(const value_type *ptr,size_type off = npos) const;
    int find_last_of(char Char, SIZE_TYPE Offset = npos) const;
//    size_type find_first_not_of(const basic_string& right,size_type off = 0) const;
//    size_type find_first_not_of(const value_type *ptr,size_type off, size_type count) const;
//    size_type find_first_not_of(const value_type *ptr,size_type off = 0) const;
//    size_type find_first_not_of(value_type ch,size_type off = 0) const;
//    size_type find_last_not_of(const basic_string& right,size_type off = npos) const;
//    size_type find_last_not_of(const value_type *ptr,size_type off, size_type count) const;
//    size_type find_last_not_of(const value_type *ptr,size_type off = npos) const;
//    size_type find_last_not_of(value_type ch,size_type off = npos) const;
    cDString substr(SIZE_TYPE Offset = 0, int Count = npos) const;
//    int compare(const basic_string& right) const;
//    int compare(size_type off, size_type n0,const basic_string& right);
//    int compare(size_type off, size_type n0,const basic_string& right, size_type roff,size_type count);
//    int compare(const value_type *ptr) const;
//    int compare(size_type off, size_type n0,const value_type *ptr) const;
//    int compare(size_type off, size_type n0,const value_type *ptr, size_type off) const;
//    allocator_type get_allocator() const;
	 cDString & trimleft(int count); 
	 cDString & trimright(int count);
};

// Inline Function Definitions: 
//-----------------------------

//-----------------------------------------------------------------------------
// Internal methods
//-----------------------------------------------------------------------------
inline unsigned cDString::BufferSizeRequired(const unsigned & NewStringSize)
{
    if (NewStringSize == 0)
        // will point to mBufferSize
        return 0;
    else
        // String Length + null char
        return (((NewStringSize) / mChunkSize) + 1) * mChunkSize;
}
inline bool cDString::IsAllocated() const
{
    return (mpString != (char *) &mBufferSize);  
}

inline char * cDString::Allocate(const unsigned & BufferSize)
{
    if (IsAllocated())
        free(mpString);

    if (BufferSize == 0)
    {
        mpString = (char *) &mBufferSize;
        mBufferSize = 0;
        return NULL;
    }
    else if ((mpString = (char *) malloc(BufferSize)) != NULL)
    {
        mpString[0] = 0;
        mBufferSize = BufferSize;
        return mpString;
    }
    else
    {
        mpString = (char *) &mBufferSize;
        mBufferSize = 0;
        ++mNumAllocErrors;
        return NULL;
    }
}
inline char * cDString::Reallocate(const unsigned & BufferSize)
{
    // Reallocate only makes the buffer bigger 
    // (BufferSise must be bigger than mBufferSize)!!!!
    if (IsAllocated())
    {
        // Reallocate
        char * pTemp = (char *) realloc(mpString,BufferSize);
        if (!pTemp)
            return NULL;    // Couldn't allocate, nothing changed
            
        mpString = pTemp;
        mBufferSize = BufferSize;
        return mpString;    // We reallocated
    }
    else
    {
        // Allocate
        if (BufferSize == 0)
        {
            return NULL;    // Increasing a 0 buffer to 0
        }
        else if ((mpString = (char *) malloc(BufferSize)) != NULL)
        {
            mpString[0] = 0;
            mBufferSize = BufferSize;
            return mpString;// Allocated a new buffer
        }
        else
        {
            mpString = (char *) &mBufferSize;
            mBufferSize = 0;
            ++mNumAllocErrors;
            return NULL;    // Couldn't allocate a new buffer
        }
    }
}

//-----------------------------------------------------------------------------
// Constructor / Destructor methods
//-----------------------------------------------------------------------------
inline cDString::~cDString()
{
    if (IsAllocated())
        free(mpString);
}

inline cDString::cDString(unsigned ChunkSize)
{
    mpString = (char *) &mBufferSize;
    mBufferSize = 0;
    mChunkSize = ChunkSize;
    mNumAllocErrors = 0;
}

inline cDString::cDString(const char * pString)
{
    mpString = (char *) &mBufferSize;
    mBufferSize = 0;
    mChunkSize = DEFAULT_GEDSTRING_BLKSIZE;
    mNumAllocErrors = 0;

    if (!pString)
        return;
        
    if (Allocate(BufferSizeRequired(strlen(pString))))
        strcpy(mpString,pString);
}

inline cDString::cDString(const cDString & String)
{
    mpString = (char *) &mBufferSize;
    mBufferSize = 0;
    mChunkSize = DEFAULT_GEDSTRING_BLKSIZE;
    mNumAllocErrors = 0;

    if (Allocate(BufferSizeRequired(String.length())))
        strcpy(mpString,String.mpString);
}

//-----------------------------------------------------------------------------
// Operator methods
//-----------------------------------------------------------------------------
inline cDString & cDString::operator=(const cDString & String)
{
    if (this == &String)        // Cannot Assign yourself
        return *this;
        
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(String.length());
    if (mBufferSize != NewBufferSize)
        Allocate(NewBufferSize);
    
    // If the buffer is allocated
    if (IsAllocated())
        strcpy(mpString,String.mpString);

    return *this;  
}

inline cDString & cDString::operator=(const char *pString)
{
    if (mpString == pString)    // Cannot assign yourself
        return *this;

    if (!pString)               // Protect from NULL char *
    {
        Allocate(0);
        return *this;
    }
        
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(strlen(pString));
    if (mBufferSize != NewBufferSize)
        Allocate(NewBufferSize);

    // If the buffer is allocated
    if (IsAllocated())
        strcpy(mpString,pString);

    return *this;  
}

inline cDString & cDString::operator=(char Char)
{
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(1);
    if (mBufferSize != NewBufferSize)
        Allocate(NewBufferSize);

    // If the buffer is allocated
    if (IsAllocated())
    {
        mpString[0] = Char;
        mpString[1] = 0;
    }

    return *this; 
}

inline cDString & cDString::operator+=(const cDString & String)
{
    if (this == &String)        // Cannot Assign yourself
        return *this;
        
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(length() + String.length());
    if (mBufferSize != NewBufferSize)
        Reallocate(NewBufferSize);
    
    if (mBufferSize == NewBufferSize)   // If the buffer was reallocated
        strcat(mpString,String.mpString);

    return *this;  
}

inline cDString & cDString::operator+=(const char *pString)
{
    if (mpString == pString)    // Cannot Assign yourself
        return *this;
        
    if (!pString)               // Protect from NULL char *
        return *this;
    
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(length() + strlen(pString));
    if (mBufferSize != NewBufferSize)
        Reallocate(NewBufferSize);

    if (mBufferSize == NewBufferSize)   // If the buffer was reallocated
        strcat(mpString,pString);

    return *this; 
}

inline cDString & cDString::operator+=(char Char)
{
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(length() + 1);
    if (mBufferSize != NewBufferSize)
        Reallocate(NewBufferSize);

    if (mBufferSize == NewBufferSize)   // If the buffer was reallocated
    {
        mpString[strlen(mpString)+1] = 0;
        mpString[strlen(mpString)] = Char;
    }

    return *this; 
}

inline const char & cDString::operator[](SIZE_TYPE Offset) const
{
    return at(Offset);
}

inline cDString operator+(const cDString & Left, const cDString & Right)
{
    cDString NewString;
       
    // Allocate memory for our new string
    SIZE_TYPE NewBufferSize = NewString.BufferSizeRequired(Left.length() + Right.length());
    if (NewString.Allocate(NewBufferSize))
    {
        strcpy(NewString.mpString, Left.mpString);
        strcat(NewString.mpString, Right.mpString);
    }

    return NewString;
}
inline cDString operator+(const cDString & String,  const char *pString)
{
    if (!pString)                // Protect from NULL char *
        return String;

    cDString NewString;
       
    // Allocate memory for our new string
    SIZE_TYPE NewBufferSize = NewString.BufferSizeRequired(String.length() + strlen(pString));
    if (NewString.Allocate(NewBufferSize))
    {
        strcpy(NewString.mpString, String.mpString);
        strcat(NewString.mpString, pString);
    }

    return NewString;
}
inline cDString operator+(const char *pString, const cDString & String)
{
    if (!pString)                // Protect from NULL char *
        return String;

    cDString NewString;
       
    // Allocate memory for our new string
    SIZE_TYPE NewBufferSize = NewString.BufferSizeRequired(String.length() + strlen(pString));
    if (NewString.Allocate(NewBufferSize))
    {
        strcpy(NewString.mpString, pString);
        strcat(NewString.mpString, String.mpString);
    }

    return NewString;
}
inline cDString operator+(const cDString & String,  const char Char)
{
    cDString NewString;
       
    // Allocate memory for our new string
    SIZE_TYPE NewBufferSize = NewString.BufferSizeRequired(String.length() + 1);
    if (NewString.Allocate(NewBufferSize))
    {
        strcpy(NewString.mpString, String.mpString);
        NewString.mpString[strlen(NewString.mpString) + 1] = 0;
        NewString.mpString[strlen(NewString.mpString)] = Char;
    }

    return NewString;
}
inline cDString operator+(const char Char, const cDString & String)
{
    cDString NewString;
       
    // Allocate memory for our new string
    SIZE_TYPE NewBufferSize = NewString.BufferSizeRequired(String.length() + 1);
    if (NewString.Allocate(NewBufferSize))
    {
        NewString.mpString[1] = 0;
        NewString.mpString[0] = Char;
        strcat(NewString.mpString, String.mpString);
    }

    return NewString;
}

inline bool operator==(const cDString & Left,   const cDString & Right)
{
    return strcmp(Left.mpString,Right.mpString) == 0;
}
inline bool operator==(const char * pString,    const cDString & String)
{
    return strcmp(pString,String.mpString) == 0;
}
inline bool operator==(const cDString & String, const char * pString)
{
    return strcmp(pString,String.mpString) == 0;
}

inline bool operator!=(const cDString & Left,   const cDString & Right)
{
    return strcmp(Left.mpString,Right.mpString) != 0;
}
inline bool operator!=(const char * pString,    const cDString & String)
{
    return strcmp(pString,String.mpString) != 0;
}
inline bool operator!=(const cDString & String, const char * pString)
{
    return strcmp(pString,String.mpString) != 0;
}

inline bool operator<(const cDString & Left,const cDString & Right)
{
    return strcmp(Left.mpString,Right.mpString) < 0;
}

inline bool operator>(const cDString & Left,const cDString & Right)
{
    return strcmp(Left.mpString,Right.mpString) > 0;
}

inline bool operator<=(const cDString & Left,const cDString & Right)
{
    return strcmp(Left.mpString,Right.mpString) <= 0;
}

inline bool operator>=(const cDString & Left,const cDString & Right)
{
    return strcmp(Left.mpString,Right.mpString) >= 0;
}


//inline cDString::operator const char*() const
//{
//    return mpString;
//}


//-----------------------------------------------------------------------------
// Textual methods - Alphabetical
//-----------------------------------------------------------------------------

inline cDString & cDString::append(const char * pString)
{
    SIZE_TYPE Count; 

    if (mpString == pString)        // Cannot Assign yourself
        return *this;

    if (!pString)               // Protect from NULL char *
    {
        Allocate(0);
        return *this;
    }

    Count = strlen(pString);
    
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(length() + Count);
    if (mBufferSize != NewBufferSize)
        Reallocate(NewBufferSize);
    
    if (mBufferSize == NewBufferSize)   // If the buffer was reallocated
        strcat(mpString,pString);

    return *this;  
}

inline cDString & cDString::append(const char *pString, SIZE_TYPE Count)
{
    if (mpString == pString)    // Cannot assign yourself
        return *this;

    if (!pString)               // Protect from NULL char *
    {
        Allocate(0);
        return *this;
    }

    if (Count == npos)
        Count = strlen(pString);
        
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(length() + Count);
    if (mBufferSize != NewBufferSize)
        Reallocate(NewBufferSize);

    // If the buffer is allocated
    if (IsAllocated())
    {
        strncat(mpString,pString,Count);
    }
    
    return *this;
}

inline cDString & cDString::append(char Char)
{
    *this += Char;
    return *this;  
}

/* vivo: comment out
inline cDString & cDString::assign(const char * pString, SIZE_TYPE Count)
{
    if (mpString == pString)    // Cannot assign yourself
        return *this;

    if (!pString)               // Protect from NULL char *
    {
        Allocate(0);
        return *this;
    }

    if (Count == npos)
        Count = strlen(pString);
        
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired(__min((SIZE_TYPE)strlen(pString),Count));
    if (mBufferSize != NewBufferSize)
        Allocate(NewBufferSize);

    // If the buffer is allocated
    if (IsAllocated())
    {
        memset(mpString,0,mBufferSize);
        strncpy(mpString,pString,Count);
    }

    return *this;  
}

*/


inline cDString & cDString::assign(const char * pString)
{
    if (mpString == pString)    // Cannot assign yourself
        return *this;

    if (!pString)               // Protect from NULL char *
    {
        Allocate(0);
        return *this;
    }
    // Reallocate memory if our buffer is not the right number of chunks 
    SIZE_TYPE NewBufferSize = BufferSizeRequired((SIZE_TYPE)strlen(pString) + 1);
    if (mBufferSize != NewBufferSize)
        Allocate(NewBufferSize);

    // If the buffer is allocated
    if (IsAllocated())
    {
        memset(mpString,0,mBufferSize);
        strcpy(mpString,pString);
    }
    
    return *this;  
}

// It is up to the user to make sure that offset off is within the string.
inline const char & cDString::at(SIZE_TYPE Offset) const
{
    return mpString[Offset];
}

inline const char * cDString::c_str() const
{
    return mpString;
}

inline void cDString::clear()
{
    erase();
}

// The member function copies up to count elements from the controlled sequence, 
// beginning at position off, to the array of char * beginning at pString. It 
// returns the number of elements actually copied. (May not be NULL trminated)
/* vivo commentout
inline SIZE_TYPE cDString::copy(char * pString, SIZE_TYPE Count,SIZE_TYPE Offset) const
{
    if (Count == npos)
        Count = length();
    strncpy(pString ,mpString + Offset, Count);
    return __min(Count,(SIZE_TYPE) strlen(mpString) - Offset);
}
*/

inline const char * cDString::data() const
{
    return mpString;
}

inline bool cDString::empty() const
{
    return strlen(mpString) == 0;
}

// Doesn't reallocate memory as the string shrinks.
inline cDString & cDString::erase(SIZE_TYPE Offset,SIZE_TYPE Count)
{
    SIZE_TYPE Pos;

    if (Count == npos)
        Count = strlen(mpString);  // Remove all elements

    for (Pos = Offset;
         Pos + Count < (SIZE_TYPE) strlen(mpString);
         ++Pos)
        mpString[Pos] = mpString[Pos + Count];

    mpString[Pos] = 0;
    
    if (length() == 0 && IsAllocated())
        Allocate(0);

    return *this;
}


inline SIZE_TYPE cDString::find(const cDString & String, SIZE_TYPE Offset) const
{
    if (!String.length())
        return npos;
        
    SIZE_TYPE Pos;
    for (Pos = Offset;
         Pos + String.length() - 1 < length();
         ++Pos)
    {
        if (strncmp(mpString + Pos,String.mpString,String.length()) == 0)
            return Pos;
    }
    return npos;
}
inline SIZE_TYPE cDString::find(const char * pString, SIZE_TYPE Offset) const
{
    if (!pString || !*pString)
        return npos;

    SIZE_TYPE Pos;
    for (Pos = Offset;
         Pos + (SIZE_TYPE)strlen(pString) - 1 < length();
         ++Pos)
    {
        if (strncmp(mpString + Pos,pString,strlen(pString)) == 0)
            return Pos;
    }
    return npos;
}

/* vivo: comment out
inline int cDString::find_first_of(char Char, SIZE_TYPE Offset) const
{
    for (int Pos = __max(Offset,0); Pos < length(); ++Pos)
    {
        if (mpString[Pos] == Char)
            return Pos;
    }            
    return npos;
}


inline int cDString::find_last_of(char Char, SIZE_TYPE Offset) const
{
    int Pos = Offset == npos ? length()-1 : Offset;
    Pos = __min(Pos,length() - 1);
    for (;Pos >= 0; --Pos)
    {
        if (mpString[Pos] == Char)
            return Pos;
    }
    return npos;
}*/

inline SIZE_TYPE cDString::length() const
{
    return !IsAllocated() ? 0 : strlen(mpString);
}

inline SIZE_TYPE cDString::size() const
{
    return !IsAllocated() ? 0 : strlen(mpString);
}

inline cDString cDString::substr(SIZE_TYPE Offset, int Count) const
{
    cDString NewString;

    // String is non-null
    if (Offset >= length()) // Illegal parameters
        return NewString;
        
    if (Count == npos)
        Count = strlen(mpString);  // Copy to the end

    NewString = (mpString + Offset);
    if (NewString.length() > Count)
        NewString.mpString[Count] = 0;

    return NewString;
}

// Internal Function Prototypes:
//------------------------------

inline char *cDString::GetFreeChunk(unsigned int Size)
{
    SIZE_TYPE CurrentLength = length();
    SIZE_TYPE NewBufferSize = BufferSizeRequired(CurrentLength + Size + 1);

    if (mBufferSize != NewBufferSize)
        if(Reallocate(NewBufferSize) == NULL)
            return NULL;

    return mpString + CurrentLength;
}

inline cDString & cDString::replace(char CharToReplace, char NewChar)
{
    SIZE_TYPE CurrentLength = length();
    SIZE_TYPE i;
    
    if(NewChar == '\0')
    {
        return *this;    
    }
    
    for(i = 0; i < CurrentLength; ++i)
    {
        if(mpString[i] == CharToReplace)
        {
            mpString[i]= NewChar;        
        }    
    }
    return *this;
}
inline cDString & cDString::trimleft(int count)
{
	if(count == 0)
	{
		//remove leading spaces
		return *this;
	}
	else
	{
		if(count > length())
			return *this;
		//remove characters
		SIZE_TYPE CurrentLength = length();
		int i, j;
		for(j = 0; j < count; j++)
		{
			for(i = 0; i < CurrentLength-1; i++)
			{
				mpString[i] = mpString[i+1];
				if(mpString[i] == '\0')
					break;
			}
			mpString[i] = '\0';
		}
	}
	return *this;
}
inline cDString &cDString::trimright(int count)
{
	if(count == 0)
	{
		//remove leading spaces
		return *this;
	}
	else
	{
		if(count > length())
			return *this;
		//remove characters
		SIZE_TYPE CurrentLength = length();
		mpString[CurrentLength - count] = '\0';
	}
	return *this;
}

/* vivo:comment out
inline void cDString::xmldecode()
{
	char *pHolding;
	int length;
	int holdinglen		  =0;
	const char quote[]	  ="&quot;";
	const char amp[]  	  ="&amp;";
	const char lt[]   	  ="&lt;";
	const char gt[]   	  ="&gt;";
	int quotelen 	  	  = strlen(quote);
	int amplen   		  = strlen(amp);
	int ltlen    		  = strlen(lt);
	int gtlen    		  = strlen(gt);
	bool SpecialCharFound = false;
	
	length = strlen(mpString);
	
	for(int i=0; i < length;i++)
	{
		if( mpString[i] == '\042')
		{
			SpecialCharFound = true;
			holdinglen += quotelen;
		}
		else if (mpString[i] == '\046')
		{
			SpecialCharFound = true;
			holdinglen += amplen;
		}
		else if (mpString[i] == '\074')
		{
			SpecialCharFound = true;
			holdinglen += ltlen;
		}
		else if (mpString[i] == '\076')
		{
			SpecialCharFound = true;
			holdinglen += gtlen;
		}
		else
		{
			holdinglen++;
		}
	}

	//Allocate new temporary buffer

	if(SpecialCharFound)
	{

		pHolding = new (std::nothrow) char[holdinglen + 1];
		if(pHolding == NULL)
		{
			return;
		}

		memset(pHolding, 0, holdinglen + 1);

		holdinglen = 0;

		for(int i=0; i < length;i++)
		{
			if( mpString[i] == '\042')
			{
				strcat(pHolding, quote);
				holdinglen += quotelen;
			}
			else if (mpString[i] == '\046')
			{
				strcat(pHolding, amp);
				holdinglen += amplen;
			}
			else if (mpString[i] == '\074')
			{
				strcat(pHolding, lt);
				holdinglen += ltlen;
			}
			else if (mpString[i] == '\076')
			{
				strcat(pHolding, gt);
				holdinglen += gtlen;
			}
			else
			{
				pHolding[holdinglen] = mpString[i];
				holdinglen++;
			}
		}

		Reallocate(holdinglen+1);
		strcpy(mpString, pHolding);

		delete [] pHolding;
	}
}
*/

#endif





#ifndef __GESLIST_H__
#define __GESLIST_H__


template<typename LIST_ITEM>
class cSLinkedListNode
{
public:
    cSLinkedListNode *mpNext;          // Pointer to next element in the list
    LIST_ITEM         mItem;           // Data store in the list (May be a pointer)
    cSLinkedListNode();                // Constructor
};

template<typename LIST_ITEM>
inline cSLinkedListNode<LIST_ITEM>::cSLinkedListNode()
{
    mpNext = NULL;
}

template<typename LIST_ITEM>
class cSLinkedList
{        
public:
    class cIterator
    {
        private:
            cSLinkedListNode<LIST_ITEM> *mpCurr;
        public:
            cIterator() 
                    { mpCurr = NULL; }
            cIterator(cSLinkedListNode<LIST_ITEM> * pNode) 
                    { mpCurr = pNode; }
            // Assignment Methods
            cIterator & operator++()                 
                    { mpCurr = mpCurr->mpNext;  return *this; }     
            // Equality Methods
            bool operator==(const cIterator & Iter) const
                    { return (mpCurr == Iter.mpCurr); } 
            bool operator!=(const cIterator & Iter) const
                    { return (mpCurr != Iter.mpCurr); } 
            // Dereferencing Methods
            LIST_ITEM * operator->()                 
                    { return &mpCurr->mItem; }
            LIST_ITEM & operator*()                  
                    { return mpCurr->mItem; }
            
            friend class cSLinkedList;
    };    
    cIterator   Begin()                         {return NULL;}
    cIterator   End()                           {return NULL;}
    cIterator   Insert(const LIST_ITEM Item,cIterator Iter = cIterator(NULL))   {return NULL;}
    int         Remove(const LIST_ITEM Item)    {return 0;}
    int         NumElements()                   {return 0;}
        
};





#endif  // __GESLIST_H__
























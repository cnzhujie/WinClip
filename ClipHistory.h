//---------------------------------------------------------------------------

#ifndef ClipHistoryH
#define ClipHistoryH


#include <time.h>
#include <Classes.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <list>
using namespace std;
#include <Forms.hpp>

struct ClipHistoryItem
{
        long time;
        AnsiString content;
};

struct ClipHistoryNode
{
        ClipHistoryItem item;
        ClipHistoryNode* pPrev;
        ClipHistoryNode* pNext;
};

class ClipHistory
{
  public:
        ClipHistory(int maxSize);
        ~ClipHistory();
  public:
        void Add(AnsiString content);
        void Remove(AnsiString content); 
        void Clear();
        list<ClipHistoryItem*> Search(AnsiString pattern, int maxRecords);

        bool LoadFromFile(AnsiString fn); 
        bool SaveToFile(AnsiString fn);

        AnsiString getHeadContent();
  private:
        ClipHistoryNode* getNode(AnsiString content);
        ClipHistoryNode* getTail();
  private:
        ClipHistoryNode* pHead;
        int size;
        int maxSize;
};
//---------------------------------------------------------------------------
#endif

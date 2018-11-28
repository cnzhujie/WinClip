//---------------------------------------------------------------------------


#pragma hdrstop

#include "ClipHistory.h"

ClipHistory::ClipHistory(int maxSize)
{
     this->maxSize = maxSize;
     size = 0;
     pHead = NULL;
}
ClipHistory::~ClipHistory()
{

}
void ClipHistory::Clear()
{
     if(pHead != NULL)
     {
          ClipHistoryNode* pCur = pHead;
          while(pCur != NULL)
          {
              ClipHistoryNode* pNext = pCur->pNext;
              delete pCur;
              pCur = pNext;
          }
     }
     pHead = NULL;
}
void ClipHistory::Add(AnsiString content)
{
      // 如果之前存在，则删除，重新再头部插入  
      Remove(content);
      // 插入在开头
      ClipHistoryItem item;
      item.time = time(0);
      item.content = content;
      ClipHistoryNode* pNode = new ClipHistoryNode();
      pNode->item = item;
      pNode->pPrev = NULL;
      pNode->pNext = pHead;
      if(pHead != NULL)
      {
                pHead->pPrev = pNode;
      }
      pHead = pNode;
      size++;

      if(size > maxSize)
      {
           ClipHistoryNode* pTail = getTail();
           if(pTail != NULL)
           {
                if(pTail->pPrev != NULL)
                {
                    pTail->pPrev->pNext = NULL;
                }
                delete pTail;
                size--;
           }
      }
}
void ClipHistory::Remove(AnsiString content)
{
      ClipHistoryNode* pNode = getNode(content);
      if(pNode != NULL)
      {
            if(pNode == pHead)
            {
                 pHead = pHead->pNext;
            }
            ClipHistoryNode* pPrev = pNode->pPrev;  
            ClipHistoryNode* pNext = pNode->pNext;
            if(pPrev != NULL)
            {
                 pPrev->pNext = pNext;
            }
            if(pNext != NULL)
            {
                 pNext->pPrev = pPrev;
            }
            delete pNode;
            size--;
      }
}
list<ClipHistoryItem*> ClipHistory::Search(AnsiString pattern, int maxRecords)
{
       list<ClipHistoryItem*> result;

       pattern = pattern.LowerCase();
       ClipHistoryNode* pCur = pHead;
       while(pCur != NULL)
       {
           //Application->HandleMessage();
           AnsiString content = pCur->item.content;
           if(pattern.IsEmpty() || content.LowerCase().Pos(pattern) != 0)
           {
                result.push_back(&(pCur->item));
           }
           if(maxRecords > 0 && result.size() >= maxRecords)
           {
                break;
           }
           pCur = pCur->pNext;
       }

       return result;
}
   
AnsiString ClipHistory::getHeadContent()
{
        return pHead == NULL ? AnsiString("") : pHead->item.content;
}
ClipHistoryNode* ClipHistory::getNode(AnsiString content)
{
       ClipHistoryNode* pCur = pHead;
       while(pCur != NULL)
       {
             if(pCur->item.content == content)
             {
                 return pCur;
             }
             pCur = pCur->pNext;
       }
       return NULL;
}
ClipHistoryNode* ClipHistory::getTail()
{
       ClipHistoryNode* pPrev = NULL;
       ClipHistoryNode* pCur = pHead;
       while(pCur != NULL)
       {
             pPrev = pCur->pPrev;
             pCur = pCur->pNext;
       }
       return pPrev;
}         
bool ClipHistory::LoadFromFile(AnsiString fn)
{
       Clear(); 
       FILE* fp = fopen(fn.c_str(), "rb");
       if(!fp)
       {
           return false;
       }

       ClipHistoryNode* pPrev = NULL;
       while(!feof(fp))
       {
             long time;
             fread(&time, sizeof(long), 1, fp);
             if(feof(fp))
             {
                 break;
             }

             long contentLen;
             fread(&contentLen, sizeof(long), 1, fp); 
             if(feof(fp))
             {
                 break;
             }

             char* pContent = new char[contentLen+1];
             fread(pContent, sizeof(char), contentLen, fp);
             pContent[contentLen] = 0;

             ClipHistoryItem item;
             item.time = time;
             item.content = pContent;

             ClipHistoryNode* pNode = new ClipHistoryNode();
             pNode->item = item;
             pNode->pPrev = pPrev;
             pNode->pNext = NULL;

             if(pHead == NULL)
             {
                 pHead = pNode;
             }
             else
             {
                 pPrev->pNext = pNode;
             }
             pPrev = pNode;
       } 
       fclose(fp);
}
bool ClipHistory::SaveToFile(AnsiString fn)
{
       FILE* fp = fopen(fn.c_str(), "wb");
       if(!fp)
       {
           return false;
       }

       ClipHistoryNode* pCur = pHead;
       while(pCur != NULL)
       {
             ClipHistoryItem item = pCur->item;
             long contentLen = item.content.Length();
             fwrite(&(item.time), sizeof(long), 1, fp);
             fwrite(&(contentLen), sizeof(long), 1, fp);
             fwrite(item.content.c_str(), sizeof(char), contentLen, fp);

             pCur = pCur->pNext;
       }

       fclose(fp);
}
//---------------------------------------------------------------------------

#pragma package(smart_init)

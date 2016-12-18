#ifdef CHANGED

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "copyright.h"
#include "bitmap.h"
//Partie I terminer
class Semaphore;

class PageProvider:dontcopythis
{
 public:
  PageProvider (int numPages);
  ~PageProvider ();
  int GetEmptyPage ();
  void ReleasePage (int page_nb);
  int NumAvailPage ();
  
 private:
  BitMap* bitmap;
  Semaphore* semPageProvider;
};

#endif //PAGEPROVIDER_H
#endif //CHANGED

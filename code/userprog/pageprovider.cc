#ifdef CHANGED

#include "pageprovider.h"
#include "system.h"
#include "bitmap.h"
#include "synch.h"

PageProvider::PageProvider(int numPages) {

    bitmap = new BitMap(numPages);
    semPageProvider = new Semaphore("PageProvider Semaphore", numPages);
}

PageProvider::~PageProvider() {
    delete bitmap;
    delete semPageProvider;
}

int
PageProvider::GetEmptyPage() {
    semPageProvider -> P();

    int emptyPage = bitmap -> Find();
    ASSERT(emptyPage != -1);

    memset(&(machine -> mainMemory[ PageSize * emptyPage ]), '0', PageSize);

    return emptyPage;
}

void
PageProvider::ReleasePage(int page_nb) {
    bitmap -> Clear(page_nb);

    semPageProvider -> V();
}

int
PageProvider::NumAvailPage() {
    return bitmap -> NumClear();

}
#endif //CHANGED

#include <QMutex>
#include <QSet>
#include "librarymanager.h"
#include "book.h"

using namespace freeLib;

static LibraryManager* m_Instance;

LibraryManager::LibraryManager(QObject *parent) :
    QObject(parent)
{
    localBooks = new QList<Book>;
}

LibraryManager* LibraryManager::instance() {
    static QMutex mutex;
    if (m_Instance == NULL)
    {
        mutex.lock();

        if (!m_Instance)
            m_Instance = new LibraryManager(0);

        mutex.unlock();
    }

    return m_Instance;
}

void LibraryManager::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = 0;
    mutex.unlock();
}

QList<Book> *LibraryManager::getBooks(){
    return localBooks;
}

void LibraryManager::addBook(const Book &book) {
    if (!localBooks->contains(book))
        localBooks->append(book);
}

void LibraryManager::addBooks(const QList<Book> &books) {
    QSet<Book> set = books.toSet();
    set.intersect(localBooks->toSet());
    localBooks->clear();
    localBooks->append(set.toList());
}

void LibraryManager::removeBook(const Book& book) {
    localBooks->removeOne(book);
}

void LibraryManager::removeBook(const int id) {
    QList<Book>::const_iterator stlIter;
    for( stlIter = localBooks->begin(); stlIter != localBooks->end(); ++stlIter ) {
        if ((*stlIter).id() == id) {
            localBooks->removeOne(*stlIter);
            break;
        }
    }
}

LibraryManager::~LibraryManager() {
    delete localBooks;
}

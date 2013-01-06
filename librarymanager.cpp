#include <QMutex>
#include <QSet>
#include "librarymanager.h"
#include "book.h"

using namespace freeLib;

static LibraryManager* m_Instance;

LibraryManager::LibraryManager(QObject *parent) :
    QObject(parent)
{
    localBooks = new QSet<Book>;
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

QSet<Book> *LibraryManager::getBooks(){
    return localBooks;
}

void LibraryManager::addBook(const Book &book) {
    localBooks->insert(book);
}

void LibraryManager::addBooks(const QList<Book> &books) {
    localBooks->intersect(books.toSet());
}

void LibraryManager::addBooks(const QSet<Book> &books) {
    localBooks->intersect(books);
}

void LibraryManager::removeBook(const Book& book) {
    localBooks->remove(book);
}

void LibraryManager::removeBook(const int id) {
    QSet<Book>::const_iterator stlIter;
    for( stlIter = localBooks->begin(); stlIter != localBooks->end(); ++stlIter ) {
        if ((*stlIter).id() == id) {
            localBooks->remove(*stlIter);
            break;
        }
    }
}

LibraryManager::~LibraryManager() {
    delete localBooks;
}

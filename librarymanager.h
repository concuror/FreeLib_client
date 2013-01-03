#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include <QObject>

namespace freeLib {

class Book;

class LibraryManager : public QObject
{
    Q_OBJECT

private:

    explicit LibraryManager(QObject *parent = 0);

    LibraryManager() {}

    LibraryManager(const LibraryManager &); // hide copy constructor
    LibraryManager& operator=(const LibraryManager &); // hide assign op
                                 // we leave just the declarations, so the compiler will warn us
                                 // if we try to use those two functions by accident

    QList<Book> *localBooks;

    virtual ~LibraryManager();

public:

    static LibraryManager* instance();

    QList<Book> *getBooks();

    void addBook(const Book& book);

    void removeBook(const Book& book);

    void removeBook(const int id);

    void addBooks(const QList<Book>& books);

    static void drop();
    
signals:
    
public slots:
    



};

}

#endif // LIBRARYMANAGER_H

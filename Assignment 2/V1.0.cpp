/** Version: 1.0 - March 12th 2019
*   Author(s): Ahmad Khaled Fawzy - 20170377
*              Ahmad Mohammad Hanafy
*/

///NOTE: DO NOT CREATE BOOKS MANUALLY, read books from user using cin.getline to append \0 instead.
///NOTE: all seeking operations will be shifted by sizeof(short) because the first 2 bytes of the file contain the current head
///NOTE: RRN is 0 based throughout the whole program


/**
 *  TODO apply more testing on compactFile function
 *  TODO UPDATE BOOK
 *  TODO terminate while loops using the size of the file and comparing it when tellp() (OPTIONAL)
 *  TODO in function deleteBook, the condition that breaks the loop IS NOT VALID ios::end DOES NOT return the size of the file
 */
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

struct Book
{
    ///STRUCT SIZE = 144 (extra 2 bytes from padding)
    ///SUMMATION OF SIZES OF ALL MEMBERS = 142 = sizeof(ISBN) + sizeof(Title) + ...
    ///SIZE OF 1 RECORD = 142 + 6 delimiters = 148
    char ISBN[6]; /// 5 chars for ISBN, the 6th char is for \0
    char Title[60];
    char Author[60];
    double  Price;
    int  Year;
    int  nPages;
    friend istream&operator >> (istream & in , Book &obj ) ///TODO handle cascaded input by sitting ignore() flag.
    {
        memset(obj.ISBN , 0 , sizeof(obj.ISBN));
        memset(obj.Title , 0 , sizeof(obj.Title));
        memset(obj.Author , 0 , sizeof(obj.Author));


        in.getline(obj.ISBN , sizeof(obj.ISBN));
        in.getline(obj.Title , sizeof(obj.Title));
        in.getline(obj.Author , sizeof(obj.Author));
        in>>obj.Price>>obj.Year>>obj.nPages;

        return in ;
    }
    friend ostream &operator << (ostream & out  , Book &obj)
    {
        out<<"ISBN: " << obj.ISBN<<"\n";
        out<<"Author: " << obj.Author<<"\n";
        out<<"Title: " << obj.Title<<"\n";
        out<<"Price: " << obj.Price<<"$\n";
        out<<"Year: " << obj.Year << "\n";
        out<<"Pages: " << obj.nPages<<"\n";
        return  out ;
    }
};
void addBook (fstream &recordsFile, Book &someBook)
{
    short currentHead;
    recordsFile.seekg(0, ios::beg);
    recordsFile.read((char*) &currentHead, sizeof(currentHead));
    char delimiter = '#';
    if (currentHead == -1) /// if there exists no deleted records then append the new record at the end of the file
    {
        recordsFile.seekp(0,ios::end);

        recordsFile.write(someBook.ISBN, sizeof(someBook.ISBN));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write(someBook.Title, sizeof(someBook.Title));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write(someBook.Author, sizeof(someBook.Author));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write((char*)&someBook.Price, sizeof(someBook.Price));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write((char*)&someBook.Year, sizeof(someBook.Year));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write((char*)&someBook.nPages, sizeof(someBook.nPages));
        recordsFile.write(&delimiter, sizeof(delimiter));
    }
    else
    {
        short stuffedHead;
        recordsFile.seekg( (2 + (currentHead) * 148 + 1), ios::beg); /// seek the byte-offset of the latest deleted book, (+1) to skip '*' while reading
        recordsFile.read((char*) &stuffedHead, sizeof(stuffedHead));
        recordsFile.seekp(0, ios::beg); ///update AVAIL_LIST head
        recordsFile.write((char*) &stuffedHead, sizeof(stuffedHead));
        recordsFile.seekp( 2 + (currentHead)* 148); /// seek the byte-offset of the deleted book to write in its place

        recordsFile.write(someBook.ISBN, sizeof(someBook.ISBN));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write(someBook.Title, sizeof(someBook.Title));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write(someBook.Author, sizeof(someBook.Author));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write((char*)&someBook.Price, sizeof(someBook.Price));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write((char*)&someBook.Year, sizeof(someBook.Year));
        recordsFile.write(&delimiter, sizeof(delimiter));

        recordsFile.write((char*)&someBook.nPages, sizeof(someBook.nPages));
        recordsFile.write(&delimiter, sizeof(delimiter));
    }
}
Book  readBook(fstream &out, short RRN)
{
    long long byte = 2 + (RRN) * 148; /// 148 is the size of each record (including all delimiters) in the file
    out.seekg(byte, ios::beg);
    assert(!(out.end > byte));       /// asserting if the RRN of the desired book outside the file scope
    Book temp;
    char delim ;
    out.read(temp.ISBN , sizeof(temp.ISBN));
    out.read(&delim, sizeof(delim));
    out.read(temp.Title, sizeof(temp.Title));
    out.read(&delim, sizeof(delim));
    out.read(temp.Author, sizeof(temp.Author));
    out.read(&delim, sizeof(delim));
    out.read((char *) &temp.Price, sizeof(temp.Price));
    out.read(&delim, sizeof(delim));
    out.read((char *) &temp.Year, sizeof(temp.Year));
    out.read(&delim, sizeof(delim));
    out.read((char *) &temp.nPages, sizeof(temp.nPages));
    out.read(&delim, sizeof(delim));
    return temp;
}
void printBook(fstream &out, char isbn[])
{
    for (short rrn = 0; ; ++rrn)
    {
        Book temp = readBook(out, rrn);
        if(strcmp(temp.ISBN , isbn) == 0)
        {
            cout<<temp;
            break;
        }
    }
}

void deleteBook(fstream &recordsFile, char isbn[])
{
    short RRN = 0 ;
    while(true)
    {
        recordsFile.seekg( (2 + RRN * 148), ios::beg);

        char tempISBN [6];
        recordsFile.read(tempISBN , sizeof(tempISBN));
        if(strcmp(isbn , tempISBN) == 0) /// if we found the book we need to update the avail list
        {
            /**
             * once the desired book has been found the following procedures are taken:
             * 1- read the head of the AVAIL_LIST (located at the first 2 bytes of the file) and store them in currentHead (short)
             * 2- update the head of the AVAIL_LIST with the byte-offset of the desired book, the byte-offset is stored in RRN (short)
             * 3- add '*' and currentHead at the first 3 bytes of the record being deleted.
             */
            short currentHead; // -1
            recordsFile.seekg(0, ios::beg);
            recordsFile.read((char*) &currentHead, sizeof(currentHead)); /// read current head (located at the first 2 bytes of the file).
            recordsFile.seekp(0, ios::beg);
            recordsFile.write((char*) &RRN, sizeof(RRN)); /// update the head with the current index being deleted.
            /**
             * after updating the AVAIL_LIST the following procedures are taken:
             * 1- seek byte-offset of book being deleted
             * 2- write '*' on 1st byte
             * 3- write currentHead at 3rd and 4th bytes.
             * e.g. (ASSUME CURRENT HEAD = 3) BEFORE: isbn1 - AFTER: *3n1, "isb" has been replaced with *3
             */
            recordsFile.seekp((2 + RRN * 148), ios::beg);
            char deletingChar = '*';
            recordsFile.write(&deletingChar, sizeof(deletingChar));
            recordsFile.write((char*) &currentHead, sizeof(currentHead));
            break;
        }
        else {++RRN;}
        if(recordsFile.tellp() == ios::end) { break; }
    }
}

void printAllBooks (fstream &recordsFile)
{
    short RRN = 0;
    while(true)
    {
        recordsFile.seekg((2 + RRN * 148), ios::beg);
        char dummyChar;
        recordsFile.read(&dummyChar, sizeof(dummyChar));
        if(recordsFile.tellg() == -1) {break;}
        if(dummyChar == '*')
        {
            RRN++;
            continue;
        }
        Book toPrint = readBook(recordsFile, RRN);
        cout << toPrint << endl;
        RRN++;
    }
}
/**
 * 1- opens new file (tempfile) to store non-deleted records in it
 * 2- clears (records.txt)
 * 3- transfers data from (tempfile.txt) to (records.txt)
 * @param recordsFile
 */
void compactFile (fstream &recordsFile)
{
    short RRN = 0;
    short head = -1;
    fstream tempFile;
    tempFile.open("tempfile.txt", ios::out | ios::in | ios::binary | ios::trunc);
    tempFile.write((char*) &head, sizeof(head));
    while(true)
    {
        recordsFile.seekg((2 + RRN * 148), ios::beg);
        char dummyChar;
        recordsFile.read(&dummyChar, sizeof(dummyChar));
        if(recordsFile.tellg() == -1) {break;}
        if(dummyChar == '*')
        {
            RRN++;
            continue;
        }
        Book toAdd = readBook(recordsFile, RRN);
        addBook(tempFile, toAdd);
        RRN++;
    }
    /**
     * clearing records file by opening it in truncate mode then reopening it in binary mode
     */
    recordsFile.close();
    recordsFile.open("records.txt", ios::trunc | ios::out | ios::in | ios::binary );
    recordsFile.close();
    recordsFile.open("records.txt", ios::out | ios::in | ios::binary);
    /**
     * reading from tempfile then writing on records file
     */
    recordsFile.write((char*) &head, sizeof(head)); /// writing -1 on the first 2 bytes of records file
    RRN = 0; /// resetting RRN for looping
    while(true)
    {
        tempFile.seekg((2 + RRN * 148), ios::beg);
        char dummyChar;
        tempFile.read(&dummyChar, sizeof(dummyChar)); ///dummyChar to set tellg() flag to false in case the pointer is at the end of the file
        if (tempFile.tellg() == -1) { break; }
        Book toAdd = readBook(tempFile, RRN);
        addBook(recordsFile, toAdd);
        RRN++;
    }
    tempFile.close();
}
int main()
{
    freopen("in.txt","r",stdin);
    short head = -1;
    Book testBook;
    fstream recordsFile;
    recordsFile.open("records.txt", ios::out | ios::in | ios::binary);
    recordsFile.write((char * )& head    , sizeof(head));
//    for(int i=0; i<3; ++i)
//    {
//        cin >> testBook;
//        addBook(recordsFile, testBook);
//        cin.ignore();
//    }


//    deleteBook(recordsFile, "xxx1");
//    compactFile(recordsFile);
//    printAllBooks(recordsFile);

//    cin>>testBook;
//    cin.ignore();
//    addBook(recordsFile,testBook);
}
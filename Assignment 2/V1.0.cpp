/** Version: 1.0 - March 12th 2019
*   Author(s): Ahmad Khaled Fawzy - 20170377
*              Ahmad Mohammad Hanafy
*/

///NOTE: DO NOT CREATE BOOKS MANUALLY, read books from user using cin.getline to append \0 instead.
///NOTE: all seeking operations will be shifted by sizeof(short) because the first 2 bytes of the file contain the current head
///NOTE: RRN is 0 based throughout the whole program

/* when adding 3 records the rrn breaks from the file when the RRN reaches 4 not 3 i don't know why , checking with tellp != -1
 *  adding to the file when previously deleted not working
 *  todo check clearing file if end is reached
 *
 *
 */
/**
 *  TODO apply more testing on compactFile function
 *  TODO UPDATE BOOK
 *  TODO terminate while loops using the size of the file and comparing it when tellp() (OPTIONAL)
 *  TODO in function deleteBook, the condition that breaks the loop IS NOT VALID ios::end DOES NOT return the size of the file
 */
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

struct Book ///done
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

void addBook (fstream &recordsFile, Book &someBook) /// done
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
Book  readBook(fstream &out, short RRN) /// done
{
    long long byte = 2 + (RRN) * 148; /// 148 is the size of each record (including all delimiters) in the file
    out.seekg(byte, ios::beg);
    Book temp;
    if(out.tellg() == -1){
        assert(0);
    }
/* todo assertion not working i have no idea why
 * i solved this by out.tellg == -1
 *
 */
//    assert(!(out.end > byte));       /// asserting if the RRN of the desired book outside the file scope

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
short Search(fstream &recordsFile , char isbn[]){   ///sequential search on all file to find by ISBN

    for (short rrn = 0; ; ++rrn)
    {
        Book temp = readBook(recordsFile, rrn);
        if(strcmp(temp.ISBN , isbn) == 0)
        {
            return (rrn);
        }
    }

}
void printBook(fstream &out, char isbn[])       ///done
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
        long long Byte = 2+(RRN *148);
        bool OutsideFileScope = !(recordsFile.end> Byte);
        assert(OutsideFileScope); /// assert when deleting a record not in the file , Hanafy
        recordsFile.seekg( Byte, ios::beg);
        Book temp = readBook(recordsFile ,RRN);
        char tempISBN [6];
//        recordsFile.read(tempISBN , sizeof(tempISBN)); /// we have problem here
        if(strcmp(isbn , temp.ISBN) == 0) /// if we found the book we need to update the avail list
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
    puts("finished");
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
void writeWithByteOFFSET(fstream &recordsFile ,long long BYTE ,Book &temp){
    recordsFile.seekp(BYTE, ios::beg);
    char delimiter = '#';
    recordsFile.write(temp.ISBN, sizeof(temp.ISBN));
    recordsFile.write(&delimiter, sizeof(delimiter));

    recordsFile.write(temp.Title, sizeof(temp.Title));
    recordsFile.write(&delimiter, sizeof(delimiter));

    recordsFile.write(temp.Author, sizeof(temp.Author));
    recordsFile.write(&delimiter, sizeof(delimiter));

    recordsFile.write((char*)&temp.Price, sizeof(temp.Price));
    recordsFile.write(&delimiter, sizeof(delimiter));

    recordsFile.write((char*)&temp.Year, sizeof(temp.Year));
    recordsFile.write(&delimiter, sizeof(delimiter));

    recordsFile.write((char*)&temp.nPages, sizeof(temp.nPages));
    recordsFile.write(&delimiter, sizeof(delimiter));

}
void readWithByteOFFSET(fstream &recordsFile ,long long BYTE ,Book &temp){
    recordsFile.seekp(BYTE, ios::beg);
    char delim ;
    recordsFile.read(temp.ISBN , sizeof(temp.ISBN));
    recordsFile.read(&delim, sizeof(delim));
    recordsFile.read(temp.Title, sizeof(temp.Title));
    recordsFile.read(&delim, sizeof(delim));
    recordsFile.read(temp.Author, sizeof(temp.Author));
    recordsFile.read(&delim, sizeof(delim));
    recordsFile.read((char *) &temp.Price, sizeof(temp.Price));
    recordsFile.read(&delim, sizeof(delim));
    recordsFile.read((char *) &temp.Year, sizeof(temp.Year));
    recordsFile.read(&delim, sizeof(delim));
    recordsFile.read((char *) &temp.nPages, sizeof(temp.nPages));
    recordsFile.read(&delim, sizeof(delim));
}
void update(fstream &recordsFile,char isbn[]){
    short RRN = Search(recordsFile ,isbn);

    long long BYTE = 2 + (RRN) * 148;
    bool OutsideFileScope = !(recordsFile.end > BYTE);
    assert(OutsideFileScope);           /// aserting if the record not in the file or deleted before
    cout<<"you can update\n1- isbn \n2-Title \n3-Author\n4-price\n5-Year\n6-Pages\n";
    int option;
    do{
        cout<<"what do you want to update : ";
        cin >> option;
    }
    while(option<0 ||option>6);

    Book temp;
    readWithByteOFFSET(recordsFile , BYTE , temp);  /// utility function , no checking for boundaries of the file it is used within the system
    /// the dev should handle it and don't pass BYTE outside the file scope
    cout<<"The record before update \n";
    cout<<temp;
    switch (option){
        case 1:
            cin.ignore();
            cin.getline(temp.ISBN , sizeof(temp.ISBN) );
            break;
        case 2:
            cin.ignore();
            cin.getline(temp.Title , sizeof(temp.Title) );
            break;
        case 3:
            cin.ignore();
            cin.getline(temp.Author , sizeof(temp.Author) );
            break;
        case 4:
            cin>>temp.Price;
            break;
        case 5:
            cin>>temp.Year;
            break;
        case 6:
            cin>>temp.nPages;
            break;

        default:
            break;
    }
    writeWithByteOFFSET(recordsFile,BYTE , temp);



}


int main()
{
//    freopen("in.txt","r",stdin);
    short head = -1;
    Book testBook;
    fstream recordsFile;
    recordsFile.open("records.txt", ios::out | ios::in | ios::binary);
//    update(recordsFile ,"isb3");
//    recordsFile.write((char * )& head    , sizeof(head));
//    for(int i=0; i<6; ++i)
//    {
//        cin >> testBook;
//        addBook(recordsFile, testBook);
//        cin.ignore();
//    }
//    deleteBook(recordsFile, "xxx1");
//    compactFile(recordsFile);
//    printBook(recordsFile, "isb5");
//    printBook(recordsFile, "isb5");
//    printAllBooks(recordsFile);
//    deleteBook(recordsFile ,"isb5");
//    compactFile(recordsFile);
//    printBook(recordsFile, "isb2");
//    printAllBooks(recordsFile);

//    cin>>testBook;
//    cin.ignore();
//    addBook(recordsFile,testBook);
}
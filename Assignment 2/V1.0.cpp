/** Version: 1.0 - March 10th 2019
 *   Author(s): Ahmad Khaled Fawzy
 *              Ahmad Mohammad Hanafy
 */

///NOTE: DO NOT CREATE BOOKS MANUALLY, read books from user using cin.getline to append \0 instead.
///NOTE: all seeking operations will be shifted by sizeof(short) because the first 2 bytes of the file contain the current head

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
};
void addBook (fstream &out, Book &someBook) ///TODO handle the case of writing at the current AVAIL_LIST head instead of appending at the end of the file
{
    char delimiter = '#';
    ///if no deleted records exist (head = -1)
        ///write at the end of the file
        out.seekp(0,ios::end);

        out.write(someBook.ISBN, sizeof(someBook.ISBN));
        out.write(&delimiter, sizeof(delimiter));

        out.write(someBook.Title, sizeof(someBook.Title));
        out.write(&delimiter, sizeof(delimiter));

        out.write(someBook.Author, sizeof(someBook.Author));
        out.write(&delimiter, sizeof(delimiter));

        out.write((char*)&someBook.Price, sizeof(someBook.Price));
        out.write(&delimiter, sizeof(delimiter));

        out.write((char*)&someBook.Year, sizeof(someBook.Year));
        out.write(&delimiter, sizeof(delimiter));

        out.write((char*)&someBook.nPages, sizeof(someBook.nPages));
        out.write(&delimiter, sizeof(delimiter));

    ///else
        ///write at the current head
}
int main()
{
    short head = -1;
    Book testBook;
    fstream recordsFile;
    recordsFile.open("records.txt", ios::out | ios::in | ios::trunc); ///TODO make sure the program runs correctly in ios::app mode
    recordsFile.write((char*)&head, sizeof(head));
    /// Reading 1 book as a test
    cin.getline(testBook.ISBN, sizeof(testBook.ISBN));
    cin.getline(testBook.Title, sizeof(testBook.Title));
    cin.getline(testBook.Author, sizeof(testBook.Author));
    cin.ignore(); /// Why won't it run properly without this cin.ignore? $Khaled
    cin >> testBook.Price;
    cin >> testBook.Year;
    cin >> testBook.nPages;
    addBook(recordsFile, testBook);
}
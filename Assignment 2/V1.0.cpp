/** Version: 1.0 - March 10th 2019
*   Author(s): Ahmad Khaled Fawzy
*              Ahmad Mohammad Hanafy
*/

///NOTE: DO NOT CREATE BOOKS MANUALLY, read books from user using cin.getline to append \0 instead.
///NOTE: all seeking operations will be shifted by sizeof(short) because the first 2 bytes of the file contain the current head


/**
*   we have the same output files when we use the >> operator and the input in the main without the cin.ignore i commented it
*   we need to check how to delete the record
*   to make compaction write all file in temp file and rewrite it back to the original one
*   we need to read the head from the file itself
*
*
*/

/** 
 *  DONE        1-  Delete book (given the ISBN), you are asked to use the avail list technique for fixed length
 *                  records, so that you can reuse the deleted records in new insertions. The list head is initially
 *                  assigned ‐1, its data type is: short, and stored at the beginning of the file.
 * TODO         2- Add book, you must check first if there is a deleted record in the available list or not, if the
 *                  list head is = ‐1, this means that you should append the new record.
 * TODO         3 - UPDATE BOOK
 *
 *  DONE         4 - PRINT BOOK GIVEN isbn
 *  TODO         5- PRINT ALL BOOOKS
 *  TODO         6 - COMPACT FILE
 *
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
    double  Price;          /// i need to change those to chars, why doubles and int ??,  i used them anyway
    int  Year;
    int  nPages;
    int RRN ;
    friend istream&operator >> (istream & in , Book &obj ){

        in.getline( obj.ISBN , sizeof(obj.ISBN));
        in.getline(obj.Author , sizeof(obj.Author));
        in.getline(obj.Title , sizeof(obj.Title));
        in>>obj.Price>>obj.Year>>obj.nPages;
        return in ;
    }
    friend ostream &operator << (ostream & out  , Book &obj){
        out<<"ISBN :"<<obj.ISBN<<"\n";
        out<<"Author :"<<obj.Author<<"\n";
        out<<"Title :"<<obj.Title<<"\n";
        out<<"Price :"<<obj.Price<<"$\n";
        out<<"pages :"<<obj.Year<<"\n";
        return  out ;
    }
};
/** to add to the middle of the file we need to provide a byteoffset
*  update : i made the byteoffset to make the function work abstractly and we handle the deletion and insertion position somewhere else, Hanafy
*/
void addBook (fstream &out, Book &someBook, long long BYTEOFF = ios::end) ///TODO handle the case of writing at the current AVAIL_LIST head instead of appending at the end of the file
{
//    out.seekp(BYTEOFF ,  ios:: beg); ///todo check this again
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
Book  readBook(fstream &out , short RRN){
    long long byte =2+ RRN * sizeof(Book) + 6 * RRN;
    out.seekg(byte, ios::beg);
    assert(!(out.end >byte));       /// asserting if the RRN of the desired book outside the file scope
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
void printBook(fstream &out, char isbn[]){
    for (short rrn = 0; ; ++rrn) {
      Book temp = readBook(out, rrn);
      if(strcmp(temp.ISBN , isbn) ==0){
          cout<<temp;
          break;
      }
    }
}
void updateAVAIL(fstream &out , short nOFF){/// not working yet
    out.seekg(0 , ios::beg);
    short temp = 0;
    out.read((char *) &temp, sizeof(temp) );    /// reading probabely
    cout<<"inside the update ; "<<temp<<endl;
    out.seekp(0 , ios::beg);
    out.write((char *)&nOFF , sizeof(nOFF));
    out.seekp(2 +  nOFF * sizeof(Book)  + 6*nOFF *sizeof(char) + sizeof(char) , ios::beg); /// handling the * of deletion
    out.write((char *)&temp , sizeof(temp));
    return;
}

void deleteBook(fstream & out, char isbn[]){
    short RRN = 0 ;
    while(true){
        out.seekg(2 + RRN * sizeof(Book) + 6*RRN* sizeof(char) ,ios::beg);///2 for the head + RRN *sizeof record + 6 delimiters per record

        char tempISBN [6];
        out.read(tempISBN , sizeof(tempISBN));
        /*if we found the book we need to update the avail list
         *
         */
        if(strcmp(isbn , tempISBN) == 0){
            short nOFF =RRN;
//            out.seekp(0 , ios::beg);
            out.seekp(sizeof(short) + nOFF* sizeof(Book )+ 6*nOFF * sizeof(char)   , ios::beg);/// handling the new short written
            /// new problem here
            char del_Delimiter = '*';
            out.write((char *)&del_Delimiter , sizeof(del_Delimiter));
            updateAVAIL(out, nOFF);/// handle the deletion delimiter

            break;
        }
        if(out.tellp() == ios::end) {
            break;
        }

    }

}

int main() {
    freopen("in.txt","r",stdin);
    short head = -1;

    Book testBook;
    fstream recordsFile;
    recordsFile.open("Hanfy.txt", ios::out | ios::in | ios::trunc); ///TODO make sure the program runs correctly in ios::app mode
    recordsFile.write((char*)&head, sizeof(head));
    cin>>testBook;
    addBook(recordsFile, testBook);
//    return 0 ;
//    deleteBook(recordsFile , "123");
    recordsFile.seekg(0 ,ios::beg);
    printBook(recordsFile,"123");
    recordsFile.close();
}
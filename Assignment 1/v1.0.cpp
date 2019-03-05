/**
 *  author : Ahmed Hanfy
 *  version : 1.0
 *  Date : 28/02/19
 *
 *
 *
 */

/*
 *  1- todo  we have a problem when the user inputs data bigger than the size of the char array
 *   todo we need to store the RRN in a file to make the update and deletio possible for any record not the added in the runtime only
 *
 */
/*  checklist
 *  1 - read from file
 *  2 - edit a record
 *  3 - delete a record
 *
 *
 *
 *
 */
#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
using namespace std;

class Book {
private:
    char isbn[6];             /// always 5 chars
    char author [100];         /// variable
    char title [100];         /// variable
    char year[5];             /// always 4 chars
    char pages[5] ;         ///variable max 9999 page
    char price[20] ;
    int RRN;
    static int KEY ;
public:
    Book(){
        char null[]="NULL";
        strcpy(this->author ,null);
        strcpy(this->title ,null);
        strcpy(this->isbn , null);
        strcpy(this->year  , null);
        strcpy(this->pages , null);
        strcpy(this->price , null);
        RRN = -1;
    }
    Book(char  _isbn[] ,char _author[] , char _title[],
         char _year[] , char _pages[] , char _price[] ){
        strcpy(this->isbn , _isbn);
        strcpy(this->author , _author);
        strcpy(this->title , _title);
        strcpy(this->year  , _year);
        strcpy(this->pages , _pages);
        strcpy(this->price , _price);
        RRN = -1;

    }
    friend ostream &operator<<(ostream& out , Book & obj ){
        cout<<"Book ispn  : "<<obj.isbn<<endl;
        cout<<"Book author  : "<<obj.author<<endl;
        cout<<"Book title  : "<<obj.title<<endl;
        cout<<"Book year  : "<<obj.year <<endl;
        cout<<"Book number of pages  : "<<obj.pages<<endl;
        cout<<"Book price  : "<<obj.price<<" $ "<<endl;
        cout<<"Book RRN : "<<obj.RRN<<endl;
    }
    friend  istream &operator >> (istream & in , Book &obj){
        cout<<"input the Book ISBN  : ";
        cin>>obj.isbn;
        cout<<"input the Book author name : ";
        cin.ignore();
        cin.getline(obj.author,100);
        cout<<"input the Book title  : ";
        cin.ignore();
        cin.getline(obj.title,100);
        cout<<"input the Book publish year : ";
        cin>>obj.year;
        cout<<"input the Book number of pages : ";
        cin>>obj.pages;
        cout<<"input the Book price : ";
        cin>>obj.price;
    }
    void addRecordtofile(ostream &out , map<string , int> &mmap ){
        RRN =KEY++;/// giving RRN to every object if added to the file only otherwise = -1;
        string strISBN = this->isbn;
        mmap[strISBN] = RRN;
        out<<strlen(this->isbn)<<this->isbn;
        out<<strlen(this->author)<<this->author;
        out<<strlen(this->title)<<this->title;
        out<<strlen(this->year)<<this->year;
        out<<strlen(this->pages)<<this->pages;
        out<< strlen(this->price)<<this->price<<'|';
    }
    void writeRecordtofile(ostream &out , map<string , int> &mmap ){
        RRN =KEY++;/// giving RRN to every object if added to the file only otherwise = -1;
        string strISBN = this->isbn;
        mmap[strISBN] = RRN;
        int len = strlen(this->isbn);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->isbn , sizeof(this->isbn));
        len = strlen(this->author);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->author , sizeof(author));
        len = strlen(this->title);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->title , sizeof(title));
        len = strlen(this->year);
        out.write(this->year , sizeof(year));
        len = strlen(this->pages);
        out.write(this->pages , sizeof(pages));
        len = strlen(this->price);
        out.write(this->price , sizeof(price));
        char del = '|';
        out.write((char *)&del , sizeof(del));
    }



};
/// we have problem with the deleting because i think we have a  problem with the size of Book , because of not using write
/// the deleting working iff we write it during the run time
void deleteRecord(fstream &in , map<string , int> &mmap , string ISBN){
    in.seekg(0,in.beg);
    in.seekp(mmap[ISBN] *sizeof(Book),ios::beg);
    in.put('*');

}
int Book :: KEY = 0 ;
int main() {
    Book b1("123" , "ahmed hanfy","Kafka On The Shore" ,"1997" , "231" , "13.5" );
    Book b2("125" , "harouki","Kafka" ,"2003" , "452" , "62.5" );
//    cout<<b1;

    map<string , int> RRNINDX;  /// mapping all the isbns and searching in O(1)
    fstream out;
    out.open("out.txt", ios::in | ios::out );

    if(out.fail()){
        cout<<"failed to open the output file\n";
    }
    else {
        b1.writeRecordtofile(out , RRNINDX);
        b2.writeRecordtofile(out , RRNINDX);
    }
    deleteRecord(out , RRNINDX ,"125");

}
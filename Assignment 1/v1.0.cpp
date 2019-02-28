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
using namespace std;

class Book {
private:
    char isbn[6]; /// fixed
    char author [100]; /// variable
    char title [100]; /// variable
    char year[5]; /// fixedd
    char pages[5] ; ///variable max 9999 page
    char  price[20] ;
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
        this->price =  _price;
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
    void addRecordtofile(ostream &out ){
        RRN =KEY++;                     /// giving RRN to every object if added to the file only otherwise = -1;
        out<<strlen(this->isbn)<<this->isbn;
        out<<strlen(this->author)<<this->author;
        out<<strlen(this->title)<<this->title;
        out<<strlen(this->year)<<this->year;
        out<<strlen(this->pages)<<this->pages;
        out<< strlen(this->price)<<this->price<<'|';
    }


};
int Book :: KEY = 0 ;
int main() {
    Book b1("123" , "ahmed hanfy","Kafka On The Shore" ,"1997" , "231" , "13.5" );
    cout<<b1;
    Book b2 ;

    ofstream out;
    out.open("out.txt");

    if(out.fail()){
        cout<<"failed to open the output file\n";
    }
    else {
        b1.addRecordtofile(out);
    }

}
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;
struct date {
    int year , day , month ;
};
class Book {
private:
    char isbn[6]; /// fixed
    char author [100]; /// variable
    char year[5]; /// fixedd
    char pages[5] ; ///variable max 9999 page
    double  price ;
public:
    Book(char  _isbn[] ,char _author[] ,
         char _year[] , char _pages[] , double _price ){

        strcpy(this->author , _author);
        strcpy(this->isbn , _isbn);
        strcpy(this->year  , _year);
        strcpy(this->pages , _pages);
        this->price =  _price;

    }
    friend ostream &operator<<(ostream& out , Book & obj ){
        cout<<
            cout<<"Book ispn  : "<<obj.isbn<<endl;
        cout<<"Book author  : "<<obj.author<<endl;
        cout<<"Book year  : "<<obj.year <<endl;
        cout<<"Book price  : "<<obj.price<<" $ "<<endl;
    }


};
int main() {
    Book b1("123" , "ahmed hanfy" ,"1997" , "231" , 13.5 );
    cout<<b1;

}
/**
 *  Description: Creating a library of books using files processing and manipulation
 *  author(s) : Ahmed Mohammad Hanafy 201703**
 *              Ahmad Khaled Fawzy    20170377
 *  version : 3.0
 *  Date : March 1st 2019
 */

/*
 *  1- TODO create a static map instead of passing a map by reference each time we initialize a new BOOK object
 *  2- TODO improve code to make use of delimiters (optional) 
 *  6- TODO update book by ISBN
 *  7- TODO fix the problem where inserting characters less than the array size would cause garbage to be printed
 */

#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
using namespace std;

class Book { /// Total size of Book is fixed
private:
    char isbn[6];              /// always 5 chars
    char author [100];         /// variable
    char title [100];          /// variable
    char year[5];              /// always 4 chars
    char pages[5] ;            ///variable max 9999 page
    char price[20] ;
    long long RRN;
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
    friend ostream &operator<<(ostream& out , Book & obj ) {
        cout<<"ISBN: "<<obj.isbn<<endl;
        cout<<"Author: "<<obj.author<<endl;
        cout<<"Title: "<<obj.title<<endl;
        cout<<"Year: "<<obj.year <<endl;
        cout<<"Number of pages: "<<obj.pages<<endl;
        cout<<"Price: " <<obj.price << "$" <<endl;
        cout<<"RRN: "<<obj.RRN<<endl;
    }
    friend  istream &operator >> (istream & in , Book &obj){
        cout<<"input ISBN: ";
        cin>>obj.isbn;
        cout<<"input author name: ";
        cin.ignore();
        cin.getline(obj.author,100);
        cout<<"input title: ";
        //cin.ignore();
        cin.getline(obj.title,100);
        cout<<"input the Book publish year: ";
        cin>>obj.year;
        cout<<"input number of pages: ";
        cin>>obj.pages;
        cout<<"input book price: ";
        cin>>obj.price;
    }
    void writeRecordtofile( fstream &out , map<string, long long> &mmap, map<string
            , long long> &titleMap , map<string ,Book> & isbnObject )
    {
        RRN = out.tellp();
        string strISBN = this->isbn;
        mmap[strISBN] = RRN;
        string sISBN = this->isbn;
        isbnObject[sISBN] = *this;
        titleMap[this->title] = RRN;
        int len = strlen(this->isbn);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->isbn , len);
        len = strlen(this->author);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->author , len);
        len = strlen(this->title);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->title , len);
        len = strlen(this->year);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->year , len);
        len = strlen(this->pages);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->pages , len);
        len = strlen(this->price);
        out.write((char *)&len ,sizeof(len) );
        out.write(this->price , len);
        char del = '|';
        out.write(&del , sizeof(del));
    }
    static void menu(){                          /// utility
        cout<<"what do you want to update \n";
        cout<<"1 - ISBN \n";
        cout<<"2 - Author \n";
        cout<<"3 - Title \n";
        cout<<"4 - Year \n";
        cout<<"5 - Pages \n";
        cout<<"6 - Price \n";
        cout<<"0 - No further updates needed \n";
    }
    static  void update(map <string  ,Book> & isbnObject , string BookISBN
             , map<string, long long> &mmap, map<string
            , long long> &titleMap){
            Book currBook  = isbnObject[BookISBN];
            cout<<currBook<<endl;

            int option ;
            do{
                Book::menu();
                cin>>option;
                cin.ignore();
                switch (option ){
                    case 0 :
                        cout<<"Good Bye \n";
                        break;
                    case 1:
                        cout<<"Give me the ISBN \n";
                        cin.getline(currBook.isbn , 6);
                        cout<<currBook.isbn<<endl;
                        break;
                    case 2:
                        cout<<"Give  me the Author\n";
                        cin.getline(currBook.author , 100);
                        break;

                    case 3:
                        cout<<"Give me the Title \n";
                        cin.getline(currBook.title , 100);
                        break;
                    case 4:                 ///year     5
                        cout<<"Give me the Year \n";
                        cin.getline(currBook.year , 5);
                        break;
                    case 5:                 /// page    5
                        cout<<"Give me the Pages \n";
                        cin.getline(currBook.pages, 5);
                        break;
                    case 6:                 /// price 20
                        cout<<"Give me the Price \n";
                        cin.getline(currBook.price , 20);
                        break;
                    default:
                        cout<<"Give me an option";

                }
            }
            while(option != 0 );
            fstream out ;
            out.open("out.txt",ios::app);   /// when sending the same file in the main we have a problem by overwriting multiple records
                                            /// i haven't checked why yet , but now we have garbage in the isbn of the first record i don't know why too
            currBook.writeRecordtofile(out ,mmap,titleMap , isbnObject );

    }

    /** Uses a map to access all byte-offsets of the records in the data file then prints info of all records
     *  @param fstream, map<string, long long>
     *  @return void
     */
    static void printBooks (fstream &booksFile, map<string, long long> &mmap)
    {
        booksFile.seekg(0, ios::end);
        if(booksFile.tellg() == 0) /// Checking if the file is empty before reading from it
        {
            cout << "This file contains no records." << endl;
        }
        else
        {
            booksFile.seekg(0, ios::beg);
            for(auto itr = mmap.begin(); itr != mmap.end(); ++itr) ///using map to access all byte-offsets in records file.
            {
                cout << "=====================" << endl;
                long long byteOffset = itr->second;
                booksFile.seekg(byteOffset, ios::beg);
                char deleteChecker;
                booksFile.get(deleteChecker);
                if(deleteChecker == '*')
                {
                    continue;
                }
                booksFile.seekp(byteOffset, ios::beg);

                for (int i=0; i<6; ++i)
                {
                    int length;
                    booksFile.read((char *) &length, sizeof(length));
                    char *BUFFER = new char[length];
                    booksFile.read(BUFFER, length);
                    switch(i)
                    {
                        case 0:
                            cout << "ISBN: ";
                            break;
                        case 1:
                            cout << "Author: ";
                            break;
                        case 2:
                            cout << "Title: ";
                            break;
                        case 3:
                            cout << "Year: ";
                            break;
                        case 4:
                            cout << "Pages: ";
                            break;
                        case 5:
                            cout << "Price: ";
                            break;
                    }
                    cout << BUFFER << endl;
                    delete[] BUFFER;
                }
                char delim;
                booksFile.read(&delim, 1);
            }
        }
    }


    /** Uses a map to access byte-offset of a certain book then deletes it by adding char ('*') to the first byte of the record
     *  @param string, map<string, long long>, fstream
     *  @return void
     */
    static void deleteBook (string ISBN, map<string, long long> &mmap, fstream &booksFile )
    {
        long long byteOffset = mmap[ISBN];
        booksFile.seekp(byteOffset, ios::beg);
        char deletedBookChar = '*';
        booksFile.write(&deletedBookChar, 1);
    }
    /** Uses a map to access byte-offset of a certain book then prints its information
     *  @param string, map<string, long long>, fstream
     *  @return void
     */
    static void printBook (string title, map<string, long long> &titleMap, fstream &booksFile)
    {
        long long byteOffset = titleMap[title];
        booksFile.seekg(byteOffset, ios::beg);
        char deleteChecker;
        booksFile.get(deleteChecker);
        if(deleteChecker == '*')
        {
            cout << "This book has been deleted" << endl;
        }
        else
        {
            booksFile.seekg(byteOffset, ios::beg);
            for (int i=0; i<6; ++i)
            {
                int length;
                booksFile.read((char *) &length, sizeof(length));
                char *BUFFER = new char[length];
                booksFile.read(BUFFER, length);
                switch(i)
                {
                    case 0:
                        cout << "ISBN: ";
                        break;
                    case 1:
                        cout << "Author: ";
                        break;
                    case 2:
                        cout << "Title: ";
                        break;
                    case 3:
                        cout << "Year: ";
                        break;
                    case 4:
                        cout << "Pages: ";
                        break;
                    case 5:
                        cout << "Price: ";
                        break;
                }
                cout << BUFFER << endl;
                delete[] BUFFER;
            }
        }
    }
};

///todo mapping all the books in a string to Book * map to update the books in it except for the isbn update we will map a new book


int main() {
    Book b1("123" , "ahmed hanfy","Kafka On The Shore" ,"1997" , "231" , "13.5" );
    Book b2("125" , "harouki","Kafka" ,"2003" , "452" , "62.5" );
    Book b3("1234", "Ahmad Khaled", "Ahbabt Waghdan", "2018", "6", "2");
    map<string , long long> isbnMap;  /// mapping ISBN to RRN
    map<string , long long> titleMap;
    map<string , Book> isbnObject; /// mapping with the isbn to book, when writing to file only
    fstream out;
    out.open("out.txt", ios::in | ios::out );
    if(out.fail()){
        cout<<"failed to open the output file\n";
    }
    else
    {
        b1.writeRecordtofile(out , isbnMap, titleMap , isbnObject);
        b2.writeRecordtofile(out , isbnMap, titleMap ,isbnObject);
        b3.writeRecordtofile(out, isbnMap, titleMap , isbnObject);
    }
//    Book::deleteBook("125", isbnMap, out);
    Book::printBooks(out, isbnMap);
    Book::update(isbnObject , "125" , isbnMap , titleMap);
    Book::printBooks(out ,isbnMap );
//    Book::deleteBook("123", isbnMap, out);
//    Book::printBook("Kafka", titleMap, out);
//    Book::printBook("Kafka On The Shore", titleMap, out);
}

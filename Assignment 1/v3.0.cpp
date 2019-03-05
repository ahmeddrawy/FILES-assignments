/**
 *  Description: Creating a library of books using files processing and manipulation
 *  author(s) : Ahmed Mohammad Hanafy 20170357
 *              Ahmad Khaled Fawzy    20170377
 *  version : 3.0
 *  Date : March 1st 2019
 */

/*
 *  6- TODO update book by ISBN
 */

/**
 * todo we have problem with updating the ISBN ,HANAFY
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
//        cout<<"RRN: "<<obj.RRN<<endl;  /// we don't need the RRN in printing , HANAFY
    }
    friend  istream &operator >> (istream & in , Book &obj){
        memset(&obj.author , 0, sizeof(obj) );
        cout<<"input ISBN: ";
        in>>obj.isbn;
        cout<<"input author name: ";
        in.ignore();
        in.getline(obj.author,100,'\n');
        cout<<"input title: ";
        in.getline(obj.title,100,'\n');
        cout<<"input the Book publish year: ";
        in>>obj.year;
        cout<<"input number of pages: ";
        in>>obj.pages;
        cout<<"input book price: ";
        in>>obj.price;
        return in ;
    }
    void writeRecordtofile( fstream &out , map<string, long long> &isbnMap,
                            map<string, long long> &titleMap , map<string ,Book> & isbnObject )
    {
        out.seekp(0, ios::end );
        RRN = out.tellp();
/*       cout<<"RRN : "<<RRN<<endl;
        cout<<*this<<endl;
        */
        string strISBN = this->isbn;
        isbnMap[strISBN] = RRN;         /// i think we don't have to update the object in file because we rewrite it and updat in the map
        isbnObject[strISBN] = *this;
        titleMap[this->title] = RRN;
        int len = strlen(this->isbn);   /// we need to check this too
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

        char del = '|'; /// we don't put the delimmiter size
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
            ,fstream &out , map<string, long long> &isbnMap, map<string
            , long long> &titleMap){
            Book currBook  = isbnObject[BookISBN];
            /** the update is working with this right now  5:14AM 05/03/19 , HANAFY
             * cout<<"GIVE me the New record \n";
                cin>>currBook;


               currBook.writeRecordtofile(out ,isbnMap,titleMap , isbnObject );
            return;
             */
            int option ;
            do{
                Book::menu();
                cin>>option;
                switch (option ){
                    case 0 :
                        cout<<"Good Bye \n";
                        break;
                    case 1:
                        cout<<"Give me the ISBN \n";
                        cin.ignore();
                        cin.getline(currBook.isbn , 6);
                        cout<<currBook.isbn<<endl;
                        break;
                    case 2:
                        cout<<"Give  me the Author\n";
                        cin.ignore();
                        cin.getline(currBook.author , 100);
                        break;

                    case 3:
                        cout<<"Give me the Title \n";
                        cin.ignore();
                        cin.getline(currBook.title , 100);
                        break;
                    case 4:                 ///year     5
                        cout<<"Give me the Year \n";
                        cin.ignore();
                        cin.getline(currBook.year , 5);
                        break;
                    case 5:                 /// page    5
                        cout<<"Give me the Pages \n";
                        cin.ignore();
                        cin.getline(currBook.pages, 5);
                        break;
                    case 6:                 /// price 20
                        cout<<"Give me the Price \n";
                        cin.ignore();
                        cin.getline(currBook.price , 20);
                        break;
                    default:
                        cout<<"Give me an option";
                        break;

                }
            }
            while(option != 0 );
        /*  fstream fout ;
            out.open("out.txt",ios::app);   /// when sending the same file in the main we have a problem by overwriting multiple records
                                            /// i haven't checked why yet , but now we have garbage in the isbn of the first record i don't know why to
          */
            cout<<currBook<<endl;
            cout<<"in the upate\n";
            currBook.writeRecordtofile(out ,isbnMap,titleMap , isbnObject );

    }

    /** Uses a map to access all byte-offsets of the records in the data file then prints info of all records
     *  @param fstream, map<string, long long>
     *  @return void
     */
    static void printBooks (fstream &booksFile, map<string, long long> &isbnMap)
    {
        booksFile.seekg(0, ios::end);
        if(booksFile.tellg() == 0) /// Checking if the file is empty before reading from it
        {
            cout << "This file contains no records." << endl;
        }
        else
        {
            booksFile.seekg(0, ios::beg);
            for(auto itr = isbnMap.begin(); itr != isbnMap.end(); ++itr) ///using map to access all byte-offsets in records file.
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

                for (int i=0; i<6; ++i) {
                    int length;
                    booksFile.read((char *) &length, sizeof(length));
                    char *BUFFER = new char[length + 1];    /// done +1 for the delimiters
                    for(int j = 0; j < length + 1; ++j){
                        BUFFER[j] = 0;
                    }
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
    static void printBook (string title, map<string, long long> &titleMap, fstream &booksFile
            , map<string , Book> &isbnObject)
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
                char *BUFFER = new char[length +1];
                BUFFER[length]=0;
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



int main() {
//    freopen("/home/www/Desktop/files/Files Assignments/Assignment 1/in.txt", "r",stdin);
//    Book b1,b2,b3;


    Book b1("123" , "ahmed hanfy","Kafka On The Shore" ,"1997" , "231" , "13.5" );
    Book b2("125" , "harouki","Kafka" ,"2003" , "452" , "62.5" );
    Book b3("1234", "Ahmad Khaled", "Ahbabt Waghdan", "2018", "6", "2");
/*
    cin>>b1;
    cin>>b2;
    cin>>b3;

*/
    map<string , long long> isbnMap;  /// mapping ISBN to RRN
    map<string , long long> titleMap;
    map<string , Book> isbnObject; /// mapping with the isbn to book, when writing to file only
    fstream out;
    out.open("out.txt", ios::in | ios::out  );
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
//    Book::printBooks(out, isbnMap);
    Book::update(isbnObject , "125" ,out, isbnMap , titleMap);
//    Book::update(isbnObject , "123" ,out, isbnMap , titleMap);
//    Book::printBooks(out, isbnMap);
//    cout<<"MAIN \n"<<endl;
//    for(auto it : isbnObject)cout<<it.second<<endl;

//    Book::deleteBook("123", isbnMap, out);
//    Book::printBooks(out ,isbnMap );
//    Book::printBooks(out, isbnMap);
    Book::printBook("Kafka", titleMap, out, isbnObject);
//    Book::printBook("Kafka On The Shore", titleMap, out , isbnObject);
//    Book::printBook("Ahbabt Waghdan", titleMap, out , isbnObject);

}

#include <iostream>
#include "DataBase.h"
int main() {
//   freopen("input.txt", "r" ,stdin);
    DataBase m ;
    m.printIndxes();
    cout<<"1-get record by name \n2-get record by id \n3-insert new record \n 0- to quit\n";
    while(true){

        cout<<"Enter your choice : ";
        int x  ; cin >> x ;
        if(x== 0 ){
            break;
        }
        else  if (x == 1 ){ /// insert record
            m.getRecordsByName();
        }
        else if(x == 2){
            m.getRecordById();
        }
        else if(x == 3 ){

            m.insertRecord();

        }
    }

//    m.insertRecord();
//    m.insertRecord();
//    m.insertRecord();
//    m.insertRecord();
//    m.writeVectorsToFiles();
//    m.compare();
//    m.readPrimary();
//    puts("-----");
//    m.readSecondary();
//    puts("-----");
//    m.readInverted();
//  m.readRecordByOffset(1);
//    char ar[3]= "32";
//    cout<<atoi(ar) +101;
    return 0;
}
#include <iostream>
#include "DataBase.h"
int main() {
    int y;
    bool continuelooping  = true;
    DataBase m ;

    m.printIndxes();
    do
    {
        cout<< "What Would You Like To Do \n";
        cout << "\t1. Insert New Record \n " << "\t2. Get Record By ID \n" << "\t3. Get Record By Name   \n";
        cout<<  "\t4. Delete Record By ID \n" <<"\t5. Delete Record By Name \n" << "\t6. Update Record  by ID\n" << "\t7. update Record By Name\n\t8.reload the index\n";
        cout<<  "\t0.EXIT\n";
        cout<< " Enter Choice \n ";
        cin >> y;
        switch(y){
            case 1:
                cout << "Insert New Record : \n";
                m.insertRecord();
                break;
            case 2:
                cout << "Get Record By ID : \n";
                m.getRecordById();
                break;

            case 3:
                cout << "Get Record By Name : \n";
                m.getRecordsByName();
                break;
            case 4:
                cout << "Delete Record By ID : \n";
                cin.ignore();
                char id [6];
                cin.getline(id,5);
                m.deleteById(id);
                break;
            case 5:
                cout << "Delete Record By Name : \n";
                char Name[51];
                cin>>Name;
                m.deleteByName(Name);
                break;
            case 6:
                cout << "Update Record by ID \n";
                char ID [6];
                cin.ignore();
                cin.getline(ID ,6);
                m.updateById(ID);
                break;
            case 7:
                cout << "updating by Name \n";
                char name [51];
                cin.ignore();
                cin.getline(name ,6);
                m.updateByName(name);
                break;
            case 8:
                m.ConstructVectorsIndx();
                m.printIndxes();
                break;
            case 0:
                continuelooping = false;
                break;
            default :
                cout << "Invalid Input \n";
                continuelooping =false;
                break;
        }
    }while (continuelooping);
    return 0;
}
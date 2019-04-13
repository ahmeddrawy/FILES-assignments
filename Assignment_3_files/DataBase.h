//
// Created by Hanfy on 13/04/19.
//
#include <vector>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#ifndef ASSIGNMENT_3_FILES_DATABASE_H
#define ASSIGNMENT_3_FILES_DATABASE_H
class DataBase {
private:
    bool mIndxUpdated = false ;
    struct sDataRecord{
        char ID[6];
        char CouseName[51];
        char Instructor[51];
        char Weeks [3] ;
        friend istream&operator>> (istream & in, sDataRecord & temp ){
            cout<<"Please enter the Course ID " ;
            in.getline(temp.ID , 5);
            cout<<"Please enter the Course Name " ;
            in.getline(temp.CouseName , 50);
            cout<<"Please enter the Course Instructor " ;
            in.getline(temp.Instructor , 50);
            cout<<"Please enter the Course Weeks " ;
            in.getline(temp.Weeks , 50);
            return  in ;
        }
        friend ostream &operator<< (ostream&out , sDataRecord &obj){
            out << "course ID : "<<obj.ID<<endl;
            out << "course Name : "<<obj.CouseName<<endl;
            out << "course Instructor : "<<obj.Instructor<<endl;
            out << "course Weeks : "<<obj.Weeks<<endl;
        }
        bool operator<( sDataRecord &y)const{
            return atoi(ID)<=atoi(y.ID);
        }
    };
    struct sPrimaryIndx{
        char ID [6];
        int offset = 0 ;

        bool operator<( sPrimaryIndx &y)const{
            return atoi(ID)<=atoi(y.ID);
        }

    };
    struct sSecondaryIndx{
        char InstructorName[51];
        int PrimaryIndx = -1 ;
    };
    struct sInvertedList{
        char PrimaryIndx[6];
        int nxt = -1;
    };
    vector<sPrimaryIndx> mPrimaryIndx;
    vector<sSecondaryIndx> msecondaryIndx;
    vector<sInvertedList> mInvertedList;
    string mDataFilePath = "/home/www/Desktop/files/Files Assignments/Assignment_3_files/DataFile.txt";
    string mPrimaryIndxPath = "";
    string mSecondaryIndxFilePath = "";
    string mInvertedListFilePath = "";

public:
    DataBase(){

    }
    void insertRecord(){
        this->ChangeFlag(false);
        sDataRecord temp ;
        cin >> temp ;
        char Buffer [200];
        strcpy(Buffer , temp.ID); strcat(Buffer , "|");
        strcat(Buffer , temp.CouseName); strcat(Buffer , "|");
        strcat(Buffer , temp.Instructor); strcat(Buffer , "|");
        strcat(Buffer , temp.Weeks); strcat(Buffer , "|");
        int len = strlen(Buffer);
        fstream out ;
        out.open(mDataFilePath.c_str() , ios:: in |ios ::binary | ios::out);
        out.seekp(0 , ios::end  );
        int address = out.tellg();
        out.write((char *) &len , sizeof(len));
        out.write(Buffer , len );
        out.close();
        sPrimaryIndx tPrimary ;
        strcpy(tPrimary.ID , temp.ID);
        tPrimary.offset = address;

        mPrimaryIndx.push_back(tPrimary);




    }
    void compare(){/// todo remove this
        sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
        for (int i = 0; i <mPrimaryIndx.size() ; ++i) {
            cout<<mPrimaryIndx[i].ID<<endl;
        }

    }
    sDataRecord readRecord( int offset){
        fstream out ;
        out.open(mDataFilePath.c_str() , ios::in | ios::binary);
        out.seekg(offset , ios::beg);
        int len ;
        out.read((char *)&len , sizeof(len));
        char * buffer = new char [len];
        out.read(buffer , len );
        istringstream str(buffer);
        sDataRecord ret ;
        str.getline(ret.ID,5, '|');
        str.getline(ret.CouseName,50, '|');
        str.getline(ret.Instructor,50, '|');
        str.getline(ret.Weeks,2, '|');
//        cout<<ret<<endl;
        return  ret;

    }
    void  ChangeFlag(bool state){
        fstream out;
        out.open(mDataFilePath.c_str() , ios::in | ios::out  | ios::binary );
        out<<state;
        out.close();
    }


};

#endif //ASSIGNMENT_3_FILES_DATABASE_H

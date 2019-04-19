//
// Created by Hanfy on 13/04/19.
//
#include <vector>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
/**
 * todo make function to read all the file then update the indexes by calling the previously made functions
 *
 */

#ifndef ASSIGNMENT_3_FILES_DATABASE_H
#define ASSIGNMENT_3_FILES_DATABASE_H
class DataBase {
private:
    bool mIndxUpdated = false ;
    int invertedRRN = 0 ;
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

        int InvertedListIndx = -1 ;  /// todo to be update to Inverted list indx
        bool operator<(sSecondaryIndx & obj){
            return (strcmp(InstructorName , obj.InstructorName)<0);
        }
    };
    struct sInvertedList{   /// todo recheck this and update it to the indx in the primary file
        char PrimaryIndx[6];
        int nxt = -1;
    };
    vector<sPrimaryIndx> mPrimaryIndx;
    vector<sSecondaryIndx> msSecondaryIndx;
    vector<sInvertedList> mInvertedList;
    string mDataFilePath = "/home/www/Desktop/files/Files Assignments/Assignment_3_files/DataFile.txt";
    string mPrimaryIndxPath = "/home/www/Desktop/files/Files Assignments/Assignment_3_files/PrimaryIndex";
    string mSecondaryIndxFilePath = "/home/www/Desktop/files/Files Assignments/Assignment_3_files/SecondaryIndex";
    string mInvertedListFilePath = "/home/www/Desktop/files/Files Assignments/Assignment_3_files/InvertedList";

public:
    DataBase(){
        this->readPrimary();
        this->readSecondary();
        this->readInverted();
        sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
        sort(msSecondaryIndx.begin() , msSecondaryIndx.end());

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
        sInvertedList tInverted;
        mPrimaryIndx.push_back(tPrimary);
        int indxOfSecondary =alreadyInSecondary(temp);
        if(indxOfSecondary>= 0 ) {
            int curr = msSecondaryIndx[indxOfSecondary ].InvertedListIndx;
            while(curr != -1 ){
                sInvertedList & node=  mInvertedList[curr];
              curr = node.nxt;
              if(curr == -1){
                  node.nxt = invertedRRN++;
                  sInvertedList iTemp;
                  iTemp.nxt = -1 ;
                  strcpy(iTemp.PrimaryIndx ,temp.ID);
                  mInvertedList.push_back(iTemp);
                  break;
              }
            }
        }
        else {
            sSecondaryIndx tSecondary;
            tSecondary.InvertedListIndx = invertedRRN++;
            strcpy(tSecondary.InstructorName , temp.Instructor);
            msSecondaryIndx.push_back(tSecondary);
            strcpy(tInverted.PrimaryIndx,  temp.ID);
            tInverted.nxt = -1;
            mInvertedList.push_back(tInverted);
        }


    }

    int alreadyInSecondary(sDataRecord & obj){
        for (int i = 0; i <msSecondaryIndx.size() ; ++i) {
            if(strcmp(msSecondaryIndx[i].InstructorName , obj.Instructor) == 0){
                return i ;
            }
        }
        return -1 ;
    }
    int  nxtRRNinInverted(){
        fstream out ; out.open(mInvertedListFilePath.c_str() , ios::out | ios :: binary);
        out.seekg(0 , ios::end  );
        auto it = out.tellg();
        return it / sizeof(sInvertedList);
    }
//    void compare(){/// todo remove this
//        sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
//        for (int i = 0; i <mPrimaryIndx.size() ; ++i) {
//            cout<<mPrimaryIndx[i].ID<<endl;
//        }
//
//    }
    void getRecordById(){
        char tempId[6];
        cout<<"Please enter the ID you want to search for : ";
        cin>>tempId;
        int indx = this->BinarySearchPrimary(tempId);
        if(indx != -1 ){
            cout<<mPrimaryIndx[indx].ID<<" "<<mPrimaryIndx[indx].offset<<endl; /// todo remove this
            sDataRecord temp = readRecord(mPrimaryIndx[indx].offset);
            cout<<temp<<endl;
        }
        else {
            cout<<"Record Not Found\n";
        }

    }
    void getRecordByName(){
        char Name [51];cin>>Name;


    }
    void WritePrimaryFile(){    /// todo 13/4/19 17:25,take care of the secondary indx and inverted list
        if(!this->mIndxUpdated){ /// todo

        }
        sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
        fstream out ;
        out.open (mPrimaryIndxPath.c_str() , ios::trunc | ios::binary);
        for (int i = 0; i <mPrimaryIndx.size() ; ++i) {
            out.write((char *)&mPrimaryIndx[i] , sizeof(mPrimaryIndx[i]));
            sDataRecord toBeSearched = readRecord(mPrimaryIndx[i].offset);
            sSecondaryIndx tempSecondary ;
            strcpy(tempSecondary.InstructorName , toBeSearched.Instructor);
//            tempSecondary.PrimaryIndx = i ; /// todo
            msSecondaryIndx.push_back(tempSecondary);

        }
    }
    int BinarySearchPrimary(char * arr){        /// return -1 if not found and indx in primary vector otherwise
        if(!this->mIndxUpdated){ ///todo  construct the primary

        }
        int l = 0 , r = mPrimaryIndx.size() -1 ;
        while(l<= r){
            int mid = (l+r )/2;
            if(strcmp( mPrimaryIndx[mid].ID , arr) == 0 ){
                return mid;
            }
            else if(atoi(mPrimaryIndx[mid].ID ) < atoi(arr)){
                l = mid +1 ;
            }
            else {
                r = mid -1;
            }

        }
        return -1 ;

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
    void writeVectorsToFiles() {
        sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
        sort(msSecondaryIndx.begin() , msSecondaryIndx.end());

        for (int i = 0; i < mPrimaryIndx.size(); ++i) {
            writePrimary(mPrimaryIndx[i]);
        }
        for (int j = 0; j <msSecondaryIndx.size() ; ++j) {
            writeSecondary(msSecondaryIndx[j]);
        }
        for (int k = 0; k <mInvertedList.size() ; ++k) {
            writeInverted(mInvertedList[k]);
        }
    }
    void writePrimary(sPrimaryIndx & obj){
        fstream out ; out.open(mPrimaryIndxPath.c_str() ,ios::app| ios:: out | ios::binary);
        out.seekp(0, ios::end);
        out.write((char *) &obj , sizeof(obj));
        out.close();
    }
    void writeSecondary(sSecondaryIndx & obj){
        fstream out ; out.open(mSecondaryIndxFilePath.c_str() , ios::app|ios:: out | ios:: binary);
        out.seekp(0, ios::end);
        out.write((char *)& obj , sizeof(obj));
        out.close();
    }
    void writeInverted(sInvertedList &obj){
        fstream out ; out.open(mInvertedListFilePath.c_str() ,ios::app| ios::out | ios ::binary);
        out.seekp(0, ios::end);
        out.write((char *)& obj , sizeof(obj));
        out.close();
    }
    void readPrimary() {
        ifstream out;
        out.open(mPrimaryIndxPath.c_str());
        while (true) {
            if (out.eof()) break;
            sPrimaryIndx temp;
            out.read((char *) &temp, sizeof(temp));
            if(out.eof()) break;
            mPrimaryIndx.push_back(temp);
//            cout << temp.ID << " " << temp.offset << endl;
        }
        out.clear();
        out.close();
    }
    void readSecondary() {
        ifstream out;
        out.open(mSecondaryIndxFilePath.c_str());
        while (true) {
            if (out.eof()) break;
            sSecondaryIndx temp;
            out.read((char *) &temp, sizeof(temp));
            if(out.eof()) break;
            msSecondaryIndx.push_back(temp);
//            cout << temp.InvertedListIndx << " " << temp.InstructorName << endl;
        }
        out.clear();
        out.close();
    }
    void readInverted(){
        ifstream out ; out.open(mInvertedListFilePath.c_str() );
        while(true) {
            if(out.eof()) break;
            sInvertedList temp;
            out.read((char *)&temp, sizeof(temp));
            if(out.eof()) break;
            mInvertedList.push_back(temp);
//            cout << temp.PrimaryIndx << " " << temp.nxt << endl;
        }
        out.clear();
        out.close();
    }

};

#endif //ASSIGNMENT_3_FILES_DATABASE_H

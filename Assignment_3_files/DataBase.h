//
// Created by Hanfy on 13/04/19.
//
#include <vector>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
/**
 * todo make function to read all the file then update the indexes by calling the previously made functions
 *  if      updated , then load from indx files
 *  else    load from data file then insert make what we make in insertion by constructing the indxs
 *  todo check if deleted
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
            in.ignore();
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
        readFlag();
        if(this->mIndxUpdated){
            cout<<"The Index is Updated and loading\n";
            this->readPrimary();
            this->readSecondary();
            this->readInverted();
            /*it should be sorted but we sort */
            sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
            sort(msSecondaryIndx.begin() , msSecondaryIndx.end());
            this->invertedRRN = mInvertedList.size();
        }
        else { /// construct indx
            ConstructVectorsIndx();
        }

    }
    void clearAllvectors(){
        mPrimaryIndx.clear();
        msSecondaryIndx.clear();
        mInvertedList.clear();
        this->invertedRRN = 0 ;
    }
    void ConstructVectorsIndx(){
        ifstream out ; out.open(mDataFilePath.c_str() , ios::in | ios:: binary );
        this->TruncIndx();
        clearAllvectors();
        out.seekg(0 , ios::end  );
        if(out.tellg() ==0 ){ /// open file in append mode and add the flag, we may add flag
            out.close();
            cout<<"the data file is currently empty"<<endl;
        }
        else {
            out.seekg(1, ios::beg);


//                out.close();
            while(true) {
                int offset = out.tellg();
                if(offset == -1 || out.eof() || out.fail() ){ /// end of file
                    break ;
                }
                sDataRecord tData = readRecordByOffset(out,offset);
                if(offset == -1 || out.eof() || out.fail() ){ ///checking again to avoid garbage
                    break ;
                }
                if(tData.ID[0] !='*')
                    constructIndxForRecord(tData , offset);
            }
            sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
            sort(msSecondaryIndx.end() , msSecondaryIndx.end());
            out.close();

        }

    }
    void insertRecord(){
        this->ChangeFlag(false);
        sDataRecord temp ;
        cin >> temp ;
        WriteDataRecord(temp);

    }
    void WriteDataRecord(sDataRecord &temp ){
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
        constructIndxForRecord(temp , address);
    }
    void constructIndxForRecord(sDataRecord & obj, int offset){
        sPrimaryIndx tPrimary ;
        strcpy(tPrimary.ID , obj.ID);
        tPrimary.offset = offset;
        sInvertedList tInverted;
        mPrimaryIndx.push_back(tPrimary);
        int indxOfSecondary =BinarySearchSecondary(obj.Instructor);
        if(indxOfSecondary>= 0 ) {
            int curr = msSecondaryIndx[indxOfSecondary ].InvertedListIndx;
            while(curr != -1 ){ /// the first iteration can't be -1 because curr is the indx of  inverted list of the first occurence for this name
                sInvertedList & node=  mInvertedList[curr]; /// reading by reference to update it
                curr = node.nxt;
                if(curr == -1){
                    node.nxt = invertedRRN++;
                    sInvertedList iTemp;
                    iTemp.nxt = -1 ;
                    strcpy(iTemp.PrimaryIndx ,obj.ID);
                    mInvertedList.push_back(iTemp);
                    break;
                }
            }
        }
        else {
            sSecondaryIndx tSecondary;
            tSecondary.InvertedListIndx = invertedRRN++;
            strcpy(tSecondary.InstructorName , obj.Instructor);
            msSecondaryIndx.push_back(tSecondary);
            strcpy(tInverted.PrimaryIndx,  obj.ID);
            tInverted.nxt = -1;
            mInvertedList.push_back(tInverted);
        }
    }

    void getRecordById(char * tempId = 0) {
        if(tempId == 0 ) {
            tempId = new char [6];
            cout << "Please enter the ID you want to search for : ";
            cin >> tempId;
        }
        int indx = this->BinarySearchPrimary(tempId);
        ifstream out ; out.open(mDataFilePath.c_str() , ios::in | ios::binary);
        if(indx != -1 ){
            cout<<mPrimaryIndx[indx].ID<<" "<<mPrimaryIndx[indx].offset<<endl; /// todo remove this
            sDataRecord temp = readRecordByOffset(out,mPrimaryIndx[indx].offset);
            cout<<temp<<endl;
        }
        else {
            cout<<"Record Not Found\n";
        }
        out.close();

    }
    void getRecordsByName(char * Name = 0 ){
        if(Name == 0 ) {
             Name = new char[51];
            cout << "Please provide the Instructor Name  you want to search for : ";
            cin >> Name;
        }
        int indxInSecondary = BinarySearchSecondary(Name) ; /// getting the indx of record in secondary

        if(indxInSecondary == -1 ){
            cout<<"No Such a Record \n";
        }
        else {
            int nxt = msSecondaryIndx[indxInSecondary].InvertedListIndx; ///getting the invertedd list indx
            while(nxt != -1) {
                this->getRecordById(mInvertedList[nxt].PrimaryIndx);    /// getting the record by Id , utility functinon
                nxt = mInvertedList[nxt].nxt;   /// moving the indx to the nxt one and if -1 we break
            }
        }

    }
    /*void WritePrimaryFile(){    /// todo 13/4/19 17:25,take care of the secondary indx and inverted list
        if(!this->mIndxUpdated){ /// todo

        }
        sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
        fstream out ;
        out.open (mPrimaryIndxPath.c_str() , ios::trunc | ios::binary);
        for (int i = 0; i <mPrimaryIndx.size() ; ++i) {
            out.write((char *)&mPrimaryIndx[i] , sizeof(mPrimaryIndx[i]));
            sDataRecord toBeSearched = readRecordByOffset(mPrimaryIndx[i].offset);
            sSecondaryIndx tempSecondary ;
            strcpy(tempSecondary.InstructorName , toBeSearched.Instructor);
//            tempSecondary.PrimaryIndx = i ; /// todo
            msSecondaryIndx.push_back(tempSecondary);

        }
    }*/
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
    int BinarySearchSecondary(char * Name){
        if(!this->mIndxUpdated){ /// todo

        }

        int l = 0 , r = msSecondaryIndx.size()-1;
        while(l <= r ){
            int mid = (l+ r)/2;
            if(strcmp(msSecondaryIndx[mid].InstructorName ,Name) == 0  ){
                return mid ;
            }
            else if(strcmp(msSecondaryIndx[mid].InstructorName , Name) < 0){
                l = mid +1;

            }
            else {
                 r= mid -1;
            }
        }
        return -1;


    }
    sDataRecord readRecordByOffset(ifstream &out , int offset){ /// reading record by offset
//        fstream out ;
//        out.open(mDataFilePath.c_str() , ios::in | ios::binary);
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
    void deleteRecord(int offset){
        fstream out; out.open(mDataFilePath.c_str() , ios::in | ios::out | ios :: binary );
        out.seekp(offset +4 , ios::beg);
        char del = '*';
        out.write((char *)&del , 1 );
        out.close();
    }
    void deleteById(char * mId){
        int indx = BinarySearchPrimary(mId);
        if(indx !=-1){
            deleteRecord(mPrimaryIndx[indx].offset);
        }
        else {
            cout<<"Can't delete this ID not found \n";
        }
        this->ConstructVectorsIndx();


    }
    void deleteByName(char * Name ){
        int indx = BinarySearchSecondary(Name);
        if(indx != -1){
            int nxt = msSecondaryIndx[indx].InvertedListIndx; ///getting the invertedd list indx
            while(nxt != -1) {
                this->deleteById(mInvertedList[nxt].PrimaryIndx);    /// getting the record by Id , utility functinon
                nxt = mInvertedList[nxt].nxt;   /// moving the indx to the nxt one and if -1 we break
            }
        }
        else {
            cout<<"Record can't be deleted  - Not found - \n";
        }
        this->ConstructVectorsIndx();
    }
    void updateById(char * mId){
        int indx = BinarySearchPrimary(mId );
        if(indx != -1){
            ifstream out ; out.open(mDataFilePath.c_str() , ios::in |ios::binary);
            sDataRecord tData = readRecordByOffset(out , mPrimaryIndx[indx].offset);
            cout<<"Record before update \n";
            cout<<tData;
            this->deleteRecord(mPrimaryIndx[indx].offset);
            cout<<"Please enter the new record";
            cin>>tData;
            this->WriteDataRecord(tData);

        }
        else {
            cout<<"Can't update the record is not in the file\n";
        }
        this->ConstructVectorsIndx();

    }
    void updateByName(char * Name){
        int indx = BinarySearchSecondary(Name);
        vector<sDataRecord> tempVec;

        if(indx != -1){
            cout<<"Name before Update\n";
            cout<<msSecondaryIndx[indx].InstructorName<<endl;
            int nxt = msSecondaryIndx[indx].InvertedListIndx; ///getting the invertedd list indx
            while(nxt != -1) {
                int indxPrimary = BinarySearchPrimary(  mInvertedList[nxt].PrimaryIndx);

                if(indxPrimary != -1){
                    ifstream out ; out.open(mDataFilePath.c_str() , ios::in | ios :: binary);
                    sDataRecord tData= readRecordByOffset(out, mPrimaryIndx[indxPrimary].offset);
                    tempVec.push_back(tData);
                    this->deleteRecord(mPrimaryIndx[indxPrimary].offset);

                }

                nxt = mInvertedList[nxt].nxt;   /// moving the indx to the nxt one and if -1 we break
            }
            cout<<"Enter the New Name ";
            char Name [51];
//            cin.ignore();
            cin.getline(Name , 51);
            for (sDataRecord & it : tempVec) {
                strcpy(it.Instructor,Name );
                WriteDataRecord(it);
            }
        }
        else {
            cout<<"Record can't be Updated  - Not found - \n";
        }


        this->ConstructVectorsIndx();
    }



    void  ChangeFlag(bool state){
        fstream out;
        out.open(mDataFilePath.c_str() , ios::in | ios::out  | ios::binary );
        out.seekp(0, ios::beg);
        out.write((char *)&state , 1);
        out.close();
    }
    void TruncIndx(){
        ofstream Primary ; Primary.open(mPrimaryIndxPath.c_str(), ios::trunc );
        ofstream Secondary ; Secondary.open(mSecondaryIndxFilePath.c_str(), ios::trunc );
        ofstream Inverted ; Inverted.open(mInvertedListFilePath.c_str(), ios::trunc );
        Primary.close();Secondary.close(); Inverted.close();
    }
    void readFlag(){
        ifstream out; out.open(mDataFilePath.c_str() , ios::in);
        out.seekg(0 , ios::beg);
        if(out.good())
            out.read((char *)&this->mIndxUpdated , 1);
        out.close();
    }
    void writeVectorsToFiles() {
        sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
        sort(msSecondaryIndx.begin() , msSecondaryIndx.end());
        this->TruncIndx();

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
    void printIndxes(){
        sort(mPrimaryIndx.begin() , mPrimaryIndx.end());
        sort(msSecondaryIndx.begin() , msSecondaryIndx.end());
        for (auto it : mPrimaryIndx){
            cout<<it.ID<<" "<<it.offset<<endl;
        }
        puts("---");
        for(auto it: msSecondaryIndx){
            cout << it.InstructorName<<" "<<it.InvertedListIndx<<endl;
        }
        puts("---");
        for(auto it : mInvertedList){
            cout<<it.PrimaryIndx<<" "<<it.nxt<<endl;
        }
    }
    ~DataBase(){
        this->writeVectorsToFiles();
        this->ChangeFlag(true);

    }
};

#endif //ASSIGNMENT_3_FILES_DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
#define ll long long
#define F first
#define S second

using namespace std;

typedef pair <string, int> ppi;

void mergeFiles(char* outputFile, string tfile, int numChunks, ll runSize){

    // open temporary files
    FILE *sortedFiles[numChunks];
    for (int i = 0; i < numChunks; i++){
        string nameFile = tfile + to_string(i) + ".txt";
        char fileTemp[nameFile.size()];
        strcpy(fileTemp, nameFile.c_str());
        sortedFiles[i] = fopen(fileTemp, "r");
    }
    // Create priority_queue used STL to stored the smallest string in each chunks
    priority_queue<ppi, vector<ppi>, greater<ppi> > pq;
    for (int i = 0; i < numChunks; i++){
        char getLine[runSize];
        fgets(getLine, runSize, sortedFiles[i]);
        string s = getLine;
        if (s.size() > 1)
            pq.push({s,i});
    }
    // open output file
    FILE* fp;
    fp = fopen(outputFile, "w");
    int count=0;

    while (pq.size()){
    // get the smallest string in priority queue
        ppi root = pq.top();
        pq.pop();

        string value = root.F;
        int fileNum = root.S;

    // write the smallest string to output file
        fprintf(fp, "%s", value.c_str());

    // get the next smallest string in the same chunk into queue
        char *getLine = (char*) malloc( sizeof(char) * runSize);
        if (fgets(getLine, runSize, sortedFiles[fileNum])!= NULL){
            value = getLine;
            if (value.size() > 1){
                pq.push({value,fileNum});
            }
        }
    }
    // close file
    for (int i = 0; i < numChunks; i++)
        fclose(sortedFiles[i]);
    fclose(fp);
}

void createInitialRun(char* inputFile, string tfile, ll runSize, int &numChunks, ll fileSize){

    //open inputFile
    FILE* fp;
    fp = fopen(inputFile, "r");
    ll sizeLeft = fileSize;
    string line;
    string lineNextFile="";
    numChunks = 0;
    while (1){

        //allocate arr
        vector <string> arr;
        ll sizeTempF = 0;

        // push this line has get from previous stage to arr
        if (lineNextFile.size()>1){
            arr.push_back(lineNextFile);
            cout<<sizeLeft<<" "<<lineNextFile;
            sizeTempF += lineNextFile.size();
            lineNextFile="";
        }
        // Read file elements into arr from input file
        while (sizeLeft > 0){
            char *getLine = (char*) malloc( sizeof(char) * runSize);
            if (fgets(getLine, runSize, fp)!= NULL){
                line = getLine;
                sizeLeft -= (line.size() + 1);

                // skip blank
                if (line.size() > 1){
                    // add end line
                    if (line[line.size()-1] != '\n'){
                        line += '\n';
                    }
                // Because each lines have different size.
                // I push_back as much as line until reach RAM limit
                    if (line.size() + sizeTempF <= runSize){
                        arr.push_back(line);
                        sizeTempF+=line.size();
                    }else{
                    // I stored this line has get from inputFile but does not push into arr
                        lineNextFile=line;
                        break;
                    }
                }
            }
            free(getLine);
        }
        if (arr.size()>0){
            //sort array using STLsort(Quick sort)
            sort(arr.begin(), arr.end());

            // Create temporary files and write sorted data into those files
            FILE* tf;
            string nameFileTemp = tfile + to_string(numChunks) + ".txt";
            char fileTemp[nameFileTemp.size()];
            strcpy(fileTemp, nameFileTemp.c_str());
            tf = fopen(fileTemp, "w");
            for (int i = 0; i < arr.size(); i++){
                fprintf(tf, "%s", arr[i].c_str());
            }
            fclose(tf);
            // increase number of chunks
            numChunks++;
        }else{
            break;
        }
    }
    fclose(fp);
}

ll findSize(char* inputFile){
    FILE* fp;

    fp = fopen(inputFile,"r");
    fseek(fp, 0, SEEK_END);
    ll fileSize = ftell(fp);

    fclose(fp);

    return fileSize;
}

void externalSort(char* inputFile, char* outputFile, ll runSize){

    ll fileSize = findSize(inputFile);
    int numChunks;
    // Prefix name of temporary files
    string tfile = "temp-file-";

    cout<<"Size of file: "<<fileSize<<endl;

    createInitialRun(inputFile, tfile, runSize, numChunks, fileSize);

    cout<<"Number of Chunks: "<<numChunks<<endl;

    mergeFiles(outputFile, tfile, numChunks, runSize);

}

int main(){
    string inputF, outputF;
    // run_size is RAM size
    ll runSize, fileSize;
/*
    cout<<"Input File Name: ";
    cin>>inputF;
    cout<<"Output File Name: ";
    cin>>outputF;
    cout<<"Size of RAM: ";
    cin>>runSize;
*/
    inputF = "input.txt";
    outputF = "output.txt";
    char inputFile[inputF.size()];
    strcpy(inputFile, inputF.c_str());
    char outputFile[outputF.size()];
    strcpy(outputFile, outputF.c_str());
    runSize = 10000000;

    externalSort(inputFile, outputFile, runSize);

    return 0;
}

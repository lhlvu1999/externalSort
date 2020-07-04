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

void mergeFiles(string outputFile, string tfile, int numChunks){

    // open temporary files
    ifstream sortedFiles[numChunks];
    for (int i = 0; i < numChunks; i++){
        string nameFile = tfile + to_string(i) + ".txt";
        sortedFiles[i].open(nameFile, ifstream::in);
    }
    // Create priority_queue used STL to stored the smallest string in each chunks
    priority_queue<ppi, vector<ppi>, greater<ppi> > pq;
    for (int i = 0; i < numChunks; i++){
        string s="";
        getline(sortedFiles[i], s);
        if (s.size() != 0)
            pq.push({s,i});
    }
    // open output file
    ofstream outputF;
    outputF.open(outputFile, ofstream::out);
    int count=0;
    while (pq.size()){
    // get the smallest string in priority queue
        ppi root = pq.top();
        pq.pop();

        string value = root.F;
        int fileNum = root.S;

        value+="\n";
    // write the smallest string to output file
        outputF<< value;

    // get the next smallest string in the same chunk into queue
        getline(sortedFiles[fileNum], value);
        if (value.size() != 0){
            pq.push({value,fileNum});
        }
    }
    // close file
    for (int i = 0; i < numChunks; i++)
        sortedFiles[i].close();
    outputF.close();
}

void createInitialRun(string inputFile, string tfile, ll runSize, int &numChunks, ll fileSize){

    //open inputFile
    ifstream infile;
    infile.open(inputFile, ifstream::in);
    ll sizeLeft = fileSize;
    string line;
    string lineNextFile="";
    numChunks = 0;

    while (sizeLeft > 0){

        //allocate arr
        vector <string> arr;
        ll sizeTempF = 0;

        // push this line has get from previous stage to arr
        if (lineNextFile.size()>0){
            arr.push_back(lineNextFile);
            sizeTempF += lineNextFile.size();
            lineNextFile="";
        }
        // Read file elements into arr from input file
        while (sizeLeft > 0){
            getline(infile, line);
            sizeLeft -= (line.size() + 2);
            if (line.size()>0){
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

        //sort array using STLsort(Quick sort)
        sort(arr.begin(), arr.end());

        // Create temporary files and write sorted data into those files
        ofstream tf;
        tf.open(tfile + to_string(numChunks) + ".txt", ofstream::out);
        ostream_iterator<string> outputIterator(tf, "\n");
        copy(arr.begin(), arr.end(), outputIterator);
        tf.close();

        // increase number of chunks
        numChunks++;
    }
    infile.close();
}

ll findSize(string inputFile){
    ifstream fileInput (inputFile, ifstream::binary);

    fileInput.seekg (0, fileInput.end);
    ll fileSize = fileInput.tellg();
    fileInput.seekg (0, fileInput.beg);

    return fileSize;
}

void externalSort(string inputFile, string outputFile, ll runSize){

    ll fileSize = findSize(inputFile);
    int numChunks;
    // Prefix name of temporary files
    string tfile = "temp-file-";

    cout<<"Size of file: "<<fileSize<<endl;

    createInitialRun(inputFile, tfile, runSize, numChunks, fileSize);

    cout<<"Number of Chunks: "<<numChunks<<endl;

    mergeFiles(outputFile, tfile, numChunks);

}

int main(){
    string inputFile, outputFile;
    // run_size is RAM size
    ll runSize, fileSize;
    /*
    cout<<"Input File Name: ";
    cin>>inputFile;
    cout<<"Output File Name: ";
    cin>>outputFile;
    cout<<"Size of RAM: ";
    cin>>run_size;
    */
    /*
    inputFile = "input.txt";
    outputFile = "output.txt";
    runSize = 1000000;
    */
    externalSort(inputFile, outputFile, runSize);

    return 0;
}

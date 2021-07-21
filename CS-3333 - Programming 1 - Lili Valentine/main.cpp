#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <bitset>
#include <bits/stdc++.h>
#include <math.h>
using namespace std;

/********************************************************************************************************************
    This program will create a key and use it to encrypt a text file. This file will be encrypted by performing     *
        XOR operations on sections of the file the size of the key that was generated in function generateKey.      *
        The vignereEncryption function will then store these segements, in order, in a new text file called         *
        cipherText.txt. The vignereBreaking function will take a cipherText file and generate the key that was used *
        to encrypt it. Finally, a vignereDecode will be able to reverse this encryption by using the key            *
        and cipherText files and writing the results in binary in a separate file.                                  *
*********************************************************************************************************************/

//String to binary converter
string string2Binary(string s)
{
    string bin = "";
    string c = s;
    for (unsigned int g = 0; g < s.length(); g++) {
        bin += bitset<8>(static_cast<int>(c[g])).to_string();
    }

    return bin;
}

//Binary to char converter (Assumes that an 8 bit sequence is given)
int binary2Char(string s)
{
    int num = 0;

    //Looks at the first number in the binary sequence. If '1', then add 2^(7-p) to num.
    for(int p = 0; p < 8; p++){
        if(s[p] == '1'){
            num += pow(2, (7-p));
        }
    }

    if(num > 128){
        return (num % 128);
    }else{
        return num;
    }
}

//This function generates a random key length, such that a different size key is selected each time the program runs.
int randomKeyLength ()
{
    //Rand seed initializer
    srand(time(NULL));

    //Generates a random length between 2 and 10
    int kL = rand() % 9 + 2;

    return kL;
}

//This function generates a random key of alphabet characters and stores them in a key text file.
//  In order to return the array of chars this function must be declared as a pointer and the key made static.
char * generateKey (int kL)
{
    //This section creates the key file
    ofstream keyTextFile;
    keyTextFile.open("key.txt", ios::trunc);
    if (!keyTextFile.is_open()) {
        cerr << "Failed to open file 'keyText'";
        exit(1);
    }

    //Rand seed initializer
    srand(time(NULL));

    //This section will be the key generator
    char alphabet[52] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    static char key[10];

    //Generate the key and store it in the key text file
    string bin1 = "";
    for(int i = rand() % 52, k = 0; k < kL; i = rand() % 52, k++){
        key[k] = alphabet[i];
        bin1 += key[k];
    }

    cout << "The key is: " << bin1 << endl;
    keyTextFile << string2Binary(bin1);

    keyTextFile.close();

    //Returns a pointer to the array that stores the key
    return key;
}

//This function will use the key to convert the given file into cipherText
void vignereEncryption (int kL, char key[])
{
    //This section creates the ciphertext file
    ofstream cipherTextFile;
    cipherTextFile.open("cipherText.txt", ios::trunc);
    if (!cipherTextFile.is_open()) {
        cerr << "Failed to open file 'cipherText'";
        exit(1);
    }

    //This section will handle importing a file into C++
    ifstream plainTextFile;
    plainTextFile.open("C:\\Users\\theba\\OneDrive\\Documents\\Coding\\Computer Science\\C++ Files\\CS-3333 - Programming 1 - Lili Valentine\\USAmendments.txt", ios::in);
    if (!plainTextFile.is_open()) {
        cerr << "Failed to open file 'plainText'";
        exit(1);
    }

    string plainTextTemp; //Store keylength chunks of binary from file into here for immediate processing

    char ch; //Push chars to temp cipher holder
    string kh; //Holds pointer to array position char

    //The key is stored in a string and converted to binary
    for(int k = 0; k < kL; k++){
        kh += key[k];
    }
    kh = string2Binary(kh);

    //Collect key sized chunks of text from the file being encrypted
    plainTextFile.seekg(0, ios::beg);
    while(!plainTextFile.eof()){
        for(int m = 0; m < kL && !plainTextFile.eof(); m++){
            plainTextFile.get(ch);
            plainTextTemp += ch;
        }
        //Convert string to binary
        plainTextTemp = string2Binary(plainTextTemp);
        //Perform XOR, store binary in file
        for(unsigned int j = 0; j < kh.length(); j++){
            if(kh[j] == plainTextTemp[j]) {
                cipherTextFile << '0';
            }
            else if(kh[j] != plainTextTemp[j]) {
                cipherTextFile << '1';
            }
            else {
                cerr << "Something has gone terribly wrong.";
                exit(1);
            }
        }
        plainTextTemp = "";
    }

    cout << "Encryption completed.\n";
    cipherTextFile.close();
    plainTextFile.close();
}

//This function will use the key to convert the cipherText into the original file
void vignereDecode ()
{
    //This section will handle importing the key file
    ifstream keyBreak;
    keyBreak.open("C:\\Users\\theba\\OneDrive\\Documents\\Coding\\Computer Science\\C++ Files\\CS-3333 - Programming 1 - Lili Valentine\\key.txt", ios::in);
    if (!keyBreak.is_open()) {
        cerr << "Failed to open file 'key'";
        exit(1);
    }

    //This section will handle importing the cipherText file
    ifstream cipherBreak;
    cipherBreak.open("C:\\Users\\theba\\OneDrive\\Documents\\Coding\\Computer Science\\C++ Files\\CS-3333 - Programming 1 - Lili Valentine\\cipherText.txt", ios::in);
    if (!cipherBreak.is_open()) {
        cerr << "Failed to open file 'cipherText'";
        exit(1);
    }

    //This section will create a file to store the broken plainText
    ofstream plainBreak;
    plainBreak.open("brokenCipher.txt", ios::trunc);
    if (!plainBreak.is_open()) {
        cerr << "Failed to open file 'brokenCipher'";
        exit(1);
    }

    //Temporary variables for controlling loops and storing file data
    string cipherTemp = "";
    string binaryPlain = "";
    char ci; //Store char from file
    char nkey[80]; //Store binary form of key
    int keySize = 0; //Get size of binary key
    for(; keyBreak.get(nkey[keySize]) && !keyBreak.eof(); keySize++); //Gets the size and makes an array of the key from the key file

    //Run through the file and get key sized chunk of data
    cipherBreak.seekg(0, ios::beg);
    while(!cipherBreak.eof()){
        for(int m = 0; m < keySize && !cipherBreak.eof(); m++){
            cipherBreak.get(ci);
            cipherTemp += ci;
        }
        //Perform XOR but we still have a binary string.
        for(int j = 0; j < keySize; j++){
            if(nkey[j] == cipherTemp[j]) {
                binaryPlain += '0';
            }
            else if(nkey[j] != cipherTemp[j]) {
                binaryPlain += '1';
            }
            else {
                cerr << "Something has gone terribly wrong.";
                exit(1);
            }
        }

        //This will send the binary of the decoded plainText to the plainBreak file
        //In order to convert to ASCII, you must separate each 8 bit sequence and send it
        //  through binary2Char(), which can only handle 8 bit numbers
        //Alternatively, I could separate the 8 bit sequence in the function.
        plainBreak << binaryPlain;

        cipherTemp = "";
        binaryPlain = "";

    }

    cout << "File has been decoded." << endl;

    plainBreak.close();
    cipherBreak.close();
    keyBreak.close();
}

//This function will calculate the distribution of characters of an array
double calcDistribution(int arr[], int size, bool inclAll)
{
    double p = 0.000;
    double sum = 0;
    //calculate sum of all indexes
    for(int t = 0; t < size; t++){
        sum += arr[t];
    }
    //divide each point point in the index by the sum of all indexes
    int c;
    if(inclAll)
        c = 0;
    else
        c = 32;
    for(; c < size; c++){
        if(c > 90 && c < 97 && !inclAll)
            c = 97;
        try{
            p += pow((arr[c] / sum), 2);
        } catch (int e) {
            cerr << "Sum is 0, error occurred.";
            exit(1);
        }
    }

    return p;
}

//This will take the cipherText file and find the key
void vignereBreaking ()
{
    //This section will handle importing the cipherText file
    ifstream cipherText;
    cipherText.open("C:\\Users\\theba\\OneDrive\\Documents\\Coding\\Computer Science\\C++ Files\\CS-3333 - Programming 1 - Lili Valentine\\cipherText.txt", ios::in);
    if (!cipherText.is_open()) {
        cerr << "Failed to open file 'cipherText'";
        exit(1);
    }

    const int ASCIIBYTELENGTH = 128;
    int ascii[ASCIIBYTELENGTH] = {0};
    const int MAXKEY = 10;
    const int LENGTH = 8;
    int ml = 1;
    float keyDistribution[MAXKEY] = {0};
    char ci;
    string byte;

    //Find the size of the cipherText
    int fileSize = 0;
    while(!cipherText.eof()){
        cipherText >> ci;
        fileSize++;
    }
    cipherText.clear();

    //Find the byte at the location corresponding to the position of ml
    //Then, calculate the distribution of that character
    for(; ml <= MAXKEY; ml++){
        cipherText.seekg(0, ios::beg);
        for(int y = 0; y < 500 && !cipherText.eof(); y++){
            //Move the position
            cipherText.seekg((LENGTH * y * ml), ios::beg);
            //Get the byte at the position we are searching
            for(int x = 0; x < LENGTH && !cipherText.eof(); x++){
                cipherText >> ci;
                byte += ci;
            }
            //Convert the first byte at this step to a char and count how many times it appears
            ci = binary2Char(byte);
            ascii[ci]++;
            byte = "";
        }
        keyDistribution[ml] = calcDistribution(ascii, ASCIIBYTELENGTH, true);
        cout << "Length " << ml << ": " << keyDistribution[ml] << endl;
        for(int z = 0; z < ASCIIBYTELENGTH; z++){
            ascii[z] = 0;
        }
    }

    //Find the max distribution for key length
    double maxD = 0;
    int keyGuess = 0;
    for(int u = 0; u <= MAXKEY; u++){
        if((keyDistribution[u] - maxD) >= 0.06){
            maxD = keyDistribution[u];
            keyGuess = u;
        }
    }
    if(keyGuess == 0)
        keyGuess = 10;
    //Create an exception for keylength = 10

    cout << "My guess as to the size of the key is: " << keyGuess << endl;

    //Now we need to actually find the key
    //First, we need to pick a character to XOR against the cipherText. reuse char ci, string byte
    char kg;
    bool flag = false;
    string keyBreakGuess = "";
    string ranOuttaNames1 = "";
    string ranOuttaNames2 = "";
    double charDistribution[128-65][MAXKEY];
    for(int d = 0; d < keyGuess; d++){ //This loop will search each spot up to the length of the key
        for(ci = 65; ci < 123; ci++){ //This loop will look at every possible key character
            cipherText.seekg(0, ios::beg);
            cipherText.clear();
            //Weed out the few characters that aren't letters
            if(ci > 90 && ci < 97)
                ci = 97;
            for(int e = 0; e < 5000 && !cipherText.eof(); e++){  //This loop will handle the sampling
                int seeker = (LENGTH * keyGuess * e) + (LENGTH * d);
                if(seeker < fileSize) {
                    cipherText.seekg(seeker, ios::beg); //f(x) = 8*(c)*(x) + 8*(b)
                } else {
                    cipherText.seekg(0, ios::beg);
                    break;
                }
                for(int f = 0; f < LENGTH && !cipherText.eof(); f++){
                    cipherText >> kg;
                    byte += kg;
                }
                ranOuttaNames2 = ci;
                ranOuttaNames1 = string2Binary(ranOuttaNames2);
                ranOuttaNames2 = "";
                for(int u = 0; u < LENGTH; u++){
                    if(byte[u] == ranOuttaNames1[u])
                        ranOuttaNames2 += '0';
                    else
                        ranOuttaNames2 += '1';
                }
                kg = binary2Char(ranOuttaNames2);
                byte = "";
                ranOuttaNames1 = "";
                if(((int)kg >= 32 && (int)kg < 123) || (int)kg == 10 || (int)kg == 11){
                    ascii[(int)kg]++;
                    flag = true;
                } else {
                    flag = false;
                    for(int h = 0; h < ASCIIBYTELENGTH; h++){
                        ascii[h] = 0;
                    }
                    break;
                }
            }
            //This is where we will calcDist for each char and pick the max character for that piece of the key
            if(flag){
                charDistribution[(int)ci-65][d] = calcDistribution(ascii, ASCIIBYTELENGTH, false);
                cout << ci << " " << (d+1) << ": " << charDistribution[(int)ci-65][d] << endl;
                flag = false;
            } else {
                charDistribution[(int)ci-65][d] = 0;
            }
            for(int r = 0; r < ASCIIBYTELENGTH; r++){
                ascii[r] = 0;
            }
        }
    }

    for(int s = 0; s < keyGuess; s++){
        maxD = 0.000;
        for(int q = 65; q < 123; q++){
            //Weed out the few characters that aren't letters
            if(q > 90 && q < 97)
                q = 97;
            if(maxD < charDistribution[q-65][s] && charDistribution[q-65][s] != 0){
                maxD = charDistribution[q-65][s];
                kg = (char)q;
            }
        }
        keyBreakGuess += kg;
    }

    cout << "My guess for the full key: " << keyBreakGuess << endl; //This should have the full key

    cipherText.close();
}

int main()
{
    int keyLength = randomKeyLength(); //Generate keyLength
    cout << "The key length is: " << keyLength << endl;

    char *pkey; //Must be a pointer to hold address of array returned from function
    pkey = generateKey(keyLength); //Generate key

    vignereEncryption(keyLength, pkey); //Encrypt text
    vignereBreaking(); //Return key given ciphertext
    vignereDecode(); //Decrypt text (into binary)

    return 0;
}

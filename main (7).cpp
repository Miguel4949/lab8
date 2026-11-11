////
// Name: Miguel Angel Puga
// Date: October 4, 2026
// Class: CSCI 1470.0X
// Semester: Fall 2026
// CSCI 1470 Instructor: Dr. Jonatan Reyes
//
// Program Description: Spell checker (Part 2) 
//////

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <cctype>

using namespace std;
using namespace std::chrono;

const bool debug = true;
const int MAX_WORDS = 466551;
const string LINEAR_UNSORTED = "LINEAR_UNSORTED";
const string LINEAR_SORTED = "LINEAR_SORTED";
const string BINARY = "BINARY";

void initializeArray(string dict[], int size);
void loadDictionary(string dict[], int size, int &wordCount, bool &fileErrorFlag);
bool settings(int &maxMistakes, string &searchType);
bool searchUnsorted(const string dict[], int size, const string &target);
bool searchSorted(const string dict[], int size, const string &target);
bool searchBinary(const string dict[], int size, const string &target);
bool spelling(const string dict[], int size, const string &word, const string &searchType);

int main() {
    int maxMistakes = 3;
    string searchType = LINEAR_UNSORTED;

    if (!settings(maxMistakes, searchType)) {
        cout << "Error: settings.txt  could not be opened.\n";
        return 1;
    }

    string *dictionary = new string[MAX_WORDS];

    initializeArray(dictionary, MAX_WORDS);

    int wordCount = 0;
    bool fileError = false;
    loadDictionary(dictionary, MAX_WORDS, wordCount, fileError);
    
    if (fileError) {
        cout << "Error: dictionary.txt could not be found or opened.\n";
        delete[] dictionary;
        return 1;
    }

    if (searchType == LINEAR_SORTED || searchType == BINARY) 
    {
        sort(dictionary, dictionary + wordCount);
    }

    cout << "*****************************\n";
    cout << "Welcome to the spell checker!\n";
    cout << "*****************************\n";

    int mistakeCount = 0;
    string userWord;
    char userChoice = 'Y';
    bool firstWord = true;

    while (userChoice == 'Y' || userChoice == 'y') 
    {
        if (firstWord) 
        {
            cout << "Please enter a word: ";
            firstWord = false;
        } 
        else 
        {
            cout << "Enter another word: ";
        }
        
        cin >> userWord;

        string lowerWord = userWord;
        for (char &c : lowerWord) 
        {
            c = tolower(c);
        }
        bool isCorrect = spelling(dictionary, MAX_WORDS, lowerWord, searchType);

        if (isCorrect) 
        {
            cout << "The word " << userWord << " is spelled correctly!\n";
            mistakeCount = 0;
        } 
        else 
        {
            mistakeCount++;
            cout << "Oops! It looks like '" << userWord << "' isn't quite right. No worries, you're doing great-let's try spelling it again.\n";
            
            if (mistakeCount >= maxMistakes) 
            {
                cout << "You have reached the maximum number (" << maxMistakes << ") of misspelled words allowed.\n";
                break;
            }
        }

        cout << "Do you want to continue (Y/N): ";
        cin >> userChoice;
    }

    delete[] dictionary;
    return 0;
}


void initializeArray(string dict[], int size) 
{
    for (int i = 0; i < size; i++) 
    {
        dict[i] = "";
    }
}

bool settings(int &maxMistakes, string &searchType)
{
    ifstream file("settings.txt");
    if(!file)
    {
        return false;
    }
    file >> maxMistakes >> searchType;
    file.close();
    return true;
}

bool searchUnsorted(const string dict[], int size, const string &target) 
{
    for(int i = 0; i < size; i++)
    {
        
        if (dict[i] == target)
            return true;
    }
    return false;
}
bool searchSorted(const string dict[], int size, const string &target) 
{
    for(int i = 0; i < size; i++)
    {
        if(dict[i].empty())
            continue;
            
        if (dict[i] == target)
            return true;  
        
        if (dict[i] > target) 
            break;  
    }
    return false;
}
bool searchBinary(const string dict[], int size, const string &target) 
{
    int low = 0;
    int high = size - 1;
    
    while (low <= high) 
    {
        int mid = low + (high - low) / 2;
        
        if (dict[mid].empty())
        {
            high = mid - 1;
            continue;
        }
        if (dict[mid] == target)
            return true;
            
        if(dict[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return false;
}

void loadDictionary(string dict[], int size, int &wordCount, bool &fileErrorFlag) {
    ifstream file("dictionary.txt");
    if (!file) 
    {
        fileErrorFlag = true;
        return;
    }
    
    fileErrorFlag = false;
    string word;
    wordCount = 0;

    while (file >> word && wordCount < size) 
    {
        if (!word.empty() && word.back() == '\r') 
        {
            word.pop_back();
        }
        
        for (char &c : word) 
        {
            c = tolower(c);
        }
        
        dict[wordCount] = word;
        wordCount++;
    }
    file.close();
}

bool spelling(const string dict[], int capacity, const string &word, const string &searchType) 
{
    bool found = false;
    
    auto start = high_resolution_clock::now();

    if (searchType == LINEAR_UNSORTED) 
    {
        found = searchUnsorted(dict, capacity, word);
    }
    else if (searchType == LINEAR_SORTED) 
    {
        found = searchSorted(dict, capacity, word);
    } 
    else if (searchType == BINARY) 
    {
        found = searchBinary(dict, capacity, word);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    if (debug) {
        if (searchType == BINARY) 
        {
            cout << "Binary search took " << duration.count() << " microseconds.\n";
        }
        else 
        {
            cout << "Linear search took " << duration.count() << " microseconds.\n";
        }
    }

    return found;
}



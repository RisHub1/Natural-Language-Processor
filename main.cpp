#include <map>
#include <set>
#include "csvstream.h"
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// EFFECTS: Returns a set containing the unique "words" in the original
//          string, delimited by whitespace.
set<string> unique_words(const string &str) {
 istringstream source(str);
 set<string> words;
 string word;

 // Read word by word from the stringstream and insert into the set
 while (source >> word) {
   words.insert(word);
 }
 return words;
}

class MachineLearning 
{
    public: 
        MachineLearning(bool isDebug) : debug(isDebug), numPosts(0), wordBag(0),
        points(0), total(0) {}

        void readIn(csvstream &cvFile)
        {      
            map<string, string> row;
            set<string> uniqWords;
            if (debug) cout << "training data:\n";
            while (cvFile >> row) 
            {
                labels[row["tag"]] += 1;
                numPosts++;
                uniqWords = unique_words(row["content"]);
                for (auto word : uniqWords)
                {
                    words[word] += 1;
                    labelWords[{row["tag"], word}] += 1;
                }
                if (debug) 
                {
                    cout << "  label = " << row["tag"] << ", content = " << 
                    row["content"] << endl; 
                }
            }
            wordBag = words.size();
            cout << "trained on " << numPosts << " examples\n";
            if (debug) cout << "vocabulary size = " << wordBag << endl;
            cout << "\n";
            double score;
            if (debug) 
            {
                cout << "classes:\n";
                for (auto i : labels)
                {
                    cout << "  " << i.first << ", " << i.second 
                    << " examples, log-prior = ";
                    score = log(static_cast<double>(i.second) / numPosts);
                    cout << score << endl;
                }
                cout << "classifier parameters:\n";
                for (auto i : labelWords)
                {
                    cout << "  " << i.first.first << ":" << i.first.second << ", count = "
                    << i.second << ", log-likelihood = "; 
                    score = log(static_cast<double>(i.second) / labels[i.first.first]);
                    cout << score << endl;
                }
                cout << endl;
            }
        }

        void readTest(csvstream &cvFile)
        {
            map<string, string> row;
            set<string> uniqWords;
            string predicted;
            cout << "test data:\n";
            while (cvFile >> row) 
            {
                uniqWords = unique_words(row["content"]);
                for (auto l : labels)
                    calculateScore(l.first, uniqWords);
                cout << "  correct = " << row["tag"];
                predicted = printResults();
                cout << "\n  content = " << row["content"] << "\n\n";
                if (predicted == row["tag"])
                    points++;
                total++;
            }
            cout << "performance: " << points << " / " << total 
            << " posts predicted correctly\n";
        }

        void calculateScore(string label, set<string> uniqWords)
        {
            double score = 0;
            score += log(static_cast<double>(labels[label]) / numPosts);
            for (string word: uniqWords)
            {
                int often = labelWords[{label, word}];
                int appear = words[word];
                if (appear == 0)
                    score += log(static_cast<double>(1)/numPosts);
                else if (often == 0)
                    score += log(static_cast<double>(words[word])/numPosts);
                else
                    score += 
                    log(static_cast<double>(labelWords[{label, word}])/labels[label]);
            }
            scores[label] = score;
        }

        string printResults()
        {
            double max = -1 * numeric_limits<double>::max();
            string mostProb = "";
            for (auto i : scores)
                if (i.second > max)
                {
                    max = i.second; 
                    mostProb = i.first;
                }
            cout << ", predicted = " << mostProb << ", log-probability score = " << max;
            return mostProb;
        }

        void printData()
        {
            cout << "Total number of posts: " << numPosts << endl;
            cout << "Total number of unique words: " << wordBag << endl;
            cout << "Printing unique bag of words\n";
            for (auto a : words)
                cout << a.first << ": " << a.second << "\n";
            cout << endl;

            cout << "Printing  number of labels\n";
            for (auto a : labels)
                cout << a.first << ": " << a.second << "\n";
            cout << endl;

            cout << "Printing  word labels\n";
            for (auto a : labelWords)
                cout << a.first.first << ", " << 
                a.first.second << ": " << a.second << "\n";
        }

    private: 
        bool debug;
        int numPosts;
        int wordBag; 
        int points;
        int total;
        map<string, int> words; 
        map<string, int> labels; 
        map<pair<string, string>, int> labelWords;
        map<string, double> scores;
};

int main(int argc, char ** argv)
{
    cout.precision(3);
    if (argc != 3 && argc != 4)
    {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;     
    }

    string trainFile;
    string testFile;
    string debug;

    trainFile = argv[1];
    testFile = argv[2];

    if (argc == 4)
    {
        debug = argv[3];
        if (debug != "--debug")
        {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return 1;
        }
    }

    string errorTrain = "Error opening file: " + trainFile;
    string errorTest = "Error opening file: " + testFile;
    try 
    {
        csvstream cTrain(trainFile);
    }
    catch (csvstream_exception &c)
    {
        cout << "Error opening file: " << trainFile << endl;
        return 1;
    }
    csvstream cvFile(trainFile);
    try 
    {
        csvstream cTest(testFile);
    }
    catch (csvstream_exception &c)
    {
        cout << "Error opening file: " << testFile << endl;
        return 1;
    }
    csvstream cvTest(testFile);

    /*
    csvstream cvFile(trainFile);
    fstream file(trainFile);
    if (! file.is_open())
    {
       
    }
    
        
    fstream fTest(testFile);
    if (! fTest.is_open())
    {
        cout << "Error opening file: " << testFile << endl;
        return 1;
    }
    csvstream cvTest(testFile);
    

    csvstream cvFile(trainFile);
    if (!cvFile)
    {
        cout << "Error opening file: " << trainFile << endl;
        return 1;
    }
    csvstream cvTest(testFile);
    if (!cvTest)
    {
        cout << "Error opening file: " << testFile << endl;
        return 1;
    }
    */

    bool isDebug = debug == "--debug";
    MachineLearning algo(isDebug);
    algo.readIn(cvFile);
    algo.readTest(cvTest);
}

/*
 * A word ladder is a connection from one word to anotehr formed by
 * changing one letter at a time with the constraint that at each
 * step the sequence of letters still forms a valid word. E.g:
 *
 * A word ladder connecting the word "code" to the word "data" might
 * look like this:
 *
 * code --> cade --> cate --> date --> data
 *
 * This program finds the shortest word ladder between two words entered
 * by the user.
 *
 * It uses Stack and Queue collections.
 * Assume valid input.
 * Use the file dictionary.txt as a source of valid english words.
 *
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "hashmap.h"
#include "hashset.h"
#include "map.h"
#include "queue.h"
#include "random.h"
#include "set.h"
#include "simpio.h"
#include "stack.h"
#include "vector.h"

using namespace std;

/* Function prototypes */

HashSet<string> loadDictionary(string dictionaryFile);
Stack<string> wordladder(string w1, string w2, HashSet<string> &lexicon);

/* Constants */

const string DICT_FILE = "dictionary.txt";

/* Main code */

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Courier New-16");
    setConsoleEcho(true);

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    cout << "Please type two words: ";
    string word1;
    string word2;
    cin >> word1 >> word2;

    // Load dictionary
    cerr << "loading dictionary file " << DICT_FILE << "..." << endl;
    HashSet<string> lexicon = loadDictionary(DICT_FILE);
    cerr << "loaded dictionary, computing solution..." << endl;

    // Find winning (i.e. shortest) ladder
    Stack<string> winningLadder = wordladder(word1, word2, lexicon);

    // Print winning ladder
    if (winningLadder.isEmpty()) {
        cout << "No word ladder found from " << word2 << " back to "
                << word1 << endl;
    } else {
        cout << "Ladder from " << word2 << " back to " << word1 << ":"
                << endl;
        while (!winningLadder.isEmpty()) {
            cout << winningLadder.pop() << " ";
        }
        cout << endl;
    }

    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * Function loadDictionary()
 *
 * Loads dictionary from specified file. Assumes file contains one word
 * per line
 */
HashSet<string> loadDictionary(string dictionaryFile) {
    HashSet<string> lexicon;
    ifstream input;
    input.open(dictionaryFile.c_str());
    string line;
    while (getline(input, line)) {
        lexicon.add(line);
    }
    input.close();
    return lexicon;
}

/*
 * Function wordladder()
 *
 * @param w1 - start word
 * @param w2 - target word
 * @param &lexicon - reference to set containg valid dictionary words
 *
 * @return - a Stack of strings containing the word ladder from
 * w1 to w2. Returns an empty stack if no ladder exists.
 *
 * NOTE: "neighbour" is defined as a word that differs by one letter
 * from the current
 * NOTE2: assume supplied word is all lower case
 */
Stack<string> wordladder(string w1, string w2,
                         HashSet<string> &lexicon) {
    /* Scope-wide Variables */
    Queue< Stack<string> > pQueue;
    HashSet<string> usedWords;
    Stack<string> emptyLadder;

    // seed queue with initial stack containing {w1}
    Stack<string> s;
    s.push(w1);
    pQueue.enqueue(s);

    //DEBUG variable
    int iteration = 0;


    // performance tweak - words with mismatching lengths will NEVER
    // return a valid word ladder
    if (w1.length() != w2.length()) {
        return emptyLadder;
    }

    // if queue becomes empty, there is no valid solution
    while(!pQueue.isEmpty()) {
        //DEBUG printout
        iteration += 1;
        cerr << "iteration: " << iteration << endl;

        // dequeue next ladder, make iterations on top word
        Stack<string> ladder = pQueue.dequeue();
        string word = ladder.peek();

        // if word on top of the stack is the destination word
        if (word == w2) {
            // hooray! this stack is the shortest solution
            return ladder;
        } else {
            // for each new word that is a "neighbour" of word
            // on top of stack
            for (int i = 0; i < word.length(); i++) {
                // for every character in word
                for (int j = 0; j < 26; j++) {
                    // for every letter in the alphabet
                    // if valid english word AND hasn't been used before
                    // then push to new copy of stack, and requeue
                    string tryNextRung = word;
                    tryNextRung[i] = 'a' + j;

                    // performance is better if we quit immediately after
                    // finding a solution
                    if (tryNextRung == w2) {
                        ladder.push(tryNextRung);
                        return ladder;
                    }
                    // if new word is valid english and hasn't been used
                    // before, add it to a new ladder and enqueue
                    if (lexicon.contains(tryNextRung) &&
                            !usedWords.contains(tryNextRung)) {
                        Stack<string> nextLadder = ladder;
                        nextLadder.push(tryNextRung);
                        pQueue.enqueue(nextLadder);
                        usedWords.add(tryNextRung);
                    }
                }
            }

        }
    }
    // if we reached this point, the loop quit without finding a valid
    // word ladder at all
    return emptyLadder;
}

//
//  main.cpp
//  HashTagCounter
//
//  Created by Yao Heng on 11/9/16.
//  Copyright © 2016 Yao Heng. All rights reserved.
//

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "FibonacciHeap.h"

using namespace std;

int main(int argc, const char * argv[]) {
    if (argc == 1)
    {
        printf("Please include the input filename!\n");
        return -1;
    }
    ifstream input(argv[1]);
    string input_line;
    string hash_tag;
    string word_count;
    string::size_type pos = 0;
    int queryNum = 0;
    
    FibonHeap<int> *fibonHeap = new FibonHeap<int>();                // Create a new node store newly input hashtag
    unordered_map<string, FibonNode<int>*> hashMap;                  // Declare the map to store hashtag string and pointer to corresponding node
    unordered_map<string, FibonNode<int>*>::const_iterator findKey;  // Declare the iterator to check if there exists the same hashtag before
    FibonNode<int> *resNode = NULL;                                  // Declare the node to store the result
    
    ofstream output("output_file.txt");
    if (input.is_open())
    {
        while (getline(input, input_line))
        {
            // If this input line is a hashtag string
            if (input_line[0] == '#')
            {
                // Convert the input line to hashtag string and count of it
                pos = input_line.find(" ");
                hash_tag = input_line.substr(1, pos-1);
                word_count = input_line.substr(pos, input_line.length());
                
                // Define a new node by the hashtag and count
                FibonNode<int> *newNode = new FibonNode<int>(stoi(word_count), hash_tag);
                
                // If there exists the same hashtag, increase the existed node's key by count, if not, insert the node to fabionacci heap
                findKey = hashMap.find(hash_tag);
                if (findKey == hashMap.end())
                {
                    hashMap.insert(make_pair(hash_tag, newNode));
                    fibonHeap->insertNode(newNode);
                }
                else
                    fibonHeap->increaseKey(hashMap[hash_tag], stoi(word_count));
            }
            // If this input line is "STOP" sign
            else if (input_line == "STOP")
            {
                // Deallocate the fabionacci heap's memory block
                fibonHeap->deleteHeap();
                output.close();
            }
            // If this input line is query number
            else
            {
                // Output the query result to output txt file
                queryNum = stoi(input_line);
                for (int i = 0; i < queryNum; i++)
                {
                    resNode = fibonHeap->removeMax();
                    string hash = resNode->hashString;
                    if (i == queryNum - 1)
                        output<<hash<<endl;
                    else
                        output<<hash<<",";
                }
                // Reinsert the removed max node to fabionacci heap
                fibonHeap->reinsertNodes(fibonHeap->nodesVec);
                // Clear the removed max node vector
                fibonHeap->nodesVec.clear();
            }
        }
    }
    else
        cout << "error" <<endl;
    
    return 0;
}

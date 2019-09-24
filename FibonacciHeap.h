//
//  FibonacciHeap.h
//  HashTagCounter
//
//  Created by Yao Heng on 11/9/16.
//  Copyright © 2016 Yao Heng. All rights reserved.
//

#ifndef FibonacciHeap_h
#define FibonacciHeap_h

#include <string>
#include <vector>
#include <cmath>

// The node in Fabonacci heap
template <class T>
class FibonNode
{
public:
    T key;                         // Key value of node
    int degree;                    // Number of children of current node
    FibonNode<T> *child;           // Pointer to node's first child
    FibonNode<T> *leftSibling;     // Pointer to node's left sibling
    FibonNode<T> *rightSibling;    // Pointer to node's right siblig
    FibonNode<T> *parent;          // Pointer to node's parent
    bool childCut;                 // Flag to show if node has lost a child or not
    std::string hashString;        // Hashtag string
    
    // Fibonacci Heap node's constructor
    FibonNode(T value, std::string tag): key(value), hashString(tag), degree(0),
    child(NULL), leftSibling(NULL),rightSibling(NULL), parent(NULL), childCut(false) {
        key = value;
        hashString = tag;
        degree = 0;
        leftSibling = this;
        rightSibling = this;
        parent = NULL;
        child = NULL;
        childCut = false;
    }
};

template <class T>
class FibonHeap {
private:
    FibonNode<T> *maxNode;                      // The node has the maximum key
    int nodeNum;                                // Total number of node in heap
    FibonNode<T> **treeTable;                   // The table to keep track of trees by degree
    
public:
    FibonHeap();
    ~FibonHeap();
    
    std::vector<FibonNode<T>*> nodesVec;            // Vector to store deleted max nodes
    void insertNode(FibonNode<T> *node);            // Insert a new node to heap
    void addRootList(FibonNode<T> *node,
                     FibonNode<T> *root);           // Add node to root’s sibling list
    void removeFromLinkedList(FibonNode<T> *node);  // Remove node from its double linked list
    void increaseKey(FibonNode<T> *node, T value);  // Increase node key by value
    void cascadingCut(FibonNode<T> *node);          // Cascading cut heap from node
    void cut(FibonNode<T> *parent,
             FibonNode<T> *node);                   // Cut node from its parent and add it to root list
    FibonNode<T>* extractMaxtree();                 // Return the tree with maximum node in heap
    FibonNode<T>* removeMax();                      // Remove the maximum node in heap
    FibonNode<T>* link(FibonNode<T> *root,
                       FibonNode<T> *node);         // Insert node to root's children list
    void pairwiseCombine();                         // Combine trees which root node have equal degree
    void reinsertNodes(std::vector<FibonNode<T>*> vec);   // Reinsert deleted nodes
    void deleteHeap();                              // Release heap memory space
    void releaseNodes(FibonNode<T> *max);           // Release nodes memory space
};

/*
 * Constructor to create a new Fibonacci Heap
 */
template <class T>
FibonHeap<T>::FibonHeap()
{
    maxNode = NULL;
    nodeNum = 0;
    treeTable = NULL;
}

/*
 * Destructor to destroy the heap and release memory space
 */
template <class T>
FibonHeap<T>::~FibonHeap()
{
    deleteHeap();
}

/*
 * Add node to root’s sibling list
 */
template <class T>
void FibonHeap<T>::addRootList(FibonNode<T> *node, FibonNode<T> *root)
{
    if (root == NULL) {
        std::cout<< "Can't add a node to null node list!" << std::endl;
        return;
    }
    if (node == NULL)
    {
        std::cout<< "Can't add a null node to root list!" << std::endl;
        return;
    }
    
    // Insert the node between the root node and it's left sibling
    node->rightSibling = root;
    root->leftSibling->rightSibling = node;
    node->leftSibling = root->leftSibling;
    root->leftSibling = node;
}

/*
 * Insert a new node to heap
 */
template <class T>
void FibonHeap<T>::insertNode(FibonNode<T> *node)
{
    if (nodeNum == 0)
        maxNode = node;
    else
    {
        // Always insert the new node directly to root doubly linked list
        addRootList(node, maxNode);
        // Update the max node
        if (node->key > maxNode->key)
            maxNode = node;
    }
    nodeNum++;
}

/*
 * Increase the node's key by value
 */
template <class T>
void FibonHeap<T>::increaseKey(FibonNode<T> *node, T value)
{
    node->key += value;
    FibonNode<T> *parent = node->parent;
    // If the node's key value is larger than it's parent, remove it from it's sibling list and cut it from it's parent child list
    if(parent != NULL && node->key > parent->key)
    {
        cut(parent, node);
        cascadingCut(parent);
    }
    // Update the max node
    if(node->key > maxNode->key)
        maxNode = node;
}

/*
 * Remove the node from it's doubly linked sibling list
 */
template <class T>
void FibonHeap<T>::removeFromLinkedList(FibonNode<T> *node)
{
    node->leftSibling->rightSibling = node->rightSibling;
    node->rightSibling->leftSibling = node->leftSibling;
}

/*
 * Cut the node from it's sibling list and parent's child list
 */
template <class T>
void FibonHeap<T>::cut(FibonNode<T> *parent, FibonNode<T> *node)
{
    // Remove the node from it's sibling list
    removeFromLinkedList(node);
    
    // Update the node's parent's child pointer and degree
    if (node->rightSibling == node)
        parent->child = NULL;
    else
        parent->child = node->rightSibling;
    parent->degree--;
    
    // Update node's status and add it to root doubly linked list
    node->leftSibling = node;
    node->rightSibling = node;
    node->parent = NULL;
    node->childCut = false;
    addRootList(node, maxNode);
}

/*
 * Cascading cut follow path from parent of the node to the root
 */
template <class T>
void FibonHeap<T>::cascadingCut(FibonNode<T> *node)
{
    FibonNode<T> *parent = node->parent;
    if (parent != NULL)
    {
        // If childCut flag of the node is true then remove the node from it's sibling list and remove it from it's parent's child list
        if (node->childCut == false)
            node->childCut = true;
        else
        {
            cut(parent, node);
            cascadingCut(parent);
        }
    }
}

/*
 * Return the whole tree which root is the max node
 */
template <class T>
FibonNode<T>* FibonHeap<T>::extractMaxtree()
{
    if (maxNode == NULL)
        return NULL;
    FibonNode<T>* max = maxNode;
    
    // Update the max node of heap after deleting the current max node
    if (maxNode->rightSibling == maxNode)
        maxNode = NULL;
    else
    {
        removeFromLinkedList(maxNode);
        maxNode = maxNode->rightSibling;
    }
    
    // Update the deleted max node's sibling list and return it
    max->leftSibling = max;
    max->rightSibling = max;
    return max;
}

/* 
 * Remove the max node of fibonacci heap
 */
template <class T>
FibonNode<T>* FibonHeap<T>::removeMax()
{
    if (maxNode == NULL)
        return NULL;
    FibonNode<T>* max = maxNode;
    FibonNode<T>* child = NULL;
    
    // Remove every child of max node from it's sibling list and add it to root list
    while (max->child != NULL)
    {
        child = max->child;
        if (child->rightSibling == child)
            max->child = NULL;                 // If this is the last child node then delete it and return
        else
            max->child = child->rightSibling;  // If there are some other child nodes then assign the max node child pointer to current node's right sibling
        removeFromLinkedList(child);
        child->parent = NULL;
        addRootList(child, maxNode);
    }
    
    // After deleting every child node of max node, remove max node from it's sibling list and update heap's max node then pairwise combine the remaining nodes in root list
    removeFromLinkedList(max);
    if (max->rightSibling == max)
        maxNode = NULL;
    else
    {
        maxNode = max->rightSibling;
        pairwiseCombine();
    }
    
    // Push the deleted former max node to the node vector
    nodesVec.push_back(max);
    return max;
}

/*
 * Link the node to the root's child list
 */
template <class T>
FibonNode<T>* FibonHeap<T>::link(FibonNode<T> *root, FibonNode<T> *node)
{
    // Remove node from it's former sibling list
    removeFromLinkedList(node);
    
    // Add node to root's child list
    if (root->child == NULL)
    {
        root->child = node;
        node->leftSibling = node;
        node->rightSibling = node;
    }
    else
        addRootList(node, root->child);
    
    // Update the status of root after adding node to it's child list
    node->parent = root;
    root->degree++;
    node->childCut = false;
    return root;
}

/*
 * Pairwise combine the remaining nodes in root list
 */
template <class T>
void FibonHeap<T>::pairwiseCombine()
{
    // Alloc the memory space to the table to keep track of trees by degree and initial it
    int max_degree = nodeNum - 1;
    treeTable = (FibonNode<T>**) realloc(treeTable, sizeof(FibonNode<T>*) * max_degree);
    for (int i = 0; i < max_degree; i++)
        treeTable[i] = NULL;

    FibonNode<T> *max = NULL;
    FibonNode<T> *tableTemp = NULL;
    int curDegree = 0;
    // Extract the whole tree which root is max node and then combine this tree with current combined result
    while (maxNode != NULL)
    {
        max = extractMaxtree();
        curDegree = max->degree;
        // Combine trees with the same degree till there are no equal degree trees in current combined result
        while (treeTable[curDegree] != NULL)
        {
            tableTemp = treeTable[curDegree];
            // Add the smaller node to larger node's root list and store the link result to max
            if (tableTemp->key > max->key)
                max = link(tableTemp, max);
            else
                max = link(max, tableTemp);
            treeTable[curDegree] = NULL;
            // If there exists node which degree is curDegree+1 in current combined result, then combine it with current link result max
            curDegree++;
        }
        treeTable[curDegree] = max;
    }
    
    // Insert the pairwise combine result to a fabionacci heap
    for (int i = 0; i < max_degree; i++) {
        if (treeTable[i] != NULL)
        {
            if (maxNode == NULL)
                maxNode = treeTable[i];
            else
            {
                addRootList(treeTable[i], maxNode);
                if (treeTable[i]->key > maxNode->key) {
                    maxNode = treeTable[i];
                }
            }
        }
    }
}

/*
 * Insert nodes in node vector to fibonacci heap
 */
template <class T>
void FibonHeap<T>::reinsertNodes(std::vector<FibonNode<T>*> vec) {
    for (typename std::vector<FibonNode<T> *>::iterator iter = vec.begin() ; iter != vec.end(); ++iter)
        insertNode(*iter);
}

/*
 * Deallocate the memory block pointed to fibonacci nodes recursively
 */
template <class T>
void FibonHeap<T>::releaseNodes(FibonNode<T> *max)
{
    if(max == NULL)
        return;
    FibonNode<T> *temp_max = max;
    FibonNode<T> *temp;
    
    // Deallocate every child list's memory block
    do {
        temp = temp_max;
        temp_max = temp_max->rightSibling;
        releaseNodes(temp->child);
    }
    while(temp_max != max);
    
    // Deallocate every node's memory block in a child list
    do {
        temp = temp_max;
        temp_max = temp_max->rightSibling;
        delete temp;
    }
    while(temp_max != max);
}

// Deallocate the memory block pointed to fibonacci heap
template <class T>
void FibonHeap<T>::deleteHeap()
{
    releaseNodes(maxNode);
    free(treeTable);
    treeTable = NULL;
}
#endif /* FibonacciHeap_h */

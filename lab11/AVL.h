//
// Created by Daniel Devadas on 11/8/26.
//

#ifndef LAB11_AVL_H
#define LAB11_AVL_H
#include <string>
#include <vector>

class AVL {
public:
    AVL();
    void insert(int value);
    void del(int value);
    std::string serialize();
    void deserialize(std::string data);

private:
    std::vector<int> tree;
    int height(int index);
    void rebalance(int index);
    void clearSubtree(int index);
    void copySubtree(std::vector<int>& source, int sourceIndex, int destinationIndex);
    void rotateLeft(int index);
    void rotateRight(int index);
    void rotateLL(int index);
    void rotateRR(int index);
    void rotateLR(int index);
    void rotateRL(int index);
};


#endif //LAB11_AVL_H

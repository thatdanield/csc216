//
// Created by Daniel Devadas on 11/8/26.
//

#include "AVL.h"
#include <sstream>
#include <stdexcept>

AVL::AVL() : tree() {}

void AVL::insert(int value) {
    std::vector<int> path;
    int index = 0;
    while (index < tree.size() && tree[index] != -1) {
        if (tree[index] == value)
            return;
        path.push_back(index);
        if (value < tree[index])
            index = 2 * index + 1;
        else
            index = 2 * index + 2;
    }

    if (index >= tree.size())
        tree.resize(index + 1, -1);
    tree[index] = value;
    path.push_back(index);

    for (int pathIndex = path.size() - 1; pathIndex >= 0; pathIndex--)
        rebalance(path[pathIndex]);
}

void AVL::del(int value) {
    if (tree.empty() || tree[0] == -1)
        return;

    std::vector<int> path;
    int index = 0;
    while (index < tree.size() && tree[index] != -1 && tree[index] != value) {
        path.push_back(index);
        if (value < tree[index])
            index = 2 * index + 1;
        else
            index = 2 * index + 2;
    }

    if (index >= tree.size() || tree[index] == -1)
        return;
    path.push_back(index);

    if (2 * index + 1 < tree.size() && tree[2 * index + 1] != -1 &&
        2 * index + 2 < tree.size() && tree[2 * index + 2] != -1) {
        int deleteIndex = index;
        index = 2 * index + 2;
        path.push_back(index);
        while (2 * index + 1 < tree.size() && tree[2 * index + 1] != -1) {
            index = 2 * index + 1;
            path.push_back(index);
        }

        tree[deleteIndex] = tree[index];
        if (2 * index + 2 < tree.size() && tree[2 * index + 2] != -1) {
            std::vector<int> source = tree;
            clearSubtree(index);
            copySubtree(source, 2 * index + 2, index);
        } else {
            tree[index] = -1;
        }
    } else if ((2 * index + 1 < tree.size() && tree[2 * index + 1] != -1) ||
               (2 * index + 2 < tree.size() && tree[2 * index + 2] != -1)) {
        std::vector<int> source = tree;
        clearSubtree(index);
        copySubtree(source,
                    2 * index + 1 < source.size() && source[2 * index + 1] != -1
                        ? 2 * index + 1
                        : 2 * index + 2,
                    index);
    } else {
        tree[index] = -1;
    }

    for (int pathIndex = path.size() - 1; pathIndex >= 0; pathIndex--)
        if (tree[path[pathIndex]] != -1)
            rebalance(path[pathIndex]);
}

std::string AVL::serialize() {
    int last = tree.size() - 1;
    while (last >= 0 && tree[last] == -1)
        last--;

    std::string data;
    for (int index = 0; index <= last; index++) {
        if (index > 0)
            data += ',';
        data += std::to_string(tree[index]);
    }
    return data;
}

void AVL::deserialize(std::string data) {
    tree.clear();
    std::stringstream input(data);
    std::string value;
    while (std::getline(input, value, ','))
        tree.push_back(std::stoi(value));

    while (!tree.empty() && tree.back() == -1)
        tree.pop_back();
}

int AVL::height(int index) {
    if (index >= tree.size() || tree[index] == -1)
        return 0;
    int leftHeight = height(2 * index + 1);
    int rightHeight = height(2 * index + 2);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

void AVL::rebalance(int index) {
    if (height(2 * index + 1) - height(2 * index + 2) > 1) {
        if (height(4 * index + 3) - height(4 * index + 4) < 0)
            rotateLR(index);
        else
            rotateLL(index);
    } else if (height(2 * index + 1) - height(2 * index + 2) < -1) {
        if (height(4 * index + 5) - height(4 * index + 6) > 0)
            rotateRL(index);
        else
            rotateRR(index);
    }
}

void AVL::clearSubtree(int index) {
    if (index >= tree.size() || tree[index] == -1)
        return;

    tree[index] = -1;
    clearSubtree(2 * index + 1);
    clearSubtree(2 * index + 2);
}

void AVL::copySubtree(std::vector<int>& source, int sourceIndex, int destinationIndex) {
    if (sourceIndex >= source.size() || source[sourceIndex] == -1)
        return;

    if (destinationIndex >= tree.size())
        tree.resize(destinationIndex + 1, -1);
    tree[destinationIndex] = source[sourceIndex];
    copySubtree(source, 2 * sourceIndex + 1, 2 * destinationIndex + 1);
    copySubtree(source, 2 * sourceIndex + 2, 2 * destinationIndex + 2);
}

void AVL::rotateLeft(int index) {
    std::vector<int> source = tree;
    int right = 2 * index + 2;
    int left = 2 * index + 1;

    clearSubtree(index);
    if (left >= tree.size())
        tree.resize(left + 1, -1);
    tree[index] = source[right];
    tree[left] = source[index];
    copySubtree(source, left, 2 * left + 1);
    copySubtree(source, 2 * right + 1, 2 * left + 2);
    copySubtree(source, 2 * right + 2, right);
}

void AVL::rotateRight(int index) {
    std::vector<int> source = tree;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    clearSubtree(index);
    if (right >= tree.size())
        tree.resize(right + 1, -1);
    tree[index] = source[left];
    tree[right] = source[index];
    copySubtree(source, 2 * left + 1, left);
    copySubtree(source, 2 * left + 2, 2 * right + 1);
    copySubtree(source, right, 2 * right + 2);
}

void AVL::rotateLL(int index) {
    rotateRight(index);
}

void AVL::rotateRR(int index) {
    rotateLeft(index);
}

void AVL::rotateLR(int index) {
    rotateLeft(2 * index + 1);
    rotateRight(index);
}

void AVL::rotateRL(int index) {
    rotateRight(2 * index + 2);
    rotateLeft(index);
}

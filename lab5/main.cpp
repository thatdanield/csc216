#include <iostream>
#include <string>
#include <fstream>
#include <list>
using namespace std;


template<typename T>
class Node {
public:
    Node(T data) : data(data), next(nullptr) {}

    Node<T>* next;
    T data;
};

template<typename T>
class LinkedList {
    public:
        Node<T>* head;
        Node<T>* tail;
        Node<T>* curr;
        LinkedList() { //constructor-like function to make monopoly game
            ifstream file("monopoly_spaces.txt");
            string squareName;
            while(getline(file, squareName)) {
                Node<string>* newNode = new Node<string>(squareName);
                append(newNode);
            }
            file.close();
            curr = head;
            srand(time(0));
        }

        void append(Node<T>* newNode) {
            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                newNode->next = head;
                tail = newNode;
            }

        }
    void append(T data) {
        Node<T>* newNode = new Node<T>(data);
        append(newNode);
    }
        void removeNode() {
            if(head == nullptr) {}
            else if (head==tail) {
                head = nullptr;
                tail = nullptr;
            } else {
                Node<T>* curr = head;
                while(head->next != tail) {
                    head = head->next;
                }
                tail->next = nullptr;
                curr->next = head;
                tail = curr;
            }
        }
        void step() {
            curr = curr->next;
        }
        void roll() {
            int roll1 = rand() % 6 + 1;
            int roll2 = rand() % 6 + 1;
            int total = roll1 + roll2;
            for (int i = 0; i < total; i++) {
                step();
            }
        }

};

int main() {
    LinkedList<string> game;
    game.Monopoly();
    Node<string>* curr = game.head;
    do {
        std::cout << curr->data << std::endl;
        curr = curr->next;
    } while (curr != game.head);
    std::cout << curr->data << std::endl;
    return 0;
}
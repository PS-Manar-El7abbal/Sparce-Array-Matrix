#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int index;
    int data;
    Node* next;
    Node* prev;

    Node(int index, int data) : index(index), data(data), next(nullptr), prev(nullptr) {}
};

class ArrayDoubleLinkedList {
private:
    Node* head;
    Node* tail;
    int size;
    int elements;

public:
    ArrayDoubleLinkedList(int n) : head(nullptr), tail(nullptr), size(n), elements(0) {}

    ~ArrayDoubleLinkedList() {
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    Node *getIndex(int index, bool is) {
        if (index < 0 || index >= size) 
            return nullptr;

        Node *prev = head;
        while (prev && prev->next && prev->next->index < index)
            prev = prev->next;

        if (prev && prev->next && prev->next->index == index)
            return prev->next;
        
        if (is)
            return add_after(prev, 0, index);
        return nullptr;
    }

    void link(Node* first, Node* second) {
        if (first) {
            first->next = second;
        }
        if (second) {
            second->prev = first;
        }
    }

    void insert_end(int value, int index) {
        Node* item = new Node(index, value);
        if (!head) {
            head = tail = item;
        } else {
            link(tail, item);
            tail = item;
        }
        elements++;
    }

    void insert_front(int value, int index) {
        Node* item = new Node(index, value);
        if (!head) {
            head = tail = item;
        } else {
            link(item, head);
            head = item;
        }
        elements++;
    }

    Node *add_after(Node *before, int value, int index) {
        Node *middle = new Node(index, value);
        elements++;
        if (!before) {
            insert_front(value, index);
            return head;
        }
        Node *after = before->next;
        link(before, middle);
        link(middle, after);
        
        if (!after)
            tail = middle;
        return middle;
    }

    void insert_sort(int value, int index) {
        if (!head || index < head->index)
            insert_front(value, index);
        else if (index > tail->index)
            insert_end(value, index);
        else {
            Node *cur = head;
            while (cur && cur->index < index)
                cur = cur->next;
            add_after(cur->prev, value, index);
        }
    }

    void setValue(int value, int index) {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds" << endl;
            return;
        }
        insert_sort(value, index);
    }

    void printArray() {
        Node* cur = head;
        int currentIndex = 0;
        while (cur) {
            while (currentIndex < cur->index) {
                cout << "0 ";
                currentIndex++;
            }
            cout << cur->data << " ";
            currentIndex++;
            cur = cur->next;
        }
        while (currentIndex < size) {
            cout << "0 ";
            currentIndex++;
        }
        cout << endl;
    }

    void printWithoutZeros() {
        Node* cur = head;
        while (cur) {
            cout << cur->data;
            if (cur != tail) {
                cout << " ";
            }
            cur = cur->next;
        }
        cout << endl;
    }

    int getValue(int index) {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds" << endl;
            return 0;
        }
        Node *node = getIndex(index, false);
        return node ? node->data : 0;
    }

    void add(ArrayDoubleLinkedList &other) {
        assert(size == other.size);
        Node *cur = other.head;
        while (cur) {
            setValue(getValue(cur->index) + cur->data, cur->index);
            cur = cur->next;
        }
    }

    void addition(ArrayDoubleLinkedList *list) {
        Node *cur1 = head;
        Node *cur2 = list->head;

        while (cur2) {
            while (cur1 && cur1->index < cur2->index) {
                cur1 = cur1->next;
            }
            if (cur1 && cur1->index == cur2->index) {
                cur1->data += cur2->data;
                cur2 = cur2->next;
            } else {
                setValue(cur2->data, cur2->index);
                cur2 = cur2->next;
            }
        }
    }
};

class Nodem {
public:
    int index;
    Nodem *next;
    ArrayDoubleLinkedList* list;
    Nodem(int index, int cols) : index(index), next(nullptr), list(new ArrayDoubleLinkedList(cols)) {}
};

class SparceMatrix {
private:
    int rows;
    int cols;
    Nodem* head;

public:
    SparceMatrix(int rows, int cols) : rows(rows), cols(cols), head(nullptr) {}

    ~SparceMatrix() {
        Nodem* cur = head;
        while (cur) {
            delete cur->list;
            Nodem* nextNode = cur->next;
            delete cur;
            cur = nextNode;
        }
    }

    void setVal(int value, int row, int col) {
        if (row < 0 || col < 0 || row >= rows || col >= cols) {
            cout << "Index out of bounds" << endl;
            return;
        }

        Nodem *cur = head;
        Nodem *prev = nullptr;

        while (cur && cur->index < row) {
            prev = cur;
            cur = cur->next;
        }

        if (!cur || cur->index != row) {
            Nodem *newNode = new Nodem(row, cols);
            newNode->next = cur;
            if (prev) 
                prev->next = newNode;
            else 
                head = newNode;
            cur = newNode;
        }
        cur->list->setValue(value, col);
    }

    void print() {
        for (int i = 0; i < rows; i++) {
            Nodem *cur = head;
            while (cur && cur->index != i)
                cur = cur->next;
            
            if (cur) 
                cur->list->printArray();
            else {
                for (int j = 0; j < cols; j++) cout << "0 ";
                cout << endl;
            }
        }
    }

    void printWithoutZeros() {
        for (int i = 0; i < rows; i++) {
            Nodem *cur = head;
            while (cur && cur->index != i)
                cur = cur->next;
            
            if (cur) 
                cur->list->printWithoutZeros();
        }
    }

    void add(SparceMatrix &other) {
        assert(rows == other.rows && cols == other.cols);

        Nodem *cur1 = head;
        Nodem *cur2 = other.head;
        Nodem *prev = nullptr;

        while (cur2) {
            while (cur1 && cur1->index < cur2->index) {
                prev = cur1;
                cur1 = cur1->next;
            }
            if (cur1 && cur1->index == cur2->index) {
                cur1->list->addition(cur2->list);  // Adding the lists together
                cur2 = cur2->next;
            } else {
                Nodem *newNode = new Nodem(cur2->index, cols);
                newNode->list->addition(cur2->list);  // Initialize newNode with values from cur2
                newNode->next = cur1;
                if (prev) prev->next = newNode;
                else head = newNode;
                prev = newNode;
                cur2 = cur2->next;
            }
        }
    }
};

void test1() {
    ArrayDoubleLinkedList arr(10);
    arr.setValue(50, 5);
    arr.setValue(20, 2);
    arr.setValue(70, 7);
    arr.setValue(40, 4);

    ArrayDoubleLinkedList arr2(10);
    arr2.setValue(1, 4);
    arr2.setValue(3, 7);
    arr2.setValue(4, 6);

    cout << "Array : \n\n";
    arr.printArray();
    cout << "-----------------------------------------------------\n";
    cout << "Array without zeros : \n\n";
    arr.printWithoutZeros();
    cout << "-----------------------------------------------------\n";
    cout << "The value at index : 7 is : " << arr.getValue(7) << endl;
    cout << "-----------------------------------------------------\n";
    cout << "After adding the two arrays : \n\n";
    arr.add(arr2);
    arr.printArray();
    cout << "-----------------------------------------------------\n";
}

void test2() {
    SparceMatrix matrix(10, 10);
    matrix.setVal(5,3,5);
    matrix.setVal(7,3,7);
    matrix.setVal(2,3,2);
    matrix.setVal(0,3,2);
    matrix.setVal(6,5,6);
    matrix.setVal(4,5,4);
    matrix.setVal(3,7,3);
    matrix.setVal(1,7,1);

    cout << "Print Matrix : 10 X 10 \n\n";
    matrix.print();
    cout << "----------------------------------------\n";
    cout << "Print Matrix without zeros : 10 X 10 \n\n";
    matrix.printWithoutZeros();
    cout << "----------------------------------------\n";
    
    SparceMatrix matrix2(10, 10);
    matrix2.setVal(5,1,9);
    matrix2.setVal(6,3,8);
    matrix2.setVal(9,9,9);
    
    
    cout << "Print Matrix 2 : 10 X 10 \n\n";
    matrix2.print();
    cout << "----------------------------------------\n";
    cout << "Print Matrix 2 without zeros : 10 X 10 \n\n";
    matrix2.printWithoutZeros();
    cout << "----------------------------------------\n";
    
    cout << "After addition : \n\n";
    matrix.add(matrix2);
    matrix.printWithoutZeros();
    cout << "----------------------------------------\n";
}

int main() {
    //test1();
    test2();
    return 0;
}


#include <iostream>
using namespace std;

class Data{
public:
    int number{};
    char character{};

    Data() {
        number = 1 + rand() % 10000;
        character = 'A' + (rand() % 26);
    }

    Data(int num, char chara){
        number = num;
        character = chara;
    }

    void display() {
        cout
                << "Number: " << number << "    "
                << "Character: " << character << endl;
    }

    void update(Data* node){
        number = node->number;
        character = node->character;
    }
};

int compareData(Data* firstNode, Data* secondNode) {
    if(firstNode == nullptr){
        return false;
    }
    else {
        if(firstNode->number == secondNode->number){
            if(firstNode->character == secondNode->character){
                return 0;
            }
            return firstNode->character < secondNode->character ? -1 : 1;
        }
        return firstNode->number < secondNode->number ? -1 : 1;
    }
}

template <class T>
class DynamicArray {
public:
    int arraySize = {};
    T* arrayPointer = nullptr;
    float resizeMultiplier = {};
    int firstAvailableIndex = {};

    DynamicArray() {
        arraySize = 1;
        arrayPointer = new T[1];
        resizeMultiplier = 2.0;
        firstAvailableIndex = 0;
    }

    ~DynamicArray() {
        delete [] arrayPointer;
    }

    void resizeArray() {
        int arrayNewSize = arraySize * resizeMultiplier;
        T* tempPointer = new T[arrayNewSize];

        for (int i = 0; i < arraySize; ++i) {
            tempPointer[i] = arrayPointer[i];
        }

        delete arrayPointer;
        arraySize = arrayNewSize;
        arrayPointer = tempPointer;
    }

    void addNewNode(T* newNode) {
        if(firstAvailableIndex >= arraySize) { resizeArray(); }
        arrayPointer[firstAvailableIndex] = *newNode;
        firstAvailableIndex++;
    }

    void deleteNode(int index) {
        if (checkIndex(index) || !checkIfEmpty()) { return; }
        delete arrayPointer[index];
        arrayPointer[index] = nullptr;
        firstAvailableIndex--;
    }

    void swapNodes(int firstNodeIndex, int secondNodeIndex){
        swap(arrayPointer[firstNodeIndex], arrayPointer[secondNodeIndex]);
    }
    bool checkIndex(int index){
        return (index < 0 || index >= arraySize) ? true : false;
    }

    bool checkIfEmpty(){
        return (arrayPointer != nullptr || arrayPointer[0] != nullptr) ? true : false;
    }

    T* returnNode(int index) {
        return checkIndex(index) ? arrayPointer[index] : nullptr;
    }

    bool updateNode(int index, int newNumber, char newCharacter) {
        if(!checkIndex(index)) { return false; }
        arrayPointer[index].update(newNumber,newCharacter);
        return true;
    }

    void printArrayInfo() {
        if(arraySize == 0 || arrayPointer[0] == nullptr) {
            cout
                << "======================================" << endl
                << "Array is empty" << endl;
            return;
        }

        cout
            << "Index:\tNodes: " << endl
            << "======================================" << endl;

        for (size_t i = 0; i < firstAvailableIndex; ++i) {
            cout << i << "\t";
            arrayPointer[i]->display();
        }
    }

    void clearArray() {
        while(firstAvailableIndex != 0) {
            arrayPointer[ firstAvailableIndex - 1] = nullptr;
            firstAvailableIndex--;
        }
    }
};

template <class T>
class Heap{
public:
    class DynamicArray<T*>* array = {};

    Heap(){
        array = new DynamicArray<T*>;
    }

    ~Heap(){
        array->clearArray();
        delete array;
    }

    int getParentIndex(int index){
        return (index - 1) / 2;
    }

    int getLeftIndex(int index){
        return (2 * index) + 1;
    }

    int getRightIndex(int index){
        return (2 * index) + 2;
    }

    void swapNodes(int firstNodeIndex, int secondNodeIndex){
        array->swapNodes(firstNodeIndex, secondNodeIndex);
    }

    void heapifyUp(int index){
        if(index < array->firstAvailableIndex){
            int j = index;

            while(j > 0 && compareData(array->arrayPointer[j], array->arrayPointer[j/2]) == 1){
                swapNodes(j/2,j);
                j = j/2;
            }
            heapifyUp(++index);
        }
    }

    void heapifyDown(int index){
        int leftIndex = getLeftIndex(index);
        int rightIndex = getRightIndex(index);
        int largestChildIndex;
        int largestValueIndex = index;

        T* leftNode = array->arrayPointer[leftIndex];
        T* rightNode = array->arrayPointer[rightIndex];
        T* largestNode = array->arrayPointer[largestValueIndex];
        T* largestChild;

        if (leftIndex < array->firstAvailableIndex -1 && compareData(leftNode, rightNode) != -1){
            largestChild = leftNode;
            largestChildIndex = leftIndex;
        }
        else {
            largestChild = rightNode;
            largestChildIndex = rightIndex;
        }

        if (largestChildIndex < array->firstAvailableIndex && compareData(largestChild, largestNode) == 1) {
            largestValueIndex = largestChildIndex;
        }

        if (largestValueIndex != index) {
            swapNodes(index, largestValueIndex);
            heapifyDown(largestValueIndex);
        }
    }

    void insertNode(T* newNode){
        int newNodeIndex = array->firstAvailableIndex;
        array->addNewNode(&newNode);
        int newNodeParentIndex = getParentIndex(newNodeIndex);

        while (newNodeIndex != 0 ){
            if (compareData(newNode, array->arrayPointer[newNodeParentIndex]) != -1) {
                swapNodes(newNodeIndex, newNodeParentIndex);
                newNodeIndex = newNodeParentIndex;
                newNodeParentIndex = getParentIndex(newNodeIndex);
            }
            else {
                break;
            }
        }
    }

    T* popNode(T* poppedNode){
        int lastNodeIndex = array->firstAvailableIndex - 1;
        poppedNode->update(array->arrayPointer[0]);
        swapNodes(0,lastNodeIndex);
        array->deleteNode(lastNodeIndex);
        heapifyUp(0);
        return poppedNode;
    }

    void printHeap(){
        array->printArrayInfo();
    }

    void clearHeap(){
        array->clearArray();
    }
};


int main() {
    srand(time(nullptr));

    class Heap<Data>* heap = new Heap<Data>;

    for (int i = 0; i < 10; ++i) { // Adding new random elements
        class Data* newNode = new Data(i, 'a');
        heap->insertNode(newNode);
    }
    heap->printHeap();
    cout << "======================================" << endl;
    cout << "POP: ";
    Data* pop = new Data();
    heap->popNode(pop);
    pop->display();
    cout << "======================================" << endl;
    heap->printHeap();
    cout << "======================================" << endl;
    cout << "POP: ";
    heap->popNode(pop);
    pop->display();
    cout << "======================================" << endl;
    heap->printHeap();
    cout << "======================================" << endl;
    cout << "POP: ";
    heap->popNode(pop);
    pop->display();
    cout << "======================================" << endl;
    heap->printHeap();

    return 0;
}

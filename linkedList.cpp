#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

template<typename NODETYPE> class List;
template<typename NODETYPE>
class ListNode{
    friend class List<NODETYPE>;
public:
    explicit ListNode(const NODETYPE& inform)
        : data{inform},nextPtr{nullptr} {/*empty body*/}

    NODETYPE getData() const {return data;}
private:
    NODETYPE data;
    ListNode<NODETYPE>* nextPtr;
};

template<typename NODETYPE>
class List{
friend  ostream& operator<<(ostream& output,const List<NODETYPE>& list){
    if(list.isEmpty()){
        output<<"List is Empty"<<endl;
        return output;
    }

    ListNode<NODETYPE>* currentPtr{list.firstPtr};

    output<<"The list is : ";

    while(currentPtr!=nullptr){
        output<<currentPtr->data<<" ";
        currentPtr=currentPtr->nextPtr;
    }
    output<<endl<<endl;
    return output;
}
public:
    List() : firstPtr(nullptr), lastPtr(nullptr) {}

    List(size_t t)  : firstPtr(nullptr), lastPtr(nullptr) {
        for(size_t i=0;i<t;i++){
            push_back(0);
        }
    }

    List(const initializer_list<NODETYPE>& values) : firstPtr(nullptr), lastPtr(nullptr) {
        for (const auto& value : values) {
            push_back(value);
        }
    }

    NODETYPE& operator[](int index) {
        ListNode<NODETYPE>* currentPtr{firstPtr};
        int counter = 0;
        while (counter != index) {
            currentPtr = currentPtr->nextPtr;
            ++counter;
        }
        return currentPtr->data;
    }

    const NODETYPE& operator[](int index) const {
        ListNode<NODETYPE>* currentPtr{firstPtr};
        int counter = 0;
        while (counter != index) {
            currentPtr = currentPtr->nextPtr;
            ++counter;
        }
        return currentPtr->data;
    }

    ~List(){
        if(!isEmpty()){
            ListNode<NODETYPE>* currentPtr{firstPtr};
            ListNode<NODETYPE>* tempPtr{nullptr};
            while(currentPtr!=nullptr){
                tempPtr=currentPtr;
                currentPtr=currentPtr->nextPtr;
                delete tempPtr;
            }
        }
    }

    void push_front(const NODETYPE& value){
        ListNode<NODETYPE>* newPtr{getNewPtr(value)};
        if(isEmpty()){
            firstPtr=lastPtr=newPtr;
        }
        else{
            newPtr->nextPtr=firstPtr;
            firstPtr=newPtr;
        }
    }

    void push_back(const NODETYPE& value){
        ListNode<NODETYPE>* newPtr{getNewPtr(value)};
        if(isEmpty()){
            firstPtr=lastPtr=newPtr;
        }
        else{
            lastPtr->nextPtr=newPtr;
            lastPtr=newPtr;
        }
    }

    bool pop_front(){
        if(isEmpty()){
            return false;
        }
        else{
            ListNode<NODETYPE>* tempPtr{firstPtr};
            if(firstPtr==lastPtr){
                firstPtr=lastPtr=nullptr;
            }
            else{
                firstPtr=firstPtr->nextPtr;
            }
            delete tempPtr;
            return true;
        }
    }

    bool pop_back(){
        if(isEmpty()){
            return false;
        }
        else{
            ListNode<NODETYPE>* tempPtr{lastPtr};
            if(firstPtr==lastPtr){
                firstPtr=lastPtr=nullptr;
            }
            else{
                ListNode<NODETYPE>* currentPtr{firstPtr};
                while(currentPtr->nextPtr != lastPtr){
                    currentPtr=currentPtr->nextPtr;
                }
                lastPtr=currentPtr;
                currentPtr->nextPtr=nullptr;
            }
            delete tempPtr;
            return true;
        }
    }


    bool isEmpty() const{
        return firstPtr==nullptr;
    }

    int length(){
        int counter=0;
        ListNode<NODETYPE>* currentPtr{firstPtr};
        while(currentPtr!=nullptr){
            ++counter;
            currentPtr=currentPtr->nextPtr;
        }
        return counter;
    }

    void insert(int pos, const NODETYPE& DATA) {
        if (pos < 0 || pos > length()) {
            throw std::out_of_range("Invalid position");
        }
        ListNode<NODETYPE>* temp{ new ListNode<NODETYPE>(DATA) };
        if (firstPtr) {
            if (pos == 0) {
                temp->nextPtr = firstPtr;
                firstPtr = temp;
                if (!lastPtr) {
                    lastPtr = firstPtr;
                }
                return;
            }
            ListNode<NODETYPE>* currentPtr{ firstPtr };
            for (int i = 0; i < pos - 1; ++i) {
                currentPtr = currentPtr->nextPtr;
            }
            temp->nextPtr = currentPtr->nextPtr;
            currentPtr->nextPtr = temp;
            if (!temp->nextPtr) {
                lastPtr = temp;
            }
        } else {
            firstPtr = lastPtr = temp;
        }
    }

    void erase(int pos) {
        if (isEmpty() || pos < 0 || pos >= length()) {
            throw std::out_of_range("Invalid position");
        }
        if (pos == 0) {
            ListNode<NODETYPE>* tempPtr{ firstPtr };
            firstPtr = firstPtr->nextPtr;
            delete tempPtr;
            if (!firstPtr) {
                lastPtr = nullptr;
            }
            return;
        }
        ListNode<NODETYPE>* currentPtr{ firstPtr };
        for (int i = 0; i < pos - 1; ++i) {
            currentPtr = currentPtr->nextPtr;
        }
        ListNode<NODETYPE>* tempPtr{ currentPtr->nextPtr };
        currentPtr->nextPtr = tempPtr->nextPtr;
        delete tempPtr;
        if (!currentPtr->nextPtr) {
            lastPtr = currentPtr;
        }
    }

private:
    ListNode<NODETYPE>* firstPtr{nullptr};
    ListNode<NODETYPE>* lastPtr{nullptr};

    ListNode<NODETYPE>* getNewPtr(const NODETYPE& value){
        return new ListNode<NODETYPE>{value};
    }
};

template<typename NODETYPE>
class IndexedList : public List<NODETYPE> {
private:
    vector<ListNode<NODETYPE>*> index;
public:
    IndexedList() : List<NODETYPE>() {
        index.resize(26, nullptr);
    }

    void insertInIndexedList(int pos, const NODETYPE& DATA) {
        List<NODETYPE>::insert(pos, DATA);
        updateIndex(pos);
    }

    void deleteFromIndexedList(int pos) {
        List<NODETYPE>::erase(pos);
        updateIndex(pos);
    }

    bool searchIndexedList(char key) {
        int indexPos = getIndexPos(key);
        if (indexPos == -1) {
            return false;
        }
        int listPos = getListPos(indexPos);
        return searchList(listPos, key);
    }

private:
    void updateIndex(int pos) {
        if (pos < 0 || pos >= List<NODETYPE>::length()) {
            throw std::out_of_range("Invalid position");
        }
        index.assign(26, nullptr);
        ListNode<NODETYPE>* currentPtr = List<NODETYPE>::firstPtr;
        while (currentPtr != nullptr) {
            int indexPos = getIndexPos(currentPtr->data);
            if (index[indexPos] == nullptr) {
                index[indexPos] = currentPtr;
            }
            currentPtr = currentPtr->nextPtr;
        }
    }

    int getIndexPos(char key) {
        if (isalpha(key)) {
            return tolower(key) - 'a';
        }
        return -1;
    }

    int getListPos(int indexPos) {
        ListNode<NODETYPE>* node = index[indexPos];
        if (node == nullptr) {
            return -1;
        }
        ListNode<NODETYPE>* currentPtr = List<NODETYPE>::firstPtr;
        int pos = 0;
        while (currentPtr != node) {
            currentPtr = currentPtr->nextPtr;
            ++pos;
        }
        return pos;
    }

    bool searchList(int start, char key) {
        ListNode<NODETYPE>* currentPtr = List<NODETYPE>::firstPtr;
        for (int i = 0; i < start; ++i) {
            currentPtr = currentPtr->nextPtr;
        }
        while (currentPtr != nullptr) {
            if (currentPtr->data == key) {
                return true;
            }
            currentPtr = currentPtr->nextPtr;
        }
        return false;
    }
};

int main(){
    IndexedList<char> nameList;
    nameList.insertInIndexedList(0, 'c');
    nameList.insertInIndexedList(1, 'a');
    nameList.insertInIndexedList(2, 't');
    nameList.insertInIndexedList(3, 's');
    nameList.insertInIndexedList(4, 'b');
    nameList.insertInIndexedList(5, 'd');
    nameList.insertInIndexedList(6, 'e');
    nameList.insertInIndexedList(7, 'z');
    nameList.insertInIndexedList(8, 'y');
    nameList.insertInIndexedList(9, 'm');

    cout << "Searching for 'b': " << (nameList.searchIndexedList('b') ? "Found" : "Not Found") << endl;
    cout << "Searching for 'x': " << (nameList.searchIndexedList('x') ? "Found" : "Not Found") << endl;

    return 0;
}

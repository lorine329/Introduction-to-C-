#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <sstream>

using namespace std;

// hash_it function from assignment 1
std::size_t hash_it (std::string someString) {
    std::size_t str1 = std::hash<std::string>()(someString);
    return str1;
}

// class Node
class Node {
    public:
        int ver_id;
        string content; 
        size_t hashValue;
        Node* prev;
        Node* next;

        Node() { // default value
            ver_id = 0;
            content = "";
            hashValue = hash_it(content);
            //prev = NULL;
            next = NULL;
        }

        Node(int version, string contents) {
            ver_id = version;
            content = contents;
            hashValue = hash_it(contents);
            //prev = NULL;
            next = NULL;
        }

        // ~Node() {
		//     delete next;
		//     //delete prev;
	    // }   
};

// Initial global variables
int ver_id = 1;
int num_ver = 0;

Node* head = NULL;
Node* tail = NULL;

// press 'a'
void add(string content) {
    Node* newNode = new Node(ver_id, content);
    if (tail == NULL) {
        head = newNode;
        tail = newNode;

        cout<<"Your content has been added successfully."<<endl;
                
        num_ver += 1;
        ver_id += 1;
    } else {
        int exist = 1;
        Node* currNode = head;
        while (currNode != NULL){
            if (newNode->hashValue == currNode->hashValue) {
                exist = 0;
            }
            currNode = currNode->next;
        }
        if (exist == 1) {
            tail->next = newNode;
            //newNode->prev = tail;
            tail = newNode;
                    
            num_ver += 1;
            ver_id += 1;
                    
            cout<<"Your content has been added successfully."<<endl;

        } else {
            cout << "git322 did not detect any change to your file and will not create a new version." << endl;
        }           
    }
}

// press 'p'
void print(void) {
    cout << "Number of versions: " << num_ver << endl;
    Node* currNode = head;
    while (currNode != NULL) {
        cout << "Version number: " << currNode->ver_id << endl;
        cout << "Hash value: " << currNode->hashValue << endl;
        cout << "Content: " << currNode->content << "\n" <<endl;
        currNode = currNode->next;
    }
}

// press 'l'
void load(int version) {
    int exist = 0;
    string tar_content = "";
    Node* currNode = head;
    while (currNode != NULL) {
        if (version == currNode->ver_id) {
            tar_content = currNode->content;
            exist =1;
            break;
        }
        currNode = currNode->next;
    }
    // check the validity of the input
    if (exist == 0) {
        cout<<"Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers." << endl;
        return;
    }
    fstream file;
    file.open("file.txt", ios::out | ios::trunc | ios::in);
    file << tar_content;
    file.close();
    cout << "Version "<< version<< " loaded successfully. Please refresh your text editor to see the changes." << endl;
    
}

// Helper recursive function that is used to compare lines
void compare_lines(string list1[], string list2[], int index1, int index2, int length1, int length2) {
        // If file2 has more or equal lines than file1, print the extra lines of file2
        if (index1 == length1){
            if (index2 == length2){
                return;
            } else {
                cout << "Line " << index2+1 << ": " << "<Empty line>";
                cout << " <<>> " << list2[index2] << endl;
                //only update the index of file2
                compare_lines(list1, list2, index1, index2+1, length1, length2);
            }
        // If file1 has more or equal lines than file2, print the extra lines of file1
        } else if (index2 == length2) {
            if (index1 == length1){
                return;
            } else {
                cout << "Line " << index1+1 << ": " << list1[index1];
                cout << " <<>> " << "<Empty line>" << endl;
                //only update the index of file1
                compare_lines(list1, list2, index1+1, index2, length1, length2);
            }
        // General case, comparing lines
        } else {
            std::size_t h1 = hash_it (list1[index1]);
            std::size_t h2 = hash_it (list2[index2]);

            if (h1 != h2) {
                cout << "Line " << index1+1 << ": " << list1[index1];
                cout << " <<>> " << list2[index2] << endl;
            } else {
                cout << "Line " << index1+1 << ": <Identical>" << endl;
            }
        
            compare_lines(list1, list2, index1+1, index2+1, length1, length2);
        }
}

// press 'c'
void compare(int version1, int version2){
    // get the content of two versions
    string content1;
    Node* currNode1 = head;
    int exist1 = 0;
    while(currNode1 != NULL){
        if (version1 == currNode1->ver_id) {
            content1 = currNode1->content;
            exist1 =1;
            break;
        }
        currNode1 = currNode1->next;
    }

    string content2;
    Node* currNode2 = head;
    int exist2 = 0;
    while(currNode2 != NULL) {
        if (version2 == currNode2->ver_id) {
            content2 = currNode2->content;
            exist2 = 1;
            break;
        }
        currNode2 = currNode2->next;
    }

    // separate the contents by \n
    // first to check whether the input version is valid or not
    if (exist1 == 0 || exist2 == 0) {
        cout<<"Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers." << endl;
        return;
    }
    stringstream ss1(content1);
    string ver1_lines[1000];
    int i = 0;
    int linecount1 = 0;
    while(getline(ss1,ver1_lines[i],'\n')){
        //cout<<lines[i]<<endl;
        i++;
        linecount1++;
    }

    stringstream ss2(content2);
    string ver2_lines[1000];
    int j = 0;
    int linecount2 = 0;
    while(getline(ss2,ver2_lines[j],'\n')){
        //cout<<lines[i]<<endl;
        j++;
        linecount2++;
    }
    compare_lines(ver1_lines, ver2_lines, 0, 0, linecount1, linecount2);
}

// press 's'
void search(string keyword){
    Node *currNode = head;
    int count = 0;
    int exist = 0;
    while (currNode != NULL) {
        string contents = currNode->content;
        if (contents.find(keyword) != std::string::npos) {
            if (count == 0) {
                cout << "The keyword \'" << keyword<< "\' has been found in the following versions:"<<endl;
            }
            cout << "Version number: " << currNode->ver_id << endl;
            cout << "Hash value: " << currNode->hashValue << endl;
            cout << "Content: " << currNode->content << "\n" <<endl;

            count++;
            exist = 1;
        }
        currNode = currNode->next;
    }
    if (exist == 0) { // check the validity of the input
        cout << "Your keyword \'"<< keyword<<"\' was not found in any version."<<endl;
    } 
}

// press 'r'
void remove(int version) {
    int exist = 0;
    Node* tmp = head;
    Node* prev = NULL;
    while (tmp != NULL){
        if (tmp->ver_id == version){
            exist = 1;
            if (prev == NULL){ // if removed node is head
                head = tmp->next;
            } else if (tmp == tail) { // if removed node is tail
                prev->next = tmp->next;
                tail = prev;
            } else {
                prev->next = tmp->next;
            }
            delete (tmp);
           
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    if (exist == 0) { // check the validity of the input
        cout << "Please enter a valid version number." << endl;
    } else {
        cout << "Version " << version<< " deleted successfully ."<<endl;
        num_ver = num_ver-1;
    }
}

// delete all nodes
void deleteList() {
    if (head == NULL) {
        return;
    }
    Node *tmp = head;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        tmp->next = NULL;
        delete (tmp);
    }
}

void git322(){
    string response = "";

    cout << "Welcome to the Comp322 file versioning system!\n" << endl;
    cout << "To add the content of your file to version control press \'a\'" << endl;
    cout << "To remove a version press \'r\'" << endl;
    cout << "To load a version press \'l\'" << endl;
    cout << "To print to the screen the detailed list of all versions press \'p\'" << endl;
    cout << "To compare any 2 versions press \'c\'" << endl;
    cout << "To search versions for a keyword press \'s\'" << endl;
    cout << "To exit press \'e\'" << endl;

    while(response.compare("e") != 0){
        cin >> response;
        
        // select options
        if (response.compare("a") == 0){
            ifstream file("file.txt");
            string contents;
            stringstream buffer;
            buffer << file.rdbuf();
            contents = buffer.str();
            file.close();

            add(contents);

        } else if (response.compare("p") == 0){

            print();

        } else if (response.compare("l") == 0){
            cout << "Which version would you like to load?" << endl;
            int version;
            cin >> version;
            load(version);

        } else if (response.compare("c") == 0){
            cout << "Please enter the number of the first version to compare:" << endl;
            int version1;
            cin >> version1;
            
            cout << "Please enter the number of the second version to compare:" << endl;
            int version2;
            cin >> version2;

            compare(version1, version2);

        } else if (response.compare("s") == 0){
            cout<<"Please enter the keyword that you are looking for:"<<endl;
            string keyword;
            cin>>keyword;
            search(keyword);
        
        } else if (response.compare("r") == 0){
            cout<<"Enter the number of the version that you want to delete:"<<endl;
            int version;
            cin>>version;
            remove(version);
        
        } else if (response.compare("e") == 0){
            cout<<"Exit successfully!"<<endl;
            deleteList();
        
        } else {

            cout<<"Please enter a valid value."<<endl;

        }
    }
}

int main(){
    git322();
    // add("HEllO");
    // add("hello world");
    // add("COMP 322");
    // print();
    // remove(2);
    // print();

    return 0;
}


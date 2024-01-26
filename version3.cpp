#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <sstream>

using namespace std;


// Part 1

// Define the structure of Node globally
class Node {
    public:
        int ver_id1;
        string content;
        size_t hashValue;
        Node* next;

        // hash_it function from assignment 1
        std::size_t hash_it (std::string someString) {
            std::size_t str1 = std::hash<std::string>()(someString);
            return str1;
        }

        Node() { // default value
            ver_id1 = 0;
            content = "";
            hashValue = hash_it(content);
            next = NULL;
        }

        Node(int version, string contents) {
            ver_id1 = version;
            content = contents;
            hashValue = hash_it(contents);
            next = NULL;
        }

        ~Node() {
            cout << "Node ~dtor" << endl;
        } 
};

class LinkedList {
    private:

        std::size_t hash_it (std::string someString) {
            std::size_t str1 = std::hash<std::string>()(someString);
            return str1;
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

    public:
        Node* head;
        Node* tail;
        int ver_id;
        int num_ver;

        LinkedList() { // default value
            head = NULL;
            tail = NULL;
            ver_id = 1;
            num_ver = 0;
        }

        ~LinkedList() { // destructor
            cout << "LinkedList ~dtor"<<endl;
        }

        void add(string content) { // add a Node to the LinkedList

            Node* newNode = new Node(ver_id, content);
            if (tail == NULL) { // if the LinkedList is empty
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
                        break;
                    }
                    currNode = currNode->next;
                }
                if (exist == 1) {
                    tail->next = newNode;
                    tail = newNode;
                            
                    num_ver += 1;
                    ver_id += 1;
                            
                    cout<<"Your content has been added successfully."<<endl;

                } else {
                    cout << "git322 did not detect any change to your file and will not create a new version." << endl;
                }           
            }
        }

        void print(void) { // print all the nodes of the linkedlist
            cout << "Number of versions: " << num_ver << endl;
            Node* currNode = head;
            while (currNode != NULL) { // print from head to tail
                cout << "Version number: " << currNode->ver_id1 << endl;
                cout << "Hash value: " << currNode->hashValue << endl;
                cout << "Content: " << currNode->content << "\n" <<endl;
                currNode = currNode->next;
            }
        }

        void load(int version) { // load the selected version to the file
            int exist = 0; // check if the version of currNode is the selected version
            string tar_content = "";
            Node* currNode = head;
            while (currNode != NULL) {
                if (version == currNode->ver_id1) {
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

        void compare(int version1, int version2){ // compare contents from two versions
            // get the content of two versions
            string content1;
            Node* currNode1 = head;
            int exist1 = 0;
            while(currNode1 != NULL){
                if (version1 == currNode1->ver_id1) {
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
                if (version2 == currNode2->ver_id1) {
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
                    cout << "Version number: " << currNode->ver_id1 << endl;
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

        void remove(int version) {
            int exist = 0;
            Node* tmp = head;
            Node* prev = NULL;
            while (tmp != NULL){
                if (tmp->ver_id1 == version){
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
};

class Git322 {
    protected:
        LinkedList mylist;
    public:
        Git322() { // default value
            cout << "Welcome to the Comp322 file versioning system!\n" << endl;
            cout << "To add the content of your file to version control press \'a\'" << endl;
            cout << "To remove a version press \'r\'" << endl;
            cout << "To load a version press \'l\'" << endl;
            cout << "To print to the screen the detailed list of all versions press \'p\'" << endl;
            cout << "To compare any 2 versions press \'c\'" << endl;
            cout << "To search versions for a keyword press \'s\'" << endl;
            cout << "To exit press \'e\'" << endl;
        }

        virtual ~Git322(){}

        void add(){
            //string fileName = "file.txt";
            ifstream file("file.txt");
            string contents;
            stringstream buffer;
            buffer << file.rdbuf();
            contents = buffer.str();
            file.close();
            mylist.add(contents);
        }

        void print() {
            
            mylist.print();
        }

        void load(){
            cout << "Which version would you like to load?" << endl;
            int version;
            cin >> version;
            mylist.load(version);
        }

        void compare(){
            cout << "Please enter the number of the first version to compare:" << endl;
            int version1;
            cin >> version1;
            
            cout << "Please enter the number of the second version to compare:" << endl;
            int version2;
            cin >> version2;
            mylist.compare(version1, version2);
        }

        void search() {
            cout<<"Please enter the keyword that you are looking for:"<<endl;
            string keyword;
            cin>>keyword;
            mylist.search(keyword);
        }

        void remove() {
            cout<<"Enter the number of the version that you want to delete:"<<endl;
            int version;
            cin>>version;
            mylist.remove(version);
        }

        void deleteList() {
            cout << "Exit sucessfully!" << endl;
            mylist.deleteList();
        }

        
};

// Part 2
class EnhancedGit322: public Git322 {

    public:
        virtual ~EnhancedGit322(){}

        void saveVersions() {
            // Create a file
            if (mylist.head == NULL){
                return;
            }
            ofstream savedVer("memory.txt");

            // Write to the file
            if (savedVer.is_open()) {
                Node *currNode = mylist.head;
                int ver = mylist.head->ver_id1;
                while (currNode != NULL) {
                    savedVer << "/////" << ver << endl;
                    savedVer << currNode->content << endl;
                    if (currNode->next != NULL){
                        ver = currNode->next->ver_id1;
                    }
                    currNode = currNode->next;
                }
                delete currNode;
            } else {
                cout <<"File opening is fail." << endl;  
            }

            // Close a file
            cout << "Saved all versions successfully!"<<endl;
            savedVer.close();
            
        }

        void loadVersions(){
            //string fileName = "memory.txt";
            ifstream file("memory.txt");
            if (!file){
                cout<<"There's no previous versions."<<endl;
                return;
            }
            string allContents;
            stringstream buffer;
            buffer << file.rdbuf();
            allContents = buffer.str();
            //cout<<allContents<<endl;

            stringstream ss(allContents);
            //string ver_num[100];
            string line;
            string content = "";
            string target = "/////";
            string ver_s = "0";
            int ver = 0;
            while(getline(ss,line)){
                //cout<<lines[i]<<endl;
                if (line.find(target)!= std::string::npos){
                    if (ver != 0){
                        mylist.ver_id = ver;
                        content = content.erase(content.size()-1);
                        mylist.add(content);
                        //cout<<"s1"<<endl;
                    }
                    ver_s = line.back();
                    ver = stoi(ver_s);
                    content = "";
                } else {
                    content = content + line + "\n";
                }
            }
            if (ver != 0){
               mylist.ver_id = ver;
               content = content.erase(content.size()-1);
               mylist.add(content);
            }
            

            file.close();
            //mylist.add(contents);
        }

};




int main() {

    string response = "";
    EnhancedGit322 git322;
    git322.loadVersions();
    while(response.compare("e") != 0){
        cin >> response;
        
        // select options
        if (response.compare("a") == 0){
            git322.add();

        } else if (response.compare("p") == 0){

            git322.print();

        } else if (response.compare("l") == 0){
            git322.load();

        } else if (response.compare("c") == 0){
            git322.compare();

        } else if (response.compare("s") == 0){
            git322.search();
        
        } else if (response.compare("r") == 0){
            git322.remove();
        
        } else if (response.compare("e") == 0){
            git322.saveVersions();
            git322.deleteList();
            //git322.print();
        
        } else {
            cout<<"Please enter a valid value."<<endl;

        }
    }

}



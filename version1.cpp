#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <sstream>

using namespace std;

/* Question 1 */
bool word_diff(std::string word1, std::string word2) {
    if (word1.compare(word2) == 0)
        return true;
    else
        return false;
}

/* Question 2 */
bool classical_file_diff(std::string file1, std::string file2) {
    // Read the files
    std::ifstream f1; 
    f1.open(file1);
    std::ifstream f2; 
    f2.open(file2);

    string f1str, f2str;
    // Compare the content of two files
    while ((getline(f1, f1str)) && (getline(f2, f2str))) {
        if (!word_diff(f1str, f2str)) {
            f1.close();
            f2.close();
            return false;
        }
    }
    f1.close();
    f2.close();
    return true;
}

/* Question 3 */
std::size_t hash_it (std::string someString) {
    std::size_t str1 = std::hash<std::string>()(someString);
    return str1;
}

/* Question 4 */
bool enhanced_file_diff(std::string file1, std::string file2) {
    std::ifstream f1; 
    f1.open(file1);
    std::ifstream f2; 
    f2.open(file2);

    string f1str, f2str;
    // Compare the content of two files
    while ((getline(f1, f1str)) && (getline(f2, f2str))) {
        std::size_t h1 = hash_it (f1str);
        std::size_t h2 = hash_it (f2str);
        if (h1 != h2) {
            f1.close();
            f2.close();
            return false;
        }
    }
    f1.close();
    f2.close();
    return true;
}

/* Question 5 */
// Helper recursive function that is used to compare lines
void compare_lines(std::string file1, std::string file2, std::string list1[], std::string list2[], int index1, int index2, int length1, int length2) {
        // If file2 has more or equal lines than file1, print the extra lines of file2
        if (index1 == length1){
            if (index2 == length2){
                return;
            } else {
                cout << file1 << ": " << "" << endl;
                cout << file2 << ": " << list2[index2] << endl;
                //only update the index of file2
                compare_lines(file1, file2, list1, list2, index1, index2+1, length1, length2);
            }
        // If file1 has more or equal lines than file2, print the extra lines of file1
        } else if (index2 == length2) {
            if (index1 == length1){
                return;
            } else {
                cout << file1 << ": " << list1[index1] << endl;
                cout << file2 << ": " << "" << endl;
                //only update the index of file1
                compare_lines(file1, file2, list1, list2, index1+1, index2, length1, length2);
            }
        // General case, comparing lines
        } else {
            std::size_t h1 = hash_it (list1[index1]);
            std::size_t h2 = hash_it (list2[index2]);

            if (h1 != h2) {
                cout << file1 << ": " << list1[index1] << endl;
                cout << file2 << ": " << list2[index2] << endl;
            }
        
            compare_lines(file1, file2, list1, list2, index1+1, index2+1, length1, length2);
        }
}

void list_mismatched_lines(std::string file1, std::string file2) {
    std::ifstream f1; 
    f1.open(file1);
    std::ifstream f2; 
    f2.open(file2);

    const int size = 1000;
    string f1text[size];
    string f2text[size];

    // Put file1 and file2 into an array since it is iterable
    int i=0;
    int linecount1 = 0;
    while(getline(f1, f1text[i])){
        //std::cout << f1text[i] << endl;
        linecount1++;
        i++;
    }

    int j=0;
    int linecount2 = 0;
    while(getline(f2, f2text[j])){
        //std::cout << f2text[j] << endl;
        linecount2++;
        j++;
    }
    string filename1 = file1.substr(file1.find_last_of("/\\") + 1);
    string filename2 = file2.substr(file2.find_last_of("/\\") + 1);
    compare_lines(filename1, filename2, f1text, f2text, 0, 0, linecount1, linecount2);
    
    f1.close();
    f2.close();

}

/* Question 6 */
// Helper recursive function that is used to compare words from lines
void compare_words_from_line(std::string file1, std::string file2, std::string list1[], std::string list2[], int index1, int index2, int length1, int length2, int line1, int line2) {
        // If file2 has more or equal words than file1, print the extra words of file2
        if (index1 == length1){
            if (index2 == length2){
                return;
            } else {
                cout << file1 << ": " << "" << " (line " << line1 << ")" << endl;
                cout << file2 << ": " << list2[index2] << " (line " << line2 << ")" << endl;
                
                compare_words_from_line(file1, file2, list1, list2, index1, index2+1, length1, length2, line1, line2);
            }
        // If file1 has more or equal words than file2, print the extra words of file1
        } else if (index2 == length2) {
            if (index1 == length1){
                return;
            } else {
                cout << file1 << ": " << list1[index1] << " (line " << (line1) << ")" << endl;
                cout << file2 << ": " << "" << " (line " << (line2) << ")" << endl;
                compare_words_from_line(file1, file2, list1, list2, index1+1, index2, length1, length2, line1, line2);
            }
        // General case, comparing words
        } else {
            std::size_t h1 = hash_it (list1[index1]);
            std::size_t h2 = hash_it (list2[index2]);

            if (h1 != h2) {
                cout << file1 << ": " << list1[index1] << " (line " << (line1) << ")" << endl;
                cout << file2 << ": " << list2[index2] << " (line " << line2 << ")" << endl;
            }
        
            compare_words_from_line(file1, file2, list1, list2, index1+1, index2+1, length1, length2, line1, line2);
        }
}
// Helper recursive function that is used to compare lines
void compare_words(std::string file1, std::string file2, std::string list1[], std::string list2[], int index1, int index2, int length1, int length2, int line1, int line2) {
        // If file2 has more or equal lines than file1, print the extra lines of file2
        if (index1 == length1){
            if (index2 == length2){
                return;
            } else {
                istringstream ss(list2[index2]);
                string word;
                while (ss >> word){
                    cout << file1 << ": " << "" << " (line " << line1 << ")" << endl;
                    cout << file2 << ": " << word << " (line " << line2 << ")" << endl;
                }
                compare_words(file1, file2, list1, list2, index1, index2+1, length1, length2, line1+1, line2+1);
            }
        // If file1 has more or equal lines than file2, print the extra lines of file1
        } else if (index2 == length2) {
            if (index1 == length1){
                return;
            } else {
                istringstream ss(list1[index1]);
                string word;
                while (ss >> word){
                    cout << file1 << ": " << word << " (line " << (line1) << ")" << endl;
                    cout << file2 << ": " << "" << " (line " << (line2) << ")" << endl;
                }
                compare_words(file1, file2, list1, list2, index1+1, index2, length1, length2, line1+1, line2+1);
            }
        // General case
        } else {
            std::size_t h1 = hash_it (list1[index1]);
            std::size_t h2 = hash_it (list2[index2]);

            // If the two sentences are not the same, we would compare each words
            if (h1 != h2) {
                istringstream ss1(list1[index1]);
                istringstream ss2(list2[index2]);
                string word1[100];
                string word2[100];
                // Put sentence1 and sentence2 into array to make it iterable
                int i = 0;
                int wordcount1 = 0;
                while (ss1 >> word1[i]) {
                    i++;
                    wordcount1++;
                }

                int j = 0;
                int wordcount2 = 0;
                while (ss2 >> word2[j]){
                    j++;
                    wordcount2++;
                }
                // call the helper function to compare words
                compare_words_from_line(file1, file2, word1, word2, 0, 0, wordcount1, wordcount2, line1, line2);
            }
        
            compare_words(file1, file2, list1, list2, index1+1, index2+1, length1, length2, line1+1, line2+1);
        }
}
void list_mismatched_words(std::string file1, std::string file2) {
    std::ifstream f1; 
    f1.open(file1);
    std::ifstream f2; 
    f2.open(file2);

    const int size = 2000;
    string f1text[size];
    string f2text[size];
    // Put file1 and file2 into an array since it is iterable
    int i=0;
    int linecount1 = 0;
    while (getline(f1, f1text[i])) {
        linecount1++;
        i++;
    }

    int j=0;
    int linecount2 = 0;
    while (getline(f2, f2text[j])) {
        linecount2++;
        j++;
    }
    string filename1 = file1.substr(file1.find_last_of("/\\") + 1);
    string filename2 = file2.substr(file2.find_last_of("/\\") + 1);
    compare_words(filename1, filename2, f1text, f2text, 0, 0, linecount1, linecount2, 1, 1);
    f1.close();
    f2.close();
}


int main(){
    // My tester
    // Q1
    // std::string str1 = "Hello World";
    // std::string str2 = "hEllO World";
    // std::string str3 = "World";
    // std::string str4 = "Hello World";

    // bool result1 = word_diff(str1, str2); // False
    // bool result2 = word_diff(str1, str3); // False
    // bool result3 = word_diff(str1, str4); // True

    // std::cout << std::boolalpha << result1 << std::endl;
    // std::cout << std::boolalpha << result2 << std::endl;
    // std::cout << std::boolalpha << result3 << std::endl;

    // // Q2
    // std::string file1 = "./txt_folder/file1.txt";
    // std::string file2 = "./txt_folder/file2.txt";
    // bool result4 = classical_file_diff(file1, file2);
    // std::cout << std::boolalpha << result4 << std::endl;

    // // Q3
    // std::string mystr = "I love this assignment";
    // std::size_t h1 = hash_it (mystr);
    // std::cout << h1 << std::endl;

    // // Q4
    // bool result = enhanced_file_diff(file1, file2); // False
    // std::cout << std::boolalpha << result << std::endl;

    // // Q5
    // list_mismatched_lines(file1, file2);

    // // Q6
    // list_mismatched_words(file1, file2);
    

    // Q1
    std::string str1 = "Hello World";
    std::string str2 = "hEllO World";
    std::string str3 = "World";
    std::string str4 = "Hello World";
    bool result = word_diff(str1, str2); // False
    result = word_diff(str1, str3); // False
    result = word_diff(str1, str4); // True
    // Q2
    std::string file1 = "./txt_folder/file1.txt";
    std::string file2 = "./txt_folder/file2.txt";
    result = classical_file_diff(file1, file2); // False
    // Q3
    std::string mystr = "I love this assignment";
    std::size_t h1 = hash_it (mystr);
    std::cout << h1 << std::endl;
    // Q4
    result = enhanced_file_diff(file1, file2); // False
    // Q5
    list_mismatched_lines(file1, file2); // This should print to the screen the mismatched lines
    // Q6
    list_mismatched_words(file1, file2); // This should print to the screen the mismatched words
}
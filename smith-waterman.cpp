/*
Score values are as follows:
Match: +1       (can be seen at line 123)
Mismatch: -5    (can be seen at line 125)
Gap: -10        (can be seen at line 114)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>	// std::sort

// This class represents the relation between two words and includes a matrix which is filled according to smith-waterman algorithm
class Pair {
public:
    Pair(std::string, std::string);
    ~Pair();
    void fill_matrix();
    int match_score(unsigned, unsigned);
    std::string traceback();   //returns the common strings as embedded inside the wanted format.
private:
    int** matrix;
    const std::string word_1;
    const std::string word_2;
};

//returns the maximum of three integers or if they are all negative, return 0
int max_of_3(int, int, int);

int main(int argc, char* argv[])
{
    //The file where all the words to be processed are obtained is opened here.
    std::string file_name = argv[1];
    std::ifstream strings_file(file_name);
    if (!strings_file) {
        std::cerr << "File cannot be opened!";
        exit(1);
    }

    std::vector<std::string> strings;   //Holds all the words, its vector::size() function is useful.
    std::string buffer;
    while (!strings_file.eof()) {
        getline(strings_file, buffer);
        strings.push_back(buffer);
    }
    std::sort(strings.begin(), strings.end());  //Strings file is sorted alphabetically because this helps in acquiring the wanted output format.
    
    std::vector<Pair*> all_pairs;   //All distinct word Pairs will be pushed into this vector. Memory Allocation for the Pairs will be dynamic, so this is a vector of pointers.
    
    for (unsigned i = 0; i < strings.size(); i++) {
        for (unsigned j = i + 1; j < strings.size(); j++) {
            Pair* temp_pair = new Pair(strings[i], strings[j]); //constructor deals with the creation of matrix.
            all_pairs.push_back(temp_pair);
        }
    }

    //Output file is opened here.
    std::string file_name_out = argv[2];
    std::ofstream out_file(file_name_out);
    if (!strings_file) {
        std::cerr << "File cannot be opened!";
        exit(1);
    }

    // Pair::traceback() is run for all pairs and their return values are written to output file.
    for (unsigned i = 0; i < all_pairs.size(); i++) {
        out_file << all_pairs[i]->traceback();
    }

    //Dynamically allocated memory is deallocated.
    for (auto p : all_pairs)
    {
        delete p;
    }
}

// Members are initialized, matrix is created dynamically. After the creation of matrix, Pair::fill_matrix() is called.
Pair::Pair(std::string word_1_arg, std::string word_2_arg)
    :word_1(word_1_arg), word_2(word_2_arg)
{
    unsigned word_1_size = word_1.size();   //written in order to remove warning C6386 (Visual Studio)
    matrix = new int*[word_1_size + 1];
    for (unsigned i = 0; i < word_1_size + 1; i++) {
        matrix[i] = new int[word_2.size() + 1];
    }

    this->fill_matrix();
}

// Deallocation of matrix's memory takes place here.
Pair::~Pair()
{
    for (unsigned i = 0; i < word_1.size() + 1; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// matrix is filled according to the algorithm.
void Pair::fill_matrix()
{
    // First row and first column is filled with 0's. Because of this, memory allocation was 1 bigger than word sizes (see Pair::Pair's memory allocation part).
    for (unsigned i = 0; i < word_1.size() + 1; i++) {
        matrix[i][0] = 0;
    }
    for (unsigned i = 0; i < word_2.size() + 1; i++) {
        matrix[0][i] = 0;
    }
    for (unsigned i = 1; i < word_1.size() + 1; i++) {
        for (unsigned j = 1; j < word_2.size() + 1; j++) {
            // Line below is the essential part of Smith-Waterman algorithm. Dynamic programming takes place here. Maximum of these three choices is selected and written to the new matrix cell.
            matrix[i][j] = max_of_3(matrix[i - 1][j - 1] + match_score(i - 1, j - 1), matrix[i - 1][j] - 10, matrix[i][j - 1] - 10);  //  -10 is the score that I've decided for matches
        }
    }
}

// Checks for the current matrix cell that if corresponding letters of each word match. In case of a match, a positive value of choice is returned; if there is no match, a negative value of choice is returned.
int Pair::match_score(unsigned i, unsigned j)
{
    if (word_1[i] == word_2[j]) {
        return 1;   // 1 is the score that I've decided for matches
    }
    return -5;  // -5 is the score that I've decided for mismatches
}

// Traceback, which is acquiring the common or similar strings using the matrix values, takes place here. Common substrings of both words are embedded inside a string (which also caters for the wanted output format) which is then returned.
std::string Pair::traceback()
{
    // Maximum value is found and its position is stored inside two vectors for i and j values. If multiple max values exists, all of their positions can be stored this way.
    int max = 0;
    std::vector<unsigned> all_i;
    std::vector<unsigned> all_j;
    for (unsigned i = 1; i < word_1.size() + 1; i++) {
        for(unsigned j = 1; j < word_2.size() + 1; j++) {
            if (matrix[i][j] > max) {
                all_i.clear();
                all_j.clear();
                all_i.push_back(i);
                all_j.push_back(j);
                max = matrix[i][j];
            }
            else if (matrix[i][j] == max) {
                all_i.push_back(i);
                all_j.push_back(j);
            }
        }
    }

    // This block repeats for every max value that exists inside the matrix. substrings are recreated here, and stored to "sequences".
    std::set<std::string> sequences; //A set is very useful here because: 1) They hold sorted values    2) They don't hold duplicates
    for (unsigned i = 0; i < all_i.size(); i++) {
        unsigned local_i = all_i[i], local_j = all_j[i];
        std::string temp;
        while (matrix[local_i][local_j] != 0) { // While didn't traceback'ed to a 0
            if (word_1[local_i - 1] == word_2[local_j - 1]) {   // (-1) is used while indexing because matrix's first line and column are filled with 0's and are independent from the words.
                temp.insert(0, 1, word_1[local_i - 1]);     // If the corresponding letters to that cell are the same, insert it at the front of the temp string, which then will be pushed to sequences set.
                // Go to diagonal.
                local_i--;
                local_j--;
            }
            else {
                // If there isn't a letter match on this cell, pick the max value from these neighbours: upper-left diagonal, left or upper
                temp.insert(0, 1, '?'); // This represents a mismatch when printed.
                if ((matrix[local_i - 1][local_j - 1] > matrix[local_i - 1][local_j]) && (matrix[local_i - 1][local_j - 1] > matrix[local_i][local_j - 1])) {
                    // Go to diagonal.
                    local_i--;
                    local_j--;
                }
                else if (matrix[local_i - 1][local_j] > matrix[local_i][local_j - 1]) {
                    // Go up a line.
                    local_i--;
                }
                else {
                    // Go left a column.
                    local_j--;
                }
            }
        }
        sequences.insert(temp);
    }
    
    std::string return_value;   // This will be returned.

    // The wanted output format can be seen here. They are all appended to return_value as if they were getting printed out.
    return_value.append(word_1 + " - " + word_2 + "\n");
    return_value.append("Score: " + std::to_string(max) + " Sequence(s):");
    if (max != 0) {
        for (auto i = sequences.begin(); i != sequences.end(); i++) {
            return_value.append(" \"" + *i + "\"");
        }
    }
    return_value.append("\n");

    return return_value;
}

//if the maximum of them is negative, 0 is returned
int max_of_3(int num1, int num2, int num3)
{
    if (num1 > num2) {
        if (num1 > num3) {
            return (num1 > 0) ? num1 : 0;
        }
        return (num3 > 0) ? num3 : 0;
    }
    if (num2 > num3) {
        return(num2 > 0) ? num2 : 0;
    }
    return (num3 > 0) ? num3 : 0;
}

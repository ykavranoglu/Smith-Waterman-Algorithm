# Smith-Waterman Algorithm
Smith-Waterman Algorithm's implementation in C++

"The Smith–Waterman algorithm performs local sequence alignment; that is, for determining similar regions between two strings of nucleic acid sequences or protein sequences. Instead of looking at the entire sequence, the Smith–Waterman algorithm compares segments of all possible lengths and optimizes the similarity measure."
Wikipedia contributors. (2021, May 16). Smith–Waterman algorithm. In Wikipedia, The Free Encyclopedia. Retrieved 16:35, May 22, 2021, from https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm

Here, given examples are English words, instead of nucleic acid sequences, which are checked against other English words.

The program takes a txt file as input and calculates the similarity score for each uniqe word pair. Then writes the sequence which gets the highest similarity score for each pair to the output txt file. Given input.txt and output.txt files should be examined. An object oriented approach is used where each pair is an object of the class "Pair".

The program should take two command arguments, first one is the name of the input file and second one is the name of the output file. Ex: "input.txt output.txt"

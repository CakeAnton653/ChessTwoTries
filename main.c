#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CVECTOR_LOGARITHMIC_GROWTH

#include "cvector.h"

const char* convertToNotationCode(int pos1, int pos2, char character);
int** getBoardOfMatches(int row, int column);

void checkPawnMoves(int** board);
void checkKingMoves(int** board);
void checkBishopMoves(int** board);

cvector_vector_type(char*) solutionsVector = NULL;

int main()
{
    int** boardOfMatches = getBoardOfMatches(8, 8);
    // 2r5/5K1P/6Pp/3R2b1/2P2k2/2p5/8/8
    // For each element in the FEN code, there can only be a max size of 8 in a line.
    // Therefore in eight columns, there are eight rows, so 8 * 8 characters max.
    // Oh yeah and the additional 7 /'s so plus 7 I guess.
    // Terminate to 0 (null) in case the user doesn't provide any input.
    char fen[(8 * 8) + 7] = {0};
    scanf("%s", fen);

    // https://linux.die.net/man/3/strtok
    // https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    const char s[2] = "/";
    char *token;
    token = strtok(fen, s);

    // The row keeps track of which row we're at. This helps with navigating in the multidimensional array.
    int row = 0;

    // 2r5/5K1P/6Pp/3R2b1/2P2k2/2p5/8/8
    // This value sets how much we need to jump in the current row. What this means, is that if we are in row 0, and we need to go to
    // the 5th element, (since everything previusly was empty (example: 5r2)).
    int jumpSize = 0;

    /* walk through other tokens */
    while(token != NULL) {

        // Since we are using an int based multidimensional array.
        // We need to explicitly specify which characters hold which int value, and for which side. White or black.
        // Here is the full reference table for readers:
        // For white:
        // 2 - rook - R
        // 3 - knight - N
        // 4 - bishop - B
        // 5 - queen - Q
        // 6 - king - K
        // 7 - pawn - P
        // For black:
        // 8 - rook - r
        // 9 - knight - n
        // 10 - bishop - b
        // 11 - queen - q
        // 12 - king - k
        // 13 - pawn - p
        // 0 is the initial value. It also represents a square that is empty, so no characters are standing on it.
        // 1 is reserved for something
        // Remember uppercase are white, lowercase are black.

        // The token length here is the length of the specified line.
        // So for as an example, 2r5/5K1P/6Pp/3R2b1/2P2k2/2p5/8/8” -> 2r5 is the token, it's length is 2.
        // We iterate thru, and set the values in the boardOfMatches accordingly.
        // Remember uppercase are white, lowercase are black.
        int tokenLength = strlen(token); // 3
        for (int i = 0; i < tokenLength; i++) {

            // We increment by that much
            if (isdigit(token[i])) {
                jumpSize += token[i] - '0';
            }
            if (token[i] == 'r') {
                boardOfMatches[row][jumpSize] = 8;
                jumpSize += 1;
            } else if (token[i] == 'R') {
                boardOfMatches[row][jumpSize] = 2;
                jumpSize += 1;
            } else if (token[i] == 'n') {
                boardOfMatches[row][jumpSize] = 9;
                jumpSize += 1;
            } else if (token[i] == 'N') {
                boardOfMatches[row][jumpSize] = 3;
                jumpSize += 1;
            } else if (token[i] == 'b') {
                boardOfMatches[row][jumpSize] = 10;
                jumpSize += 1;
            } else if (token[i] == 'B') {
                boardOfMatches[row][jumpSize] = 4;
                jumpSize += 1;
            } else if (token[i] == 'q') {
                boardOfMatches[row][jumpSize] = 11;
                jumpSize += 1;
            } else if (token[i] == 'Q') {
                boardOfMatches[row][jumpSize] = 5;
                jumpSize += 1;
            } else if (token[i] == 'k') {
                boardOfMatches[row][jumpSize] = 12;
                jumpSize += 1;
            } else if (token[i] == 'K') {
                boardOfMatches[row][jumpSize] = 6;
                jumpSize += 1;
            } else if (token[i] == 'p') {
                boardOfMatches[row][jumpSize] = 13;
                jumpSize += 1;
            } else if (token[i] == 'P') {
                boardOfMatches[row][jumpSize] = 7;
                jumpSize += 1;
            }
        }

        jumpSize = 0;
        row++;
        token = strtok(NULL, s);
   }

    printf("Solutions: \n");
    checkPawnMoves(boardOfMatches);
    checkKingMoves(boardOfMatches);

    for(int i=0; i<8; i++) {
        for(int j=0;j<8;j++) {
            printf("%d ", boardOfMatches[i][j]);
        if(j==7){
            printf("\n");
         }
      }
   }

   printf("Solutions: \n");
	if (solutionsVector) {
		size_t i;
		for (i = 0; i < cvector_size(solutionsVector); ++i) {
			printf("v[%llui] = %s\n", i, solutionsVector[i]);
		}
	}

    cvector_free(solutionsVector);
    // TODO: Free everything here.
    return 0;

}
// TODO: Instead of iterating thru all elements for each character, go thru only once, and provide functions the table, and initial functions.

void checkBishopMoves(int** board) {

    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            if (board[i][k] == 4) {
                for (int j = 0; j < 7 - k; j++) {
                    if (board[i + 1][k + 1] == 0) {
                        cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k + 1, 'B'));
                    } else if (board[i + 1][k + 1] > 7) {
                        // TODO: Add + sign to notation code
                        cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k + 1, 'B'));
                        break;
                    } else if (board[i + 1][k + 1] < 7) {
                        cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k + 1, 'B'));
                        break;
                    }

                    if (board[i + 1][k - 1] == 0) {
                        cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k - 1, 'B'));
                        break;
                    } else if (board[i + 1][k + 1] > 7) {
                        cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k + 1, 'B'));
                        break;
                    } else if (board[i - 1][k + 1] < 7) {
                        cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k + 1, 'B'));
                        break;
                    } else if (board[i - 1][k - 1] < 7) {
                        cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k - 1, 'B'));
                        break;
                    }
                }
            }
        }
    }
}

void checkKingMoves(int** board) {
  for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            if (board[i][k] > 1 && board[i][k] < 8) {
                if (board[i][k] == 6) {
                        if ((k > 1 && k < 7) && (i > 0 && i < 7)) {

                            if (board[i - 1][k] == 0 || board[i - 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k, 'K'));
                            }

                            if (board[i - 1][k - 1] == 0 || board[i - 1][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k - 1, 'K'));
                            }

                            if (board[i - 1][k + 1] == 0 || board[i - 1][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k + 1, 'K'));
                            }

                            if (board[i][k + 1] == 0 || board[i][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i, k + 1, 'K'));
                            }

                            if (board[i][k - 1] == 0 || board[i][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i, k - 1, 'K'));
                            }

                            if (board[i + 1][k] == 0 || board[i + 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k, 'K'));
                            }

                            if (board[i + 1][k + 1] == 0 || board[i + 1][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k + 1, 'K'));
                            }

                            if (board[i + 1][k - 1] == 0 || board[i - 1][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k - 1, 'K'));
                            }

                        } else if (k == 0 && i == 0) {

                            if (board[i + 1][k + 1] == 0 || board[i + 1][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k + 1, 'K'));
                            }

                            if (board[i][k + 1] == 0 || board[i][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i, k + 1, 'K'));
                            }

                            if (board[i + 1][k] == 0 || board[i + 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k, 'K'));
                            }


                        } else if (k == 0 && i == 7) {

                            if (board[i - 1][k + 1] == 0 || board[i - 1][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k + 1, 'K'));
                            }

                            if (board[i][k + 1] == 0 || board[i][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i, k + 1, 'K'));
                            }

                            if (board[i - 1][k] == 0 || board[i - 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k, 'K'));
                            }


                        } else if (k == 7 && i == 0) {

                            if (board[i + 1][k - 1] == 0 || board[i + 1][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k - 1, 'K'));
                            }

                            if (board[i][k - 1] == 0 || board[i][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i, k - 1, 'K'));
                            }

                            if (board[i + 1][k] == 0 || board[i + 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k, 'K'));
                            }

                        } else if (i == 7 && k == 7) {

                            if (board[i - 1][k - 1] == 0 || board[i - 1][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k - 1, 'K'));
                            }

                            if (board[i][k - 1] == 0 || board[i][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i, k - 1, 'K'));
                            }

                            if (board[i - 1][k] == 0 || board[i + 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k, 'K'));
                            }

                        } else if (k == 0) {
                            if (board[i + 1][k] == 0 || board[i + 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k, 'K'));
                            }

                            if (board[i + 1][k + 1] == 0 || board[i + 1][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k + 1, 'K'));
                            }

                            if (board[i][k + 1] == 0 || board[i][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i, k + 1, 'K'));
                            }

                            if (board[i - 1][k] == 0 || board[i - 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k, 'K'));
                            }

                            if (board[i + 1][k - 1] == 0 || board[i + 1][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k - 1, 'K'));
                            }

                        } else if (k == 7) {
                            if (board[i - 1][k] == 0 || board[i - 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k, 'K'));
                            }

                            if (board[i - 1][k - 1] == 0 || board[i - 1][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k - 1, 'K'));
                            }

                            if (board[i][k - 1] == 0 || board[i][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i, k - 1, 'K'));
                            }

                            if (board[i + 1][k] == 0 || board[i + 1][k] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k, 'K'));
                            }

                            if (board[i + 1][k - 1] == 0 || board[i + 1][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i + 1, k - 1, 'K'));
                            }
                        }
                }

            }
        }
    }
}

void checkPawnMoves(int** board) {
  for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            // If we are bigger than 1, that means that there is a piece there.
            // We are only interested in white pieces
            if (board[i][k] > 1 && board[i][k] < 8) {
                if (board[i][k] == 7) {
                    // On the last line we promote, so don't run the checking logic
                    if (i > 0) {
                        // We have to be on the right spot to be able to check both sides.
                        if (k > 1 && k < 7) {

                            // Move is possible on the right!
                            if (board[i - 1][k + 1] == 0 || board[i - 1][k + 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k + 1, 'P'));
                            }

                            // And left!
                            if (board[i - 1][k - 1] == 0 || board[i - 1][k - 1] > 7) {
                                cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k - 1, 'P'));
                            }
                        // Here we can only check the right side.
                        } else if (k == 0) {
                            if (board[i - 1][k + 1] == 0 || board[i - 1][k + 1] > 7) {
                               cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k + 1, 'P'));
                            }
                        // Here we can only check the left side.
                        } else if (k == 7) {
                            if (board[i - 1][k - 1] == 0 || board[i - 1][k - 1] > 7) {
                              cvector_push_back(solutionsVector, convertToNotationCode(i - 1, k - 1, 'P'));
                            }
                        }
                    }
                }

            }
        }
    }
}

int** getBoardOfMatches(int row, int column) {
    int **temp;
    int j;

    temp = (int **)calloc(row, sizeof(int *));
    temp[0] = (int *)calloc(row * column, sizeof(int));

    for(j = 1; j < row; j++) {
        temp[j] = temp[j-1]+column;
    }

    return temp;
}

// Converts two array indexes and a given char into an algebrathic notation code
// For as an example, 2, 5 P ->
const char* convertToNotationCode(int pos1, int pos2, char character) {
    char pos2char;
    char numberAsChar;

    switch (pos2) {
        case 0:
            pos2char = 'a';
            break;
        case 1:
            pos2char = 'b';
            break;
        case 2:
            pos2char = 'c';
            break;
        case 3:
            pos2char = 'd';
            break;
        case 4:
            pos2char = 'e';
            break;
        case 5:
            pos2char = 'f';
            break;
        case 6:
            pos2char = 'g';
            break;
        case 7:
            pos2char = 'h';
            break;
    };

    switch(pos1) {
    case 0:
        numberAsChar = '8';
        break;
    case 1:
        numberAsChar = '7';
        break;
    case 2:
        numberAsChar = '6';
        break;
    case 3:
        numberAsChar = '5';
        break;
    case 4:
        numberAsChar = '4';
        break;
    case 5:
        numberAsChar = '3';
        break;
    case 6:
        numberAsChar = '2';
        break;
    case 7:
        numberAsChar = '1';
        break;

    };

    char *solution;

    solution = (char *)calloc(4, sizeof(char));

    sprintf(solution, "%c%c%c", character, pos2char, numberAsChar);

    return solution;
}



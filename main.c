#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CVECTOR_LOGARITHMIC_GROWTH

#include "cvector.h"

const char* convertToNotationCode(int pos1, int pos2, char character);
int** getBoardOfMatches(int row, int column);

int canMove(int cord1, int cord2, int** board);

void checkUp(int cord1, int cord2, int** board, char character);
void checkUpLeft(int cord1, int cord2, int** board, char character);
void checkUpRight(int cord1, int cord2, int** board, char character);

void checkLeft(int cord1, int cord2, int** board, char character);
void checkRight(int cord1, int cord2, int** board, char character);

void checkBelow(int cord1, int cord2, int** board, char character);
void checkBelowLeft(int cord1, int cord2, int** board, char character);
void checkBelowRight(int cord1, int cord2, int** board, char character);

int checkEdgeCase(int cord1, int cord2);

void checkPawnMoves(int** board);
void checkKingMoves(int i, int k, int** board);
void checkBishopMoves(int i, int k, int** board);

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

    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            switch (boardOfMatches[i][k]) {
                case 7:
                    // checkPawnMoves(boardOfMatches);
                    break;
                case 6:
                    checkKingMoves(i, k, boardOfMatches);
                    break;
                case 5:
                    break;
                case 4:
                    checkBishopMoves(i, k, boardOfMatches);
                    break;
            case 3:
                break;
            case 2:
                break;
            }
        }
    }

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


/**
 * Evaluates wether we are in an edge case. And edge case is when a character is next to an edge, or in a corner.
 * 0 - We are not in an edge case.
 * 1 - We are in the left side upper corner (0,0)
 * 2 - We are in the left side lower corner (7,0)
 * 3 - We are in the right side upper corner (0,7)
 * 4 - We are in the right side lower corner (7, 7)
 * 5 - We are facing the edge on the left side (column = 0)
 * 6 - We are facing the edge on the right side (column = 7)
 * 7 - We are facing the edge on the upper side (row = 0)
 * 8 - We are facing the edge on the lower side (row = 7)
 * 9 - We are out of bounds, and should absolutely not be checking stuff anymore.
 * @param cord1 Cord 1 is the row. 0 - 7
 * @param cord2 Cord 2 is the column 0 - 7
 * @return If we are in an edge case, and if yes which
 */
int checkEdgeCase(int cord1, int cord2) {

    if ((cord1 > 0 && cord1 < 7) && (cord2 > 1 && cord2 < 7)) {
        return 0;
    } else if (cord1 == 0 && cord2 == 0) {
        return 1;
    } else if (cord1 == 7 && cord2 == 0) {
        return 2;
    } else if (cord1 == 0 && cord2 == 7) {
        return 3;
    } else if (cord1 == 7 && cord2 == 7) {
        return 4;
    } else if (cord2 == 0) {
        return 5;
    } else if (cord2 == 7) {
        return 6;
    } else if (cord1 == 0) {
        return 7;
    } else if (cord1 == 7) {
        return 8;
    } else if ((cord1 > 7 || cord1 < 0) || (cord2 < 0 || cord2 > 7)) {
        return 9;
    }

}

/**
 * Checks if the given square can be moved to.
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @return 0 if we can't move there, due to either one of our characters occupying the space or it not being free.
 * 1 if any of the above are false.
 */
int canMove(int cord1, int cord2, int** board) {
    return board[cord1][cord2] == 0 || board[cord1][cord2] > 7;
}


/**
 * Checks the value of the square above of the current character.
 * x,y,x
 * x,x,x
 * x,x,x
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @param The character to use for the notation code.
 * @return If it's true, so the square is free, and there isn't a character of our own is standing there, it is a move, so we add it to the vector.
 */
void checkUp(int cord1, int cord2, int** board, char character) {
    if (canMove(cord1 - 1, cord2, board)) {
        cvector_push_back(solutionsVector, convertToNotationCode(cord1 - 1, cord2, character));
    }
}

/**
 * Checks the value of the square right above us.
 * x,x,y
 * x,x,x
 * x,x,x
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @param The character to use for the notation code.
 * @return If it's true, so the square is free, and there isn't a character of our own is standing there, it is a move, so we add it to the vector.
 */
void checkUpRight(int cord1, int cord2, int** board, char character) {
    if (canMove(cord1 - 1, cord2 + 1, board)) {
        cvector_push_back(solutionsVector, convertToNotationCode(cord1 - 1, cord2 + 1, character));
    }
}

/**
 * Checks the value of the square left above us.
 * y,x,x
 * x,x,x
 * x,x,x
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @param The character to use for the notation code.
 * @return If it's true, so the square is free, and there isn't a character of our own is standing there, it is a move, so we add it to the vector.
 */
void checkUpLeft(int cord1, int cord2, int** board, char character) {
    if (canMove(cord1 - 1, cord2 - 1, board)) {
        cvector_push_back(solutionsVector, convertToNotationCode(cord1 - 1, cord2 - 1, character));
    }
}

/**
 * Checks the value of the square to the left of us.
 * x,x,x
 * y,x,x
 * x,x,x
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @param The character to use for the notation code.
 * @return If it's true, so the square is free, and there isn't a character of our own is standing there, it is a move, so we add it to the vector.
 */
void checkLeft(int cord1, int cord2, int** board, char character) {
    if (canMove(cord1, cord2 - 1, board)) {
        cvector_push_back(solutionsVector, convertToNotationCode(cord1, cord2 - 1, character));
    }
}

/**
 * Checks the value of the square to the right of us.
 * x,x,x
 * x,x,y
 * x,x,x
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @param The character to use for the notation code.
 * @return If it's true, so the square is free, and there isn't a character of our own is standing there, it is a move, so we add it to the vector.
 */
void checkRight(int cord1, int cord2, int** board, char character) {
    if (canMove(cord1, cord2 + 1, board)) {
        cvector_push_back(solutionsVector, convertToNotationCode(cord1, cord2 + 1, character));
    }
}

/**
 * Checks the value of the square below us.
 * x,x,x
 * x,x,x
 * x,y,x
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @param The character to use for the notation code.
 * @return If it's true, so the square is free, and there isn't a character of our own is standing there, it is a move, so we add it to the vector.
 */
void checkBelow(int cord1, int cord2, int** board, char character) {
    if (canMove(cord1 + 1, cord2, board)) {
        cvector_push_back(solutionsVector, convertToNotationCode(cord1 + 1, cord2, character));
    }
}

/**
 * Checks the value of the square left below us.
 * x,x,x
 * x,x,x
 * y,x,x
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @param The character to use for the notation code.
 * @return If it's true, so the square is free, and there isn't a character of our own is standing there, it is a move, so we add it to the vector.
 */
void checkBelowLeft(int cord1, int cord2, int** board, char character) {
    if (canMove(cord1 + 1, cord2 - 1, board)) {
        cvector_push_back(solutionsVector, convertToNotationCode(cord1 + 1, cord2 - 1, character));
    }
}

/**
 * Checks the value of the square right below us.
 * x,x,x
 * x,x,x
 * y,x,x
 * @param The current row coordinate.
 * @param The current column coordinate.
 * @param The 2d multidimensional array to preform the check on.
 * @param The character to use for the notation code.
 * @return If it's true, so the square is free, and there isn't a character of our own is standing there, it is a move, so we add it to the vector.
 */
void checkBelowRight(int cord1, int cord2, int** board, char character) {
    if (canMove(cord1 + 1, cord2 + 1, board)) {
        cvector_push_back(solutionsVector, convertToNotationCode(cord1 + 1, cord2 + 1, character));
    }
}

/**
 * Checks for all the possible moves a Bishop can make.
 * @param The 2d multidimensional array to check the moves for.
 * @return Doesn't return, however adds the possible moves to the vector.
 */
void checkBishopMoves(int i, int k, int** board) {
    char bishopIdentifier = 'B';
    int edgeCase = checkEdgeCase(i, k);
    int internalEdgeCase = checkEdgeCase(i, k);

    switch (edgeCase) {
        case 0:
            // Since there are no edge cases, we can preform a check on each side, and go as long as we don't encounter any issues.
            for (int z = 0; z < 7; z++) {
                if (internalEdgeCase > 8) {
                    break;
                }
                checkUpRight(i + z, k + z, board, bishopIdentifier);
                internalEdgeCase = checkEdgeCase(i + z, k + z);
            }

            // Reset so we can check the other angles.
            internalEdgeCase = checkEdgeCase(i, k);

            for (int z = 0; z < 7; z++) {
                if (internalEdgeCase > 8) {
                    break;
                }
                checkUpLeft(i + z, k + z, board, bishopIdentifier);
                internalEdgeCase = checkEdgeCase(i + z, k + z);
            }

            internalEdgeCase = checkEdgeCase(i, k);

            for (int z = 0; z < 7; z++) {
                if (internalEdgeCase > 8) {
                    break;
                }
                checkBelowLeft(i - z, k - z, board, bishopIdentifier);
                internalEdgeCase = checkEdgeCase(i - z, k - z);
            }

            internalEdgeCase = checkEdgeCase(i, k);

            for (int z = 0; z < 7; z++) {
                if (internalEdgeCase > 8) {
                    break;
                }
                checkBelowRight(i - z, k - z, board, bishopIdentifier);
                internalEdgeCase = checkEdgeCase(i - z, k - z);
            }

            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;

    }
}

/**
 * Checks for all the possible moves a King can make.
 * The king is special, since if in mate, it is the only character that has moves.
 * @param The 2d multidimensinal array to check the moves for.
 * @return Doesn't return, however adds the possible moves to the vector.
 */
void checkKingMoves(int i, int k, int** board) {
    char kingIdentifier = 'K';
    int edgeCase = checkEdgeCase(i, k);

    switch (edgeCase) {
        case 0:
            checkUp(i, k, board, kingIdentifier);
            checkUpLeft(i, k, board, kingIdentifier);
            checkUpRight(i, k, board, kingIdentifier);

            checkLeft(i, k, board, kingIdentifier);
            checkRight(i, k, board, kingIdentifier);

            checkBelow(i, k, board, kingIdentifier);
            checkBelowLeft(i, k, board, kingIdentifier);
            checkBelowRight(i, k, board, kingIdentifier);
            break;
        case 1:
            checkRight(i, k, board, kingIdentifier);
            checkBelowRight(i, k, board, kingIdentifier);
            checkBelow(i, k, board, kingIdentifier);
            break;
        case 2:
            checkUp(i, k, board, kingIdentifier);
            checkUpRight(i, k, board, kingIdentifier);
            checkRight(i, k, board, kingIdentifier);
            break;
        case 3:
            checkBelow(i, k, board, kingIdentifier);
            checkLeft(i, k, board, kingIdentifier);
            checkBelowLeft(i, k, board, kingIdentifier);
            break;
        case 4:
            checkUp(i, k, board, kingIdentifier);
            checkLeft(i, k, board, kingIdentifier);
            checkUpLeft(i, k, board, kingIdentifier);
            break;
        case 5:
            checkUp(i, k, board, kingIdentifier);
            checkRight(i, k, board, kingIdentifier);
            checkUpRight(i, k, board, kingIdentifier);
            checkBelow(i, k, board, kingIdentifier);
            checkBelowRight(i, k, board, kingIdentifier);
            break;
        case 6:
            checkUp(i, k, board, kingIdentifier);
            checkLeft(i, k, board, kingIdentifier);
            checkUpLeft(i, k, board, kingIdentifier);
            checkBelow(i, k, board, kingIdentifier);
            checkBelowLeft(i, k, board, kingIdentifier);
            break;
        case 7:
            checkBelow(i, k, board, kingIdentifier);
            checkLeft(i, k, board, kingIdentifier);
            checkRight(i, k, board, kingIdentifier);
            checkBelowLeft(i, k, board, kingIdentifier);
            checkBelowRight(i, k, board, kingIdentifier);
            break;
        case 8:
            checkUp(i, k, board, kingIdentifier);
            checkLeft(i, k, board, kingIdentifier);
            checkRight(i, k, board, kingIdentifier);
            checkUpLeft(i, k, board, kingIdentifier);
            checkUpRight(i, k, board, kingIdentifier);
            break;
    }
}

/**
 * Checks for all the possible moves a Pawn can make.
 * @param The 2d multidimensinal array to check the moves for.
 * @return Doesn't return, however adds the possible moves to the vector.
 */
void checkPawnMoves(int** board) {
        int i = 0;
        int k = 0;
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

/**
 * Creates, and allocates a 2d multidimensional array achieved thru the use of pointers.
 * @param The amount of rows.
 * @param The amount of columns.
 * @return The pointer for the 2d multidimensional array.
 */
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

/**
 * Converts the given parameters to an algebratic notation code.
 * @param The current row.
 * @param The current column.
 * @param The character to construct the notation code for.
 * @return A char* containing the algebratic notation code.
 */
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



// The following code executes the game 2048. It includes functions to move the cells around that work on the basis of checking if there ae empty cells and if merging is allowed on the basis of the
// rules of the game and also update the score of the game. There is also a function which checks if there are any more legal moves left and functions to create and remake games.

#include "game.h"


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));   
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    (*mygame).rows = rows;     //initializing the attributes for the struct varibale game
    (*mygame).cols = cols;
    (*mygame).score = 0;
    for (int i = 0; i < rows; i++){     //looping through all cells to initialize all cells as empty cells(-1)
        for (int j = 0; j < cols; j++){
            *((*mygame).cells + i * cols + j) = -1;        //derefrencing the cell and initialising with -1
        }
    }
    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
    (*(* _cur_game_ptr)).rows = new_rows;     //initializing the attributes for the struct varibale game
    (*(* _cur_game_ptr)).cols = new_cols;
    (*(*_cur_game_ptr)).score = 0;
    for (int i = 0; i < new_rows; i++){     //looping through all cells to initialize all cells as empty cells(-1)
        for (int j = 0; j < new_cols; j++){
           *( (*(*_cur_game_ptr)).cells + (i * new_cols) + j) = -1;        //derefrencing the cell and initialising with -1
        }
    }
	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    if (((row < (*cur_game).rows) && (col < (*cur_game).cols))  && ((row >= 0) && (col >= 0))){      //checking if the row and column given as arguments are within the bounds of row and column for the game
        cell * cell_ptr = (*cur_game).cells + (row * (*cur_game).cols) + col; //creating a pointer to point to the cell location specified by row and columns
        return cell_ptr;
    }

    return NULL;
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    //YOUR CODE STARTS HERE
    int move_flag = 0; //initialising a flag to indicate if some sort of change occurs on the board
    int (array[((*cur_game).rows)*((*cur_game).cols)]); //defining an array that stores -1 at an index value to denote that cell at that index in the game has already been merged with
    for (int row = 0; row < (*cur_game).rows; row++){  //looping through all the rows in the array
        for (int column = 0; column < (*cur_game).cols; column++){ //looping through all the columns in the array
          if (*get_cell(cur_game, row, column) != -1) { //checking if the cell at the location is an empty cell
            int i = 0;
            for (i = 0; i < row; i++){
                if (*get_cell(cur_game, i, column) == -1){ //trying to find an empty cell in the same column
                            *get_cell(cur_game,i,column) = *get_cell(cur_game,row,column); //moving cell down
                            *get_cell(cur_game, row, column) = -1; //changing the value of the moved cell to -1 (empty cell)
                            move_flag = 1; //updating move flag
                            break;
                }
            }
            if (i > 0) {
                if (*get_cell(cur_game, i - 1, column) == *get_cell(cur_game, i, column) && (array[(i-1)*(*cur_game).cols + column] != -1)){//checking for merging conditions
                    *get_cell(cur_game, i - 1, column) *= 2; //if merging is possible then we merge
                    (*cur_game).score +=  *get_cell(cur_game, i-1, column);
                    *get_cell(cur_game, i, column) = -1;
                    move_flag = 1; //updating move_flag
                    array[(i-1)*(*cur_game).cols + column] = -1;
                }
            }
            
          }
        }
    }
     
    if (move_flag == 1){ //checking if movement has occured and returning value on the basis of that
        return 1;
    }
    return 0;
};

int move_s(game * cur_game) //slide down
{
    //YOUR CODE STARTS HERE
    int move_flag = 0;
    int (array[((*cur_game).rows)*((*cur_game).cols)]); //defining an array that stores -1 at an index value to denote that cell at that index in the game has already been merged with
    for (int row = 0; row < (*cur_game).rows; row++){
        for (int col = 0; col < (*cur_game).cols; col++){
            array[row * (*cur_game).cols + col] = 0;
        }
    }
    for (int row = (*cur_game).rows - 1; row >= 0; row--){  //looping through all the rows in the array starting from the finish to allow priority for the later cells 
        for (int column = 0; column < (*cur_game).cols; column++){ //looping through all the columns in the array
          if (*get_cell(cur_game, row, column) != -1) { //checking if the cell at the location is an empty cell
            int i;
            for (i = (*cur_game).rows - 1; i > row; i--){ //looking for an empty cell from the end
                if (*get_cell(cur_game, i, column) == -1){
                            *get_cell(cur_game,i,column) = *get_cell(cur_game,row,column); //filling empty cell with appropriate value
                            *get_cell(cur_game, row, column) = -1;
                            move_flag = 1;
                            break;
                }
            }
            if (i < (*cur_game).rows - 1) {
                if (*get_cell(cur_game, i + 1, column) == *get_cell(cur_game, i, column) && (array[(i+1)*(*cur_game).cols + column] != -1)){ //merging similar to move_w
                    *get_cell(cur_game, i + 1, column) *= 2;
                    (*cur_game).score +=  *get_cell(cur_game, i+1, column);
                    *get_cell(cur_game, i, column) = -1;
                    move_flag = 1;
                    array[(i+1)*(*cur_game).cols + column] = -1;
                }
            }
            
          }
        }
    }
     
    if (move_flag == 1){ //checking for changes to board
        return 1;//checking for merging conditions
    }
    return 0;
};


int move_d(game * cur_game){ //slide to the right
    //YOUR CODE STARTS HERE
    int move_flag = 0;
     int (array[((*cur_game).rows)*((*cur_game).cols)]); //defining an array that stores -1 at an index value to denote that cell at that index in the game has already been merged with
    for (int row = 0; row < (*cur_game).rows; row++){
        for (int col = 0; col < (*cur_game).cols; col++){
            array[row * (*cur_game).cols + col] = 0;
        }
    }
    for (int column = (*cur_game).cols - 1; column >= 0; column--){  //looping through all the columns in the array
        for (int row = 0; row < (*cur_game).rows; row++){ //looping through all the rows in the array
          if (*get_cell(cur_game, row, column) != -1) { //checking if the cell at the location is an empty cell
            int i;
            for (i = (*cur_game).cols - 1; i > column; i--){
                if (*get_cell(cur_game, row, i) == -1){
                            *get_cell(cur_game,row, i) = *get_cell(cur_game,row,column);//if we find an empty cell then we move the cell to the empty cell
                            *get_cell(cur_game, row, column) = -1;
                            move_flag = 1; //updating flag
                            break;
                }
            }
            if (i < (*cur_game).cols - 1) {
                if (*get_cell(cur_game, row, i + 1) == *get_cell(cur_game, row, i) && (array[(row)*(*cur_game).cols + i+1] != -1)){ //merging 
                    *get_cell(cur_game, row, i+1) += *get_cell(cur_game, row, i);
                    (*cur_game).score +=  *get_cell(cur_game, row, i+1);
                    *get_cell(cur_game, row, i) = -1;
                    move_flag = 1;
                    array[row*(*cur_game).cols + i+1] = -1;
                }
            }
            
          }
        }
    }
     
    if (move_flag == 1){ //checking for changes
        return 1;
    }
    return 0;
};

int move_a(game * cur_game) //slide left
{
    //YOUR CODE STARTS HERE
    int move_flag = 0;
    int (array[((*cur_game).rows)*((*cur_game).cols)]); //defining an array that stores -1 at an index value to denote that cell at that index in the game has already been merged with
    for (int row = 0; row < (*cur_game).rows; row++){
        for (int col = 0; col < (*cur_game).cols; col++){
            array[row * (*cur_game).cols + col] = 0;
        }
    }
    for (int column = 0; column < (*cur_game).cols; column++){  //looping through all the columns in the array
        for (int row = 0; row < (*cur_game).rows; row++){ //looping through all the rows in the array
          if (*get_cell(cur_game, row, column) != -1) { //checking if the cell at the location is an empty cell
            int i = 0;
            for (i = 0; i < column; i++){
                if (*get_cell(cur_game, row, i) == -1){
                            *get_cell(cur_game,row,i) = *get_cell(cur_game,row,column); //filling empty cell with current cell
                            *get_cell(cur_game, row, column) = -1;
                            move_flag = 1; //updating move flag
                            break;
                }
            }
            if (i > 0) {
                if (*get_cell(cur_game, row, i - 1) == *get_cell(cur_game,row, i) && (array[(row)*(*cur_game).cols + i-1] != -1)){ //merging
                    *get_cell(cur_game, row, i - 1) *= 2;
                    (*cur_game).score +=  *get_cell(cur_game, row, i-1);
                    *get_cell(cur_game, row, i) = -1;
                    move_flag = 1;
                    array[row*(*cur_game).cols + i-1] = -1;
                }
            }
            
          }
        }
    }
     
    if (move_flag ==1) { //checking for changes
        return 1;
    }

    return 0;
}

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
   for (int row = 0; row < (*cur_game).rows; row++){
       for (int column = 0; column < (*cur_game).cols; column++){ //checking if there are any empty cells meaning legal moves still available
           if (*get_cell(cur_game, row, column) == -1){
               return 1;
           }
           if (row < (*cur_game).rows-1){ //checking for adjacent cells
               if (*get_cell(cur_game, row, column) == *get_cell(cur_game, row+1, column)){
                   return 1;
               }
           }
           if (column < (*cur_game).cols -1){
               if (*get_cell(cur_game, row, column) == *get_cell(cur_game, row, column + 1)){
                   return 1;
               }
           }
           }
       }
       return 0;
   }

/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}

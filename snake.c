/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*=========================================================================
  Constants and definitions:
==========================================================================*/

/* put your #defines and typedefs here*/
#define N 25
#define BOARD_ELEMENT '#'
#define PERCENT 0
#define FIRST_PLAYER PERCENT
#define PERCENT_HEAD '%'
#define SHTRODEL 1
#define SHTRODEL_HEAD '@'
#define ILLEGAL_MOVE 0
#define HUNGER_REASON 2
#define INIT_SN_SIZE 3
#define PERC_BODY '*'
#define SHTR_BODY '+'
#define EMPTY_CELL ' '
#define FOOD 'F'
#define LEGAL_MOVE 2
#define FOOD_MOVE 3
#define UP 8
#define DOWN 2
#define LEFT 4
#define RIGHT 6
#define INIT_VAL 0 //default value for some variables
#define ERROR 1
#define MIN_SIZE 4 //minimal size of board
#define MAX_SIZE 25 //maximal size of board
#define EXP_SCAN_RES1 1 //expected result of scanf function
#define EXP_SCAN_RES2 2 //another expected result of scanf
#define INIT_STR_ROW size-1 //initial row-position for shtrodel snake
#define INIT_PRC_ROW 0 //initial row-position for percent snake
#define PRC_CODE j+1 //represents the body of a percent snake in int array
#define STR_CODE (-1)*(j+1) //int expression of the body of shtrodel snake
#define EMPTY_SNAKES 0 //empty value for the int array for snakes
#define P_HEAD_CODE 1 //represents the head of percent snake in int array
#define P_OLDHEAD_CODE 2//represents the head of % snake one move before
#define S_HEAD_CODE -1//represents the head of @ snake in int array
#define S_OLDHEAD_CODE -2//represents the head of % snake one move before
#define MIN_STEPS 2//minimum amount of steps without food
#define LEFT_EDGE 0//represents the left-boundary of the board
#define UP_EDGE 0//represents the up-boundary of the board
#define FORWARD 1//for increasing the x or y coordinate by 1
#define BACKWARD -1// for decreasing x or y coordinate by 1
#define NEW_HEAD_CODE old_head/2//expression for the value of a new head
#define SNAKES 2//number of the snakes in the game
#define STEP_LIM 0//the limit of steps
#define INIT_MOVE_RES LEGAL_MOVE//initiating the status as a legal move


/*-------------------------------------------------------------------------
*   Function declartion
*--------------------------------------------------------------------------*/

void print_introduction_msg();                        //print welcome message , and introduction message to the game
void print_board_size_msg();                          //print message to enter board size
void print_steps_without_food();                      //print message to enter maximum number of steps can snake do without food
void print_food_location_msg();                       //print message to insert place for next food
void help_print_board(int size);                      //printing static part of the board
void print_board(char board[N][N],int size);          //print the current board for the user
void print_player_message(int player);                //print message when we switch between users
void print_insert_direction();			              //print message asking the user to insert his move
void print_finsih_msg(int player,int finish_reason ); //get the player who lose and the reason of losing and print message state
void print_error_input();                             //print message - there is error in the input
void print_invalid_input();			                  //print message - re-insert valid input
void print_full_board();		                      //print message - full board - no winner
int max(int value1,int value2);
int min(int value1,int value2);
void init_board(int size,char board[N][N],int snakes[N][N]);
int scan_board_size();
int scan_steps();
int scan_step();
int scan_food(int size,char board[N][N]);
bool check_food(int size,char board[N][N],int x, int y);
int start_game(int size,char board[N][N],int snakes[N][N], int steps);
void upd_brd(int size,char board[N][N], int snakes[N][N]);
int make_move(int size,char board[N][N],int snakes[N][N],int plyr,int stp);
int get_tail(int size,int snakes[N][N],int plyr);
bool is_valid_step(int step);
void init_snakes(int size,int snakes[N][N]);
bool fullboard(int size,char board[N][N]);
int check_res(int sc_res,int expected_res);
int mov_check(int size,char brd[N][N],int snks[N][N],int head,\
                                                      int x_stp,int y_stp);
void mov_help(int size,int snakes[N][N],int res,int pl,\
                                             int head,int x_stp,int y_stp);
bool is_mov_legal(int size,char board[N][N],int xc,int yc);
void print_turn(int size,char board[N][N], int player);
bool check_looser(int result,int step_arr[SNAKES],int player);
bool is_stplim(int stp_arr[SNAKES]);
int return_op(int input_val,int expected,int return1,int return2);

/*12 lines*/
int main()
{
  char board[N][N];
  int steps=INIT_VAL, snakes[N][N];
  int size=scan_board_size();
  if(size==ERROR)
    return ERROR;
  init_board(size,board,snakes);
  steps=scan_steps();
  if(steps==ERROR)
    return ERROR;
  if(start_game(size,board,snakes,steps)==ERROR)
     return ERROR;
  return 0;
}

/* 2 lines
  Returns the greater of the two values */
int max(int value1, int value2){
    int result = value1>value2? value1 : value2;
    return result;
}

/* 2 lines
Returns the smaller of the two values*/
int min(int value1,int value2){
    int result = value1>value2? value2 : value1;
    return result;
}

/* 7 lines
  Prints the introduction message, asks to input the specific size of the
  board and gets from the standard input the value.Checks if the value is
  an int, if not returns error.If int checks if the value is in the range*/
int scan_board_size(){
    int size = INIT_VAL, sc_res=INIT_VAL;
    print_introduction_msg();
    print_board_size_msg();
    while((sc_res=scanf("%d", &size))!=EXP_SCAN_RES1||\
                                            size<MIN_SIZE||size>MAX_SIZE){
        if(check_res(sc_res,EXP_SCAN_RES1)==ERROR)
            return ERROR;
    }
    return size;
}

/* 3 lines
  Initializes all cells in the board with spaces.*/
void clean_board(int size,char board[N][N]){
    for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                board[i][j]=EMPTY_CELL;
            }
        }
}

/* 5 lines
   Initializes an int array for snakes positions.All the values are 0,
   except from the values of the snakes. %-snake gets positive values
   starting from 1. @-snake gets negative values starting from -1.*/
void init_snakes(int size,int snakes[N][N]){
    for(int i=0;i<size; i++){
        for(int j=0;j<size;j++){
          if((i==INIT_PRC_ROW||i==INIT_STR_ROW) && j<INIT_SN_SIZE)
            snakes[i][j]=i==INIT_PRC_ROW? PRC_CODE : STR_CODE;
          else
            snakes[i][j]=EMPTY_SNAKES;
        }
    }
}

/* 12 lines
  Updates the (char) board according to the values in (int) array snakes.
  Positions of positive numbers in snakes[][] are the positions of %-snake
  in board[][].The same for @-snake and negative numbers.*/
void upd_brd(int size,char board[N][N],int snakes[N][N]){
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(snakes[i][j]>EMPTY_SNAKES){
                if(snakes[i][j]==P_HEAD_CODE)
                    board[i][j]=PERCENT_HEAD;
                else
                    board[i][j]=PERC_BODY;
            }
            if(snakes[i][j]<EMPTY_SNAKES){
                if(snakes[i][j]==S_HEAD_CODE)
                    board[i][j]=SHTRODEL_HEAD;
                else
                    board[i][j]=SHTR_BODY;
            }
            if(snakes[i][j]==EMPTY_SNAKES && board[i][j]!=FOOD)
                board[i][j]=EMPTY_CELL;
        }
    }
}

/* 3 lines
  Initializing the game board and array of snakes.*/
void init_board(int size,char board[N][N],int snakes[N][N]){
    clean_board(size,board);
    init_snakes(size,snakes);
    upd_brd(size,board,snakes);
}

/* 6 lines
  Asks to enter the minimum number of steps for the game and scans it.*/
int scan_steps(){
    int steps=INIT_VAL,sc_res=INIT_VAL;
    print_steps_without_food();
    while((sc_res = scanf("%d",&steps))!=EXP_SCAN_RES1||steps<MIN_STEPS){
        if(check_res(sc_res,EXP_SCAN_RES1)==ERROR)
            return ERROR;
    }
    return steps;
}

/* 4 lines
  Checks if the entered position of food is legal.*/
bool check_food(int size,char board[N][N],int xbrd,int ybrd){
    if(xbrd<LEFT_EDGE||xbrd>=size||ybrd<UP_EDGE||ybrd>=size||\
                                            board[xbrd][ybrd]!=EMPTY_CELL)
        return false;
    return true;
}

/* 8 lines
  Asks to enter position of food and scans it as a pair of coordinates.
  Then updates the board with the new food location.*/
int scan_food(int size,char board[N][N]){
    int xbrd=INIT_VAL,ybrd=INIT_VAL,res=INIT_VAL;
    print_food_location_msg();
    while((res=scanf("%d %d",&xbrd,&ybrd))!=EXP_SCAN_RES2||\
                                        !check_food(size,board,xbrd,ybrd)){
        if(check_res(res,EXP_SCAN_RES2)==ERROR)
            return ERROR;
    }
    board[xbrd][ybrd]=FOOD;
    return 0;
}

/* 5 lines
  Checks if the first value equals to the second.If not prints ERROR
  message.We use it to see if scanf() was successful*/
int check_res(int sc_res,int expected_res){
    if(sc_res!=expected_res){
        print_error_input();
        return ERROR;
    }
    print_invalid_input();
    return 0;
}

/* 11 lines
  Function that calculates the direction of changes in the coordinates
  according to the stp value.Calls a function that decides if the move is
  legal.If it is - executes the move, if not ILLEGAL MOVE is returned.*/
int make_move(int size,char board[N][N],int snakes[N][N],int plyr,int stp){
    int x_stp=INIT_VAL,y_stp=INIT_VAL;
    if(stp==DOWN||stp==UP)
        y_stp=return_op(stp,DOWN,FORWARD,BACKWARD);
     else
        x_stp=return_op(stp,RIGHT,FORWARD,BACKWARD);
    int head=return_op(plyr,PERCENT,P_HEAD_CODE,S_HEAD_CODE);
    int move_res=mov_check(size,board,snakes,head,x_stp,y_stp);
        if(move_res==ILLEGAL_MOVE)
            return ILLEGAL_MOVE;
    mov_help(size,snakes,move_res,plyr,head,x_stp,y_stp);
    upd_brd(size,board,snakes);
    return move_res;
}

/* 8 lines
  Checks if the move is legal or if it is a food achieving move.*/
int mov_check(int size,char brd[N][N],int snks[N][N],int head,\
                                                      int x_stp,int y_stp){

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
                if(snks[i][j]==head){
    if(is_mov_legal(size,brd,i+y_stp,j+x_stp))
        return ILLEGAL_MOVE;
    if(brd[i+y_stp][j+x_stp]==FOOD)
        return FOOD_MOVE;
            }
        }
    }
    return LEGAL_MOVE;
}

/* 8 lines
  Function that updates the snakes array with the new coordinates.
  And erases the end of the tail if needed*/
void mov_help(int size,int snakes[N][N],int res,int plyr,\
                                             int head,int x_stp,int y_stp){
    int old_head=head+head;
    int tail=get_tail(size,snakes,plyr);
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(snakes[i][j]==old_head)
                snakes[i+y_stp][j+x_stp]=NEW_HEAD_CODE;
            if(snakes[i][j]==tail && res!=FOOD_MOVE)
                snakes[i][j]=EMPTY_SNAKES;
        }
    }
}

/* 12 lines
  Function that finds the end of the tail of the snake.We need it to clean
  the tail if the snake didn't eat food in the previous move.*/
int get_tail(int size,int snakes[N][N],int plyr){
    int tail=INIT_VAL;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(plyr==PERCENT){
                if(snakes[i][j]>EMPTY_SNAKES){
                    snakes[i][j]++;
                    tail = max(tail,snakes[i][j]);
                }
            }
            if(plyr==SHTRODEL){
                if(snakes[i][j]<EMPTY_SNAKES){
                    snakes[i][j]--;
                    tail = min(tail,snakes[i][j]);
                }
            }
        }
    }
    return tail;
}

/* 3 lines
  Checks the conditions for ILLEGAL MOVE.*/
bool is_mov_legal(int size,char board[N][N],int xc,int yc){
    bool size_lim = (xc<LEFT_EDGE||xc>size-1||yc<UP_EDGE||yc>size-1);
    bool collision = (board[xc][yc]!=EMPTY_CELL&&board[xc][yc]!=FOOD);
    return (size_lim||collision);
}

/* 7 lines
  Scans from the input the code for a next step:up, down, left, right.*/
int scan_step() {
    int sc_res=INIT_VAL,step=INIT_VAL;
    while((sc_res=scanf("%d",&step))!=EXP_SCAN_RES1||!is_valid_step(step)){
        if(sc_res!=EXP_SCAN_RES1){
            print_error_input();
            return ERROR;
        }
        print_invalid_input();
    }
    return step;
}

/* 4 lines
  Checks if the input code for step is in the range of valid values.*/
bool is_valid_step(int step){
        if(step==UP || step==DOWN || step==LEFT || step==RIGHT)
            return true;
        else
            return false;
}

/* 13 lines
   After all required parameters are initialized the game is starting and
  ending in this function.Each move of each player and the results of the
   game are executed here.*/
int start_game(int size,char board[N][N],int snakes[N][N],int steps){
    int player = FIRST_PLAYER, input_step=INIT_VAL;
    /*The amount of steps that is available to make for each snake is
      stored in stp_arr[].*/
    int stp_arr[SNAKES]={steps,steps}, mov_res=INIT_MOVE_RES;
    while(!fullboard(size,board)&&!check_looser(mov_res,stp_arr,player)){
            if(scan_food(size, board)==ERROR)
                return ERROR;
     do {
                print_turn(size,board,player);
                if((input_step=scan_step())==ERROR)
                    return ERROR;

      mov_res=make_move(size,board,snakes,player,input_step);

      /*Decreasing the amount of steps if the food wasn't eaten. Otherwise,
         resetting it to the steps value.*/
      stp_arr[player]=return_op(mov_res,FOOD_MOVE,steps,stp_arr[player]-1);
      player =!player;
            }
     while(mov_res!=FOOD_MOVE&&mov_res!=ILLEGAL_MOVE&&!is_stplim(stp_arr));
    }
    return 0;
}

/* 1 line
  Checks if some of the players reached the limit of steps.*/
bool is_stplim(int stp_arr[SNAKES]){
    return (stp_arr[PERCENT]==STEP_LIM||stp_arr[SHTRODEL]==STEP_LIM);
}

/* 10 lines
  Checks if the game board is full with non-empty values. If it is full -
  prints the corresponding message.*/
bool fullboard(int size,char board[N][N]){
    bool full=true;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(board[i][j]==EMPTY_CELL||board[i][j]==FOOD){
                full = false;
                return full;
            }
        }
    }
    if(full){
        print_board(board,size);
        print_full_board();
    }
    return full;
}

/* 7 lines
  Calculates if the last player made an illegal step or reached the limit
   of steps without food.*/
bool check_looser(int move_res,int step_arr[SNAKES],int player){
        if(move_res==ILLEGAL_MOVE){
            print_finsih_msg(!player, ILLEGAL_MOVE);
            return true;
        }
        if(step_arr[PERCENT]==STEP_LIM||step_arr[SHTRODEL]==STEP_LIM){
            print_finsih_msg(!player, HUNGER_REASON);
            return true;
        }
        return false;
}

/* 2 lines
  Determines which value to return by comparing the first argument and the
   second arguments of the function.*/
int return_op(int input_val,int expected,int return1,int return2){
    int result_val=input_val==expected?return1:return2;
    return result_val;
}

/* 3 lines
  Prints a set of messages before each round of a game is started.*/
void print_turn(int size,char board[N][N],int player){
    print_board(board, size);
    print_player_message(player);
    print_insert_direction();
}

void print_introduction_msg(){
    printf("Welcome to multi-snake  game, The game have 2 player Percent and Shtrodel.\n"
    "The game starts with PERCENT player.\n");
    printf("You have to choose 4 moves :Up,Down,Right and Left "
    "The Percent player is the snake that starts in (0,0)\n\n");
}

void print_board_size_msg(){
    printf("Insert board size between 4 and 25:\n");
}

void print_steps_without_food(){
    printf("Insert the maximum number of steps the snake can do without food:\n");
}

void print_food_location_msg(){
    printf("in order to continue the game please Insert a row then column numbers to place the next food: \n");
}

void help_print_board(int size){
    int i=0;
    printf("%c",BOARD_ELEMENT);
    for(i=0;i<size;i++)
    {
        printf("%c",BOARD_ELEMENT);
    }
    printf("%c\n",BOARD_ELEMENT);
}

void print_board(char board[N][N],int size){
    int i=0,j=0;
    help_print_board(size);
    for(i=0;i<size;i++)
    {
        printf("%c",BOARD_ELEMENT);
        for(j=0;j<size;j++)
        {
            printf("%c",board[i][j]);
        }
        printf("%c\n",BOARD_ELEMENT);
    }
    help_print_board(size);
    printf("\n\n");
}

void print_player_message(int player){
    printf("******* Turn of %c ***********\n",player==PERCENT?PERCENT_HEAD:SHTRODEL_HEAD);
}

void print_insert_direction(){
    printf("Enter the direction to move your snake: (2-down,4-left,6-right,8-up):\n");
}

void print_finsih_msg(int player,int finish_reason){
    printf("\nThe game finish and the winner is:\n");
    printf("*** %s player ****.\n",player==PERCENT?"SHTRODEL":"PERCENT");
    printf("The reason is %s of the ",finish_reason==ILLEGAL_MOVE? "Illegal move" : "Snake die");
    printf("%s player.\n\n",player==PERCENT?"PERCENT":"SHTRODEL");
}

void print_invalid_input(){
    printf("Please re-Inset valid input:\n");
}

void print_full_board(){
    printf("Full Board. The game over with no Winner.\n");
}

void print_error_input(){
    printf("Error with the input.\n");
}

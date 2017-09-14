#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

#define U_ARROW 72 //those are constants of controls keys' codes
#define D_ARROW 80
#define ENTER 13
#define ESC 27

#define ROWS 9 //rows of the board array
#define COLS 9 //columns of the board array
#define PIECES 6 //number of pieces types

int showMenu();
int menuctrl();
int getcode();
void SetColorAndBackground(int ForgC, int BackC);
void display(int position,int sf,int lf);
int boardfun();
void body();

int pawn1(int from_row,int from_col,int to_row,int to_col);// this and the next six functions handles the movement of the pieces
int pawn2(int fromRow,int fromCol,int toRow,int toCol);
int knight(int fromRow,int fromCol,int toRow,int toCol);
int rock (int fromRow,int fromCol,int toRow,int toCol);
int bishop (int fromRow,int fromCol,int toRow,int toCol);
int queen(int fromRow,int fromCol,int toRow,int toCol);
int king(int fromRow,int fromCol,int toRow,int toCol);

void doMovment(int row1,int col_1,int row2 ,int col_2);

int check1(int krow,int kcolumn); // this and the five functions handles the checkmate and stalemate cases
int check2(int krow,int kcolumn);
int stalemate1(int rowk,int columnk);
int stalemate2(int rowk,int columnk);
int checkmate1(int rowk,int columnk);
int checkmate2(int rowk,int columnk);

void saveit();
void loadit();
void autosave();
void undo();
void redo();

FILE *save_load; // those are files support the game's save and load ability
FILE *saveName;
FILE *outer;
FILE *turn;

int position,errorflag=0,outCount1=0,outCount2=0,resume=0,player1=1,player2=0,ucount=0,utest=0,maxredo=0;
char savearr[15];
char player1Name[10],player2Name[10];
char outs1[100]= {0},outs2[100]= {0};
char undosupport1[8][8][50]= {},undosupport2[8][8][50]= {};
char pieces1[PIECES]= {'k','p','n','q','b','r'},pieces2[PIECES]= {'K','P','N','Q','B','R'}; //this two arrays contains players' pieces' types
char board[ROWS][COLS]= { {0,0,0,0,0,0,0,0,0},   //this is the array of the board
    {0,'r','n','b','q','k','b','n','r'},
    {0,'p','p','p','p','p','p','p','p'},
    {0,219,' ',219,' ',219,' ',219,' '},
    {0,' ',219,' ',219,' ',219,' ',219},
    {0,219,' ',219,' ',219,' ',219,' '},
    {0,' ',219,' ',219,' ',219,' ',219},
    {0,'P','P','P','P','P','P','P','P'},
    {0,'R','N','B','Q','K','B','N','R'}
};



struct kingplace //this is a structure used to define the two king's places all along the game
    {
        int row;
        int column;
    } k1p= {1,5},k2p= {8,5};

struct test  //this is a structure that tests load and save cases
{
    int er;
    int flag;
} save= {0,0},load= {0,1};


int main()
{
    saveName=fopen("savename.txt","r");
    if (saveName==NULL)
        load.flag=0;
    showMenu();
    return 0;
}


int showMenu()   //this function shows the menu using display function
{
    char re1,re2,re3;
    int test;
re1:
    test=menuctrl();   //this returns the position of the cursor when pressing enter
    system("cls");
    switch(test)    //this takes an action depending on the choice
    {
    case 0:
        if (resume==0)
        {
re2:
            SetColorAndBackground(9,0 );
            printf("player1 name : ");
            SetColorAndBackground(7,0 );
            gets(player1Name);
            if (player1Name[0]=='\0')
            {
                system("cls");
                SetColorAndBackground(4,0 );
                printf("Enter your name!\n");
                SetColorAndBackground(7,0 );
                goto re2;
            }
re3:
            SetColorAndBackground(9,0 );
            printf("player2 name : ");
            SetColorAndBackground(7,0 );
            gets(player2Name);
            if (player2Name[0]=='\0')
            {
                system("cls");
                SetColorAndBackground(4,0 );
                printf("Enter your name!\n");
                SetColorAndBackground(7,0 );
                goto re3;
            }
            else if(strcmp(player1Name,player2Name)==0)
            {
                system("cls");
                SetColorAndBackground(4,0 );
                printf("this name is taken!\n");
                SetColorAndBackground(7,0 );
                goto re3;
            }
            system("cls");
            body();
        }
        else
        {
            return 1;
        }

        break;
    case 1:
        if (save.flag==0)
        {
            save.er=1;
            goto re1;
        }
        else
        {
            saveit();
            load.flag=1;
            return 1;

        }
        break;
    case 2:
        if
        (load.flag==0)
        {
            load.er=1;
            goto re1;
        }
        else
        {
            maxredo=0;
            ucount=0;
            loadit();
            if(resume==0)
                body();
        }
        break;
    case -1 :
        return 1;
        break;
    }

    return 0 ;
}

int menuctrl() //this function allows the user to control the menu
{
    int position=0,quit=0;
    display(position,save.flag,load.flag);
    while (!quit)
    {
        display(position,save.flag,load.flag);
        switch(getcode())
        {
        case U_ARROW:
            if (position>0)
                position--;
            else
                position+=2;
            break;
        case D_ARROW :
            if (position<2)
                position++;
            else
                position-=2;
            break;
        case ENTER :
            quit=1;
            break;
        case ESC :
            position=-1;
            quit=1;
            break;
        }
    }

    return position;
}

int getcode() //this function gets the key from the user and returns it
{
    int key=getch();

    return key;
}

void SetColorAndBackground(int ForgC, int BackC)
{
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
    return;
}

void display(int position,int sf,int lf)  //this function displays the menu interface
{
    system("cls");
    int i=0;
    char *mainu[3]= {" New  "," Save "," Load "};
    if (resume==1)
        mainu[0]="Resume";
    SetColorAndBackground(9,0);
    printf("\t              88\n");
    printf("\t              88\n");
    printf("\t              88\n");
    printf("\t   ,adPPYba,  88,dPPYba,    ,adPPYba,  ,adPPYba,  ,adPPYba,\n");
    SetColorAndBackground(10,0);
    printf("\t  a8\"     \"\"  88P'    \"8a  a8P_____88  I8[    \"\"  I8[    \"\n");
    printf("\t  8b          88       88  8PP\"\"\"\"\"\"\"   `\"Y8ba,    `\"Y8ba, \n");
    printf("\t  \"8a,   ,aa  88       88  \"8b,   ,aa  aa    ]8I  aa    ]8I\n");
    SetColorAndBackground(9,0);
    printf("\t   `\"Ybbd8\"'  88       88   `\"Ybbd8\"'  `\"YbbdP\"'  `\"YbbdP\"'\n\n\n");
    SetColorAndBackground(7,0);

    SetColorAndBackground(9,0);
    printf("\t\t\t\t\xc9\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xbb"); //those are symbols to draw the menu
    printf("\n\t\t\t\t\xba              \xba");
    for (i=0; i<3; i++)
    {
        printf("\n\t\t\t\t\xba    ");
        if (sf==0 && i==1)
            SetColorAndBackground(8,0);
        if (lf==0 && i==2)
            SetColorAndBackground(8,0);
        if (i==position)
            SetColorAndBackground(0,15),position=-1;
        printf("%s",mainu[i]);
        SetColorAndBackground(9,0);
        printf("    \xba\n\t\t\t\t\xba\t       \xba");
    }

    printf("\n\t\t\t\t\xba              \xba");
    printf("\n\t\t\t\t\xc8\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xbc");
    SetColorAndBackground(7,0);
    printf("\n\n\n\n\n");

}

int boardfun() //this function displays the board
{
    int i=0,j=0,k=0;
    SetColorAndBackground(9,0 );
    printf("White's captured pieces :");
    SetColorAndBackground(7,0 );
    for (i=0; i<outCount1; i++)
        printf("%c,",outs1[i]);
    printf("\n");
    SetColorAndBackground(9,0 );
    printf("Black's captured pieces :");
    SetColorAndBackground(7,0 );
    for (i=0; i<outCount2; i++)
    {
        SetColorAndBackground(6,0);
        printf("%c",outs2[i]);
        SetColorAndBackground(7,0);
        printf(",");
    }

    printf("\n\n \t\t\t");
    for(k=65; k<73; k++)
        printf("%c   ",k);
    printf("\n\n");

    printf("\t\t      \xc9");
    for (i=0; i<7; i++)
        printf("\xcd\xcd\xcd\xd1");
    printf("\xcd\xcd\xcd\xbb");
    for (i=8; i>0; i--)
    {
        printf("\n\t\t  %d   \xba ",i);
        for (j=1; j<9; j++)
        {
            if (isupper(board[i][j]))
            {
                SetColorAndBackground(6,0);
                printf("%c",board[i][j]);
                SetColorAndBackground(7,0 );
                printf(" \xb3 ");
            }

            else
            {
                printf("%c \xb3 ",board[i][j]);
            }
        }
        printf("\b\b\xba");

        SetColorAndBackground(9,0 );
        if (i==8)
            printf("   Menu => press ESC");
        if (i==7)
            printf("   Undo => press U");
        if (i==6)
            printf("   Redo => press R");
        SetColorAndBackground(7,0 );

        if (i!=1)
        {
            int l=0;
            printf("\n\t\t      \xc7");
            for(l=0;l<8;l++)
            printf("\xc4\xc4\xc4\xc5");
            printf("\b\xb6");
        }
    }
    printf("\n\t\t      \xc8");
    for (i=0; i<7; i++)
        printf("\xcd\xcd\xcd\xcf");
    printf("\xcd\xcd\xcd\xbc");
    printf("\n\n");
    return 0;
}

void body() //this function is the skeleton of the game ,it takes co-ordinates ,make checks,and switching between the two players
{
    int i=0,j=0,k=0,test=0,loop=0,undoglag=0,row1,row2,col_1,col_2,checkflag=0,checkmate=-1,nosave=0;
    char y_co1,y_co2,element,repeate,theEnd;

repeate:
    if (nosave==0 && utest==0)
        autosave();
    nosave=0;
    utest=0;
    boardfun();
    if (checkmate1(k1p.row,k1p.column)!=0)
    {
        printf("\t\t\t\t ");
        SetColorAndBackground(9,0);
        printf("Black wins");
        SetColorAndBackground(7,0);
        printf(" \x2\n\n");
        exit(1);
    }
    else if (checkmate2(k2p.row,k2p.column)!=0)
    {
        printf("\t\t\t\t ");
        SetColorAndBackground(9,0);
        printf("White wins");
        SetColorAndBackground(7,0);
        printf(" \x2\n\n");
        exit(1);
    }

    if (errorflag==1)
    {
        SetColorAndBackground(4,0);
        printf("Error movement,try again !\n");
        SetColorAndBackground(7,0);
    }
    if (check1(k1p.row,k1p.column)!=0 && player1)
    {
        SetColorAndBackground(4,0);
        printf("Your king is threatened 1 !\n");
        SetColorAndBackground(7,0);
    }
    if (check2(k2p.row,k2p.column)!=0 && player2)
    {
        SetColorAndBackground(4,0);
        printf("Your king is threatened 2 !\n");
        SetColorAndBackground(7,0);
    }
    if (checkflag==1)
    {
        SetColorAndBackground(4,0);
        printf("watch your king !\n");
        SetColorAndBackground(7,0);
    }

    checkflag=0;
    errorflag=0;
    if (stalemate1(k1p.row,k1p.column)!=0 && player1)
    {
        if (board[2][4]=='p' && board[2][5]=='p' && board[2][6]=='p')
        {}
        else
        {
            SetColorAndBackground(4,0);
        printf("stalemate1 \n");
        SetColorAndBackground(7,0);
        }

    }
    if (stalemate2(k2p.row,k2p.column)!=0 && player2)
    {
        if (board[7][4]=='P' && board[7][5]=='P' && board[7][6]=='P')
        { }
        else
        {
            SetColorAndBackground(4,0);
        printf("stalemate1 \n");
        SetColorAndBackground(7,0);
        }
    }
    if (player1)
        {
            printf("%s's turn (white): ",player1Name);
        }
    else
        {
            SetColorAndBackground(6,0 );
            printf("%s's turn (black): ",player2Name);
            SetColorAndBackground(7,0 );
        }
    y_co1=getche();

    if(y_co1==(char)27)
    {
        resume=1;
        save.flag=1;
        if(showMenu()==1);
           utest=1;
        goto repeate;

    }
    else if (y_co1=='u' || y_co1=='U')
    {
       undo();
       system("cls");
       goto repeate;
    }
    else if (y_co1=='r' || y_co1=='R')
    {
       redo();
       system("cls");
       goto repeate;
    }

    row1=getche();
    row1-=48;
    if (y_co1=='a' || y_co1=='A')
        col_1=1;
    else if (y_co1=='b' || y_co1=='B')
        col_1=2;
    else if (y_co1=='c' || y_co1=='C')
        col_1=3;
    else if (y_co1=='d' || y_co1=='D')
        col_1=4;
    else if (y_co1=='e' || y_co1=='E')
        col_1=5;
    else if (y_co1=='f' || y_co1=='F')
        col_1=6;
    else if (y_co1=='g' || y_co1=='G')
        col_1=7;
    else if (y_co1=='h' || y_co1=='H')
        col_1=8;
    else
        col_1=0;

    y_co2=getche();
    row2=getche();
    row2-=48;
    if (y_co2=='a' || y_co2=='A')
        col_2=1;
    else if (y_co2=='b' || y_co2=='B')
        col_2=2;
    else if (y_co2=='c' || y_co2=='C')
        col_2=3;
    else if (y_co2=='d' || y_co2=='D')
        col_2=4;
    else if (y_co2=='e' || y_co2=='E')
        col_2=5;
    else if (y_co2=='f' || y_co2=='F')
        col_2=6;
    else if (y_co2=='g' || y_co2=='G')
        col_2=7;
    else if (y_co2=='h' || y_co2=='H')
        col_2=8;
    else
        col_2=0;

    if (getch()!='\r')  //this check guarantees that there is no inputs but coordinates
    {
        system("cls");
        errorflag=1;
        goto repeate;
    }
    if (row1>8 || row1<1 || row2>8 || row2<1 || col_1==0 || col_2 ==0)  //this check guarantees that the coordinates are within the matrix
    {
        system("cls");
        errorflag=1;
        goto repeate;
    }

    if (player1)
    {
        int found=0;

        for(i=0; i<PIECES; i++) // this check finds if the "from" coordinate contains a piece of player1
            if (board[row1][col_1]==pieces1[i])
            {
                element=pieces1[i];
                found++;
                break;
            }
        if (found==0)
        {
            system("cls");
            errorflag=1;
            goto repeate;

        }
        for(i=0; i<PIECES; i++) // this check finds if the "to" coordinate does not contain a friend piece
            if (board[row2][col_2]==pieces1[i])
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }

        switch (element)
        {
        case 'p':
            if (pawn1(row1,col_1,row2,col_2)==0)
            {

                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check1(k1p.row,k1p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;

                doMovment(row1,col_1,row2,col_2);

                if (row2==8)
                {
                    char pro,redo;
redo:
                    puts("promote to (q/r/n/b )===>  ");
                    pro=getche();
                    if (pro!='q' && pro!='r' && pro!='n' && pro!='b')
                    {
                        printf("Error choice!");
                        goto redo;
                    }

                    else
                    {
                        board[row2][col_2]=pro;

                        boardfun();
                    }
                }
            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'k':
            if (king(row1,col_1,row2,col_2)==0)
            {
                if (check1(row2,col_2)==0)
                {
                    doMovment(row1,col_1,row2,col_2);

                    k1p.row=row2;
                    k1p.column=col_2;
                }
                else
                {
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'q':
            if (queen(row1,col_1,row2,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check1(k1p.row,k1p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'r':
            if (rock(row1,col_1,row2,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check1(k1p.row,k1p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'b':
            if(bishop (row1,col_1,row2 ,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check1(k1p.row,k1p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'n':
            if (knight(row1,col_1,row2,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check1(k1p.row,k1p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        }
        player1=0;
        player2=1;
        system("cls");
        goto repeate;
    }
    if (player2)
    {
        int found=0;

        for(i=0; i<PIECES; i++) // this check finds if the "from" coordinate contains a piece of player2
            if (board[row1][col_1]==pieces2[i])
            {
                element=pieces2[i];
                found++;
                break;
            }
        if (found==0)
        {
            system("cls");
            errorflag=1;
            goto repeate;

        }
        printf("found");
        for(i=0; i<PIECES; i++) // this check finds if the "to" coordinate does not contain a friend piece
            if (board[row2][col_2]==pieces2[i])
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
        printf ("correct, %c",element);
        switch (element)
        {
        case 'P':
            if (pawn2(row1,col_1,row2,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check2(k2p.row,k2p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

                if (row2==1)
                {
                    char pro,redo2;
redo2:
                    puts("promote to (Q/R/N/B )===>  ");
                    pro=getche();
                    if (pro!='Q' && pro!='R' && pro!='N' && pro!='B')
                    {
                        printf("\nError choice!\n");
                        goto redo2;
                    }

                    else
                    {
                        board[row2][col_2]=pro;

                        boardfun();
                    }
                }
            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'K':
            if (king(row1,col_1,row2,col_2)==0)
            {
                if (check2(row2,col_2)==0)
                {
                    doMovment(row1,col_1,row2,col_2);

                    k2p.row=row2;
                    k2p.column=col_2;
                }
                else
                {
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }



            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'Q':
            if (queen(row1,col_1,row2,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check2(k2p.row,k2p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'R':
            if (rock(row1,col_1,row2,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check2(k2p.row,k2p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

            }

            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'B':
            if(bishop (row1,col_1,row2 ,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check2(k2p.row,k2p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        case 'N':
            if (knight(row1,col_1,row2,col_2)==0)
            {
                char r1,r2;
                r2=board[row2][col_2];
                board[row2][col_2]=board[row1][col_1];
                r1=board[row1][col_1];
                board[row1][col_1]=' ';
                if (check2(k2p.row,k2p.column)!=0)
                {
                    board[row1][col_1]=r1;
                    board[row2][col_2]=r2;
                    system("cls");
                    checkflag=1;
                    goto repeate;
                }
                board[row1][col_1]=r1;
                board[row2][col_2]=r2;
                doMovment(row1,col_1,row2,col_2);

            }
            else
            {
                system("cls");
                errorflag=1;
                goto repeate;
            }
            break;
        }
        player1=1;
        player2=0;
        system("cls");
        goto repeate;
    }
}

int pawn1(int fromRow,int fromCol,int toRow,int toCol)
{
    if (fromRow==2)
    {
        printf("pawn1");
        if (toRow==fromRow+1 && toCol==fromCol)
        {
            if (board[toRow][toCol]==(char)219 || board[toRow][toCol]==' ')
                return 0;
        }
        else if (toRow==fromRow+2 && toCol==fromCol)
        {
            if ((board[toRow][toCol]==(char)219 || board[toRow][toCol]==' ') && ((board[toRow-1][toCol]==(char)219 || board[toRow-1][toCol]==' ')))
                return 0;
        }
        else if ((toRow==fromRow+1 && toCol==fromCol+1) || (toRow==fromRow+1 && toCol==fromCol-1))
        {
            if (board[toRow][toCol]!=(char)219 && board[toRow][toCol]!=' ')
                return 0;
        }
    }
    else
    {
        if (toRow==fromRow+1 && toCol==fromCol)
        {
            if (board[toRow][toCol]==(char)219 || board[toRow][toCol]==' ')
                return 0;
        }
        else if ((toRow==fromRow+1 && toCol==fromCol+1) || (toRow==fromRow+1 && toCol==fromCol-1))
        {
            if (board[toRow][toCol]!=(char)219 && board[toRow][toCol]!=' ')
                return 0;
        }
    }
    return 1;
}

int pawn2(int fromRow,int fromCol,int toRow,int toCol)
{
    if (fromRow==7)
    {
        if (toRow==fromRow-1 && toCol==fromCol)
        {
            if (board[toRow][toCol]==(char)219 || board[toRow][toCol]==' ')
                return 0;
        }
        else if (toRow==fromRow-2 && toCol==fromCol)
        {
            if ((board[toRow][toCol]==(char)219 || board[toRow][toCol]==' ') && ((board[toRow+1][toCol]==(char)219 || board[toRow+1][toCol]==' ')))
                return 0;
        }
        else if ((toRow==fromRow-1 && toCol==fromCol+1) || (toRow==fromRow-1 && toCol==fromCol-1))
        {
            if (board[toRow][toCol]!=(char)219 && board[toRow][toCol]!=' ')
                return 0;
        }
    }
    else
    {
        if (toRow==fromRow-1 && toCol==fromCol)
        {
            if (board[toRow][toCol]==(char)219 || board[toRow][toCol]==' ')
                return 0;
        }
        else if ((toRow==fromRow-1 && toCol==fromCol+1) || (toRow==fromRow-1 && toCol==fromCol-1))
        {
            if (board[toRow][toCol]!=(char)219 && board[toRow][toCol]!=' ')
                return 0;
        }
    }
    return 1;
}

int knight(int fromRow,int fromCol,int toRow,int toCol)
{
    if ((toRow==fromRow+2 && toCol==fromCol+1) || (toRow==fromRow+2 && toCol==fromCol-1) || (toRow==fromRow-2 && toCol==fromCol+1)
            || (toRow==fromRow-2 && toCol==fromCol-1) || (toRow==fromRow+1 && toCol==fromCol+2) ||
            (toRow==fromRow+1 && toCol==fromCol-2) || (toRow==fromRow-1 && toCol==fromCol+2) || (toRow==fromRow-1 && toCol==fromCol-2) )
        return 0;

    return 1;
}

int rock (int fromRow,int fromCol,int toRow,int toCol)
{
    int i;
    if (toRow==fromRow)
    {
        if (toCol>fromCol)
        {
            for (i=fromCol+1; i<toCol; i++)
                if (board[fromRow][i]!=(char)219 && board[fromRow][i]!=' ' )
                {
                    return 1;

                }
        }
        else
        {
            for (i=fromCol-1; i>toCol; i--)
                if (board[fromRow][i]!=(char)219 && board[fromRow][i]!=' ' )
                {
                    return 1;

                }
        }

    }
    else if (toCol==fromCol)
    {
        if (toRow>fromRow)
        {
            for (i=fromRow+1; i<toRow; i++)
                if (board[i][fromCol]!=(char)219 && board[i][fromCol]!=' ' )
                {
                    return 1;
                }
        }
        else
        {
            for (i=fromRow-1; i>toRow; i--)
                if (board[i][fromCol]!=(char)219 && board[i][fromCol]!=' ' )
                {
                    return 1;
                }
        }
    }
    else
        return 1;

    return 0;
}

int bishop (int fromRow,int fromCol,int toRow,int toCol)
{
    int i,j,correct=0;
    if (toRow>fromRow && toCol>fromCol)
    {
        for (i=1; i<8; i++)
            if (toRow==fromRow+i && toCol==fromCol+i)
            {
                correct=1;
                for (j=1; j<i; j++)
                    if (board[fromRow+j][fromCol+j]!=(char)219 && board[fromRow+j][fromCol+j]!=' ')
                    {
                        return 1;
                    }
            }
        if (correct==0)
            return 1;

    }
    else if (toRow>fromRow && toCol<fromCol)
    {
        for (i=1; i<8; i++)
            if (toRow==fromRow+i && toCol==fromCol-i)
            {
                correct=1;
                for (j=1; j<i; j++)
                    if (board[fromRow+j][fromCol-j]!=(char)219 && board[fromRow+j][fromCol-j]!=' ')
                    {
                        return 1;
                    }
            }
        if (correct==0)
            return 1;

    }
    else if (toRow<fromRow && toCol>fromCol)
    {
        for (i=1; i<8; i++)
            if (toRow==fromRow-i && toCol==fromCol+i)
            {
                correct=1;
                for (j=1; j<i; j++)
                    if (board[fromRow-j][fromCol+j]!=(char)219 && board[fromRow-j][fromCol+j]!=' ')
                    {
                        return 1;
                    }
            }
        if (correct==0)
            return 1;

    }
    else if (toRow<fromRow && toCol<fromCol)
    {
        for (i=1; i<8; i++)
            if (toRow==fromRow-i && toCol==fromCol-i)
            {
                correct=1;
                for (j=1; j<i; j++)
                    if (board[fromRow-j][fromCol-j]!=(char)219 && board[fromRow-j][fromCol-j]!=' ')
                    {
                        return 1;
                    }
            }
        if (correct==0)
            return 1;

    }
    else
        return 1;

    return 0;
}



int queen(int fromRow,int fromCol,int toRow,int toCol)
{
    int i,j,correct=0;
    if (toRow==fromRow)
    {
        if (toCol>fromCol)
        {
            for (i=fromCol+1; i<toCol; i++)
                if (board[fromRow][i]!=(char)219 && board[fromRow][i]!=' ' )
                {
                    return 1;

                }
        }
        else
        {
            for (i=fromCol-1; i>toCol; i--)
                if (board[fromRow][i]!=(char)219 && board[fromRow][i]!=' ' )
                {
                    return 1;

                }
        }

    }
    else if (toCol==fromCol)
    {
        if (toRow>fromRow)
        {
            for (i=fromRow+1; i<toRow; i++)
                if (board[i][fromCol]!=(char)219 && board[i][fromCol]!=' ' )
                {
                    return 1;
                }
        }
        else
        {
            for (i=fromRow-1; i>toRow; i--)
                if (board[i][fromCol]!=(char)219 && board[i][fromCol]!=' ' )
                {
                    return 1;
                }
        }
    }
    else if (toRow>fromRow && toCol>fromCol)
    {
        for (i=1; i<8; i++)
            if (toRow==fromRow+i && toCol==fromCol+i)
            {
                correct=1;
                for (j=1; j<i; j++)
                    if (board[fromRow+j][fromCol+j]!=(char)219 && board[fromRow+j][fromCol+j]!=' ')
                    {
                        return 1;
                    }
            }
        if (correct==0)
            return 1;

    }
    else if (toRow>fromRow && toCol<fromCol)
    {
        for (i=1; i<8; i++)
            if (toRow==fromRow+i && toCol==fromCol-i)
            {
                correct=1;
                for (j=1; j<i; j++)
                    if (board[fromRow+j][fromCol-j]!=(char)219 && board[fromRow+j][fromCol-j]!=' ')
                    {
                        return 1;
                    }
            }
        if (correct==0)
            return 1;

    }
    else if (toRow<fromRow && toCol>fromCol)
    {
        for (i=1; i<8; i++)
            if (toRow==fromRow-i && toCol==fromCol+i)
            {
                correct=1;
                for (j=1; j<i; j++)
                    if (board[fromRow-j][fromCol+j]!=(char)219 && board[fromRow-j][fromCol+j]!=' ')
                    {
                        return 1;
                    }
            }
        if (correct==0)
            return 1;

    }
    else if (toRow<fromRow && toCol<fromCol)
    {
        for (i=1; i<8; i++)
            if (toRow==fromRow-i && toCol==fromCol-i)
            {
                correct=1;
                for (j=1; j<i; j++)
                    if (board[fromRow-j][fromCol-j]!=(char)219 && board[fromRow-j][fromCol-j]!=' ')
                    {
                        return 1;
                    }
            }
        if (correct==0)
            return 1;

    }
    else
        return 1;

    return 0;
}
int king(int fromRow,int fromCol,int toRow,int toCol)
{
    if ((toRow==fromRow+1 && toCol==fromCol) || (toRow==fromRow+1 && toCol==fromCol+1) || (toRow==fromRow+1 && toCol==fromCol-1)
            || (toRow==fromRow && toCol==fromCol+1) || (toRow==fromRow && toCol==fromCol-1) ||
            (toRow==fromRow-1 && toCol==fromCol) || (toRow==fromRow-1 && toCol==fromCol+1) || (toRow==fromRow-1 && toCol==fromCol-1) )
        return 0;

    return 1;
}



int check1(int krow,int kcolumn)
{
    int i;
    for (i=1; i<8; i++)
        if (board[krow+i][kcolumn]!=(char)219 && board[krow+i][kcolumn]!=' ' )
        {
            if (i==1)
            {
                if (board[krow+i][kcolumn]=='K')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow+i][kcolumn]!='Q' && board[krow+i][kcolumn]!='R' && board[krow+i][kcolumn]!='k')
            {
                break;
            }
            else if (board[krow+i][kcolumn]=='Q' || board[krow+i][kcolumn]=='R')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow-i][kcolumn]!=(char)219 && board[krow-i][kcolumn]!=' ' )
        {
            if (i==1)
            {
                if (board[krow-i][kcolumn]=='K')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow-i][kcolumn]!='Q' && board[krow-i][kcolumn]!='R'&& board[krow-i][kcolumn]!='k')
            {
                break;
            }
            else if (board[krow-i][kcolumn]=='Q' || board[krow-i][kcolumn]=='R')
            {
                return 1;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow][kcolumn+i]!=(char)219 && board[krow][kcolumn+i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow][kcolumn+1]=='K')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow][kcolumn+i]!='Q' && board[krow][kcolumn+i]!='R' && board[krow][kcolumn+i]!='k')
            {
                break;
            }
            else if (board[krow][kcolumn+i]=='Q' || board[krow][kcolumn+i]=='R')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow][kcolumn-i]!=(char)219 && board[krow][kcolumn-i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow][kcolumn-i]=='K')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow][kcolumn-i]!='Q' && board[krow][kcolumn-i]!='R' && board[krow][kcolumn-i]!='k')
            {
                break;
            }
            else if (board[krow][kcolumn-i]=='Q' || board[krow][kcolumn-i]=='R')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow+i][kcolumn+i]!=(char)219 && board[krow+i][kcolumn+i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow+i][kcolumn+i]=='P' || board[krow+i][kcolumn+i]=='K')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow+i][kcolumn+i]!='Q' && board[krow+i][kcolumn+i]!='B' && board[krow+i][kcolumn+i]!='k')
            {
                break;
            }
            else if (board[krow+i][kcolumn+i]=='Q' || board[krow+i][kcolumn+i]=='B')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow+i][kcolumn-i]!=(char)219 && board[krow+i][kcolumn-i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow+i][kcolumn-i]=='P' || board[krow+i][kcolumn-i]=='K')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow+i][kcolumn-i]!='Q' && board[krow+i][kcolumn-i]!='B' && board[krow+i][kcolumn-i]!='k')
            {
                break;
            }
            else if (board[krow+i][kcolumn-i]=='Q' || board[krow+i][kcolumn-i]=='B')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow-i][kcolumn+i]!=(char)219 && board[krow-i][kcolumn+i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow-i][kcolumn+1]=='K')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow-i][kcolumn+i]!='Q' && board[krow-i][kcolumn+i]!='B' && board[krow-i][kcolumn+i]!='k')
            {
                break;
            }
            else if (board[krow-i][kcolumn+i]=='Q' || board[krow-i][kcolumn+i]=='B')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow-i][kcolumn-i]!=(char)219 && board[krow-i][kcolumn-i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow-i][kcolumn-i]=='K')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow-i][kcolumn-i]!='Q' && board[krow-i][kcolumn-i]!='B' && board[krow-i][kcolumn-i]!='k')
            {
                break;
            }
            else if (board[krow-i][kcolumn-i]=='Q' || board[krow-i][kcolumn-i]=='B')
            {
                return 1 ;
                break;
            }
        }

    if (board[krow+2][kcolumn+1]=='N' || board[krow+2][kcolumn-1]=='N' || board[krow-2][kcolumn+1]=='N'
            || board[krow-2][kcolumn-1]=='N' || board[krow+1][kcolumn+2]=='N' ||
            board[krow+1][kcolumn-2]=='N' || board[krow-1][kcolumn+2]=='N' || board[krow-1][kcolumn-2]=='N' )
        return 1;

    return 0;
}

void doMovment(int row1,int col_1,int row2 ,int col_2)
{
    if (board[row2][col_2]!=(char)219 && board[row2][col_2]!=' ')
    {
        if (islower(board[row2][col_2]))
        {
            outs1[outCount1]=board[row2][col_2];
            outCount1++;
        }
        else
        {
            outs2[outCount2]=board[row2][col_2];
            outCount2++;
        }
    }
    board[row2][col_2]=board[row1][col_1];
    if ((row1%2==0 && col_1%2==0) || (row1%2!=0 && col_1%2!=0) )
        board[row1][col_1]=219;
    else
        board[row1][col_1]=' ';

    boardfun();
}
int check2(int krow,int kcolumn)
{
    int i;
    for (i=1; i<8; i++)
        if (board[krow+i][kcolumn]!=(char)219 && board[krow+i][kcolumn]!=' ' )
        {
            if (i==1)
            {
                if (board[krow+i][kcolumn]=='k')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow+i][kcolumn]!='q' && board[krow+i][kcolumn]!='r' && board[krow+i][kcolumn]!='K')
            {
                break;
            }
            else if (board[krow+i][kcolumn]=='q' || board[krow+i][kcolumn]=='r')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow-i][kcolumn]!=(char)219 && board[krow-i][kcolumn]!=' ' )
        {
            if (i==1)
            {
                if (board[krow-i][kcolumn]=='k')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow-i][kcolumn]!='q' && board[krow-i][kcolumn]!='r' && board[krow-i][kcolumn]!='K')
            {
                break;
            }
            else if (board[krow-i][kcolumn]=='q' || board[krow-i][kcolumn]=='r')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow][kcolumn+i]!=(char)219 && board[krow][kcolumn+i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow][kcolumn+i]=='k')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow][kcolumn+i]!='q' && board[krow][kcolumn+i]!='r' && board[krow][kcolumn+i]!='K')
            {
                break;
            }
            else if (board[krow][kcolumn+i]=='q' || board[krow][kcolumn+i]=='r')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow][kcolumn-i]!=(char)219 && board[krow][kcolumn-i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow][kcolumn-i]=='k')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow][kcolumn-i]!='q' && board[krow][kcolumn-i]!='r' && board[krow][kcolumn-i]!='K')
            {
                break;
            }
            else if (board[krow][kcolumn-i]=='q' || board[krow][kcolumn-i]=='r')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow+i][kcolumn+i]!=(char)219 && board[krow+i][kcolumn+i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow+i][kcolumn+i]=='k')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow+i][kcolumn+i]!='q' && board[krow+i][kcolumn+i]!='b' && board[krow+i][kcolumn+i]!='K')
            {
                break;
            }
            else if (board[krow+i][kcolumn+i]=='q' || board[krow+i][kcolumn+i]=='b')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow+i][kcolumn-i]!=(char)219 && board[krow+i][kcolumn-i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow+i][kcolumn-i]=='k')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow+i][kcolumn-i]!='q' && board[krow+i][kcolumn-i]!='b' && board[krow+i][kcolumn-i]!='K')
            {
                break;
            }
            else if (board[krow+i][kcolumn-i]=='q' || board[krow+i][kcolumn-i]=='b')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow-i][kcolumn+i]!=(char)219 && board[krow-i][kcolumn+i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow-i][kcolumn+i]=='p' || board[krow-i][kcolumn+i]=='k')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow-i][kcolumn+i]!='q' && board[krow-i][kcolumn+i]!='b' && board[krow-i][kcolumn+i]!='K')
            {
                break;
            }
            else if (board[krow-i][kcolumn+i]=='q' || board[krow-i][kcolumn+i]=='b')
            {
                return 1 ;
                break;
            }
        }
    for (i=1; i<8; i++)
        if (board[krow-i][kcolumn-i]!=(char)219 && board[krow-i][kcolumn-i]!=' ' )
        {
            if (i==1)
            {
                if (board[krow-i][kcolumn-i]=='p' || board[krow-i][kcolumn-i]=='k')
                {
                    return 1;
                    break;
                }
            }
            if (board[krow-i][kcolumn-i]!='q' && board[krow-i][kcolumn-i]!='b' && board[krow-i][kcolumn-i]!='K')
            {
                break;
            }
            else if (board[krow-i][kcolumn-i]=='q' || board[krow-i][kcolumn-i]=='b')
            {
                return 1 ;
                break;
            }
        }

    if (board[krow+2][kcolumn+1]=='n' || board[krow+2][kcolumn-1]=='n' || board[krow-2][kcolumn+1]=='n'
            || board[krow-2][kcolumn-1]=='n' || board[krow+1][kcolumn+2]=='n' ||
            board[krow+1][kcolumn-2]=='n' || board[krow-1][kcolumn+2]=='n' || board[krow-1][kcolumn-2]=='n' )
        return 1;

    return 0;
}

int stalemate1(int rowk,int columnk)
{
    int detect1,detect2,detect3,detect4,detect5,detect6,detect7,detect8;

    if (rowk+1>8 || islower(board[rowk+1][columnk]))
        detect1=1;
    else
        detect1=check1(rowk+1,columnk);

    if (rowk+1>8 || columnk+1>8  || islower(board[rowk+1][columnk+1]))
        detect2=1;
    else
        detect2=check1(rowk+1,columnk+1);

    if (rowk+1>8 || columnk-1<1 || islower(board[rowk+1][columnk-1]))
        detect3=1;
    else
        detect3=check1(rowk+1,columnk-1);

    if (columnk+1>8 || islower(board[rowk][columnk+1]))
        detect4=1;
    else
        detect4=check1(rowk,columnk+1);

    if (columnk-1<1 || islower(board[rowk][columnk-1]))
        detect5=1;
    else
        detect5=check1(rowk,columnk-1);

    if (rowk-1<1 || islower(board[rowk-1][columnk]))
        detect6=1;
    else
        detect6=check1(rowk-1,columnk);


    if (rowk-1<1 || columnk+1>8 || islower(board[rowk-1][columnk+1]))
        detect7=1;
    else
        detect7=check1(rowk-1,columnk+1);

    if (rowk-1<1 || columnk-1<1 || islower(board[rowk-1][columnk-1]))
        detect8=1;
    else
        detect8=check1(rowk-1,columnk-1);


    if (detect1 !=0 && detect2!=0 && detect3!=0 && detect4!=0 && detect5 !=0 && detect6 !=0 && detect7 !=0 && detect8 !=0 )
        return 1;

    return 0;
}
int stalemate2(int rowk,int columnk)
{
    int detect1,detect2,detect3,detect4,detect5,detect6,detect7,detect8;

    if (rowk+1>8 || isupper(board[rowk+1][columnk]))
        detect1=1;
    else
        detect1=check2(rowk+1,columnk);

    if (rowk+1>8 || columnk+1>8  || isupper(board[rowk+1][columnk+1]))
        detect2=1;
    else
        detect2=check2(rowk+1,columnk+1);

    if (rowk+1>8 || columnk-1<1 || isupper(board[rowk+1][columnk-1]))
        detect3=1;
    else
        detect3=check2(rowk+1,columnk-1);

    if (columnk+1>8 || isupper(board[rowk][columnk+1]))
        detect4=1;
    else
        detect4=check2(rowk,columnk+1);

    if (columnk-1<1 || isupper(board[rowk][columnk-1]))
        detect5=1;
    else
        detect5=check2(rowk,columnk-1);

    if (rowk-1<1 || isupper(board[rowk-1][columnk]))
        detect6=1;
    else
        detect6=check2(rowk-1,columnk);


    if (rowk-1<1 || columnk+1>8 || isupper(board[rowk-1][columnk+1]))
        detect7=1;
    else
        detect7=check2(rowk-1,columnk+1);

    if (rowk-1<1 || columnk-1<1 || isupper(board[rowk-1][columnk-1]))
        detect8=1;
    else
        detect8=check2(rowk-1,columnk-1);


    if (detect1 !=0 && detect2!=0 && detect3!=0 && detect4!=0 && detect5 !=0 && detect6 !=0 && detect7 !=0 && detect8 !=0 )
        return 1;

    return 0;
}

int checkmate1(int rowk,int columnk)
{
    if (stalemate1(rowk,columnk)!=0 && check1(rowk,columnk) !=0)
        return 1;

    return 0;
}

int checkmate2(int rowk,int columnk)
{
    if (stalemate2(rowk,columnk)!=0 && check2(rowk,columnk) !=0)
        return 1;

    return 0;
}
void saveit() //this function save the game
{
    int i=0,j;
    static int nmbrOfSaves=1;

    printf("save nam :");
    gets(savearr);
    while(savearr[0]=='\0')
        gets(savearr);

    saveName=fopen("savename.txt","a+");
    fputs(savearr,saveName);
    fputs("\n",saveName);
    fclose(saveName);


    outer=fopen("outer.txt","a+");
    fprintf(outer,"%d",nmbrOfSaves++);
    while(outs1[i++]!='\0')
        fputc(outs1[i-1],outer);
    i=0;
    while(outs2[i++]!='\0')
        fputc(outs2[i-1],outer);
    fclose(outer);


    save_load=fopen("save_load.txt","a+");
    for (i=1; i<9; i++)
        for(j=1; j<9; j++)
            fputc(board[i][j],save_load);

    fclose(save_load);

    turn=fopen("turn.txt","a+");
    fprintf(turn,"%d%d%d%d%d%d",player1,player2,k1p.row,k1p.column,k2p.row,k2p.column);
    fclose(turn);

    system("cls");
}
void loadit()   //this function loads the game
{
    int i,j,k=0,s=0,choose,yes=0,tempsize=0;
    int nmbrOfSaves=0;
    char ch,temp[20]= {};

    saveName=fopen("savename.txt","r");
    for (i=1; fgets(savearr,sizeof(savearr),saveName)!=NULL; i++)
    {
        printf("%d)%s",i,savearr);
        nmbrOfSaves++;
    }
    fclose(saveName);


    printf("Choose:\n");
    while (yes!=1)
    {
        printf("====>");
        scanf("%d",&choose);
        for (i=0; i<nmbrOfSaves; i++)
        {
            if (i+1==choose)
            {
                yes=1;
                break;
            }
        }
        printf("Error choice!\n");
    }

    outer=fopen("outer.txt","r");
    i=0;
    while ((ch=fgetc(outer))!=EOF)
    {
        temp[i++]=ch;
        tempsize++;
    }
    j=0;
    outCount1=0;
    outCount2=0;
    for(i=0; i<tempsize; i++)
        if ((int)temp[i]==48+choose)
        {
            printf("temp ==%d ",tempsize);
            for(j=i+1; j<tempsize; j++)
            {

                if (48+(choose+1)==(int)temp[j])
                    break;
                if (islower(temp[j]))
                {
                    outs1[outCount1]=temp[j];
                    outCount1++;
                }
                else if (isupper(temp[j]))
                {
                    outs2[outCount2]=temp[j];
                    outCount2++;
                }
            }
            break;
        }
    fclose(outer);

    save_load=fopen("save_load.txt","r");
    system("cls");
    fseek(save_load,(choose-1)*64,SEEK_SET);
    for (i=1; i<9; i++)
        for(j=1; j<9; j++)
            board[i][j]=getc(save_load);

    fclose(save_load);

    turn=fopen("turn.txt","a+");
    fseek(turn,(choose-1)*6,SEEK_SET);
    player1=(int)(fgetc(turn)-48);
    player2=(int)(fgetc(turn)-48);
    k1p.row=(int)(fgetc(turn)-48);
    k1p.column=(int)(fgetc(turn)-48);
    k2p.row=(int)(fgetc(turn)-48);
    k2p.column=(int)(fgetc(turn)-48);
    fclose(turn);

}
void autosave()
{  int j,k;
        for(j=0;j<8;j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            undosupport1[j][k][ucount]=board[j+1][k+1];

    for(j=0;j<=(outCount1/8);j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            undosupport2[j][k][ucount]=outs1[k];

    for(j=2;j<=(2+(outCount2/8));j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            undosupport2[j][k][ucount]=outs2[k];

    undosupport2[4][0][ucount]=player1;
    undosupport2[4][1][ucount]=player2;
    undosupport2[4][2][ucount]=k1p.row;
    undosupport2[4][3][ucount]=k1p.column;
    undosupport2[4][4][ucount]=k2p.row;
    undosupport2[4][5][ucount]=k2p.column;

    ucount++;
    if (ucount>maxredo)
        maxredo=ucount;
}
void undo()
{
  if(ucount-2>=0)
  {
      int j,k;
        for(j=0;j<8;j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            board[j+1][k+1]=undosupport1[j][k][ucount-2];

    for(j=0;j<=(outCount1/8);j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            outs1[k]=undosupport2[j][k][ucount-2];

    for(j=2;j<=(2+(outCount2/8));j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            outs2[k]=undosupport2[j][k][ucount-2];

    player1=(int)undosupport2[4][0][ucount-2];
    player2=(int)undosupport2[4][1][ucount-2];
    k1p.row=(int)undosupport2[4][2][ucount-2];
    k1p.column=(int)undosupport2[4][3][ucount-2];
    k2p.row=(int)undosupport2[4][4][ucount-2];
    k2p.column=(int)undosupport2[4][5][ucount-2];

    ucount-=2;
    }
    else
        utest=1;

}
void redo()
{
  if(ucount<maxredo)
  {
      int j,k;
        for(j=0;j<8;j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            board[j+1][k+1]=undosupport1[j][k][ucount];

    for(j=0;j<=(outCount1/8);j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            outs1[k]=undosupport2[j][k][ucount];

    for(j=2;j<=(2+(outCount2/8));j++)  // the rows dim loop
          for(k=0;k<8;k++)  // the columns dim loop
            outs2[k]=undosupport2[j][k][ucount];

    player1=(int)undosupport2[4][0][ucount];
    player2=(int)undosupport2[4][1][ucount];
    k1p.row=(int)undosupport2[4][2][ucount];
    k1p.column=(int)undosupport2[4][3][ucount];
    k2p.row=(int)undosupport2[4][4][ucount];
    k2p.column=(int)undosupport2[4][5][ucount];

    }
   else
        utest=1;
}

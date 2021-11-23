
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <dirent.h>

FILE * fp ;
int undo = 0;
int turn_num = 0 ;
char symbol[1000]={};
char captured[1000]={};
int moves[1000][4]={};
char prom[1000]={};
char blackpieces[]="RNBQKP";
char whitepieces[]="rnbqkp";
char capturedBlack[16]={};//16 size
char capturedWhite[16]={};//16 size
int cB=0;
int cW=0;
int turn =0 ;
int kingcheckflag=0;
int lowPieces=0;
int deadposition=0;

//two coordinates for when the king is threatened by one piece
int threatX=-1;
int threatY=-1;
int r1= 0;
int r2=0;
int R1=0;
int R2=0;
int k=0;
int K=0;
int Wcheckmated=0;
int Bcheckmated=0;
int checkingSacrifice=0;
int enpassantRight = 0;
int enpassantLeft = 0;





char Board[8][8]={{'R','N','B','Q','K','B','N','R'},
                  {'P','P','P','P','P','P','P','P'},
                  {'-','.','-','.','-','.','-','.'},
                  {'.','-','.','-','.','-','.','-'},
                  {'-','.','-','.','-','.','-','.'},
                  {'.','-','.','-','.','-','.','-'},
                  {'p','p','p','p','p','p','p','p'},
                  {'r','n','b','q','k','b','n','r'}};

// functions to change the text colur
void white () {
  printf("\033[1;37m");
}

void red () {
  printf("\033[1;31m");
}

void yellow () {
  printf("\033[1;33m");
}
  void cyan () {
  printf("\033[1;36m");
  }
  void green() {
  printf("\033[1;32m");
  }
  void purple () {
  printf("\033[1;35m");
}
  void blue () {
  printf("\033[1;34m");
}
void reset () {
  printf("\033[0m");
}
bool isPresent(int x1,int y1,int turn)
{
    switch (turn)
    {
        case 0:{
            //check if the white piece is present
            for(int i=0;i<strlen(whitepieces);i++){
                if (whitepieces[i]==Board[y1][x1])
                    return true;
                 }
            return false;
            break;
        }
        case 1:{
              //check if the black piece is present
            for(int i=0;i<strlen(blackpieces);i++){
                if (blackpieces[i]==Board[y1][x1])
                    return true;
                 }
            return false;
            break;
        }
    }
    return false;
}





//prints initial board
void printBoard(char b[8][8])
{

   system("cls");

    printf("\n   ");
    yellow();
    printf("\t\t____________________\n\n");
    printf("\t\t ~~~ CHESS GAME ~~~ \n");
     printf("\t\t____________________\n\n");
     reset();
     yellow();
    printf("\t\t   A B C D E F G H \t\t ");
    white();
    printf("White");
    yellow();
    printf("-Moves LOG-");
    cyan();
    printf("Black\n\n");
    reset();
    for (int i=0;i<8;i++)
    {   yellow();
        printf("\t\t%d  ",8-i);
        reset();
        for (int j=0;j<8;j++)
        {
            if (b[i][j]=='k'||b[i][j]=='q'||b[i][j]=='r'||b[i][j]=='n'||b[i][j]=='b'||b[i][j]=='p'||b[i][j]=='-'){
            white();
            printf("%c ",b[i][j]);
            reset();
            }
            else if (b[i][j]=='K'||b[i][j]=='Q'||b[i][j]=='R'||b[i][j]=='N'||b[i][j]=='B'||b[i][j]=='P'||b[i][j]=='.'){
              cyan();
              printf("%c ",b[i][j]);
            reset();
            }
            else {
            printf("%c ",b[i][j]);
            }

        }
        yellow();
        printf(" %d",8-i);
        reset();
        if(turn_num<=16){
           if((turn_num - 2*i >0)&&(turn_num!=0))
            {
           printf("\t\t %c%d%c%d \t",moves[2*i][0]+97,8-moves[2*i][1],moves[2*i][2]+97,8-moves[2*i][3]);
            }
            if((turn_num - 2*i -1>0)&&(turn_num!=0))
            {
            cyan();
            printf("\t %c%d%c%d \t",moves[2*i+1][0]+97,8-moves[2*i+1][1],moves[2*i+1][2]+97,8-moves[2*i+1][3]);
            reset();
            }
        }
       else{
           if((turn_num - 16>0)&&(turn_num!=0))
            {
           printf("\t\t %c%d%c%d \t",moves[2*i+turn*(turn_num-15)+(turn^1)*(turn_num-16)][0]+97,8-moves[2*i+turn*(turn_num-15)+(turn^1)*(turn_num-16)][1],moves[2*i+turn*(turn_num-15)+(turn^1)*(turn_num-16)][2]+97,8-moves[2*i+turn*(turn_num-15)+(turn^1)*(turn_num-16)][3]);
            }
            if((turn_num -16 >0)&&(turn_num!=0)&&((i!=7)||((i==7)&&(turn_num%2==0))))
            {
                cyan();
                printf("\t %c%d%c%d \t",moves[2*i+turn*(turn_num-15)+(turn^1)*(turn_num-16)+1][0]+97,8-moves[2*i+turn*(turn_num-15)+(turn^1)*(turn_num-16)+1][1],moves[2*i+turn*(turn_num-15)+(turn^1)*(turn_num-16)+1][2]+97,8-moves[2*i+turn*(turn_num-15)+(turn^1)*(turn_num-16)+1][3]);
                reset();
            }
        }


        printf("\n");

    }
    yellow();
    printf("\n   ");
    printf("\t\t   A B C D E F G H");
    printf("\n\n");
    yellow();
    printf("Captured from ");
    reset();
    white();
    printf("White: ");
    for (int i=0;i<cW;i++)
        printf("%c ",capturedWhite[i]);
    reset();
    yellow();
    printf("\nCaptured from ");
    reset();
    cyan();
    printf("Black: ");
    for (int i=0;i<cB;i++)
        printf("%c ",capturedBlack[i]);
    reset();

 printf("\n");
}





int check(int x,int y, int turn)
{
    int checked=0; //flag to tell if that square is threatened
    int encounteredPieceUp=0;
    int encounteredPieceDown=0;
    int encounteredPieceRight=0;
    int encounteredPieceLeft=0;
    int encounteredPieceUpRight=0;
    int encounteredPieceUpLeft=0;
    int encounteredPieceDownRight=0;
    int encounteredPieceDownleft=0;

    threatX=-1;
    threatY=-1;
    //flag to stop checking if found a piece in the way
    //to check whether that square is checked
    //to check if it is threatened
    switch(turn)
    {
    case 0:
        {
         //case 0 means white
         //search for capital enemies
         for (int i=1;i<8;i++)
         {
             for(int j=1;j<8;j++)
             {
                 //checking lower right quarter
                 if((x+i<8)&&(y+j<8))
                 {

                     //checking lower right diagonally
                     if((isalpha(Board[y+j][x+i]))&&(i==j)&&(!encounteredPieceDownRight))
                           {
                               encounteredPieceDownRight=1;
                                //check for queen or bishop or king
                                if ((Board[y+j][x+j]=='Q')||(Board[y+j][x+j]=='B')||(Board[y+1][x+1]=='K'))
                                {

                                    if(((checkingSacrifice)&&(Board[y+1][x+1]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x+i;
                                        threatY=y+j;
                                    }

                                }
                           }
                    //checking that vertical line below
                    if((isalpha(Board[y+j][x]))&&(!encounteredPieceDown)){
                        encounteredPieceDown=1;
                        //check for queen or rook
                        if ((Board[y+j][x]=='Q')||(Board[y+j][x]=='R')||(Board[y+1][x]=='K'))
                                {

                                    if(((checkingSacrifice)&&(Board[y+1][x]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x;
                                        threatY=y+j;
                                    }

                                }
                    }

                    //checking horizontal line to the right
                    if((isalpha(Board[y][x+i]))&&(!encounteredPieceRight))
                    {
                        encounteredPieceRight=1;
                        //check for queen or rook or king
                        if ((Board[y][x+i]=='Q')||(Board[y][x+i]=='R')||(Board[y][x+1]=='K'))
                                {

                                    if(((checkingSacrifice)&&(Board[y][x+1]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x+i;
                                        threatY=y;
                                    }
                                }
                    }
                    //check for knights in that quarter
                    if((x+2<8)&&(y+1<8))
                    {
                        if((Board[y+1][x+2]=='N')&&(j==1)&&(i==2))
                        {
                            checked+=1;
                            threatX=x+2;
                            threatY=y+1;
                        }
                    }
                    if((x+1<8)&&(y+2<8))
                    {
                        if((Board[y+2][x+1]=='N')&&(j==2)&&(i==1))
                        {
                            checked+=1;
                            threatX=x+1;
                            threatY=y+2;
                        }
                    }
                 }//end of lower right

                 /////////////////////////////////////////////////


                 //checking upper right quarter (y decrease,x increase)
                 if((x+i<8)&&(y-j>=0))
                 {
                     //check diagonally
                     if(((isalpha(Board[y-j][x+i])))&&(i==j)&&(!encounteredPieceUpRight))
                     {
                         encounteredPieceUpRight=1;
                         //check for queen or bishop or king or pawn
                         if((Board[y-j][x+i]=='Q')||(Board[y-j][x+i]=='B')||(Board[y-1][x+1]=='K')||((!checkingSacrifice)&&(Board[y-1][x+1]=='P')))
                         {
                             if(((checkingSacrifice)&&(Board[y-1][x+1]!='K'))||(!checkingSacrifice))
                             {
                                checked+=1;
                                threatX=x+i;
                                threatY=y-j;
                             }

                         }
                     }
                     //check upper vertical part
                     if((isalpha(Board[y-j][x]))&&(!encounteredPieceUp))
                     {
                         encounteredPieceUp=1;
                         //check for rook or queen or king
                         if((Board[y-j][x]=='Q')||(Board[y-j][x]=='R')||(Board[y-1][x]=='K')||((checkingSacrifice)&&((Board[y-1][x]=='P'))))
                         {
                             if(((checkingSacrifice)&&(Board[y-1][x]!='K'))||(!checkingSacrifice))
                             {
                                checked+=1;
                                    threatX=x;
                                    threatY=y-j;
                             }

                         }
                     }
                     //checking horizontal line to the right
                     if((isalpha(Board[y][x+i]))&&(!encounteredPieceRight))
                     {
                        encounteredPieceRight=1;
                        //check for queen or rook
                        if ((Board[y][x+i]=='Q')||(Board[y][x+i]=='R')||(Board[y][x+1]=='K'))
                                {
                                    if(((checkingSacrifice)&&(Board[y][x+1]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x+i;
                                        threatY=y;
                                    }

                                }
                     }
                     //check for knights in that quarter
                    if((x+2<8)&&(y-1>=0))
                    {
                        if((Board[y-1][x+2]=='N')&&(j==1)&&(i==2))
                        {
                            checked+=1;
                            threatX=x+2;
                            threatY=y-1;
                        }
                    }
                    if((x+1<8)&&(y-2>=0))
                    {
                        if((Board[y-2][x+1]=='N')&&(j==2)&&(i==1))
                        {
                            checked+=1;
                            threatX=x+1;
                            threatY=y-2;
                        }
                    }
                 }//end of upper right
                 //////////////////////////////////////////////

                 //Checking lower left quarter(x decrease,y increase)
                 if((x-i>=0)&&(y+j<8))
                 {
                     //checking lower left diagonally
                     if((isalpha(Board[y+j][x-i]))&&(i==j)&&(!encounteredPieceDownleft))
                           {
                               encounteredPieceDownleft=1;
                                //check for queen or bishop or king
                                if ((Board[y+j][x-j]=='Q')||(Board[y+j][x-j]=='B')||(Board[y+1][x-1]=='K'))
                                {
                                    if(((checkingSacrifice)&&(Board[y+1][x-1]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x-i;
                                        threatY=y+j;
                                    }
                                }
                           }
                    //checking that vertical line below
                    if((isalpha(Board[y+j][x]))&&(!encounteredPieceDown)){
                        //check for queen or rook
                        encounteredPieceDown=1;
                        if ((Board[y+j][x]=='Q')||(Board[y+j][x]=='R')||(Board[y+1][x]=='K'))
                                {
                                    if(((checkingSacrifice)&&(Board[y+1][x]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x;
                                        threatY=y+j;
                                    }
                                }
                    }


                    //checking horizontal line to the left
                    if((isalpha(Board[y][x-i]))&&(!encounteredPieceLeft))
                    {
                        encounteredPieceLeft=1;
                        //check for queen or rook
                        if ((Board[y][x-i]=='Q')||(Board[y][x-i]=='R')||(Board[y][x-1]=='K'))
                                {
                                    if(((checkingSacrifice)&&(Board[y][x-1]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x-i;
                                        threatY=y;
                                    }

                                }
                    }
                    //check for knights in that quarter
                    if((x-2>=0)&&(y+1<8))
                    {
                        if((Board[y+1][x-2]=='N')&&(j==1)&&(i==2))
                        {
                            checked+=1;
                            threatX=x-2;
                            threatY=y+1;
                        }
                    }
                    if((x-1>=0)&&(y+2<8))
                    {
                        if((Board[y+2][x-1]=='N')&&(j==2)&&(i==1))
                        {
                            checked+=1;
                            threatX=x-1;
                            threatY=y+2;
                        }
                    }

                  }//end of lower left


                   ///////////////////////////////////////////////

                   //Checking upper left quarter(x decrease,y decrease)
                 if((x-i>=0)&&(y-j>=0))
                 {
                     //checking upper left diagonally
                     if((isalpha(Board[y-j][x-i]))&&(i==j)&&(!encounteredPieceUpLeft))
                           {
                               encounteredPieceUpLeft=1;
                                //check for queen or bishop or king or pawn
                                if ((Board[y-j][x-j]=='Q')||(Board[y-j][x-j]=='B')||(Board[y-1][x-1]=='K')||((Board[y-1][x-1]=='P')&&(!checkingSacrifice)))
                                {
                                    if(((checkingSacrifice)&&(Board[y-1][x-1]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x-j;
                                        threatY=y-j;
                                    }
                                }
                           }
                    //checking that vertical line above
                    if((isalpha(Board[y-j][x]))&&(!encounteredPieceUp)){
                        encounteredPieceUp=1;
                        //check for queen or rook
                        if ((Board[y-j][x]=='Q')||(Board[y-j][x]=='R')||(Board[y-1][x]=='K')||((checkingSacrifice)&&(Board[y-1][x]=='P')))
                                {
                                    if(((checkingSacrifice)&&(Board[y-1][x]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x;
                                        threatY=y-j;
                                    }
                                }
                    }


                    //checking horizontal line to the left
                    if((isalpha(Board[y][x-i]))&&(!encounteredPieceLeft))
                    {
                        encounteredPieceLeft=1;
                        //check for queen or rook or king
                        if ((Board[y][x-i]=='Q')||(Board[y][x-i]=='R')||(Board[y][x-1]=='K'))
                                {
                                    if(((checkingSacrifice)&&(Board[y][x-1]!='K'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x-i;
                                        threatY=y;
                                    }
                                }
                    }
                    //check for knights in that quarter
                    if((x-2>=0)&&(y-1>=0))
                    {
                        if((Board[y-1][x-2]=='N')&&(j==1)&&(i==2))
                        {
                            checked+=1;
                            threatX=x-2;
                            threatY=y-1;
                        }
                    }
                    if((x-1>=0)&&(y-2>=0))
                    {
                        if((Board[y-2][x-1]=='N')&&(j==2)&&(i==1))
                        {
                            checked+=1;
                            threatX=x-1;
                            threatY=y-2;
                        }
                    }
                  } //end of upper left
             }//end of j counter loop
         }//end of i counter loop
         //end of checking threats by black on white
        //based on checked value we return true if it is threatened and false if not
        return checked;
        break;
        }
    case 1:
        {
         //case 1 means black
         //search for small enemies
         for (int i=1;i<8;i++)
         {
             for(int j=1;j<8;j++)
             {
                 //checking lower right quarter
                 if((x+i<8)&&(y+j<8))
                 {
                     //checking lower right diagonally
                     if((isalpha(Board[y+j][x+i]))&&(i==j)&&(!encounteredPieceDownRight))
                           {
                               encounteredPieceDownRight=1;
                                //check for queen or bishop or king
                                if ((Board[y+j][x+j]=='q')||(Board[y+j][x+j]=='b')||(Board[y+1][x+1]=='k')||((Board[y+1][x+1]=='p')&&(!checkingSacrifice)))
                                {
                                    if(((checkingSacrifice)&&(Board[y+1][x+1]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x+j;
                                        threatY=y+j;
                                    }
                                }
                           }
                    //checking that vertical line below
                    if((isalpha(Board[y+j][x]))&&(!encounteredPieceDown)){
                        encounteredPieceDown=1;
                        //check for queen or rook
                        if ((Board[y+j][x]=='q')||(Board[y+j][x]=='r')||(Board[y+1][x]=='k')||((Board[y+1][x]=='p')&&(checkingSacrifice)))
                                {
                                    if(((checkingSacrifice)&&(Board[y+1][x]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x;
                                        threatY=y+j;
                                    }
                                }
                    }


                    //checking horizontal line to the right
                    if((isalpha(Board[y][x+i]))&&(!encounteredPieceRight))
                    {
                        encounteredPieceRight=1;
                        //check for queen or rook
                        if ((Board[y][x+i]=='q')||(Board[y][x+i]=='r')||(Board[y][x+1]=='k'))
                                {
                                    if(((checkingSacrifice)&&(Board[y][x+1]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x+i;
                                        threatY=y;
                                    }

                                }
                    }
                    //check for knights in that quarter
                    if((x+2<8)&&(y+1<8))
                    {
                        if((Board[y+1][x+2]=='n')&&(j==1)&&(i==2))
                        {
                            checked+=1;
                            threatX=x+2;
                            threatY=y+1;
                        }
                    }
                    if((x+1<8)&&(y+2<8))
                    {
                        if((Board[y+2][x+1]=='n')&&(j==2)&&(i==1))
                        {
                            checked+=1;
                            threatX=x+1;
                            threatY=y+2;
                        }
                    }
                 }//end of lower right

                 /////////////////////////////////////////////////


                 //checking upper right quarter (y decrease,x increase)
                 if((x+i<8)&&(y-j>=0))
                 {
                     //check diagonally
                     if(((isalpha(Board[y-j][x+i])))&&(i==j)&&(!encounteredPieceUpRight))
                     {
                         encounteredPieceUpRight=1;
                         //check for queen or bishop or king or pawn
                         if((Board[y-j][x+i]=='q')||(Board[y-j][x+i]=='b')||(Board[y-1][x+1]=='k'))
                         {
                             if(((checkingSacrifice)&&(Board[y-1][x+1]!='k'))||(!checkingSacrifice))
                             {
                                checked+=1;
                                threatX=x+i;
                                threatY=y-j;
                             }
                         }
                     }
                     //check upper vertical part
                     if((isalpha(Board[y-j][x]))&&(!encounteredPieceUp))
                     {
                         encounteredPieceUp=1;
                         //check for rook or queen or king
                         if((Board[y-j][x]=='q')||(Board[y-j][x]=='r')||(Board[y-1][x]=='k'))
                         {
                             if(((checkingSacrifice)&&(Board[y-1][x]!='k'))||(!checkingSacrifice))
                             {
                                checked+=1;
                                threatX=x;
                                threatY=y-j;
                             }

                         }
                     }
                     //checking horizontal line to the right
                     if((isalpha(Board[y][x+i]))&&(!encounteredPieceRight))
                     {
                         encounteredPieceRight=1;
                        //check for queen or rook
                        if ((Board[y][x+i]=='q')||(Board[y][x+i]=='r')||(Board[y][x+1]=='k'))
                                {
                                    if(((checkingSacrifice)&&(Board[y][x+1]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x+i;
                                        threatY=y;
                                    }
                                }
                     }
                     //check for knights in that quarter
                    if((x+2<8)&&(y-1>=0))
                    {
                        if((Board[y-1][x+2]=='n')&&(j==1)&&(i==2))
                        {
                            checked+=1;
                            threatX=x+2;
                            threatY=y-1;
                        }
                    }
                    if((x+1<8)&&(y-2>=0))
                    {
                        if((Board[y-2][x+1]=='n')&&(j==2)&&(i==1))
                        {
                            checked+=1;
                            threatX=x+1;
                            threatY=y-2;
                        }
                    }
                 }//end of upper right

                 //////////////////////////////////////////////

                 //Checking lower left quarter(x decrease,y increase)
                 if((x-i>=0)&&(y+j<8))
                 {
                     //checking lower left diagonally
                     if((isalpha(Board[y+j][x-i]))&&(i==j)&&(!encounteredPieceDownleft))
                           {
                                encounteredPieceDownleft=1;
                                //check for queen or bishop or king
                                if ((Board[y+j][x-j]=='q')||(Board[y+j][x-j]=='b')||(Board[y+1][x-1]=='k')||((Board[y+1][x-1]=='p')&&(!checkingSacrifice)))
                                {
                                    if(((checkingSacrifice)&&(Board[y+1][x-1]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x-j;
                                        threatY=y+j;
                                    }

                                }
                           }
                    //checking that vertical line below
                    if((isalpha(Board[y+j][x]))&&(!encounteredPieceDown))
                        {
                        encounteredPieceDown=1;
                        //check for queen or rook
                        if ((Board[y+j][x]=='q')||(Board[y+j][x]=='r')||(Board[y+1][x]=='k')||((checkingSacrifice)&&(Board[y+1][x]=='p')))
                                {
                                    if(((checkingSacrifice)&&(Board[y+1][x]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x;
                                        threatY=y+j;
                                    }
                                }
                         }

                    //checking horizontal line to the left
                    if((isalpha(Board[y][x-i]))&&(!encounteredPieceLeft))
                    {
                        encounteredPieceLeft=1;
                        //check for queen or rook
                        if ((Board[y][x-i]=='q')||(Board[y][x-i]=='r')||(Board[y][x-1]=='k'))
                                {
                                    if(((checkingSacrifice)&&(Board[y][x-1]!='k'))||(!checkingSacrifice))
                                    {;
                                        checked+=1;
                                        threatX=x-i;
                                        threatY=y;
                                    }
                                }
                    }
                    //check for knights in that quarter
                    if((x-2>=0)&&(y+1<8))
                    {
                        if((Board[y+1][x-2]=='n')&&(j==1)&&(i==2))
                        {
                            checked+=1;
                            threatX=x-2;
                            threatY=y+1;
                        }
                    }
                    if((x-1>=0)&&(y+2<8))
                    {
                        if((Board[y+2][x-1]=='n')&&(j==2)&&(i==1))
                        {
                            checked+=1;
                            threatX=x-1;
                            threatY=y+2;
                        }
                    }

                  }//end of lower left


                   ///////////////////////////////////////////////

                   //Checking upper left quarter(x decrease,y decrease)
                 if((x-i>=0)&&(y-j>=0))
                 {
                     //checking upper left diagonally
                     if((isalpha(Board[y-j][x-i]))&&(i==j)&&(!encounteredPieceUpLeft))
                           {
                               encounteredPieceUpLeft=1;
                                //check for queen or bishop or king or pawn
                                if ((Board[y-j][x-j]=='q')||(Board[y-j][x-j]=='b')||(Board[y-1][x-1]=='k'))
                                {
                                    if(((checkingSacrifice)&&(Board[y-1][x-1]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x-j;
                                        threatY=y-j;
                                    }
                                }
                           }
                    //checking that vertical line above
                    if((isalpha(Board[y-j][x]))&&(!encounteredPieceUp)){
                        encounteredPieceUp=1;
                        //check for queen or rook
                        if ((Board[y-j][x]=='q')||(Board[y-j][x]=='r')||(Board[y-1][x]=='k'))
                                {
                                    if(((checkingSacrifice)&&(Board[y-1][x]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x;
                                        threatY=y-j;
                                    }
                                }
                        }
                    //checking horizontal line to the left
                    if((isalpha(Board[y][x-i]))&&(!encounteredPieceLeft))
                    {
                        encounteredPieceLeft=1;
                        //check for queen or rook
                        if ((Board[y][x-i]=='q')||(Board[y][x-i]=='r')||(Board[y][x-1]=='k'))
                                {
                                    if(((checkingSacrifice)&&(Board[y][x-1]!='k'))||(!checkingSacrifice))
                                    {
                                        checked+=1;
                                        threatX=x-i;
                                        threatY=y;
                                    }
                                }
                            }
                    //check for knights in that quarter
                    if((x-2>=0)&&(y-1>=0))
                    {
                        if((Board[y-1][x-2]=='n')&&(j==1)&&(i==2))
                        {
                            checked+=1;
                            threatX=x-2;
                            threatY=y-1;
                        }
                    }
                    if((x-1>=0)&&(y-2>=0))
                    {
                        if((Board[y-2][x-1]=='n')&&(j==2)&&(i==1))
                        {
                            checked+=1;
                            threatX=x-1;
                            threatY=y-2;
                        }
                    }

                  } //end of upper left
             }//end of j counter loop
         }//end of i counter loop
         //end of checking threats by black on white

         //based on checked value we return true if it is threatened and false if not
        return checked;
        break;
        }

    }
}

//to check the middle pieces of the straight line path
bool clearPath(int x1,int y1,int x2,int y2,int turn)
{
    int ax,bx,ay,by;
    int foundPiece=0;//flag
    if(x2==x1)//vertical movement
    {
        switch(abs(y2-y1))
        {
        case 7:
            {
                if (isalpha(Board[ ((y2+y1)/2) +3 ][x1]))
                {
                    foundPiece=1;
                }
            }
        case 6:
            {
                if (isalpha(Board[ ((y2+y1)/2) -2 ][x1]))
                {
                    foundPiece=1;
                }
            }
        case 5:
            {
                if (isalpha(Board[ ((y2+y1)/2) +2 ][x1]))
                {
                    foundPiece=1;
                }
            }
        case 4:
            {
                if (isalpha(Board[ ((y2+y1)/2) -1 ][x1]))
                {
                    foundPiece=1;
                }
            }
        case 3:
            {
                if (isalpha(Board[ ((y2+y1)/2) +1 ][x1]))
                {
                    foundPiece=1;
                }
            }
        case 2:
            {
                if (isalpha(Board[(y2+y1)/2][x1]))
                {
                    foundPiece=1;
                }
                break;
            }
        }//end of switch
    }//end of vertical
    else if(y2==y1)//horizontal movement
    {
        switch(abs(x2-x1))
        {
        case 7:
            {
                if (isalpha(Board[y1][((x2+x1)/2) +3 ]))
                {
                    foundPiece=1;
                }
            }
        case 6:
            {
                if (isalpha(Board[y1][((x2+x1)/2) -2 ]))
                {
                    foundPiece=1;
                }
            }
        case 5:
            {
                if (isalpha(Board[y1][((x2+x1)/2) +2 ]))
                {
                    foundPiece=1;
                }
            }
        case 4:
            {
                if (isalpha(Board[y1][((x2+x1)/2) -1 ]))
                {
                    foundPiece=1;
                }
            }
        case 3:
            {
                if (isalpha(Board[y1][((x2+x1)/2) +1]))
                {
                    foundPiece=1;
                }
            }
        case 2:
            {
                if (isalpha(Board[y1][(x2+x1)/2]))
                {
                    foundPiece=1;
                }
                break;
            }
        }//end of switch
    }//end of horizontal

    else if((abs(y2-y1)==abs(x2-x1)))//diagonal movement
    {
        //seeing which is the smaller index
        if(x2>x1)
        {
            ax=x1;
            bx=x2;
        }
        else if(x1>x2)
        {
            ax=x2;
            bx=x1;
        }

        if(y2>y1)
        {
            ay=y1;
            by=y2;
        }
        else if(y1>y2)
        {
            ay=y2;
            by=y1;
        }

        for (int i=1;i<abs(x2-x1);i++)
        {
            if(x2-x1==y2-y1)//increasing or decreasing together
            {
                if(isalpha(Board[ay+i][ax+i]))
                {
                    foundPiece=1;
                }
            }
            else
            {
                if(isalpha(Board[ay+i][bx-i]))
                {
                    foundPiece=1;
                }
            }
        }
    }//end of diagonal
    else
    {
    foundPiece=1;
    //extra check
    }


    //returning part
    switch(foundPiece)
    {
    case 0:
        {
            return true;
            break;
        }
    case 1:
        {
            return false;
            break;
        }
    default:
        {
            return false;
            break;
        }
    }
}

bool ClearPath_castling(int x1,int y1,int x2,int y2,int turn)
{
    // check the move of castling
    if ((Board[y1][x1]=='k'||Board[y1][x1]=='K')&&(x2==6 || x2==2)&&(y2==0||y2==7)&& (y1==y2))
        {
    int foundPiece=0;//flag
    if(y2==y1&&(abs(x2-x1)==2))
    {
         // check that there isn't any piece between them
          if (isalpha(Board[y1][(x2+x1)/2]))
          {
              foundPiece=1;
          }
    }

    //returning part
    switch(foundPiece)
    {
        //checking if any square between them is threatened or not
    case 0:
        {
         if ((!check((x1+x2)/2,y1,turn))&&(!check(x2,y2,turn))){
            return true;
         }
           else {
            return false ;
         }
         break;
        }

    case 1:
        {
            return false;
            break;
        }
    default:
        {
            return false;
            break;
        }
    }
}
 else {
    return false ;
 }
}

bool castling(int x1,int y1,int x2,int y2,int turn)
{
// check that was the first move to the king and the rook
    if (ClearPath_castling(x1,y1,x2,y2,turn)&&x1==4){


           switch (turn)
        {

            case 0 :
            {
                switch(x2)
                {
                    case 6:
                    {
                        if(!k &&!r2 ){

                        symbol[turn_num] = '.' ;
                      return true ;
                       break;
                        }
                       else{
                        return false ;
                        break;
                       }

                    }
                    case 2 :
                    {
                     if(!k && !r1){
                        symbol[turn_num] = '.' ;

                     return true ;
                       break;
                     }
                      else{
                        return false ;
                        break;
                       }
                    }
                    default :
                        {
                            return false ;
                            break;
                        }
                }
                break;
            }
            case 1 :
            {
                switch(x2)
                {
                    case 6 :
                    {
                        if(!K&&!R2){
                        symbol[turn_num] = '.' ;

                     return true ;
                       break;
                    }
                     else{
                        return false ;
                        break;
                       }
                    }
                    case 2 :
                    {
                        if(!K&&!R1){
                         symbol[turn_num] = '.' ;

                    return true ;
                       break;
                         }
                          else{
                        return false ;
                        break;
                       }
                    }
                    default :
                        {
                            return false ;
                            break;
                        }
                }
                break ;
            }

        }
    }
    else{
        return false ;
    }

}

bool Valid_promotion(int x1,int y1 ,int x2,int y2,int turn){
 // check that the pawn in the square before the last one and the move is to promote
    switch (turn){
     case 0 :
        {
            if ((Board[y1][x1]== 'p')&& (y2 == 0)&&y1==1){
                return true ;
            }
            else{
                return false ;
            }
                break;
            }
     case 1 :
        {
           if ((Board[y1][x1]== 'P')&& (y2 == 7)&&y1==6){
                return true ;
            }
            else{
                return false ;
            }
                break;
        }
     default :
         {
        return false ;
        break ;
    }
    }


}

void promotion_Move(int x1,int y1 ,int x2,int y2,int turn,char z)
{
    // doing the promotion move and put the piece that the user entered
      symbol[turn_num]='*';

     switch(turn)
     {
     case 0 :
           if (z=='q' || z == 'Q'){
                Board[y2][x2] ='q';
           }
            else if (z== 'n' || z== 'N'){

                Board[y2][x2] ='n';
            }
             else if (z== 'r' || z== 'R'){

                Board[y2][x2] ='r';
            }
              else if (z== 'b' || z== 'B'){

                Board[y2][x2] ='b';
            }

        break ;
     case 1 :
               if (z=='q' || z == 'Q'){
                Board[y2][x2] ='Q';
           }
            else if (z== 'n' || z== 'N'){

                Board[y2][x2] ='N';
            }
             else if (z== 'r' || z== 'R'){

                Board[y2][x2] ='R';
            }
              else if (z== 'b' || z== 'B'){

                Board[y2][x2] ='B';
            }

        break ;
     }
}

void Check_enpassant(int x1,int y1 ,int x2,int y2,int turn)
{
    //check the pawn moved two squares and there was a enemy pawn in the square beside
    // store the symbol '|' to do the enpassant move in the function generatemove
    switch(turn)
    {
    case 0 :
        if((Board[y1][x1]=='p' && y1==6 && y2 == 4 )&&(Board[y2][x2+1]=='P'||Board[y2][x2-1]=='P')){
          symbol[turn_num]='|';
        }
        break ;
    case 1 :
         if((Board[y1][x1]=='P' && y1==1 && y2 == 3 )&&(Board[y2][x2+1]=='p'||Board[y2][x2-1]=='p')){
            symbol[turn_num]='|';
        }
        break ;
    default :

        break;

    }

}


bool enpassant_capture(int x1,int y1 ,int x2,int y2,int turn){
    // checking that the move was enpassant and the symbol in the previous turn was '|'
    // change the flag tha indicate the capture was from right or left
     switch(turn)
        {
        case 0 :
            if (Board[y1][x1]=='p'&& y1== 3&& y2 == 2 && (x2 == x1-1) && symbol[turn_num-1]=='|' && Board[y1][x1-1]== 'P')
                {
                enpassantRight = 1;
                return true ;
                }
            else if (Board[y1][x1]=='p'&& y1== 3 && y2 == 2 && (x2 == x1+1) && symbol[turn_num-1]=='|' && Board[y1][x1+1]== 'P')
                {
                enpassantLeft = 1;
                return true ;
                }
            else{
                return false ;
                }
        break ;
    case 1 :

         if (Board[y1][x1]=='P'&& y1== 4&& y2 == 5 && (x2 == x1-1) && symbol[turn_num-1]=='|' && Board[y1][x1-1]== 'p')
            {
                enpassantRight = 1;
                return true ;
            }
        else if (Board[y1][x1]=='P'&& y1== 4 && y2 == 5 && (x2 == x1+1) && symbol[turn_num-1]=='|' && Board[y1][x1+1]== 'p')
            {
                enpassantLeft = 1;
                return true ;
            }
        else{
            return false ;
            }
        break ;

        default :
            return false ;
            break;
            }
}



//checks the piece in that position whether the move lies within its possible boundaries regardless of any other piece
//in the destination or in the way to it (will be checked later)
bool isMovable(int x1,int y1,int x2,int y2, int turn,char z)
{
    switch (turn)
    {
        case 1:{
            switch(Board[y1][x1])
            {
            case 'K':
                {
                    if ((((x1==x2)&&(abs(y2-y1)==1))||((y1==y2)&&(abs(x2-x1)==1))||((abs(x2-x1)==1)&&(abs(y2-y1)==1))|| (castling(x1,y1,x2,y2,turn)))&&((!(isPresent(x2,y2,turn)))&&(!check(x2,y2,turn))))
                    {
                            //if there is a vacancy around the king we check if the new place is threatened
                            return true;
                            break;

                    }
                    else{
                        return false;
                        break;
                    }


                    break;
                }
            case 'Q':
                {


                    //check if queen is movable
                    if((x2==x1)||(y2==y1) || (abs(x2-x1) == abs(y2-y1)))
                        {
                            if(!isPresent(x2,y2,turn))
                            {
                                if(clearPath(x1,y1,x2,y2,turn))
                                {
                                    return true;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }

                        }
                    else
                    {
                        return false;
                    }
                    break;
                }
            case 'B':
                {
                    //check if bishop is movable
                    if ((abs(x2-x1) == abs(y2-y1))&&(x1!=x2))
                    {
                            if(!isPresent(x2,y2,turn))
                            {
                                if(clearPath(x1,y1,x2,y2,turn))
                                {
                                    return true;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }

                        }
                    else
                    {
                        return false;
                    }
                    break;
                }
            case 'N':
                {
                    //check if knight is movable
                    if (((abs(x2-x1)==2)&&(abs(y2-y1)==1))||((abs(x2-x1)==1)&&(abs(y2-y1)==2)))
                    {
                        if(!isPresent(x2,y2,turn))
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }

                    }
                    else
                    {
                        return false;
                    }

                    break;
                }
            case 'R':
                {
                    //check if rook is movable
                    if ((x2==x1)||(y2==y1))
                    {
                            if(!isPresent(x2,y2,turn))
                            {
                                if(clearPath(x1,y1,x2,y2,turn))
                                {
                                    return true;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }

                        }
                    else
                    {
                        return false;
                    }
                    break;
                }

            case 'P':
                {
                    //check if pawn is movable
                    if(((((y2-y1==1)&&(x1==x2)&&(!isalpha(Board[y2][x2])))||((y2-y1==1)&&((abs(x2-x1)==1)&&(isPresent(x2,y2,turn^1)))))&&(!Valid_promotion(x1,y1,x2,y2,turn)))
                       ||((((y2-y1==1)&&(x1==x2)&&(!isalpha(Board[y2][x2])))||((y2-y1==1)&&((abs(x2-x1)==1)&&(isPresent(x2,y2,turn^1)))))&&((Valid_promotion(x1,y1,x2,y2,turn))&&(z=='q'||z=='Q'||z=='r'||z=='R'||z=='n'||z=='N'||z=='b'||z=='B'))
                          ||((x2==x1)&&(y2==3)&&(y1==1)&&(!isalpha(Board[y2][x2]))))|| enpassant_capture(x1,y1,x2,y2,turn))
                        {
                            return true;
                        }
                    else
                    {
                        return false;
                    }
                    break;
                }
            }
            break;

        }
        case 0:
            {
            switch(Board[y1][x1])
            {
            case 'k':
                {
                    if ((((x1==x2)&&(abs(y2-y1)==1))||((y1==y2)&&(abs(x2-x1)==1))||((abs(x2-x1)==1)&&(abs(y2-y1)==1))|| (castling(x1,y1,x2,y2,turn)))&&((!(isPresent(x2,y2,turn)))&&(!check(x2,y2,turn))))
                    {
                            //if there is a vacancy around the king we check if the new place is threatened
                            return true;
                    }
                    else{
                            return false;
                        }
                    break;
                }
            case 'q':
                {
                    //check if queen is movable

                    if((x2==x1)||(y2==y1) || (abs(x2-x1) == abs(y2-y1)))
                        {
                            if(!isPresent(x2,y2,turn))
                            {
                                if(clearPath(x1,y1,x2,y2,turn))
                                {
                                    return true;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }

                        }
                    else
                    {
                        return false;
                    }
                    break;
                }
            case 'b':
                {
                    //check if bishop is movable
                    if ((abs(x2-x1)==abs(y2-y1))&&(x1!=x2))
                    {
                            if(!isPresent(x2,y2,turn))
                            {
                                if(clearPath(x1,y1,x2,y2,turn))
                                {
                                    return true;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }

                        }
                    else
                    {
                        return false;
                    }
                    break;
                }
            case 'n':
                {
                    //check if knight is movable
                    if (((abs(x2-x1)==2)&&(abs(y2-y1)==1))||((abs(x2-x1)==1)&&(abs(y2-y1)==2)))
                    {;
                        if(!isPresent(x2,y2,turn))
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }

                    }
                    else
                    {
                        return false;
                    }

                    break;
                }
            case 'r':
                {
                    //check if rook is movable
                    if ((x2==x1)||(y2==y1))
                    {
                            if(!isPresent(x2,y2,turn))
                            {
                                if(clearPath(x1,y1,x2,y2,turn))
                                {
                                    return true;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }

                        }
                    else
                    {
                        return false;
                    }
                    break;
                }

            case 'p':
                {
                    //check if pawn is movable
                    if (((((y1-y2==1)&&(x1==x2)&&(!isalpha(Board[y2][x2])))||((y1-y2==1)&&((abs(x2-x1)==1)&&(isPresent(x2,y2,turn^1)))))&&(!Valid_promotion(x1,y1,x2,y2,turn)))
                        ||((((y1-y2==1)&&(x1==x2)&&(!isalpha(Board[y2][x2])))||((y1-y2==1)&&((abs(x2-x1)==1)&&(isPresent(x2,y2,turn^1)))))&&((Valid_promotion(x1,y1,x2,y2,turn))&&(z=='q'||z=='Q'||z=='r'||z=='R'||z=='n'||z=='N'||z=='b'||z=='B')))
                        ||((x2==x1)&&(y2==4)&&(y1==6)&&(!isalpha(Board[y2][x2])))||enpassant_capture(x1,y1,x2,y2,turn))
                    {

                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                }
            }//end of white switch
            break;
        }//end of case white 0
    }//end of major switch

return true;
}//end of function

//searches for the king where ever he is
//checks if his square is threatened
//if it is threatened no other move can be done
bool kingcheck(int x1,int y1,int x2,int y2,int turn)
{

    switch (turn)
    {
 case 0:
    {
       for(int y=0;y<8;y++)
       {
           for(int x=0;x<8;x++)
           {
               if(Board[y][x]=='k')//found the place of the white king
               {
                       //to escape
                       if((Board[y1][x1]=='k'))
                       {
                            Board[y1][x1]='-';
                          if (!(check(x2,y2,turn))){
                              Board[y1][x1]='k';
                             return false;
                              }
                              else {
                         Board[y1][x1]='k';
                         kingcheckflag=1;
                         return true ;
                              }
                       }
                       //do undo the move
                       //case of sacrifice
                       else if(Board[y1][x1]!='k')
                       {
                           //we will try to make the move and reverse it
                           char tempMoved;
                           char tempCaptured;
                           tempMoved=Board[y1][x1];
                           tempCaptured=Board[y2][x2];

                           Board[y2][x2]=Board[y1][x1];
                           Board[y1][x1]='=';
                           if(check(x,y,turn))
                           {
                               //REVERSE
                               Board[y2][x2]=tempCaptured;
                               Board[y1][x1]=tempMoved;
                                kingcheckflag=1;
                               return true;
                           }
                           else
                           {
                               //REVERSE
                               Board[y2][x2]=tempCaptured;
                               Board[y1][x1]=tempMoved;
                               return false;
                           }
                       }
                      else
                       {
                           kingcheckflag=1;
                           return true;
                       }

                   break;
               }
           }
       }
       return false;
       break;//reaching here means that when king was found it was not threatened
    }//end of case 0 (white)

 case 1:
    {
       for(int y=0;y<8;y++)
       {
           for(int x=0;x<8;x++)
           {
               if(Board[y][x]=='K')//found the place of the black king
               {
                       if((Board[y1][x1]=='K'))
                       {
                             Board[y1][x1]='-';
                          if (!(check(x2,y2,turn))){
                              Board[y1][x1]='K';
                             return false;
                              }
                              else {
                         Board[y1][x1]='K';
                         kingcheckflag=1;
                         return true ;
                              }
                       }
                       //do undo the move
                       //case of sacrifice
                       else if(Board[y1][x1]!='K')
                       {
                           //we will try to make the move and reverse it
                           char tempMoved;
                           char tempCaptured;
                           tempMoved=Board[y1][x1];
                           tempCaptured=Board[y2][x2];

                           Board[y2][x2]=Board[y1][x1];
                           Board[y1][x1]='=';
                           if(check(x,y,turn))
                           {
                               //REVERSE
                               Board[y2][x2]=tempCaptured;
                               Board[y1][x1]=tempMoved;
                               kingcheckflag=1;
                               return true;
                           }
                           else
                           {
                               //REVERSE
                               Board[y2][x2]=tempCaptured;
                               Board[y1][x1]=tempMoved;
                               return false;
                           }
                       }
                       else
                       {
                           kingcheckflag=1;
                           return true;
                       }

                   break;
               }
           }
       }
       return false;
       break;//reaching here means that when king was found it was not threatened
    }
    }//end of function
}


//to generate moves and captures
void generateMove(int x1,int y1,int x2,int y2,int turn,char z)
{
    char temp;
    temp=Board[y1][x1];
    if (isPresent(x2,y2,turn^1))//if there is a piece in the destined position
    {
        switch(turn)
        {
        case 0://white turn,captured black
            {
                capturedBlack[cB++]=Board[y2][x2];
                captured[turn_num]=Board[y2][x2];
                break;
            }//end of case 0
        case 1://Black turn,captured white
            {
                capturedWhite[cW++]=Board[y2][x2];
                captured[turn_num]=Board[y2][x2];
                break;
            }//end of case 1
        }//end of switch
    }
    else{
        captured[turn_num]='-';

    }
    symbol[turn_num]='-';
    switch (turn){
        case 1 :
    if( enpassant_capture(x1,y1,x2,y2,turn) && symbol[turn_num-1]=='|'){
        if(enpassantLeft ==1 ){
          symbol[turn_num]='@';
          capturedWhite[cW++]='p';
         if ((y1+(x1+1))%2==1)
         Board[y1][x1+1]='.';
         else if (((y1+(x1+1))%2==0))
         Board[y1][x1+1]='-';
        }
        else if (enpassantRight ==1 ){
            symbol[turn_num]='+';
            capturedWhite[cW++]='p';

         if ((y1+(x1-1))%2==1)
         Board[y1][x1-1]='.';
         else if (((y1+(x1-1))%2==0))
         Board[y1][x1-1]='-';
        }
            }
       enpassantLeft =0;    // make the flags 0 if if the user did not make the enpassant movement
       enpassantRight =0;
       break;
         case 0:
           if( enpassant_capture(x1,y1,x2,y2,turn)&& symbol[turn_num-1]=='|'){
         if(enpassantLeft ==1 ){
          symbol[turn_num]='@';
          capturedBlack[cB++]= 'P';
          if ((y1+(x1+1))%2==1)
          Board[y1][x1+1]='.';
          else if (((y1+(x1+1))%2==0))
          Board[y1][x1+1]='-';
         }
         else if (enpassantRight ==1 ){
          symbol[turn_num]='+';
          capturedBlack[cB++]='P';
          if ((y1+(x1-1))%2==1)
          Board[y1][x1-1]='.';
          else if (((y1+(x1-1))%2==0))
          Board[y1][x1-1]='-';
         }
     }
      enpassantLeft=0;
      enpassantRight=0;
      break ;
    }
    Check_enpassant(x1, y1, x2, y2,turn);
    if(castling(x1,y1,x2,y2,turn)){
        switch (turn)
        {
            case 0 :
            {
                switch(x2)
                {
                    case 6:
                    {
                        if(!k &&!r2 ){
                       Board[7][5]= 'r' ;
                       Board[7][7]= '-';
                       break;
                    }
                    }
                    case 2 :
                    {
                     if(!k && !r1){
                       Board[7][3]='r';
                       Board[7][0]='.';

                       break;
                     }
                    }
                    default :
                        {
                            break;
                        }
                }
                break;
            }
            case 1 :
            {
                switch(x2)
                {
                    case 6 :
                    {
                        if(!K&&!R2){
                       Board[0][5]='R' ;
                       Board[0][7]= '.';
                       break;
                    }
                    }
                    case 2 :
                    {
                         if(!K&&!R1){
                       Board[0][3]='R' ;
                       Board[0][0]= '-';
                       break;
                         }
                    }
                    default :
                        {
                            break;
                        }
                }
            break;
            }
        }
    }


// if the king or rook was moved its flag will change and that to not to make the castling if any one of them was moved
 if(Board[y1][x1]=='k' && k == 0 &&(symbol[turn_num]!='.') ){

    symbol[turn_num]= '1';
        k = 1 ;
    }
    if(Board[y1][x1]=='K' && K==0&&(symbol[turn_num]!='.')){
        K = 1 ;
        symbol[turn_num]='2';
    }
    if(x1==0&&y1==0 && R1==0 &&(symbol[turn_num]!='.')){
        R1=1 ;
        symbol[turn_num]= '3';
    }
     if(x1==7&&y1==0&&R2==0&&(symbol[turn_num]!='.')){
        R2=1 ;
        symbol[turn_num]= '4';
    }
     if(x1==0&&y1==7&&r1==0&&(symbol[turn_num]!='.')){
        r1=1 ;
        symbol[turn_num]= '5';
    }
    if(x1==7&&y1==7&&r2==0&&(symbol[turn_num]!='.')){
        r2=1 ;
        symbol[turn_num]='6';
    }

  if (Valid_promotion(x1, y1, x2, y2,turn))
  {
      promotion_Move(x1, y1, x2, y2,turn,z);
  }
  else{

    Board[y2][x2]=temp;
  }
    if ((y1+x1)%2==1)
        Board[y1][x1]='.';
    else if ((y1+x1)%2==0)
        Board[y1][x1]='-';

    return;
}

//arguments must be king's coordinates
bool trapped(int x,int y,int turn)
{
    int blockedDownRight=0;
    int blockedDownleft=0;
    int blockedDown=0;
    int blockedRight=0;
    int blockedLeft=0;
    int blockedUpRight=0;
    int blockedUp=0;
    int blockedUpLeft=0;
    char temp;
    //considering we moved the king
    temp=Board[y][x];
    Board[y][x]='=';

        //left
        if(x-1>=0)
        {
            if(y-1>=0)//up
            {
                if(isPresent(x-1,y-1,turn)||check(x-1,y-1,turn))
                {
                    blockedUpLeft=1;
                }
            }
            if(y+1<8)//down
            {
                if(isPresent(x-1,y+1,turn)||check(x-1,y+1,turn))
                {
                    blockedDownleft=1;
                }
            }
            if((y<8)&&(y>=0))//horizontally left
            {
                if(isPresent(x-1,y,turn)||check(x-1,y,turn))
                    {
                        blockedLeft=1;
                    }
            }
        }//end of left

        if(x+1<8)//right
        {
            if(y-1>=0)//up
            {
                if(isPresent(x+1,y-1,turn)||check(x+1,y-1,turn))
                {
                    blockedUpRight=1;
                }
            }
            if(y+1<8)//down
            {
                if(isPresent(x+1,y+1,turn)||check(x+1,y+1,turn))
                {
                    blockedDownRight=1;
                }
            }
            if((y<8)&&(y>=0))//vertically right
            {
                if(isPresent(x+1,y,turn)||check(x+1,y,turn))
                    {
                        blockedRight=1;
                    }
            }
        }//end of right
        if(y-1>=0) //vertical up
        {
            if(isPresent(x,y-1,turn)||check(x,y-1,turn))
            {
                blockedUp=1;
            }
        }
        if(y+1<8) //vertical down
        {
            if(isPresent(x,y+1,turn)||check(x,y+1,turn))
            {
                blockedDown=1;
            }
        }//end of vertical up

        Board[y][x]=temp;

        //if it is trapped then return true
        if( (x!=0)&&(x!=7)&&(y!=0)&&(y!=7) )
        {
            if(blockedDown&&blockedDownleft&&blockedDownRight&&blockedLeft&&blockedRight&&blockedUp&&blockedUpLeft&&blockedUpRight)
            {
                return true;
            }

        }
        else if((x==7)&&(y!=0)&&(y!=7)) //it can't go right
        {
            if(blockedDown&&blockedDownleft&&blockedLeft&&blockedUp&&blockedUpLeft)
            {
                return true;
            }
        }
        else if((y==7)&&(x!=0)&&(x!=7)) //can't go down
        {
            if(blockedLeft&&blockedRight&&blockedUp&&blockedUpLeft&&blockedUpRight)
            {
                return true;
            }
        }
        else if((y!=0)&&(x==0)&&(y!=7)) //can't go left
        {
            if(blockedDown&&blockedDownRight&&blockedRight&&blockedUp&&blockedUpRight)
            {
                return true;
            }
        }
        else if((y==0)&&(x!=0)&(x!=7))//can't go up
        {
            if(blockedDown&&blockedDownleft&&blockedDownRight&&blockedLeft&&blockedRight)
            {
                return true;
            }
        }
        else if((x==0)&&(y==0)) //can't go up or left
        {
            if(blockedDown&&blockedDownRight&&blockedRight)
            {
                return true;
            }
        }
        else if((x==7)&&(y==0))//can't go up or right
        {
            if(blockedDown&&blockedDownleft&&blockedLeft)
            {
                return true;
            }
        }
        else if((x==7)&&(y==7))//can't go down or right
        {
            if(blockedLeft&&blockedUp&&blockedUpLeft)
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    return false;
}

//function to check possibility of sacrifice
bool sacrifice(int xk,int yk,int xt,int yt,int turn)
{
    int saved=0;//flag
    int ax,ay,bx,by;
    checkingSacrifice=1;
    if(xk==xt)//vertical movement
    {
        switch(abs(yt-yk))
        {
        case 7:
            {
                if (check( xk,((yk+yt)/2)+3 , turn^1 ))
                {
                    saved=1;
                }
            }
        case 6:
            {
                if (check( xk,((yk+yt)/2)-2 , turn^1 ))
                {
                    saved=1;
                }
            }
        case 5:
            {
                if (check( xk,((yk+yt)/2) +2 , turn^1 ))
                {
                    saved=1;
                }
            }
        case 4:
            {
                if (check( xk,((yk+yt)/2)-1 , turn^1 ))
                {
                    saved=1;
                }
            }
        case 3:
            {
                if (check( xk,((yk+yt)/2)+1 , turn^1 ))
                {
                    saved=1;
                }
            }
        case 2:
            {
                if (check( xk,((yk+yt)/2) , turn^1 ))
                {
                    saved=1;
                }
                break;
            }
        }//end of switch
    }//end of vertical
    else if(yk==yt)//horizontal movement
    {
        switch(abs(xk-xt))
        {
        case 7:
            {
                if (check( ((xk+xt)/2) +3 , yk , turn^1 ))
                {
                    saved=1;
                }
            }
        case 6:
            {
                if (check( ((xk+xt)/2) -2 , yk , turn^1 ))
                {
                    saved=1;
                }
            }
        case 5:
            {
                if (check( ((xk+xt)/2) +2 , yk , turn^1 ))
                {
                    saved=1;
                }
            }
        case 4:
            {
                if (check( ((xk+xt)/2) -1 , yk , turn^1 ))
                {
                    saved=1;
                }
            }
        case 3:
            {
                if (check( ((xk+xt)/2) +1 , yk , turn^1 ))
                {
                    saved=1;
                }
            }
        case 2:
            {
                if (check( ((xk+xt)/2) +3 , yk , turn^1 ))
                {
                    saved=1;
                }
                break;
            }
        }//end of switch
    }//end of horizontal

    else if(abs(yk-yt)==abs(xk-xt))//diagonal movement
    {
        //seeing which is the smaller index
        if(xt>xk)
        {
            ax=xk;
            bx=xt;
        }
        else if(xk>xt)
        {
            ax=xt;
            bx=xk;
        }

        if(yt>yk)
        {
            ay=yk;
            by=yt;
        }
        else if(yk>yt)
        {
            ay=yt;
            by=yk;
        }

        for (int i=1;i<abs(xt-xk);i++)
        {
            if(xt-xk==yt-yk)//increasing or decreasing together
            {
                if(check(ax+i,ay+i,turn^1))
                {
                    saved=1;
                }
            }
            else
            {
                if(check(bx-i,ay+i,turn^1))
                {
                    saved=1;
                }
            }
        }
    }//end of diagonal
    else
    {
    saved=1;
    //extra check
    }

     //returning part
    switch(saved)
    {
    case 0:
        {
            checkingSacrifice=0;
            return false;
            break;
        }
    case 1:
        {
            checkingSacrifice=0;
            return true;
            break;
        }
    default:
        {
            checkingSacrifice=0;
            return false;
            break;
        }
    }//end of switch
    }//end of function

bool stalemate(int turn)
{   int xking;
    int yking ;
    int foundK=0;
    int foundk=0;
    int foundN=0;
    int foundn=0;
    int foundB=0;
    int foundb=0;
    int even=0; //sum of x and y even for white bishop
    int EVEN=0; //sum of x and y even for black bishop
    int odd=0; //sum of x and y odd for white bishop
    int ODD=0; //sum of x and y odd for black bishop
    //sum of x and y even means white square
    //sum of x and y odd means black square

    //code to check the available pieces
    if((cB>=14)&&(cW>=14))
    {
        for(int j=0;j<8;j++)
        {
            for(int i=0;i<8;i++)
            {
                if(Board[j][i]=='k')
                {
                    foundk=1;
                }
                else if(Board[j][i]=='n')
                {
                    foundn=1;
                }
                else if(Board[j][i]=='b')
                {
                    foundb=1;
                    if((i+j)%2==0)
                        even=1;
                    else if((i+j)%2==1)
                        odd=1;
                }
                else if(Board[j][i]=='K')
                {
                    foundK=1;
                }
                else if(Board[j][i]=='N')
                {
                    foundN=1;
                }
                else if(Board[j][i]=='B')
                {
                    foundB=1;
                    if((i+j)%2==0)
                        EVEN=1;
                    else if((i+j)%2==1)
                        ODD=1;
                }
            }
        }
        //end of looping
        if( (((cW==15)&&(foundk))&&((cB==15)&&(foundK))) //king vs king
          ||(((cW==14)&&(foundk)&&(foundn))&&((cB==15)&&(foundK)))  //king&knight vs king
          ||(((cW==15)&&(foundk))&&((cB==14)&&(foundK)&&(foundN)))  //king vs king&knight
          ||(((cW==14)&&(foundk)&&(foundb))&&((cB==15)&&(foundK)))  //king&bishop vs king
          ||(((cW==15)&&(foundk))&&((cB==14)&&(foundK)&&(foundB)))  //king vs king&bishop
          ||((((cW==14)&&(foundk)&&(foundb))&&((cB==14)&&(foundK)&&(foundB)))&&((even&&EVEN)||(odd&&ODD))) //king&bishop vs king&bishop and bishops on the same colour
           )
                {
                    lowPieces=1;
                    return true;
                }

    }

     for(int j=0;j<8;j++)
        {
            for(int i=0;i<8;i++)
            {
                switch(turn)
                {

                case 0 :
                if(Board[j][i]=='k'){
                    yking = j;
                    xking = i;
                    break ;
                }
                case 1 :
                    if(Board[j][i]=='K'){
                    yking = j;
                    xking = i;
                    break ;
                }
            }
            }
        }
    //checking the legal moves

    if(!check(xking,yking,turn)){
    switch(turn)
    {

    case 0:

        {
            for(int y=0;y<8;y++)
            {
                for(int x=0;x<8;x++)
                {
                    if(isPresent(x,y,0))
                       {
                           switch(Board[y][x])
                            {
                            case 'k':
                                {
                                    if((x+1<8)&&(y+1<8))//move down right
                                    {
                                        if(!isPresent(x+1,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down right
                                    if((x+1<8)&&(y-1>=0))//move up right
                                    {
                                        if(!isPresent(x+1,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up right
                                    if((x-1>=0)&&(y-1>=0))//up left
                                    {
                                        if(!isPresent(x-1,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up left
                                    if((x-1>=0)&&(y+1<8))//down left
                                    {
                                        if(!isPresent(x-1,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down left
                                    if(x-1>=0)//left horizontal
                                    {
                                        if(!isPresent(x-1,y,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if(x+1<8)//right horizontally
                                    {
                                        if(!isPresent(x+1,y,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right
                                    if(y+1<8)//down
                                    {
                                        if(!isPresent(x,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down
                                    if(y-1>=0)//up
                                    {
                                        if(!isPresent(x,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up

                                    break;
                                }//end of case k

                            case 'q':
                                {
                                    if((x+1<8)&&(y+1<8))//move down right
                                    {
                                        if(!isPresent(x+1,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down right
                                    if((x+1<8)&&(y-1>=0))//move up right
                                    {
                                        if(!isPresent(x+1,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up right
                                    if((x-1>=0)&&(y-1>=0))//up left
                                    {
                                        if(!isPresent(x-1,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up left
                                    if((x-1>=0)&&(y+1<8))//down left
                                    {
                                        if(!isPresent(x-1,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down left
                                    if(x-1>=0)//left horizontal
                                    {
                                        if(!isPresent(x-1,y,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if(x+1<8)//right horizontally
                                    {
                                        if(!isPresent(x+1,y,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right
                                    if(y+1<8)//down
                                    {
                                        if(!isPresent(x,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down
                                    if(y-1>=0)//up
                                    {
                                        if(!isPresent(x,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up

                                    break;
                                }//end of case q
                            case 'r':
                                {
                                    if(y-1>=0)//up
                                    {
                                        if(!isPresent(x,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up
                                    if(y+1<8)//down
                                    {
                                        if(!isPresent(x,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down
                                    if(x+1<8)//right horizontally
                                    {
                                        if(!isPresent(x+1,y,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right
                                    if(x-1>=0)//left horizontal
                                    {
                                        if(!isPresent(x-1,y,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                break;
                                }//end of case r
                            case 'b':
                                {
                                    if((x+1<8)&&(y+1<8))//move down right
                                    {
                                        if(!isPresent(x+1,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down right
                                    if((x+1<8)&&(y-1>=0))//move up right
                                    {
                                        if(!isPresent(x+1,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up right
                                    if((x-1>=0)&&(y-1>=0))//up left
                                    {
                                        if(!isPresent(x-1,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up left
                                    if((x-1>=0)&&(y+1<8))//down left
                                    {
                                        if(!isPresent(x-1,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down left
                                    break;
                                }//end of case b
                            case 'n':
                                {
                                    if((x+2<8)&&(y+1<8))//L right+ down
                                    {
                                        if(!isPresent(x+2,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x+2,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of L right+ down
                                    if((x+1<8)&&(y+2<8))//L right down+
                                    {
                                        if(!isPresent(x+1,y+2,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y+2,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of L right down+
                                    if((x+1<8)&&(y-2>=0))//right up+
                                    {
                                        if(!isPresent(x+1,y-2,0))
                                        {
                                            if(!kingcheck(x,y,x+1,y-2,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right up+
                                    if((x+2<8)&&(y-1>=0))//right+ up
                                    {
                                        if(!isPresent(x+2,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x+2,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right up+
                                    if((x-2>=0)&&(y+1<8))//left+ down
                                    {
                                        if(!isPresent(x-2,y+1,0))
                                        {
                                            if(!kingcheck(x,y,x-2,y+1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if((x-1>=0)&&(y+2<8))//left down+
                                    {
                                        if(!isPresent(x-1,y+2,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y+2,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if((x-2>=0)&&(y-1>=0))//left+ up
                                    {
                                        if(!isPresent(x-2,y-1,0))
                                        {
                                            if(!kingcheck(x,y,x-2,y-1,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if((x-1>=0)&&(y-2>=0))//left up+
                                    {
                                        if(!isPresent(x-1,y-2,0))
                                        {
                                            if(!kingcheck(x,y,x-1,y-2,0))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    break;
                                }
                            case 'p':
                                {
                                    if(y-1>=0)//moving up for pawn
                                    {
                                        if(!isalpha(Board[y-1][x]))
                                        {
                                            if(!kingcheck(x,y,x,y-1,0))
                                            {
                                                return false;
                                            }
                                        }//end of vertical
                                        if(x+1<8)//up right capture
                                        {
                                            if(isPresent(x+1,y-1,1))//capture
                                            {
                                                if(!kingcheck(x,y,x+1,y-1,0))
                                                {
                                                    return false;
                                                }
                                            }
                                        }
                                        if(x-1>=0)//up left
                                        {
                                            if(isPresent(x-1,y-1,1))
                                            {
                                                if(!kingcheck(x,y,x-1,y-1,0))
                                                {
                                                    return false;
                                                }
                                            }
                                        }

                                    }
                                    break;
                                }//end of case p
                            }//end of switching pieces
                       }
                }
            }
            deadposition=1;
            return true;
            break;
        }//end of case 0
    case 1:
        {
        for(int y=0;y<8;y++)
            {
                for(int x=0;x<8;x++)
                {
                    if(isPresent(x,y,1))
                       {
                           switch(Board[y][x])
                            {
                            case 'K':
                                {
                                    if((x+1<8)&&(y+1<8))//move down right
                                    {
                                        if(!isPresent(x+1,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down right
                                    if((x+1<8)&&(y-1>=0))//move up right
                                    {
                                        if(!isPresent(x+1,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up right
                                    if((x-1>=0)&&(y-1>=0))//up left
                                    {
                                        if(!isPresent(x-1,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up left
                                    if((x-1>=0)&&(y+1<8))//down left
                                    {
                                        if(!isPresent(x-1,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down left
                                    if(x-1>=0)//left horizontal
                                    {
                                        if(!isPresent(x-1,y,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if(x+1<8)//right horizontally
                                    {
                                        if(!isPresent(x+1,y,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right
                                    if(y+1<8)//down
                                    {
                                        if(!isPresent(x,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down
                                    if(y-1>=0)//up
                                    {
                                        if(!isPresent(x,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up

                                    break;
                                }//end of case k

                            case 'Q':
                                {
                                    if((x+1<8)&&(y+1<8))//move down right
                                    {
                                        if(!isPresent(x+1,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down right
                                    if((x+1<8)&&(y-1>=0))//move up right
                                    {
                                        if(!isPresent(x+1,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up right
                                    if((x-1>=0)&&(y-1>=0))//up left
                                    {
                                        if(!isPresent(x-1,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up left
                                    if((x-1>=0)&&(y+1<8))//down left
                                    {
                                        if(!isPresent(x-1,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down left
                                    if(x-1>=0)//left horizontal
                                    {
                                        if(!isPresent(x-1,y,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if(x+1<8)//right horizontally
                                    {
                                        if(!isPresent(x+1,y,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right
                                    if(y+1<8)//down
                                    {
                                        if(!isPresent(x,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down
                                    if(y-1>=0)//up
                                    {
                                        if(!isPresent(x,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up

                                    break;
                                }//end of case q
                            case 'R':
                                {
                                    if(y-1>=0)//up
                                    {
                                        if(!isPresent(x,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up
                                    if(y+1<8)//down
                                    {
                                        if(!isPresent(x,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down
                                    if(x+1<8)//right horizontally
                                    {
                                        if(!isPresent(x+1,y,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right
                                    if(x-1>=0)//left horizontal
                                    {
                                        if(!isPresent(x-1,y,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                break;
                                }//end of case r
                            case 'B':
                                {
                                    if((x+1<8)&&(y+1<8))//move down right
                                    {
                                        if(!isPresent(x+1,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down right
                                    if((x+1<8)&&(y-1>=0))//move up right
                                    {
                                        if(!isPresent(x+1,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up right
                                    if((x-1>=0)&&(y-1>=0))//up left
                                    {
                                        if(!isPresent(x-1,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of up left
                                    if((x-1>=0)&&(y+1<8))//down left
                                    {
                                        if(!isPresent(x-1,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of down left
                                    break;
                                }//end of case b
                            case 'N':
                                {
                                    if((x+2<8)&&(y+1<8))//L right+ down
                                    {
                                        if(!isPresent(x+2,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x+2,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of L right+ down
                                    if((x+1<8)&&(y+2<8))//L right down+
                                    {
                                        if(!isPresent(x+1,y+2,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y+2,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of L right down+
                                    if((x+1<8)&&(y-2>=0))//right up+
                                    {
                                        if(!isPresent(x+1,y-2,1))
                                        {
                                            if(!kingcheck(x,y,x+1,y-2,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right up+
                                    if((x+2<8)&&(y-1>=0))//right+ up
                                    {
                                        if(!isPresent(x+2,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x+2,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }//end of right up+
                                    if((x-2>=0)&&(y+1<8))//left+ down
                                    {
                                        if(!isPresent(x-2,y+1,1))
                                        {
                                            if(!kingcheck(x,y,x-2,y+1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if((x-1>=0)&&(y+2<8))//left down+
                                    {
                                        if(!isPresent(x-1,y+2,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y+2,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if((x-2>=0)&&(y-1>=0))//left+ up
                                    {
                                        if(!isPresent(x-2,y-1,1))
                                        {
                                            if(!kingcheck(x,y,x-2,y-1,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    if((x-1>=0)&&(y-2>=0))//left up+
                                    {
                                        if(!isPresent(x-1,y-2,1))
                                        {
                                            if(!kingcheck(x,y,x-1,y-2,1))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    break;
                                }
                            case 'P':
                                {
                                    if(y+1<8)//moving down for pawn
                                    {
                                        if(!isalpha(Board[y+1][x]))
                                        {
                                            if(!kingcheck(x,y,x,y+1,0))
                                            {
                                                return false;
                                            }
                                        }//end of vertical
                                        if(x+1<8)//down right capture
                                        {
                                            if(isPresent(x+1,y+1,0))//capture
                                            {
                                                if(!kingcheck(x,y,x+1,y+1,1))
                                                {
                                                    return false;
                                                }
                                            }
                                        }
                                        if(x-1>=0)//up left
                                        {
                                            if(isPresent(x-1,y+1,0))
                                            {
                                                if(!kingcheck(x,y,x-1,y+1,1))
                                                {
                                                    return false;
                                                }
                                            }
                                        }

                                    }
                                    break;
                                }//end of case p
                            }//end of switching pieces
                       }
                }
            }
            deadposition=1;
            return true;
            break;
        }//end of case 1

    }//end of switch
    } // end of if
    else {
        return false ;
    }
    return true;

    }



bool checkmate(int turn)//checks if there a checkmate on that piece
{
    int tx;
    int ty;

    switch(turn)
    {
    case 0://check against white king
        {
            for(int y=0;y<8;y++)
            {
                for(int x=0;x<8;x++)
                {
                    if(Board[y][x]=='k')
                    {
                        if(check(x,y,turn)>1)//more than one threat then king has to move
                        {
                            if(trapped(x,y,turn))
                            {
                                //then there is no motion
                                return true;//checkmate
                                //game over
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else if(check(x,y,turn)==1)
                        {
                            tx=threatX;
                            ty=threatY;
                            //then we can move a piece to capture the danger or sacrifice a piece in the way
                            //to capture
                            //if the threatening piece can be threatened by white pieces
                            //then there is hope if not we have to check other things
                            if(((check(tx,ty,turn^1)==1)&&(Board[threatY][threatX]!='k')&&(!kingcheck(threatX,threatY,tx,ty,turn)))||(check(tx,ty,turn^1)>1))
                            {
                                return false;
                            }
                            else if(check(x,y,turn)&&((threatX!=-1)&&(threatY!=-1))&&(Board[threatY][threatX]!='N')
                                    &&sacrifice(x,y,threatX,threatY,turn))
                            {
                                return false;
                            }
                            else if(!trapped(x,y,turn))
                            {
                                return false;
                            }
                            else
                            {
                                return true;
                            }
                        }
                        else if(!check(x,y,turn))
                        {
                            return false;
                        }
                    }//end of king if
                }//end of x loop
            }//end of y loop
        break;
        }//end of case 0
    case 1:
        {
            for(int y=0;y<8;y++)
            {
                for(int x=0;x<8;x++)
                {
                    if(Board[y][x]=='K')
                    {
                        if(check(x,y,turn)>1)//more than one threat then king has to move
                        {
                            if(trapped(x,y,turn))
                            {
                                //then there is no motion
                                return true;//checkmate
                                //game over
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else if(check(x,y,turn)==1)
                        {
                            tx=threatX;
                            ty=threatY;
                            //then we can move a piece to capture the danger or sacrifice a piece in the way
                            //to capture
                            //if the threatening piece can be threatened by white pieces
                            //then there is hope if not we have to check other things
                            if(((check(tx,ty,turn^1)==1)&&(Board[threatY][threatX]!='K')&&(!kingcheck(threatX,threatY,tx,ty,turn)))||(check(tx,ty,turn^1)>1))
                            {
                                return false;
                            }
                            else if(check(x,y,turn)&&((threatX!=-1)&&(threatY!=-1))&&(Board[threatY][threatX]!='n')
                                    &&sacrifice(x,y,threatX,threatY,turn))
                            {
                                return false;
                            }
                            else if(!trapped(x,y,turn))
                            {
                                return false;
                            }
                            else
                            {
                                return true;
                            }
                        }
                        else if(!check(x,y,turn))
                        {
                            return false;
                        }
                    }//end of king if

                }//end of x loop
            }//end of y loop
        break;
        }//end of case 1
    }//end of all switch
    return false;
}



//to check whether the move was valid or not
bool isValid(int x1,int y1,int x2,int y2, int turn,char z)
{

    if ((isPresent(x1,y1,turn))&&(isMovable(x1,y1,x2,y2,turn,z))&&(!kingcheck(x1,y1,x2,y2,turn)))
    {
        return true;
    }
    else
    {

        return false;
    }
}

void Undo (int x1,int y1,int x2,int y2,int turn,char z, int command)
{

            //do undo

            undo ++ ;
            // restore the moves and reverse it
            x1 = moves[turn_num-1][2];
            y1 = moves[turn_num-1][3];
            x2 = moves[turn_num-1][0];
            y2 = moves[turn_num-1][1];
            char temp;
            temp=Board[y1][x1];
            Board[y2][x2]=temp;
            if ((y1+x1)%2==1)
                Board[y1][x1]='.';
            else if ((y1+x1)%2==0)
                Board[y1][x1]='-';

                if((isalpha(captured[turn_num-1]))){
                    Board[y1][x1] = captured[turn_num-1];


    switch(turn)
      {
      case 0 :
      cW --;


        break ;
     case 1 :

    cB -- ;

        break ;
     }
      }
      //undo castling
        if(symbol[turn_num-1]=='.'){
             switch (turn)
        {
            case 1 :
            {
                k = 0;
                switch(x1)
                {
                    case 6:
                    {

                       Board[7][5]= '-' ;
                       Board[7][7]= 'r';
                       r2 = 0 ;
                       break;

                    }
                    case 2 :
                    {

                       Board[7][3]='-';
                       Board[7][0]='r';
                       r1=0;

                       break;
                     }

                    default :
                        {
                            break;
                        }
                }
                break;
            }
            case 0 :
            {
                K = 0 ;
                switch(x1)
                {
                    case 6 :
                    {

                       Board[0][5]='.' ;
                       Board[0][7]= 'R';
                       R2=0;
                       break;

                    }
                    case 2 :
                    {

                       Board[0][3]='.' ;
                       Board[0][0]= 'R';
                       R1=0;
                       break;

                    }
                    default :
                        {
                            break;
                        }
                }
            break;
            }
        }
        }
        //undo promotion
        if (symbol[turn_num-1]=='*')
            {
            switch(turn)
     {
     case 1 :
    Board[y2][x2] ='p';


        break ;
     case 0 :

      Board[y2][x2] ='P';

        break ;
     }
        }
        // undo enpassant
       if(symbol[turn_num-1]=='@'){
       switch (turn){
            case 0 :
             Board[y2][x2+1]='p';
             cW-- ;
             break ;
            case 1 :
            Board[y2][x2+1]='P';
            cB--;
            break;

        }


       }
       //undo enpassant
       if(symbol[turn_num-1]=='+'){
        switch (turn){
            case 0 :
             Board[y2][x2-1]='p';
             cW--;
             break ;
            case 1 :
            Board[y2][x2-1]='P';
            cB--;
            break ;

        }


       }
       // reset the flags to do the caslting again if the user want
    if(symbol[turn_num-1]== '1'){
        k = 0 ;
    }
    if(symbol[turn_num-1]=='2'){
        K = 0 ;
    }
    if(symbol[turn_num-1]== '3'){
        R1=0 ;
    }
     if(symbol[turn_num-1]== '4'){
        R2=0 ;

    } if(symbol[turn_num-1]== '5'){
        r1=0 ;

    } if(symbol[turn_num-1]=='6'){
        r2=0;
    }

        turn_num -- ;

        }

void Redo (int x1,int y1,int x2,int y2,int turn,char z, int command)
{
            //do redo
            // take the stored moves and regenerate the move

            undo--;
            x1 = moves[turn_num][0];
            y1 = moves[turn_num][1];
            x2 = moves[turn_num][2];
            y2 = moves[turn_num][3];
            z= prom[turn_num];
            generateMove(x1,y1,x2,y2,turn,z);
}

void Save(){

           printf("\nEnter file name:");
           int k ;
           char File_name [100] ;
           char outFileDir[13] = "saved games/";
           char merge[113] ;
           int  flags[20]={undo,turn_num,cB,cW,turn,threatX,threatY,r1,r2,R1,R2,k,K,Wcheckmated,Bcheckmated,checkingSacrifice,enpassantRight,enpassantLeft,lowPieces,deadposition};
           fflush(stdin);
            scanf("%s",File_name);

    for(int i=0; i<13; i++)
        {
               merge[i] = outFileDir[i];
               k= i ;
         }

    for(int i=0; i<100; i++)
        {
            merge[k] = File_name[i];
                 k++;
        }
    // saving all the arrays and flags
         fp = fopen(merge,"w");
         fwrite(Board, sizeof(char), sizeof(Board), fp);
         fwrite(moves, sizeof(char), sizeof(moves), fp);
         fwrite(captured, sizeof(char), sizeof(captured), fp);
         fwrite(capturedBlack, sizeof(char), sizeof(capturedBlack), fp);
         fwrite(capturedWhite, sizeof(char), sizeof(capturedWhite), fp);
         fwrite(flags, sizeof(int), sizeof(flags), fp);
         fwrite(prom, sizeof(char), sizeof(prom), fp);
         fwrite(symbol, sizeof(char), sizeof(symbol), fp);
         fclose(fp);
}

void readMove()
{
    char input[100];
    int valid=1; //flag when set to 0 indicates input is invalid
    int command=0; //flag when set to 1 indicates a special command as save or undo or redo
    yellow();
    printf("\n\nNote: Turn won't be switched if the input is invalid\n\n");
    white();
    printf("Press ("); yellow();printf(" U ");white();printf(") to undo ~~~ ( "); yellow();printf("R ");white();printf(") to redo ~~~ ( ");yellow();printf("S ");white();printf(") to save ~~~ ( ");yellow();printf("E ");white();printf(") to Exit \n");
    white();
    printf("Player %d turn:\n",turn+1);
    reset();
    scanf("%s",input);
    int x1,y1,x2,y2;
    char z; //character for promotion process
    //when input is one character long
    if(strlen(input)==1)
    {
        //check that character if it is s or u or r
        if(((input[0]=='u')||(input[0]=='U'))&& turn_num > 0)
        {
             command=1;
             Undo(x1,y1,x2,y2,turn,z,command) ;
             turn=turn^1;//switch turn
             fflush(stdin);
             printBoard(Board);
        }
        else if (((input[0]=='r')||(input[0]=='R'))&&(undo >0))
        {
            command=1;
            Redo(x1,y1,x2,y2,turn,z,command) ;
             turn_num ++ ;
            turn=turn^1;//switch turn
            fflush(stdin);
            printBoard(Board);
        }

        else if ((input[0]=='s')||(input[0]=='S'))
        {
            command=1;
           Save();
        }
        else if ((input[0]=='e')||(input[0]=='E')){

            exit(0) ;
        }
        else
        {
            valid=0;
        }
    }

    //when input is 4 characters long
    else if(strlen(input)==4)
    {

        //1st
        if((input[0]<73)&&(input[0]>=65))//uppercase letters
        {
            x1=(int)input[0]-65;
        }
        else if((input[0]<105)&&(input[0]>=97))//lowercase letters
        {
            x1=(int)input[0]-97;
        }
        else
        {
            valid=0;
        }

        //2nd digit in input
        if((input[1]<57)&&(input[1]>=49))//ascii for numbers
        {
            y1=8-((int)input[1]-48);
        }
        else
        {
            valid=0;
        }
        //3rd
        if((input[2]<73)&&(input[2]>=65))//uppercase letters
        {
            x2=(int)input[2]-65;
        }
        else if((input[2]<105)&&(input[2]>=97))//lowercase letters
        {
            x2=(int)input[2]-97;
        }
        else
        {
            valid=0;
        }
        //4th
        if((input[3]<57)&&(input[3]>=49))//ascii for numbers
        {
            y2=8-((int)input[3]-48);
        }
        else
        {
            valid=0;
        }

    }

    //when input is 5 characters long
    else if(strlen(input)==5)
    {

        //1st x1
        if((input[0]<73)&&(input[0]>=65))//uppercase letters
        {
        x1=(int)input[0]-65;
        }
        else if((input[0]<105)&&(input[0]>=97))//lowercase letters
        {
        x1=(int)input[0]-97;
        }
        else
        {
            valid=0;
        }

        //2nd digit in input
        if((input[1]<57)&&(input[1]>=49))//ascii for numbers
        {
        y1=8-((int)input[1]-48);
        }
        else
        {
            valid=0;
        }
        //3rd
        if((input[2]<73)&&(input[2]>=65))//uppercase letters
        {
        x2=(int)input[2]-65;
        }
        else if((input[2]<105)&&(input[2]>=97))//lowercase letters
        {
        x2=(int)input[2]-97;
        }
        else
        {
            valid=0;
        }
        //4th
        if((input[3]<57)&&(input[3]>=49))//ascii for numbers
        {
        y2=(8-((int)input[3]-48));
        }
        else
        {
            valid=0;
        }
        //5th
        if(((input[4]=='B')||(input[4]=='N')||(input[4]=='Q')||(input[4]=='R')||(input[4]=='b')||(input[4]=='n')||(input[4]=='q')||(input[4]=='r'))&&(Valid_promotion(x1,y1,x2,y2,turn))&&(valid))
        {
            z=input[4];
        }
        else
        {
            valid=0;
        }

    }
    else
    {
        valid=0;
    }
    //processing input over
    //passing values

    if((valid)&&(!command))//then it is a valid formula
    {
        //generate move

        if ((isValid(x1,y1,x2,y2,turn,z)))
        {
            undo = 0 ; //set undo counter to 0 to prevent redo in wrong turns
            generateMove(x1,y1,x2,y2,turn,z);
            //save the valid move
            moves[turn_num][0]= x1 ;
            moves[turn_num][1]= y1 ;
            moves[turn_num][2]= x2 ;
            moves[turn_num][3]= y2 ;
            prom[turn_num] = z ;
            turn_num ++ ;
            printf("\n");
            turn=turn^1;//switch turn
            fflush(stdin);
            printBoard(Board);
        }
        else
        {
            fflush(stdin);
            printBoard(Board);
            if(kingcheckflag)
            {
                red();
                printf("\nking is checked!");
                reset();
                kingcheckflag=0;
            }

            red();
            printf("\nEnter valid input please");
            reset();

        }
    }
    else
    {
        fflush(stdin);
        printBoard(Board);
        if(kingcheckflag)
            {
                red();
                printf("\nking is checked!");
                reset();
                kingcheckflag=0;
            }
         if (valid ==0 ){
         red();
        printf("\nEnter valid input please");
         reset();
         }
    }



    return;
}

 void Loading_Game (){

    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir("saved games");

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
    }

     while ((de = readdir(dr)) != NULL){
        printf("%s\n", de->d_name);
     }
        closedir(dr);
           int k ;
           char File_name [100] ;
           char outFileDir[13] = "saved games/";
           char merge[113] ;

         printf("Choose the name of the file you want to load :");
            fflush(stdin);
            scanf("%s",File_name);

    for(int i=0; i<13; i++)
        {
            merge[i] = outFileDir[i];
            k= i ;
        }

    for(int i=0; i<100; i++)
        {
            merge[k] = File_name[i];
            k++;
        }
         fp= fopen(merge,"r");
         while(fp == NULL ){
            printf("Enter the name correct\n");

            printf("Choose the name of the file you want to load :");
            fflush(stdin);
            scanf("%s",File_name);

    for(int i=0; i<13; i++)
        {
            merge[i] = outFileDir[i];
            k= i ;
        }

    for(int i=0; i<100; i++)
        {
            merge[k] = File_name[i];
            k++;
        }
         fp = fopen(merge,"r");
         }

       int flags[20];
     while (!feof(fp)){
            // take the saved arrays and flags
        fread(Board, sizeof(char), sizeof(Board), fp);
        fread(moves, sizeof(char), sizeof(moves), fp);
        fread(captured, sizeof(char), sizeof(captured), fp);
        fread(capturedBlack, sizeof(char), sizeof(capturedBlack), fp);
        fread(capturedWhite, sizeof(char), sizeof(capturedWhite), fp);
        fread(flags, sizeof(int), sizeof(flags), fp);
        fread(prom, sizeof(char), sizeof(prom), fp);
        fread(symbol, sizeof(char), sizeof(symbol), fp);
     }
     undo=flags[0];turn_num=flags[1];cB=flags[2];cW=flags[3];turn=flags[4];threatX=flags[5];threatY=flags[6];r1=flags[7];r2=flags[8];R1=flags[9];R2=flags[10];k=flags[11];K=flags[12];Wcheckmated=flags[13];Bcheckmated=flags[14];checkingSacrifice=flags[15];enpassantRight=flags[16];enpassantLeft=flags[17];lowPieces=flags[18];deadposition=flags[19];
 }

int main()
{
    //flag to end of the game
    int end=0;

    //create a new folder called saved games in the same path as the exe
    char* dirname = "saved games";
     mkdir(dirname);
     //starting input
    char start[100];
    system("cls");
    cyan();
    printf("                                                     __   _   __ \n");
    printf("         _____   __   __   _____    _____   _____    \\ \\_| |_/ / \n");
    printf("        / ____| |  | |  | |  ___|  / ___/  / ___/     \\_______/\n");
    printf("       / /      |  |_|  | | |___  | |___  | |___        |---| \n");
    printf("      | |       |   _   | |  ___|  \\__  \\  \\__  \\       |---| \n");
    printf("       \\ \\____  |  | |  | | |___    __| |   __|	|     _/_____\\_\n");
    printf(" 	\\_____| |__| |__| |_____|  /____/  /____/    |_________|\n");
    printf("\n_______________________________________________________________\n\n");
    white();
   printf("\t\t\tMAIN MENU\n\t\t\t__________\n\n\t\tEnter ( N ) for a new game:  \n\t\tEnter ( L ) to load a game:\n\n\t\tInput:");

   scanf("%s",start);

    //To verify the validity of the input (what character and the length of input)
   while ((start[0]!='n' && start[0]!='N' && start[0]!='L' &&start[0]!='l') || strlen(start)>1 ){
    printf("\n\t\tEnter a valid character\n");
    printf("\t\tEnter ( N ) for a new game:  \n\t\tEnter ( L ) to load a game:\n\n\t\tInput:");
     scanf("%s",start);

  }
  if (start[0]== 'l' || start[0] == 'L'){
        Loading_Game();
  }

    //print initial board
    printBoard(Board);


    while(!end){
        readMove();
        if(checkmate(turn))
        {
            end=1;
            green();
            printf("\n\nCheckmate, player %d wins\n",(turn^1)+1);
            reset();
        }
        else if(stalemate(turn))
        {
            end=1;
            green();
            printf("\n\nDraw");
            if (deadposition)
            {
                printf(" by dead position.\n");
                deadposition=0;
            }
            if (lowPieces)
            {
                printf(", pieces are too few to continue.\n");
                lowPieces=0;
            }
            reset();
        }
    }

    return 0;
}

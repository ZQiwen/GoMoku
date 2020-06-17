#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_ROW 15
#define MAX_COL 15
#define VERSION "1.2.1"
char chessboard[MAX_ROW][MAX_COL]; // 'x': black piece; 'o': white piece; '.': no piece;

int aix, aiy;// AI选择的落子位置
int halfround;// 第几个 半回合

// 定义八个方向
int dx[]={-1,-1,0,1,1,1,0,-1};
int dy[]={0,1,1,1,0,-1,-1,-1};
// 当前行动方的颜色
char color[6];
char pieceShape[2]={'x','o'};

int useColor,GameMode;// v1.2

int DropInsideBoard(int x,int y)//判断(x,y)位置是否在棋盘范围内
{
    if(x>=0&&x<MAX_COL&&y>=0&&y<MAX_ROW){
        return 1;
    }
    return 0;
}

#define DropOverlaping(x, y)  (chessboard[x][y]!='.')      //判断(x,y)位置是否已经已有棋子


void DisplayChessBoard()//打印当前棋盘状态
{
    for(int i=0;i<MAX_COL+2;i++){
        if(i==0){
            printf("\n\t\t\t\t\t**  00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  **\n");
            continue;
        }
        if(i==MAX_COL+1){
            printf("\t\t\t\t\t**  00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  **\n");
      //      if(i==0) printf("\t\t\t\t\t  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |\n");
            continue;
        }


        printf("\t\t\t\t\t%02d  ",i-1);
        for(int j=0;j<MAX_ROW;j++){
            printf(" %c  ",chessboard[i-1][j]);
        }
        printf("%02d\n",i-1);
        //printf("\t\t\t\t\t  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |\n");
        printf("\n");


    }
    printf("\n\n");
}

void Init()//初始化棋盘
{
    memset(chessboard,'.',sizeof(chessboard));
//	for (int row = 0; row < MAX_ROW; ++row) {
//		for (int col = 0; col < MAX_COL; ++col) {
//			chessboard[row][col] = ' ';
//		}
//	}
}

void DropPiece(int tmpx,int tmpy,int hr)//落子
{
    if(hr%2){//white piece
        chessboard[tmpx][tmpy]='o';
    }
    else{//black piece
        chessboard[tmpx][tmpy]='x';
    }
    system("cls");
    DisplayChessBoard();
    return;
}

int GameIsOver(int x,int y) // 判断下在(x,y)的这步棋会不会产生五子连珠或者让棋盘被铺满。如果游戏结束，打印结果。
{
    int numPiece,nextx,nexty;

    for(int dirctn=0;dirctn<4;dirctn++){// 搜索八个方向上从(x,y)出发的连续相同棋子数量
        numPiece=1;
        nextx=x+dx[dirctn];
        nexty=y+dy[dirctn];
        while(DropInsideBoard(nextx,nexty) && chessboard[nextx][nexty]==chessboard[x][y]){
            numPiece++;
            nextx=nextx+dx[dirctn];
            nexty=nexty+dy[dirctn];
        }
        nextx=x-dx[dirctn];
        nexty=y-dy[dirctn];
        while(DropInsideBoard(nextx,nexty) && chessboard[nextx][nexty]==chessboard[x][y]){
            numPiece++;
            nextx=nextx-dx[dirctn];
            nexty=nexty-dy[dirctn];
        }
        if(numPiece==5){ //达成五子连珠
            printf("Game Over: %s wins!\n",color);
            return 1;
        }
    }
    // 没有达成五子连珠。
    if(halfround==MAX_COL*MAX_ROW){ // 棋盘铺满，平局。
        printf("Game Over: Drawn!\n");
        return 1;
    }
    // 游戏继续
    return 0;
}

void ThinkNextDrop()//AI 决定落子位置
{
    // taking strategy RandomStrategy
    //RandomStrategy();

    // taking strategy NeighborStrategy
    NeighborStrategy();

    return;
}

void RandomStrategy()// 万能的随机策略
{
    srand((unsigned)time(NULL));
    aix=rand() % 15;
    aiy=rand() % 15;
    while(DropOverlaping(aix,aiy)){// 如果随机到的位置已经有棋子，则顺延到下一个位置
        aix=aix+(aiy+1)/15;
        aiy=(aiy+1)%15;
    }
    return;
}


void NeighborStrategy() //全棋盘遍历，根据周围的棋子给落点打分。
// 评分规则：从遍历到的位置向四个方向搜索，每有一个方向上存在与该位置相邻的连续n枚同色棋子，该位置分数加n^3；
// 分数相同时，优先选择距离棋盘中心更近的位置。
{
    int nextx,nexty,numFriendPiece,numOppositePiece,Score,maxScore;
    int maxlocalScore,localScore,posiEndx,posiEndy,negEndx,negEndy; // v1.1
    char myPiece=pieceShape[halfround%2];
    char oppositePiece=pieceShape[(halfround+1)%2];

    maxScore=0;
    for(int x=0;x<MAX_COL;x++){
        for(int y=0;y<MAX_ROW;y++){
            if(DropOverlaping(x,y)){
                continue;
            }
            Score=0;
            for(int dirctn=0;dirctn<4;dirctn++){// 搜索四个方向上从(x,y)出发的同一方的棋子数量, v1.1

                maxlocalScore=0;
                for(int npp=0;npp<5;npp++){
                    posiEndx=x+npp*dx[dirctn];
                    posiEndy=y+npp*dy[dirctn];
                    negEndx=x-(4-npp)*dx[dirctn];
                    negEndy=y-(4-npp)*dy[dirctn];
                    if(!DropInsideBoard(posiEndx,posiEndy) || !DropInsideBoard(negEndx,negEndy)){
                        continue;
                    }

                    localScore=0;
                    numFriendPiece=0;
                    numOppositePiece=0;
                    for(int locali=0;locali<5;locali++){
                        nextx=negEndx+locali*dx[dirctn];
                        nexty=negEndy+locali*dy[dirctn];
                        if(chessboard[nextx][nexty]==myPiece){
                            numFriendPiece++;
                        }
                        if(chessboard[nextx][nexty]==oppositePiece){
                            numOppositePiece++;
                        }
                    }
                    if(numFriendPiece&&numOppositePiece){
                        localScore=0;
                    }
                    else{
                        localScore=numFriendPiece*numFriendPiece*numFriendPiece+numOppositePiece*numOppositePiece*numOppositePiece;
                        maxlocalScore = (maxlocalScore>localScore)? maxlocalScore:localScore;
                    }
                }
//                numFriendPiece=0;
//                nextx=x+dx[dirctn];
//                nexty=y+dy[dirctn];
//                while(DropInsideBoard(nextx,nexty) && chessboard[nextx][nexty]==myPiece){
//                    numFriendPiece++;
//                    nextx=nextx+dx[dirctn];
//                    nexty=nexty+dy[dirctn];
//                }
//                numOppositePiece=0;
//                nextx=x+dx[dirctn];
//                nexty=y+dy[dirctn];
//                while(DropInsideBoard(nextx,nexty) && chessboard[nextx][nexty]==oppositePiece){
//                    numOppositePiece++;
//                    nextx=nextx+dx[dirctn];
//                    nexty=nexty+dy[dirctn];
//                }
//                Score+=numFriendPiece*numFriendPiece*numFriendPiece+numOppositePiece*numOppositePiece*numOppositePiece;
                Score+=maxlocalScore;
            }
            if(Score>=maxScore){
                if(Score>maxScore){
                    maxScore=Score;
                    aix=x;
                    aiy=y;
                }
                else{
                    if( (abs(x-MAX_COL/2)+abs(y-MAX_ROW/2)) < (abs(aix-MAX_COL/2)+abs(aiy-MAX_ROW/2)) ){
                        aix=x;
                        aiy=y;
                    }
                }

            }
        }
    }
//    if (maxScore==0){ //没有好的选择，就随缘选一个
//        RandomStrategy();
//    }
//    printf("TEST MESSAGE::maxScore=%d\n",maxScore);
    return;
}

void main() {
    //每一步落子的位置都由用户输入；输入两个正整数，以空格隔开；
    //每一步落子之后，AI会给出一个落子位置建议。
    //玩家可以扮演一方总是采纳AI的建议，从而实现人机对战。
    //也可以选择一直无视AI的建议，从而实现双人对战或者自我对战。

    system("mode con cols=120 lines=80");
    system("title=GoMoku_zqw");
//    system("color 70");

    // 打印游戏说明：
    printf("#########################################################################################\n#\n");
    printf("# 欢迎使用GoMoku_v%s\n",VERSION);
    printf("# ——它有着一个稍好但是依然简陋的界面，并提供了一个稍好的人工智障供玩家练习\n#\n#\n");
    printf("# 使用说明：\n");
    printf("#     每一步落子的位置都由用户输入。\n");
    printf("#     输入格式为两个（十进制）正整数，分别代表行号和列号（例如：2 13），以空格隔开。\n");
    printf("#     棋盘大小为15x15，用'x'代表黑子，'o'代表白子，'.'代表无棋子。\n");
    printf("#     棋盘边缘标出了行号和列号。\n");
//    printf("#     用户每输入一步落子位置之后，AI会给出一个落子位置建议。\n");
    printf("#     玩家可以选择双人对战或者人机对战。\n");
    printf("#     人机模式下的电脑AI的策略非常简单，虐爆它几乎没有难度。\n");
//    printf("#     也可以选择一直无视AI的建议，从而实现双人对战或者自己与自己对战。\n");
//    printf("#     或者同时扮演对战双方，总是采纳AI的建议，从而观察AI与AI对战。\n");
    printf("#     提示：按住Ctrl并滚动鼠标中键可以调整显示大小。\n");
    printf("#\n");
    printf("# v1.1版本的更新：\n");
    printf("#     修复了v1.0游戏结束后闪退的bug。\n");
    printf("#     增强的AI，想虐爆这个版本的AI稍微需要动一点脑子。\n");
//    printf("#     提示：让AI先手会变得稍微有一点困难。\n");
    printf("# v1.2.1版本的更新：\n");
    printf("#     修复了双人模式下一些情况下无法判定五子连珠的bug。\n");
    printf("#     增加了模式选择功能，选择人机对战模式后用户将不需要为AI落子。\n");
    printf("#     更新了棋盘显示（特别感谢来自yuukai的建议）。\n");
    printf("# v2.1版本（开发中）。\n");
    printf("#     将使用机器学习型的AI。\n");
    printf("#\n#\n#\n#\n");

    printf("# 最后，欢迎任何形式问题或建议，我的邮箱：zhuqiwen1600012437@pku.edu.cn\n");
    printf("#########################################################################################\n");
    printf("建议你将该窗口全屏，按住Ctrl滚动鼠标中键尽量让文字充满整个屏幕。");
    system("pause");
    system("cls");
    printf("请选择模式，输入数字1或者2然后回车（1：人机对战/2：双人对战）：\n");
    scanf("%d",&GameMode);
    if(GameMode==1){
        printf("已选择人机对战模式，请选择执黑子（先手）还是白子（后手）。\n");
        printf("输入数字0或者1然后回车（0：执黑子/1：执白子）：\n");
        scanf("%d",&useColor);
    }
    if(GameMode==2){
        printf("已选择双人对战模式。\n");
//        printf("输入数字0或者1然后回车（0：执黑子/1：执黑子）：\n");
//        scanf("%d",&useColor);
    }
    system("pause");
    system("cls");
    Init();

    int x, y;
    int GameCanceled=1;
    halfround=0;

    // 打印棋盘
    DisplayChessBoard();

    if(GameMode==1){ //人机对战
        if(useColor==1){
            // AI选择下一步落子位置，并打印
            ThinkNextDrop();
            DropPiece(aix,aiy,halfround);
            halfround++;

            printf("the AI has dropped: %d %d\n",aix,aiy);
            printf("Round 0.Input position to drop for white(input 'Ctrl+D' to exit):");
        }
        else{ //提示用户输入坐标
            printf("Round 0.Input position to drop for black(input 'Ctrl+D' to exit):");
        }
    }
    else{
        //黑子先行
        strcpy(color,"black");
        printf("Round %d.Input position to drop for %s(input 'Ctrl+D' to exit):",halfround/2,color);
    }

    while(scanf("%d%d",&x,&y)){// 用户输入落子位置

        if(GameMode==2){ //双人对战模式
            if( !DropInsideBoard(x,y) ){ // 输入的位置超出棋盘范围，重新输入
                printf("Drop outside board! Chose another position:");
                continue;
            }
            if( DropOverlaping(x,y) ){ // 输入的位置已经被占据，重新输入
                printf("This position has been taken! Chose another one:");
                continue;
            }

            // 落一步子
            DropPiece(x,y,halfround);

            // 裁判
            if(GameIsOver(x,y)){ //游戏结束
                GameCanceled=0;
                break;
            }
            else{// 游戏继续，进入下一个half round
                halfround++;

                // 看轮到哪方行动
                if(halfround%2){
                    strcpy(color,"white");
                }
                else{
                    strcpy(color,"black");
                }

                // 提示用户输入落子位置
                printf("Round %d.Input position to drop for %s(input 'Ctrl+D' to exit):",halfround/2,color);
            }
        }
        else{ //人机对战模式下
            if( !DropInsideBoard(x,y) ){ // 输入的位置超出棋盘范围，重新输入
                printf("Drop outside board! Chose another position:");
                continue;
            }
            if(DropOverlaping(x,y) ){ // 输入的位置已经被占据，重新输入
                printf("This position has been taken! Chose another one:");
                continue;
            }

            // 落一步子
            DropPiece(x,y,halfround);

            // 裁判
            if(GameIsOver(x,y)){ //游戏结束
                GameCanceled=0;
                break;
            }
            else{
                halfround++;
                // AI思考下一步，并落一步棋
                ThinkNextDrop();
                DropPiece(aix,aiy,halfround);
                printf("the AI has dropped: %d %d\n",aix,aiy);

                if(GameIsOver(aix,aiy)){ //游戏结束
                    GameCanceled=0;
                    break;
                }
                else{// 游戏继续，进入下一个half round
                    halfround++;

                    // 看轮到哪方行动
                    if(halfround%2){
                    strcpy(color,"white");
                    }
                    else{
                        strcpy(color,"black");
                    }

                    // 提示用户输入落子位置
                    printf("Round %d.Input position to drop for %s(input 'Ctrl+D' to exit):",halfround/2,color);
                }
            }
        }
    }
    if(GameCanceled){
        printf("Game canceled by user!\n");
    }
    system("pause");
}

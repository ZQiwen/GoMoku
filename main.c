#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_ROW 15
#define MAX_COL 15
#define VERSION "1.2.1"
char chessboard[MAX_ROW][MAX_COL]; // 'x': black piece; 'o': white piece; '.': no piece;

int aix, aiy;// AIѡ�������λ��
int halfround;// �ڼ��� ��غ�

// ����˸�����
int dx[]={-1,-1,0,1,1,1,0,-1};
int dy[]={0,1,1,1,0,-1,-1,-1};
// ��ǰ�ж�������ɫ
char color[6];
char pieceShape[2]={'x','o'};

int useColor,GameMode;// v1.2

int DropInsideBoard(int x,int y)//�ж�(x,y)λ���Ƿ������̷�Χ��
{
    if(x>=0&&x<MAX_COL&&y>=0&&y<MAX_ROW){
        return 1;
    }
    return 0;
}

#define DropOverlaping(x, y)  (chessboard[x][y]!='.')      //�ж�(x,y)λ���Ƿ��Ѿ���������


void DisplayChessBoard()//��ӡ��ǰ����״̬
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

void Init()//��ʼ������
{
    memset(chessboard,'.',sizeof(chessboard));
//	for (int row = 0; row < MAX_ROW; ++row) {
//		for (int col = 0; col < MAX_COL; ++col) {
//			chessboard[row][col] = ' ';
//		}
//	}
}

void DropPiece(int tmpx,int tmpy,int hr)//����
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

int GameIsOver(int x,int y) // �ж�����(x,y)���ⲽ��᲻���������������������̱������������Ϸ��������ӡ�����
{
    int numPiece,nextx,nexty;

    for(int dirctn=0;dirctn<4;dirctn++){// �����˸������ϴ�(x,y)������������ͬ��������
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
        if(numPiece==5){ //�����������
            printf("Game Over: %s wins!\n",color);
            return 1;
        }
    }
    // û�д���������顣
    if(halfround==MAX_COL*MAX_ROW){ // ����������ƽ�֡�
        printf("Game Over: Drawn!\n");
        return 1;
    }
    // ��Ϸ����
    return 0;
}

void ThinkNextDrop()//AI ��������λ��
{
    // taking strategy RandomStrategy
    //RandomStrategy();

    // taking strategy NeighborStrategy
    NeighborStrategy();

    return;
}

void RandomStrategy()// ���ܵ��������
{
    srand((unsigned)time(NULL));
    aix=rand() % 15;
    aiy=rand() % 15;
    while(DropOverlaping(aix,aiy)){// ����������λ���Ѿ������ӣ���˳�ӵ���һ��λ��
        aix=aix+(aiy+1)/15;
        aiy=(aiy+1)%15;
    }
    return;
}


void NeighborStrategy() //ȫ���̱�����������Χ�����Ӹ�����֡�
// ���ֹ��򣺴ӱ�������λ�����ĸ�����������ÿ��һ�������ϴ������λ�����ڵ�����nöͬɫ���ӣ���λ�÷�����n^3��
// ������ͬʱ������ѡ������������ĸ�����λ�á�
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
            for(int dirctn=0;dirctn<4;dirctn++){// �����ĸ������ϴ�(x,y)������ͬһ������������, v1.1

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
//    if (maxScore==0){ //û�кõ�ѡ�񣬾���Եѡһ��
//        RandomStrategy();
//    }
//    printf("TEST MESSAGE::maxScore=%d\n",maxScore);
    return;
}

void main() {
    //ÿһ�����ӵ�λ�ö����û����룻�����������������Կո������
    //ÿһ������֮��AI�����һ������λ�ý��顣
    //��ҿ��԰���һ�����ǲ���AI�Ľ��飬�Ӷ�ʵ���˻���ս��
    //Ҳ����ѡ��һֱ����AI�Ľ��飬�Ӷ�ʵ��˫�˶�ս�������Ҷ�ս��

    system("mode con cols=120 lines=80");
    system("title=GoMoku_zqw");
//    system("color 70");

    // ��ӡ��Ϸ˵����
    printf("#########################################################################################\n#\n");
    printf("# ��ӭʹ��GoMoku_v%s\n",VERSION);
    printf("# ����������һ���Ժõ�����Ȼ��ª�Ľ��棬���ṩ��һ���Ժõ��˹����Ϲ������ϰ\n#\n#\n");
    printf("# ʹ��˵����\n");
    printf("#     ÿһ�����ӵ�λ�ö����û����롣\n");
    printf("#     �����ʽΪ������ʮ���ƣ����������ֱ�����кź��кţ����磺2 13�����Կո������\n");
    printf("#     ���̴�СΪ15x15����'x'������ӣ�'o'������ӣ�'.'���������ӡ�\n");
    printf("#     ���̱�Ե������кź��кš�\n");
//    printf("#     �û�ÿ����һ������λ��֮��AI�����һ������λ�ý��顣\n");
    printf("#     ��ҿ���ѡ��˫�˶�ս�����˻���ս��\n");
    printf("#     �˻�ģʽ�µĵ���AI�Ĳ��Էǳ��򵥣�Ű��������û���Ѷȡ�\n");
//    printf("#     Ҳ����ѡ��һֱ����AI�Ľ��飬�Ӷ�ʵ��˫�˶�ս�����Լ����Լ���ս��\n");
//    printf("#     ����ͬʱ���ݶ�ս˫�������ǲ���AI�Ľ��飬�Ӷ��۲�AI��AI��ս��\n");
    printf("#     ��ʾ����סCtrl����������м����Ե�����ʾ��С��\n");
    printf("#\n");
    printf("# v1.1�汾�ĸ��£�\n");
    printf("#     �޸���v1.0��Ϸ���������˵�bug��\n");
    printf("#     ��ǿ��AI����Ű������汾��AI��΢��Ҫ��һ�����ӡ�\n");
//    printf("#     ��ʾ����AI���ֻ�����΢��һ�����ѡ�\n");
    printf("# v1.2.1�汾�ĸ��£�\n");
    printf("#     �޸���˫��ģʽ��һЩ������޷��ж����������bug��\n");
    printf("#     ������ģʽѡ���ܣ�ѡ���˻���սģʽ���û�������ҪΪAI���ӡ�\n");
    printf("#     ������������ʾ���ر��л����yuukai�Ľ��飩��\n");
    printf("# v2.1�汾�������У���\n");
    printf("#     ��ʹ�û���ѧϰ�͵�AI��\n");
    printf("#\n#\n#\n#\n");

    printf("# ��󣬻�ӭ�κ���ʽ������飬�ҵ����䣺zhuqiwen1600012437@pku.edu.cn\n");
    printf("#########################################################################################\n");
    printf("�����㽫�ô���ȫ������סCtrl��������м����������ֳ���������Ļ��");
    system("pause");
    system("cls");
    printf("��ѡ��ģʽ����������1����2Ȼ��س���1���˻���ս/2��˫�˶�ս����\n");
    scanf("%d",&GameMode);
    if(GameMode==1){
        printf("��ѡ���˻���սģʽ����ѡ��ִ���ӣ����֣����ǰ��ӣ����֣���\n");
        printf("��������0����1Ȼ��س���0��ִ����/1��ִ���ӣ���\n");
        scanf("%d",&useColor);
    }
    if(GameMode==2){
        printf("��ѡ��˫�˶�սģʽ��\n");
//        printf("��������0����1Ȼ��س���0��ִ����/1��ִ���ӣ���\n");
//        scanf("%d",&useColor);
    }
    system("pause");
    system("cls");
    Init();

    int x, y;
    int GameCanceled=1;
    halfround=0;

    // ��ӡ����
    DisplayChessBoard();

    if(GameMode==1){ //�˻���ս
        if(useColor==1){
            // AIѡ����һ������λ�ã�����ӡ
            ThinkNextDrop();
            DropPiece(aix,aiy,halfround);
            halfround++;

            printf("the AI has dropped: %d %d\n",aix,aiy);
            printf("Round 0.Input position to drop for white(input 'Ctrl+D' to exit):");
        }
        else{ //��ʾ�û���������
            printf("Round 0.Input position to drop for black(input 'Ctrl+D' to exit):");
        }
    }
    else{
        //��������
        strcpy(color,"black");
        printf("Round %d.Input position to drop for %s(input 'Ctrl+D' to exit):",halfround/2,color);
    }

    while(scanf("%d%d",&x,&y)){// �û���������λ��

        if(GameMode==2){ //˫�˶�սģʽ
            if( !DropInsideBoard(x,y) ){ // �����λ�ó������̷�Χ����������
                printf("Drop outside board! Chose another position:");
                continue;
            }
            if( DropOverlaping(x,y) ){ // �����λ���Ѿ���ռ�ݣ���������
                printf("This position has been taken! Chose another one:");
                continue;
            }

            // ��һ����
            DropPiece(x,y,halfround);

            // ����
            if(GameIsOver(x,y)){ //��Ϸ����
                GameCanceled=0;
                break;
            }
            else{// ��Ϸ������������һ��half round
                halfround++;

                // ���ֵ��ķ��ж�
                if(halfround%2){
                    strcpy(color,"white");
                }
                else{
                    strcpy(color,"black");
                }

                // ��ʾ�û���������λ��
                printf("Round %d.Input position to drop for %s(input 'Ctrl+D' to exit):",halfround/2,color);
            }
        }
        else{ //�˻���սģʽ��
            if( !DropInsideBoard(x,y) ){ // �����λ�ó������̷�Χ����������
                printf("Drop outside board! Chose another position:");
                continue;
            }
            if(DropOverlaping(x,y) ){ // �����λ���Ѿ���ռ�ݣ���������
                printf("This position has been taken! Chose another one:");
                continue;
            }

            // ��һ����
            DropPiece(x,y,halfround);

            // ����
            if(GameIsOver(x,y)){ //��Ϸ����
                GameCanceled=0;
                break;
            }
            else{
                halfround++;
                // AI˼����һ��������һ����
                ThinkNextDrop();
                DropPiece(aix,aiy,halfround);
                printf("the AI has dropped: %d %d\n",aix,aiy);

                if(GameIsOver(aix,aiy)){ //��Ϸ����
                    GameCanceled=0;
                    break;
                }
                else{// ��Ϸ������������һ��half round
                    halfround++;

                    // ���ֵ��ķ��ж�
                    if(halfround%2){
                    strcpy(color,"white");
                    }
                    else{
                        strcpy(color,"black");
                    }

                    // ��ʾ�û���������λ��
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

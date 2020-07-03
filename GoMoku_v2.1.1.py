import os
from graphics import *
import math

VERSION = "2.1.1"
MAX_COL = 15
MAX_ROW = 15
GRID_WIDTH = 35
GRAPH_WIDTH = GRID_WIDTH * (MAX_ROW + 1)
GRAPH_HEIGHT = GRID_WIDTH * (MAX_COL + 1)


ChessBoard = 0
aiX, aiY = MAX_COL//2, MAX_ROW//2
humanX, humanY = 0, 0
nextX, nextY = 0, 0
Halfround = 0
dX = [-1,-1, 0, 1]
dY = [ 0, 1, 1, 1]
pieceShape = ['x','o']
Color = ['Black','White']
gameMode, userColor = '1','1'
numMomery = 0
LosingGamePositions = []
WinningGamePositions = [
     10000000000000120000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 , # 花月式
     10000000000000021000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, # 花月式
     12000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, # 花月式
     1000000000000012000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, # 花月式
     21000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, # 花月式
     120000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, # 花月式
     100000000000000210000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, # 花月式
     210000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, # 花月式
     10000000000000201000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, # 以下八行是斜指的花月式
     100000000000000010000000000000200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,
     10000000000000102000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,
     1000000000000010000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,
     2000000000000010000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,
     102000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,
     201000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,
     200000000000000010000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000]
GameCanceled = False
PieceValue = 0
Choices=[]
numChoice=0
humanMove = 0
# win = GraphWin()
# win = 0

def getPieceValue(x,y):
    global ChessBoard
    pow10 = 1
    for ii in range(x * MAX_ROW + y): pow10*=10
    return ( ChessBoard - ChessBoard//(pow10*10)*pow10*10 ) // pow10
def isValidInput(x):
    return int(x)<=15 and int(x)>=-1
def DropInsideBoard(x,y):
    global MAX_COL, MAX_ROW
    return ((x>=0) and (x<MAX_COL) and (y>=0) and (y<MAX_ROW))
def DropOverlaping(x,y):
    return (getPieceValue(x,y) != 0)
def PrintFirstPage():
    global VERSION
    os.system("mode con cols=120 lines=80")
    os.system("title = GoMoku_zqw")
    print("#########################################################################################\n#")
    print("# 欢迎使用GoMoku_v",VERSION,sep="")
    print("# ——它终于有了一个可用鼠标交互的界面，并提供了一个学习型的人工智障供玩家练习\n#\n#")
    print("# 使用说明：")
    print("#     落子的位置都由用户鼠标点击对应的棋盘线交点确定。")
    print("#     棋局开始前的模式选择和每一盘结束后选择是否续局这两个部分的输入需要在命令行窗口键入正整数。")
    # print("#     输入格式为两个（十进制）正整数，分别代表行号和列号（例如：2 13），以空格隔开。")
    # print("#     棋盘大小为15x15，用'x'代表黑子，'o'代表白子，'.'代表无棋子。")
    # print("#     棋盘边缘标出了行号和列号。")
    print("#     玩家可以选择双人对战或者人机对战。")
    print("#     人机模式下的电脑AI的策略非常简单，虐爆它几乎没有难度。")
    print("#     AI能从输掉的对局中学习到经验，所以请尽情虐爆它。(短时间你可能感受不到，因为目前它学得很慢)")
    print("#     AI学习到的经验存储在同一目录下的GM_mem.dat文件中。使用结束后请务必将这个文件发送给我，感谢！")
    print("#     暂时不提供悔棋功能，但提供了续局功能。")
    # print("#     提示：按住Ctrl并滚动鼠标中键可以调整显示大小。")
    print("#")
    print("# v1.1版本的更新：")
    print("#     修复了v1.0游戏结束后闪退的bug。")
    print("#     增强的AI，想虐爆这个版本的AI稍微需要动一点脑子。")
    print("# v1.2.1版本的更新：")
    print("#     修复了双人模式下一些情况下无法判定五子连珠的bug。")
    print("#     增加了模式选择功能，选择人机对战模式后用户将不需要为AI落子。")
    print("#     更新了棋盘显示（特别感谢来自yuukai的建议）")
    print("# v2.1版本的更新：")
    print("#     源代码从C改成了Python。")
    print("#     增加了一个可用鼠标交互的图形界面。(感谢Kevinten的博客)")
    print("#     AI策略稍稍修改但是仍然几乎不变，但它现在能从输掉的对局中学到经验。(短时间你可能感受不到，因为目前它学得很慢)")
    print("# v2.1.1版本的更新：")
    print("#     修改了评分系统中个别系数，AI有了更好的表现。")
    print("#     修复了部分造成学习缓慢的BUG。")
    print("#\n#\n#\n#")
    print("# 最后，欢迎任何形式问题或建议，我的邮箱：zhuqiwen1600012437@pku.edu.cn")
    print("#########################################################################################")
    # print("建议你将该窗口全屏，按住Ctrl滚动鼠标中键尽量让文字充满整个屏幕。")
    os.system("pause")
    os.system("cls")
def SelectMode():
    global gameMode, userColor
    print("请选择模式，输入数字1或者2然后回车（1：人机对战/2：双人对战）：")
    gameMode=input()
    if(gameMode=='1'): # human-ai mode
        print("已选择人机对战模式，请选择执黑子（先手）还是白子（后手）。")
        print("输入数字0或者1然后回车（0：执黑子/1：执白子）：")
        userColor = input()
    if(gameMode=='2'): # human-human mode
        print("已选择双人对战模式。")
    os.system("pause")
    os.system("cls")
def draw_window():
    global MAX_ROW, MAX_COL, GRID_WIDTH
    """
    绘制棋盘
    """
    # 绘制画板
    win = GraphWin("GoMoku", GRAPH_HEIGHT, GRAPH_WIDTH)
    win.setBackground("gray")
    # 绘制列
    i1 = 0
    while i1 <= GRID_WIDTH * MAX_COL:
        i1 = i1 + GRID_WIDTH
        l = Line(Point(i1, GRID_WIDTH), Point(i1, GRID_WIDTH * MAX_COL))
        l.draw(win)
    # 绘制行
    i2 = 0
    while i2 <= GRID_WIDTH * MAX_ROW:
        i2 = i2 + GRID_WIDTH
        l = Line(Point(GRID_WIDTH, i2), Point(GRID_WIDTH * MAX_ROW, i2))
        l.draw(win)
    return win
def DropPiece(x, y):
    global Halfround, ChessBoard, win
    PieceValue=Halfround%2+1
    ii=0
    while(ii<MAX_ROW*x+y):
        ii+=1
        PieceValue*=10
    ChessBoard += PieceValue
    piece=Circle(Point(GRID_WIDTH * (y+1), GRID_WIDTH * (x+1)), 12)
    if(Halfround%2==1):
        piece.setFill('White')
    else:
        piece.setFill('Black')
    piece.draw(win)
    Halfround+=1 
    # print("# checking chess board:",ChessBoard) # 调试信息
def GameIsOver(x, y):
    for dirctn in range(4):
        numPiece=1
        nextX=x+dX[dirctn]
        nextY=y+dY[dirctn]
        while(DropInsideBoard(nextX,nextY) and getPieceValue(nextX,nextY)==((Halfround-1)%2+1)):
            numPiece+=1
            nextX+=dX[dirctn]
            nextY+=dY[dirctn]
        nextX=x-dX[dirctn]
        nextY=y-dY[dirctn]
        while(DropInsideBoard(nextX,nextY) and getPieceValue(nextX,nextY)==((Halfround-1)%2+1)):
            numPiece+=1
            nextX-=dX[dirctn]
            nextY-=dY[dirctn]
        if(numPiece==5):
            print("Game Over:",Color[(Halfround-1)%2],"wins!")
            return ((Halfround-1)%2+1)
    if(Halfround==MAX_COL*MAX_ROW):
        print("Game Over: Drawn!")
        return 3
    return 0
def ThinkNextDrop():
    global aiX, aiY, Halfround, Choices, LosingGamePositions, ChessBoard, MAX_ROW, WinningGamePositions
    NeighborStrategy()
    # for ii in range(MAX_COL):
    #     for jj in range(MAX_ROW):
    for point in sorted(Choices, key=lambda pp:((pp[0]-7)*(pp[0]-7)+(pp[1]-7)*(pp[1]-7))):
        PieceValue=Halfround%2+1
        for iii in range(MAX_ROW*point[0]+point[1]): PieceValue*=10
        if ChessBoard+PieceValue in WinningGamePositions:
            aiX, aiY = point[0], point[1]
            return 

    for point in sorted(Choices, key=lambda pp:((pp[0]-7)*(pp[0]-7)+(pp[1]-7)*(pp[1]-7))):
        # if(DropInsideBoard(point[0],point[1]) and not DropOverlaping(point[0],point[1])):
        badMove=False
        PieceValue=Halfround%2+1
        for iii in range(MAX_ROW*point[0]+point[1]): PieceValue*=10
        # print("# checking choice:",point,"# cressponding chessboard value:",ChessBoard+PieceValue)
        # print("# checking types:",type(ChessBoard+PieceValue),'VS',type(LosingGamePositions[0]))
        # print("# checking minus [0] :",ChessBoard+PieceValue-LosingGamePositions[0])
        # print("# checking minus [1] :",ChessBoard+PieceValue-LosingGamePositions[1])
        if ChessBoard+PieceValue in LosingGamePositions:
            print("Ah! I've learned that!")
            badMove=True
            break
        if badMove:
            continue
        else:
            aiX, aiY = point[0], point[1]
            return 
    LearnFromLosing()
  
def RandomStrategy(): # abandoned
    pass

def NeighborStrategy():
    global aiX, aiY, Choices, MAX_ROW, MAX_COL, ChessBoard, LosingGamePositions
    maxScore = 0
    for ii in range(MAX_COL):
        for jj in range(MAX_ROW):
            PieceValue=Halfround%2+1
            for iiii in range(MAX_ROW * ii + jj): PieceValue*=10
            if(DropOverlaping(ii,jj)):
                continue
            if (ChessBoard + PieceValue) in LosingGamePositions:
                continue 
            # else, an empty position
            Score = 0
            for dirctn in range(4):
                maxlocalScore = 0
                avelocalScore = 0.0
                for npp in range(5):

                    if(not DropInsideBoard(ii+npp*dX[dirctn],jj+npp*dY[dirctn])): # posi-end point outside board
                        continue
                    negEndX = ii-(4-npp)*dX[dirctn]
                    negEndY = jj-(4-npp)*dY[dirctn]
                    if(not DropInsideBoard(negEndX,negEndY)): # neg-end point outside board
                        continue

                    localScore = 0
                    numFriendPiece = 0
                    numOppositePiece = 0
                    for iii in range(5):
                        nextX = negEndX + iii*dX[dirctn]
                        nextY = negEndY + iii*dY[dirctn]
                        if(getPieceValue(nextX,nextY) == (Halfround%2+1)): numFriendPiece+=1
                        if(getPieceValue(nextX,nextY) == ((Halfround+1)%2+1)): numOppositePiece+=1
                    if(numFriendPiece and numOppositePiece): localScore = 0
                    elif(numFriendPiece==4):   localScore = 4*4*4*4*4
                    elif(numOppositePiece==4): localScore = 4*4*4*4
                    elif(numOppositePiece==3): localScore = 4*3*3
                    else:
                        localScore=numFriendPiece*numFriendPiece*numFriendPiece + \
                            numOppositePiece*numOppositePiece*numOppositePiece
                    maxlocalScore = max(maxlocalScore,localScore)
                    avelocalScore += localScore
                avelocalScore = math.floor(avelocalScore/5) + 1
                Score += (maxlocalScore*0 + avelocalScore*3)
                # Score += maxlocalScore
            if(Score >= maxScore):
                if(Score > maxScore):
                    aiX, aiY = ii, jj
                    maxScore = Score
                    Choices = [tuple([ii,jj])]
                elif(maxScore > 0): # same Score as maxScore
                    Choices.append(tuple([ii,jj]))
    return

def LearnFromLosing():
    global numMomery, LosingGamePositions, humanMove
    # print("# checking humanMove:",humanMove)
    if (ChessBoard - humanMove) in LosingGamePositions:
        return
    LosingGamePositions.append(ChessBoard - humanMove)
    # print("# check humanMove:",humanMove)
    # print("# check chessboard - humanMove inside LearnFunction:", (ChessBoard - humanMove))
    numMomery += 1
    print("AI looks at the chess board thoughtfully...\n")                
    
def ReadMemoryFromFile():
    global LosingGamePositions, numMomery
    if not os.path.exists('GM_mem.dat'):
        print("No memory file found!")
        return 
    print("Reading memory from file...")
    infile=open('GM_mem.dat','r',encoding='utf-8')
    mem=infile.read().splitlines()
    infile.close()
    for ii in mem:
        LosingGamePositions.append(int(ii))
        numMomery+=1
        print(ii)
    print("Done reading!")
    os.system("pause")
    
def WriteMemoryToFile():
    print("Writing memory to file...")
    outfile=open('GM_mem.dat','w',encoding='utf-8')
    for ii in range(numMomery):
        print(LosingGamePositions[ii],file=outfile)
        print(LosingGamePositions[ii])
    outfile.close()
    print("Done writing!")
    os.system("pause")

# def Main():
PrintFirstPage()
SelectMode()
# initialization
GameCanceled = False
# clear memory
numMomery = 0
LosingGamePositions=[]
ReadMemoryFromFile()    

while(1):
    # open a new game
    ChessBoard=0
    Halfround=0
    aiX, aiY = 7, 7
    win = draw_window()
    if gameMode == '1': # human-ai mode
        if userColor == '1': # user at white side
            ThinkNextDrop()
            DropPiece(aiX, aiY)
            print("AI has dropped at: ",aiX, aiY)
            # print("Round 0. Input position to drop: ")
        # else:
            # print("Round 0. Input position to drop: ")
    # else: # human-human mode
        # print("Round",Halfround//2,". Input position to drop: ")
    while(1):
        # take input from keyboard
        # print("Round",Halfround//2,". Input position to drop for",Color[Halfround%2],"side:")
        # humanX, humanY=input("(在[0, 14]范围内输入两个正整数，以空格隔开，输入-1 -1来终止比赛):\n").strip().split()
        pp = win.getMouse()
        humanX, humanY = round((pp.getY())/GRID_WIDTH)-1, round((pp.getX())/GRID_WIDTH)-1
        
        # print(getPieceValue(humanX,humanY))

        # while(not isValidInput(humanX) or not isValidInput(humanY)):
                # humanX, humanY=input("(Wrong input, please try again!):\n").strip().split()
        # check validity of drop and take that drop if it is a legal move and record the human move
        if(not DropInsideBoard(humanX,humanY)):
            # print(humanX,humanY)
            print("Drop outside board! Please try again!")
            continue
        if(DropOverlaping(humanX,humanY)):
            print("This position has been taken! Please try again!")
            continue
        humanMove=Halfround%2+1
        DropPiece(humanX,humanY)
        print("You dropped at:",humanX,humanY)
        for ii in range(MAX_ROW*humanX+humanY): humanMove*=10

        if(GameIsOver(humanX,humanY)):
            if(gameMode =='1'):
                # print("# check learnfromlosing is called:@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
                # print("$ check humanMove outside function:", (humanMove))
                # print("$ check chessboard - humanMove outside function:", (ChessBoard - humanMove))
                LearnFromLosing()
            break
        else: # go on
            if(gameMode=='2'): # human-human mode
                print("Round",Halfround//2,". Click a position to drop for",Color[Halfround%2],"side:")
            else: # human-ai mode
                ThinkNextDrop()
                DropPiece(aiX,aiY)
                print("AI has dropped at:",aiX,aiY)
                if(GameIsOver(aiX,aiY)):
                    break
                else: # go on
                    print("Round",Halfround//2,". Click a position to drop for",Color[Halfround%2],"side:")   
    WriteMemoryToFile()
    goOn=input("输入一个正整数(1:继续/0:退出):\n")
    if goOn=='0':
        break
        os.system("pause")
    # win = GraphWin("GoMoku", GRAPH_HEIGHT, GRAPH_WIDTH)
    # draw_window()
    win.close()
# win.getMouse()
win.close()
WriteMemoryToFile()
print("#####################################################################")
print("# 请将当前文件夹中的GM_mem.dat（AI学到的经验）发给我，感谢（比心）！#")
print("#####################################################################")
# os.system("pause")

# Main()
#include "typedef.h"
#include "time.h"
/*
class CPlayer {
      int  score;
      char gameFilePath[MAX_PATH];
      CDisplay* pDisplay;
      CBoard&   board;
      
      HWND hWnd; //main window 
      UINT_PTR hTimer;
      int  command;
      CBrick* pBrick;//CTrapezium* pTrapezium;
      void newGame();
      void saveGame();
      void openGame();
public:
       CPlayer(CBoard& board_);
       void init(HWND hWnd_);
       bool clickButton(LPARAM hButton);
       void executeCommand();
       ~CPlayer();
};
*/
CPlayer::CPlayer(CBoard& board_):board(board_) {
      hTimer = 0;
      pDisplay = NULL;
      pBrick = NULL;
      command = UNDEFINED;
      return; 
}

void CPlayer::init(HWND hWnd_) {
     hWnd = hWnd_;     
     pDisplay = new CDisplay(hWnd_);
     board.init(pDisplay);
     return; 
}

void CPlayer::newGame() {
     srand( time(NULL) );
     board.clear();
     score = 0;
     if (pBrick!=NULL) CBrick::deleteBrick(pBrick);

     pBrick = CBrick::newBrick();//newBrick();
     if ( pBrick==NULL ) return;
     command = UNDEFINED;
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
     hTimer = SetTimer(hWnd, TIMER_ID, MV_SPEED, NULL);
     pDisplay->show(score);

     return;      
}

void CPlayer::executeCommand() {
     short result;
     if (pBrick==NULL) return;
     switch ( command ) {
     case MV_LEFT:
          result = pBrick->mvLeft();
          break;
     case MV_RIGHT:
          result = pBrick->mvRight();
          break;
     case RT_LEFT:
          result = pBrick->rotateLeft();
          break;
     case RT_RIGHT:
          result = pBrick->rotateRight();
          break;
     default:
          result = pBrick->mvDown(command);
          break;     
     }
     
     if ( result != STOPPED){
          if (command!=UNDEFINED) (pBrick->score)--;
          command = UNDEFINED;
          return;
     }
     score += pBrick->stop();
     pDisplay->show(score);
     //delete pBrick;
     pBrick = CBrick::newBrick();
     if ( pBrick==NULL ) pDisplay->show("Game Over");
     return;
}

CPlayer::~CPlayer() {
      if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
      if (pDisplay != NULL) delete pDisplay;
      if (pBrick!=NULL) delete pBrick;
      return; 
}

bool CPlayer::clickButton(LPARAM hButton) {
     
     command = pDisplay->parseCommand(hButton);
     switch ( command ) {
            case NEW_GAME:
                 newGame();
                 break;
            case SAVE_GAME:
                 saveGame();
                 break;
            case OPEN_GAME:
                 openGame();
                 break;
            case EXIT_GAME:
                 return false;
            default:
                    break;
     }
     return true;
}

void CPlayer::saveGame() {
     OPENFILENAME ofnGame;
          
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
         
     ZeroMemory(&ofnGame, sizeof(ofnGame));
     ofnGame.lStructSize = sizeof(ofnGame);
     ofnGame.hwndOwner = hWnd;
     ofnGame.lpstrFilter = "brick-game files (*.brk)\0*.brk\0\0";
     ofnGame.lpstrFile = gameFilePath;
     ofnGame.nMaxFile = MAX_PATH;
     ofnGame.lpstrDefExt = "brk";
     ofnGame.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
         OFN_OVERWRITEPROMPT;

     while (gameFilePath[0] == 0) 
         if (GetOpenFileName(&ofnGame)==false) {
         hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
         return;
         }
     ofstream gameFile(gameFilePath, ios::out|ios::binary);
     /*************************************************************************/
     /*                                                                       */
     /*           添加代码，将游戏进度保存到文件"gameFile"中                  */
     /*                                                                       */
     /*************************************************************************/
     gameFile.write((char*)&score,sizeof(int));
     board.puttofile(gameFile);
     int tmp;
     if (pBrick!=NULL) tmp = pBrick->type; else tmp=0;
     gameFile.write((char*)&tmp,sizeof(int));
     switch(tmp){
            case 1:      
                 gameFile.write((char*)pBrick,sizeof(CTriangle));
                 break;
            case 2:
                 gameFile.write((char*)pBrick,sizeof(CSquare));
                 break;
            case 3:
                 gameFile.write((char*)pBrick,sizeof(CTrapezium));
                 break;
            case 4:  
                 gameFile.write((char*)pBrick,sizeof(CRectangle));
                 break;
            case 0:
                 break;
         }
     gameFile.close();
     hTimer = SetTimer(hWnd, TIMER_ID, MV_SPEED, NULL);
     command = UNDEFINED;
     return;
}

void CPlayer::openGame() {
     OPENFILENAME ofnGame;
     
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
      
     ZeroMemory(&ofnGame, sizeof(ofnGame));
     ofnGame.lStructSize = sizeof(ofnGame);
     ofnGame.hwndOwner = hWnd;
     ofnGame.lpstrFilter = "brick-game files (*.brk)\0*.brk\0\0";
     ofnGame.lpstrFile = gameFilePath;
     ofnGame.nMaxFile = MAX_PATH;
     ofnGame.lpstrDefExt = "brk";
     ofnGame.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

     gameFilePath[0] = 0;
     while (gameFilePath[0] == 0) 
         if (GetOpenFileName(&ofnGame)==false) {
         hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
         return;
         }
         
     ifstream gameFile(gameFilePath, ios::in|ios::binary);
     /*************************************************************************/
     /*                                                                       */
     /*           添加代码，从文件"gameFile"中读入游戏进度                    */
     /*                                                                       */
     /*************************************************************************/
     srand( time(NULL) );
     board.clear();
     score = 0;
     if (pBrick!=NULL) CBrick::deleteBrick(pBrick);
     gameFile.read((char*)&score,sizeof(int));
     int t;
     CBrick *tmp;
     gameFile.read((char*)&t,sizeof(int));
     while (t){
           
         switch(t){
            case 1:  
                 tmp = new CTriangle(0,0,0,0);      
                 gameFile.read((char*)tmp,sizeof(CTriangle));
                 break;
            case 2:
                 tmp = new CSquare(0,0,0,0);
                 gameFile.read((char*)tmp,sizeof(CSquare));
                 break;
            case 3:
                 tmp = new CTrapezium(0,0,0,0);
                 gameFile.read((char*)tmp,sizeof(CTrapezium));
                 break;
            case 4:
                 tmp = new CRectangle(0,0,0,0);  
                 gameFile.read((char*)tmp,sizeof(CRectangle));
         }  
         board<<tmp;
         tmp->showup();
         gameFile.read((char*)&t,sizeof(int));
     }
     gameFile.read((char*)&t,sizeof(int));
     switch(t){
            case 1:  
                 tmp = new CTriangle(0,0,0,0);      
                 gameFile.read((char*)tmp,sizeof(CTriangle));
                 break;
            case 2:
                 tmp = new CSquare(0,0,0,0);
                 gameFile.read((char*)tmp,sizeof(CSquare));
                 break;
            case 3:
                 tmp = new CTrapezium(0,0,0,0);
                 gameFile.read((char*)tmp,sizeof(CTrapezium));
                 break;
            case 4:
                 tmp = new CRectangle(0,0,0,0);  
                 gameFile.read((char*)tmp,sizeof(CRectangle));
                 break;
            case 0:
                 tmp = CBrick::newBrick();
         }
     pBrick=tmp;
     command = UNDEFINED;
     
     pDisplay->show(score);
     gameFile.close();
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
     hTimer = SetTimer(hWnd, TIMER_ID, MV_SPEED, NULL);
     return;
}

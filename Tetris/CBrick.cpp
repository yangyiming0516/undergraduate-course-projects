#include "typedef.h"
/*
class CBrick {
      const static unsigned char brickType[SHAPE_NUM];
protected:
      static CBoard &board;
      short  x,y;
      unsigned char color, direction;
      virtual void  erase()=0;
      virtual void  show()=0;
      virtual bool isProped()=0;
public:
      short  score;
      CBrick(short x_, short y_, char color_, unsigned char direction_);
      virtual ~CBrick();
      static CBrick* newBrick();
      void stop();
      int  mvDown(int command=UNDEFINED);
      virtual int  mvLeft()=0;
      virtual int  mvRight()=0; 
      virtual int  rotateLeft();
      virtual int  rotateRight();
friend class CDisplay;
};
*/
CBrick::CBrick(short x_, short y_, char color_, unsigned char direction_){
     x = x_;
     y = y_;
     color = color_ % (COLOR_NUM+1);
     direction = direction_;
     return;
}

void CBrick::showup(){
     this->show();     
}

int CBrick::stop(){
     switch (direction){
     case SQUARE:
          board.setrow(y,2,false);
          break;
     case HRECTANGLE:
          board.setrow(y,6,false);
          break;
     case VRECTANGLE:
          board.setrow(y+1,2,false);
          board.setrow(y,2,true);
          board.setrow(y-1,2,true);
          break;
     case LUTRIANGLE:
     case RUTRIANGLE:
     case RDTRIANGLE:
     case LDTRIANGLE:
          board.setrow(y,1,false);
          break;          
     case HUTRAPEZIUM:
     case HDTRAPEZIUM:
          board.setrow(y,4,false);
          break;
     case VRTRAPEZIUM:
     case VLTRAPEZIUM:
          board.setrow(y+1,1,false);
          board.setrow(y,2,true);
          board.setrow(y-1,1,true);
          break;
     default:
           break;
     }
     score += abs(x-sx);
     score += abs(y-sy);
     short temp=0;
     switch (direction){
     case SQUARE:
          break;
     case HRECTANGLE:
     case VRECTANGLE:
          if (direction!=sd) temp++; 
          break;
     case LUTRIANGLE:
     case RUTRIANGLE:
     case RDTRIANGLE:
     case LDTRIANGLE:
          temp = abs(sd-direction)/3;
          if (temp > 2) temp -= 2;
          break;          
     case HUTRAPEZIUM:
     case HDTRAPEZIUM:
     case VRTRAPEZIUM:
     case VLTRAPEZIUM:
          if ((sd&0xF) != (direction&0xF)) temp++;
          if (sd>>4 != direction>>4) temp++;
          break;
     default:
           break;
     }
     score += temp;
     return board<<this;
}

int CBrick::getRowofCenter(){
    return y;
}

int CBrick::getX(){
    return x;
}

int CBrick::deleteBrick(CBrick* ptr){
    ptr->erase();
    int r = ptr->score;
    delete ptr;
    return r;
}

void CBrick::drop(int Height){
     this->erase();
     y-=Height;
     this->show();
}

int CBrick::mvDown(int command){
    int mvSteps=0;
    while ( isProped()==false ) {
          mvSteps++;
          if ( mvSteps==1 ) erase();
          y--;
          if (command==UNDEFINED) break;
    } 
    if ( mvSteps==0 ) return STOPPED;
    show();
    return MV_DOWN;
}

int CBrick::rotateLeft(){
     return mvDown();
}

int CBrick::rotateRight(){
     return mvDown();
}


CBrick* CBrick::newBrick(){
     CBrick* result=NULL;     
     int color = rand()%COLOR_NUM + 1;
     unsigned short curBrickType = rand()%SHAPE_NUM;
     if ( board.getRim(6, 30)&LUTRIANGLE&RUTRIANGLE ) return result;
     switch ( brickType[curBrickType] ) {
     case SQUARE:
          if (board.getRim(6, 30)) return newBrick();
          result = new CSquare(6,30,color,brickType[curBrickType]);
          break;
     case VRECTANGLE:
          if ( board.getRim(6, 30)||board.getRim(6, 29)|| board.getRim(6, 28)) return newBrick();
          result = new CRectangle(6,29,color,brickType[curBrickType]);
          break;
     case HRECTANGLE:
          if ( board.getRim(5, 30)|| board.getRim(6, 30)||board.getRim(7, 30)) return newBrick();
          result = new CRectangle(6,30,color,brickType[curBrickType]);
          break;
     case LUTRIANGLE:
     case RUTRIANGLE:
     case RDTRIANGLE:
     case LDTRIANGLE:
          if ( board.getRim(6, 30)&brickType[curBrickType] ) return newBrick();
          result = new CTriangle(6, 30, color, brickType[curBrickType]);
          break;          
     case HUTRAPEZIUM:
     case HDTRAPEZIUM:
          if ( board.getRim(5, 30)|| board.getRim(6, 30)||board.getRim(7, 30)) return newBrick();
          result = new CTrapezium(6,30,color,brickType[curBrickType]);
     case VRTRAPEZIUM:
     case VLTRAPEZIUM:
          if ( board.getRim(6, 30)||board.getRim(6, 29)|| board.getRim(6, 28)) return newBrick();
          result = new CTrapezium(6, 29, color, brickType[curBrickType]);
          break;
     default:
           break;
     }
     if (result!=NULL) result->show();
     return result;
}

CBrick::~CBrick() {
}

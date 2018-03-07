#include "typedef.h"

/*
class CRectangle:public CBrick{
      virtual void  erase();
      virtual void  show();
      virtual bool  isProped();
friend class CBoard;
      public:
      CRectangle(short x_, short y_, char color_, unsigned char direction_);
      virtual int  mvLeft();
      virtual int  mvRight(); 
      virtual int  rotateLeft();
      virtual int  rotateRight();     
}
*/


CRectangle::CRectangle(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
  type=4;
   score = 6;
   switch ( direction_ ) {
            case HRECTANGLE:
                 if ( x_<2 || x_>10 ) x = 6;
                 if ( y_>30 || y_<1 ) y = 30;
                 break;
            case VRECTANGLE:
                 if ( x_<1 || x_>11 ) x = 6;
                 if ( y_>29 || y_<2 ) y = 29;
                 break;
            default:
                 x=0;
                 y=0;
                 color = 0;
                 direction = 0;
                 return;
     }
     sx =x;sy=y;sd=direction;
     return;                                                  
}

bool CRectangle::isProped(){
     bool result;
     switch ( direction ) {
     case HRECTANGLE:
          result = board.getRim(x,y-1)|board.getRim(x-1,y-1)|board.getRim(x+1,y-1);
          break;
     case VRECTANGLE:
          result = board.getRim(x,y-2);
          break;
     }
     return result;
}


int CRectangle::mvLeft(){
     bool  stopped;

     switch ( direction ) {
            case HRECTANGLE:
                 stopped = board.getRim(x-2,y);
                 break;
            case VRECTANGLE:
                 stopped = board.getRim(x-1,y-1)|board.getRim(x-1,y)|board.getRim(x-1,y+1);
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     x--;
     show();
     return MV_LEFT;
}
int CRectangle::mvRight(){
     bool  stopped;

     switch ( direction ) {
            case HRECTANGLE:
                 stopped = board.getRim(x+2,y);
                 break;
            case VRECTANGLE:
                 stopped = board.getRim(x+1,y-1)|board.getRim(x+1,y)|board.getRim(x+1,y+1);
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     x++;
     show();
     return MV_RIGHT;
}

int CRectangle::rotateLeft(){
     bool  stopped=true;
     
     switch ( direction ) {
            case HRECTANGLE:
                 stopped = board.getRim(x+1,y+1) | board.getRim(x,y+1) | board.getRim(x,y-1) | board.getRim(x-1,y-1);
                 break;
            case VRECTANGLE:
                 stopped = board.getRim(x-1,y) | board.getRim(x-1,y+1) | board.getRim(x+1,y) | board.getRim(x+1,y-1);
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     if (direction == HRECTANGLE) direction = VRECTANGLE; else direction = HRECTANGLE;
     show();
     return RT_LEFT;
}

int CRectangle::rotateRight(){
    bool  stopped=true;
     
     switch( direction){
     	case HRECTANGLE:
     		stopped = board.getRim(x-1,y+1) | board.getRim(x,y+1) | board.getRim(x,y-1) | board.getRim(x+1,y-1);
     		break;
     	case VRECTANGLE:
     		stopped = board.getRim(x-1,y) | board.getRim(x-1,y-1) | board.getRim(x+1,y) | board.getRim(x+1,y+1);
     		break;
     	default:
        return UNDEFINED;
     }
     if ( stopped ) return mvDown();
     erase();
     if (direction == HRECTANGLE) direction = VRECTANGLE; else direction = HRECTANGLE;
     show();
     return RT_RIGHT;
}

void  CRectangle::show(){
     board<<CTriangle(x, y, color, LUTRIANGLE)<<CTriangle(x, y, color, RDTRIANGLE);;
     switch ( direction ) {
            case HRECTANGLE:
                 board<<CTriangle(x-1, y, color, RDTRIANGLE)<<CTriangle(x-1, y, color, LUTRIANGLE);
                 board<<CTriangle(x+1, y, color, LDTRIANGLE)<<CTriangle(x+1, y, color, RUTRIANGLE);
                 break;
            case VRECTANGLE:
                 board<<CTriangle(x, y-1, color, LUTRIANGLE)<<CTriangle(x, y-1, color, RDTRIANGLE);
                 board<<CTriangle(x, y+1, color, LDTRIANGLE)<<CTriangle(x, y+1, color, RUTRIANGLE);
                 break;
            default:
                 break;
     }
     
     return;
}

void  CRectangle::erase(){
     board>>CTriangle(x, y,0, LUTRIANGLE)>>CTriangle(x, y,0, RDTRIANGLE);
     switch ( direction ) {
            case HRECTANGLE:
                 board>>CTriangle(x-1, y,0, RDTRIANGLE)>>CTriangle(x-1, y,0, LUTRIANGLE);
                 board>>CTriangle(x+1, y,0, LDTRIANGLE)>>CTriangle(x+1, y,0, RUTRIANGLE);
                 break;
            case VRECTANGLE:
                 board>>CTriangle(x, y-1,0, RDTRIANGLE);
                 board>>CTriangle(x, y-1,0, LUTRIANGLE);
                 board>>CTriangle(x, y+1,0, LDTRIANGLE);
                 board>>CTriangle(x, y+1,0, RUTRIANGLE);
                 break;
            default:
                 break;
     } 
     return;
}

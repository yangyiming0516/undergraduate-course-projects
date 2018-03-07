#include "typedef.h"
/*CSquare::CSquare(short x_, short y_, char color_);
virtual int  mvLeft();
      virtual int  mvRight(); 
      virtual int  rotateLeft();
      virtual int  rotateRight();  
virtual void  erase();
      virtual void  show();
      virtual bool  isProped();
      
};*/


CSquare::CSquare(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
     type=2;
     score = 2;
     if ( x_<1 || x_>11 ) x = 6;
     if ( y_>30 || y_<1 ) y = 30;
     switch ( direction_ ) {
            case SQUARE:
                 break;
            default:
                 x=0;
                 y=0;
                 color = 0;
                 direction = 0;
                 break;
     }
     sx =x;sy=y;sd=direction;
     return;                   
}

void CSquare::show(){
     board<<CTriangle(x, y, color, LUTRIANGLE)<<CTriangle(x, y, color, RDTRIANGLE);
     return;
}

void  CSquare::erase(){
     board>>CTriangle(x, y,0, LUTRIANGLE)>>CTriangle(x, y,0, RDTRIANGLE);
     return;
}

bool CSquare::isProped(){
     return board.getRim(x,y-1);

}


int  CSquare::mvLeft(){
     if (board.getRim(x-1,y)) return mvDown();
     erase();
     x--;
     show();
     return MV_LEFT;       
}


int  CSquare::mvRight(){
     if (board.getRim(x+1,y)) return mvDown();
     erase();
     x++;
     show();
     return MV_RIGHT;     
}



int CSquare::rotateLeft(){
    return mvDown();
    return RT_LEFT;   
}



int  CSquare::rotateRight(){
    return mvDown();
    return RT_RIGHT;   
}

#include "typedef.h"
/*
class CBoard {
      CDisplay* pDisplay;
      unsigned short pixels[31][13]; 
      vector<CBrick*> vecPTRBricks;
      SRowState rowStates[31];
public:
       CBoard();
       ~CBoard();
       bool init(CDisplay* pDisplay_);
       unsigned short  getRim(short x, short y);
       CBoard& operator<<(CTriangle triangle);
       CBoard& operator>>(CTriangle triangle);
};
*/
CBoard::CBoard() {
      int i;
      for( i=0; i<13; i++ ) pixels[0][i] = 0x030C;
      for( i=1; i<31; i++ ) pixels[i][0] = pixels[i][12] = 0x030C;
      for( int i=0; i<30; i++ )
          for( int j=0; j<11; j++) pixels[i+1][j+1]=0;
      pDisplay = NULL;
    
}

bool CBoard::init(CDisplay* pDisplay_) {
     if ( pDisplay != NULL ) return false;
     pDisplay = pDisplay_;
     //pDisplay->show(rowStates);
     return true;
}

unsigned short CBoard::getRim(short x, short y) {
     if ( x<1 || x>11 || y<1 || y>30 ) return UNDEFINED; 
     return ((pixels[y][x]>>8)|pixels[y][x])&0xF;
}

CBoard& CBoard::operator<<(CTriangle triangle) {
     int x=triangle.x;
     int y=triangle.y;
     unsigned char color = triangle.color;
     unsigned char direction = triangle.direction;
     
     if ( x<1 || x>11 || y<1 || y>30 ) return (*this);
     switch (((pixels[y][x]>>8)|pixels[y][x])&0xF) {
            case 0xF:
                 return (*this);
            case 0:
                 pixels[y][x] = (color<<4)|direction;
                 break;
            default:
                 if (direction&pixels[y][x]&0xF) return (*this);
                 pixels[y][x] = ((color<<4)|direction)|(pixels[y][x]<<8);
                 break;
     }
     if (pDisplay==NULL) return (*this);
     pDisplay->show(x, y, (color<<4)|direction);//(*pDisplay)<<tr;
     return (*this);      
}

CBoard& CBoard::operator>>(CTriangle triangle) {
     int x=triangle.x;
     int y=triangle.y;
     unsigned char direction = triangle.direction;


     if ( x<1 || x>11 || y<1 || y>30 ) return (*this);
     if ( (direction&0xF)== (pixels[y][x]&0xF) ) {
          triangle.color = (pixels[y][x]&0xF0)>>4;
          pDisplay->erase(x, y, direction);
          pixels[y][x]=pixels[y][x]>>8;
     }
     if ( (direction&0xF)== ((pixels[y][x]>>8)&0xF) ) {
          triangle.color = (pixels[y][x]&0xF000)>>12;
          pDisplay->erase(x, y, direction);
          pixels[y][x]=pixels[y][x]&0xFF;
     }
     return (*this);
}

CBoard::~CBoard() {
         
}


bool comparator_fun( CBrick* arg1,  CBrick* arg2) {
          if ( arg1->getRowofCenter()<arg2->getRowofCenter() ) return true;
          if ( arg1->getRowofCenter()==arg2->getRowofCenter() && arg1->getX()<arg2->getX()) return true;
          return false;
}

int CBoard::operator<<(CBrick* pBrick) {
    int result = 0; 
RECT rect;
vector<CBrick*>::iterator ptrBrick;
     
    vecPTRBricks.push_back(pBrick);
    //pDisplay->show(rowStates);
    rect.top = rect.bottom = 0;
    for (int i = 1;i <31;i++)
        if (rowStates[i].blankTriangles == 0){    
           rect.bottom = i;        
           for (int j = i;j<31;j++)
               if ((rowStates[j].blankTriangles == 0) && (!rowStates[j].linked)){
                    rect.top = j;
                    break;
               }
           break;          
        }
    if (rect.top==0) return result;
    int rowID;
    
    sort(vecPTRBricks.begin(), vecPTRBricks.end(), comparator_fun);
    
    ptrBrick = vecPTRBricks.begin();
    int height = rect.top - rect.bottom + 1;
    while ( ptrBrick!=vecPTRBricks.end() ) {
          rowID = (*ptrBrick)->getRowofCenter();
          if ( rowID>=rect.bottom && rowID<=rect.top ) {
               result += CBrick::deleteBrick(*ptrBrick);
               ptrBrick=vecPTRBricks.erase(ptrBrick);
               continue;
          }
          if ( rowID>rect.top ) (*ptrBrick)->drop(height);
          ptrBrick++;
    }
    for (int i = rect.bottom;i<=rect.top;i++){
        rowStates[i].blankTriangles = 22;
        rowStates[i].linked = false;
    }
return result;      
}


void CBoard::setrow(int n,int m,bool flag){
     rowStates[n].blankTriangles-=m;
     rowStates[n].linked = rowStates[n].linked || flag;
}
    
    
void CBoard::clear(){
     for (int i = 0;i<31;i++){
         rowStates[i].blankTriangles = 22;
         rowStates[i].linked = false;
     }     
     vector<CBrick*>::iterator ptrBrick;
     ptrBrick = vecPTRBricks.begin();
     while ( ptrBrick!=vecPTRBricks.end() ) {
               CBrick::deleteBrick(*ptrBrick);
               ptrBrick=vecPTRBricks.erase(ptrBrick);
               continue;
          }
     
}

void CBoard::puttofile(ofstream& gameFile){
     vector<CBrick*>::iterator it;
     it = vecPTRBricks.begin();
     for (it = vecPTRBricks.begin();it!=vecPTRBricks.end();it++)
      {   
          gameFile.write((char*)&((*it)->type),sizeof(int));
          switch((*it)->type){
            case 1:      
                 gameFile.write((char*)*it,sizeof(CTriangle));
                 break;
            case 2:
                 gameFile.write((char*)*it,sizeof(CSquare));
                 break;
            case 3:
                 gameFile.write((char*)*it,sizeof(CTrapezium));
                 break;
            case 4:  
                 gameFile.write((char*)*it,sizeof(CRectangle));
                 break;
            case 0:
                 break;
         }
      }
      int tmp=0;
      gameFile.write((char*)&tmp,sizeof(int));
}

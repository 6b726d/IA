class Quad {
    double x,y;     //Position in Board(Window)
    int i,j;        //Position in Board(Matrix)
    char type;      //-:White, +:Black, 0:Player, 1:AI
    //White:Disable, Black:Enable
public:
    Quad(double _x, double _y, int _i, int _j, char _type);
    double getX();
    double getY();
    int getI();
    int getJ();
    char getType();
    void setX(double nX);
    void setY(double nY);
    void setI(double nI);
    void setJ(double nJ);
    void setType(char nType);
};

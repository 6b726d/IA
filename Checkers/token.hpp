class Token {
    double x,y;     //Position
    bool player;    //0:Player, 1:AI
    bool type;      //0:Lady, 1:Queen
public:
    Token(double _x, double _y, bool _player, bool _type);
    double getX();
    double getY();
    bool getPlayer();
    bool getType();
    void setX(double nX);
    void setY(double nY);
    void setPlayer(bool nPlayer);
    void setType(bool nType);
    void Draw();
    void Move(double nX, double nY);
};

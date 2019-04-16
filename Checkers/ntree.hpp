#include <string>
#include <vector>

using namespace std;

void print_board(vector<string> board);

int board_diff(vector<string> board1, vector<string> board2);

class NNode{
public:
    vector<NNode*> children;
    NNode* parent;
    vector<string> board;
    int value;
    char player;
    char enemy;
    int player_count;
    int enemy_count;
    int direction;
    NNode(char p, char e, vector<string> b, int dir);
    int max_val_child();
    int min_val_child();
    vector<vector<string>> make_children();
};


class NTree{
public:
    int max_height;
    NNode* head;
    NTree();
    NTree(int m);
    void create_subtree(NNode* curr_node, char p, char e, int dir, int curr_h);
    void create_tree(char p, char e, vector<string> start_board);
    void print_left();
    void apply_minmax();
    void calc_value_node(NNode *curr, int h);
    NNode* search_board(vector<string> s_board);
    vector<vector<int>> MinMax();
};

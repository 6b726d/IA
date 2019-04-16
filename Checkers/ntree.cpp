#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glut.h>

#include "ntree.hpp"

using namespace std;

void print_board(vector<string> board){
    for(int i = 0; i < 8; i++){
        cout<<board[i]<<endl;
    }
    cout<<endl;
}

int board_diff(vector<string> board1, vector<string> board2){
    int c = 0;
    for(int i = 0; i < board1.size(); i++){
        for(int j = 0; j<board2[i].size();j++){
            if(board1[i][j] != board2[i][j])
                c+=1;
        }
    }
    return c;
}

//Begin NNode
NNode::NNode(char p, char e, vector<string> b, int dir) {
    parent = 0;
    direction = dir;
    board = b;
    player = p;
    enemy = e;
    int count0 = 0;
    int count1 = 0;
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j<board[i].size();j++){
            if(board[i][j] == '0'){
                count0 += 1;
            }
            if(board[i][j] == '1'){
                count1 += 1;
            }
        }
    }
    value = count1 - count0;
}

int NNode::max_val_child(){
    int max_val = -1000;
    for(int i = 0; i<children.size();i++){
        if(children[i]->value > max_val)
            max_val = children[i]->value;
    }
    return max_val;
}

int NNode::min_val_child(){
    int min_val = 1000;
    for(int i = 0; i<children.size();i++){
        if(children[i]->value < min_val)
            min_val = children[i]->value;
    }
    return min_val;
}

vector<vector<string>> NNode::make_children(){
    vector<vector<string>>new_boards;
    for(int i = 0; i < 8;i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j]==player){
                if (i+direction<8 and i+direction>0 and j+1<8 and j+1>0){
                    if(board[i+direction][j+1] == '+'){ //case empty space right
                        vector<string> board_copy(board);
                        board_copy[i+direction][j+1] = player;
                        board_copy[i][j] = '+';
                        new_boards.push_back(board_copy);
                    }
                }
                if (i+direction<8 and i+direction>0 and j-1<8 and j-1>0){
                    if(board[i+direction][j-1] == '+'){ //case empty space left
                        vector<string> board_copy(board);
                        board_copy[i+direction][j-1] = player;
                        board_copy[i][j] = '+';
                        new_boards.push_back(board_copy);
                    }
                }
                if (i+2*direction<8 and i+2*direction>0 and j+2<8 and j+2>0){
                    if(board[i+direction][j+1] == enemy and board[i+2*direction][j+2] == '+'){ //case eat right token
                        vector<string> board_copy(board);
                        board_copy[i+direction][j+1] = '+';
                        board_copy[i][j] = '+';
                        board_copy[i+2*direction][j+2] = player;
                        new_boards.push_back(board_copy);
                    }
                }
                if (i+2*direction<8 and i+2*direction>0 and j-2<8 and j-2>0){
                    if(board[i+direction][j-1] == enemy and board[i+2*direction][j-2] == '+'){ //case eat left token
                        vector<string> board_copy(board);
                        board_copy[i+direction][j-1] = '+';
                        board_copy[i][j] = '+';
                        board_copy[i+2*direction][j-2] = player;
                        new_boards.push_back(board_copy);
                    }
                }
            }
        }
    }
    return new_boards;
}
//End NNode

//Begin NTree
NTree::NTree(){
    max_height = 0;
}

NTree::NTree(int m){
    max_height = m;
}

void NTree::create_subtree(NNode* curr_node, char p, char e, int dir, int curr_h){
    if (curr_h < max_height){
        vector<vector<string>> children = curr_node->make_children();
        // make children nodes
        for(int i = 0; i<children.size();i++){
            NNode *c;
            c = new NNode(e, p, children[i], dir*(-1));
            curr_node->children.push_back(c);
            c->parent = curr_node;
            create_subtree(c, e, p, dir*(-1), curr_h+1);
        }
    }
}

void NTree::create_tree(char p, char e, vector<string> start_board){
    int dir = 1;
    if (p == '0'){
        dir == -1;
    }
    int h = 0;
    NNode* c;
    c = new NNode(p, e, start_board, dir);
    head = c;
    cout<<"Root Created"<<endl;
    create_subtree(head, p, e, dir, h);
    for(int i=0; i<head->children.size(); i++) {
        print_board(head->children[i]->board);
    }
}

void NTree::print_left(){
    NNode *current = head;
    int i = 0;
    while(current->children.size() > 0){
        print_board(current->board);
        int j = rand()%current->children.size();
        current = current->children[j];
        i+=1;
    }
    cout<<"max_left: "<<i<<endl;
    cout<<"head children size: "<<head->children.size()<<endl;
}

void NTree::apply_minmax(){
    calc_value_node(head, 0);
}

void NTree::calc_value_node(NNode *curr, int h){
    if(curr->children.size() > 0){
        for(int i = 0; i < curr->children.size();i++){
            calc_value_node(curr->children[i], h+1);
        }
        if (h%2==1){
            curr->value = curr->min_val_child();;
        }
        if (h%2==0){
            curr->value = curr->max_val_child();
        }
        int p;
    }
}

NNode* NTree::search_board(vector<string> s_board){
    // try to get closest node to given board
    NNode *curr = head;
    int diff = board_diff(curr->board, s_board);
    int max_it = 200;
    int it = 0;
    while(diff > 0 && curr->children.size()>0){
        int min_dif = 10000;
        int min_idx = 0;
        for(int i = 0; i<curr->children.size();i++){
            int d = board_diff(s_board, curr->children[i]->board);
            if (d < min_dif){
                min_dif = d;
                min_idx = i;
            }
        }
        curr = curr->children[min_idx];
        diff = min_dif;
    }
    if (diff > 0)
        return nullptr;

    return curr;
}

vector<vector<int>> NTree::MinMax() {
    calc_value_node(head, 0);//MinMax

    //Search NNode
    vector<string> pboard;
    vector<string> nboard;
    pboard = head->board;
    for(int i=0; i<head->children.size(); i++) {
        if (head->value == head->children[i]->value) {
            nboard = head->children[i]->board;
        }
    }
    //Positions
    vector<int> pos(2);
    vector<vector<int>> bpos(3);
    pos[0] = -1;
    pos[1] = -1;
    bpos[0] = pos; //Pos0 AI
    bpos[1] = pos; //Pos1 AI
    bpos[2] = pos; //Token Delete
    print_board(nboard);
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            if (pboard[i][j] != nboard[i][j]) {
                pos[0] = i;
                pos[1] = j;
                if (pboard[i][j] == '1') {
                    bpos[0] = pos;
                }
                else if (nboard[i][j] == '1') {
                    bpos[1] = pos;
                }
                else { //((pboard[i][j] == '0') && (nboard[i][j] == '+'))
                    bpos[2] = pos;
                }
            }
        }
    }
    return bpos;
}
//End NTree

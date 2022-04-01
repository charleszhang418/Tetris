#include <vector> 
#include <iostream>
#include "subject.h"
#include "observer.h"
#include "cell.h"
#include "block.h"
#include "board.h"
using namespace std;

Board::Board(int width, int height, int level) : 
width{width}, height{height}, level{level} {
    vector<vector<Cell*>> cells;
    for (int i = 0; i < height; i++) {
        vector<Cell*> line;
        for (int j = 0; j < width; j++) {
            Cell* cell = new Cell(j, i,' ');
            line.emplace_back(cell);
        }
        cells.emplace_back(line);
    }
    this->cells = cells;
}

char Board::getVal(int col, int row) {
    return cells[col][row]->getvalue();
}


void Board::spin(Block* change, bool clock) {
    
    bool suc = true;
    vector<vector<int>> placeold =  change->getalllocation();
    vector<vector<int>> placenew =  change->rotate(clock);
    
    for (int i = 0; i < 4; i++) {
        int new1 = placenew[i][0];
        int new2 = placenew[i][1];
        if (cells[new1][new2]->getiffilled()) {
            suc = false;
            
        }
    }
    
    if (suc = true) {
        char need = change->getChar();
        for (int i = 0; i < 4; i++) {
            int old1 = placeold[i][0];
            int old2 = placeold[i][1];
            cells[old1][old2]->eraseValue(' ');
            cells[old1][old2]->eraseIffilled(false);
            
            int new1 = placenew[i][0];
            int new2 = placenew[i][1];
            
            cells[new1][new2]->eraseValue(need);
            cells[new1][new2]->eraseIffilled(true);   
        }
        change->erasetype(clock);
        Cell* cell1 = cells[placenew[0][0]][placenew[0][1]];
        Cell* cell2 = cells[placenew[1][0]][placenew[1][1]];
        Cell* cell3 = cells[placenew[2][0]][placenew[2][1]];
        Cell* cell4 = cells[placenew[3][0]][placenew[3][1]];
        change->eraseallcell(cell1, cell2, cell3, cell4);
    }
}




Cell* Board::getCell(int col, int row) {
    return this->cells[row][col];
}

Block* Board::generateNewBlock(char c) {
    if (c == 'I') {
       return new IBlock(this->getCell(0, 3), this->getCell(1, 3), this->getCell(2, 3), this->getCell(3, 3));
    } else if (c == 'J') {
       return new JBlock(this->getCell(0, 2), this->getCell(0, 3), this->getCell(1, 3), this->getCell(2, 3));
    } else if (c == 'L') {
       return new LBlock(this->getCell(0, 2), this->getCell(0, 3), this->getCell(1, 3), this->getCell(2, 3));
    } else if (c == 'O') {
       return new OBlock(this->getCell(0, 2), this->getCell(1, 2), this->getCell(0, 3), this->getCell(1, 3));
    } else if (c == 'S') {
       return new SBlock(this->getCell(1, 2), this->getCell(2, 2), this->getCell(0, 3), this->getCell(1, 3));
    } else if (c == 'Z') {
       return new ZBlock(this->getCell(0, 2), this->getCell(1, 2), this->getCell(1, 3), this->getCell(1, 4));
    } else {
       return new TBlock(this->getCell(0, 2), this->getCell(1, 2), this->getCell(2, 2), this->getCell(1, 3));
    }
}

bool Board::move(int x, int y, int weight, Block* b) {
    int sum = y + weight;
    while ((x != 0) || (sum > 0)) {
        int col1 = b->getalllocation()[0][0];
        int row1 = b->getalllocation()[0][1];
        int col2 = b->getalllocation()[1][0];
        int row2 = b->getalllocation()[1][1];
        int col3 = b->getalllocation()[2][0];
        int row3 = b->getalllocation()[2][1];
        int col4 = b->getalllocation()[3][0];
        int row4 = b->getalllocation()[3][1];
        if (x != 0) {
            if (((col1 + x) >= 0) && ((col1 + x) < 11) && ((col2 + x) >= 0) && ((col2 + x) < 11) && ((col3 + x) >= 0) && ((col3 + x) < 11) && ((col4 + x) >= 0) && ((col4 + x) < 11)) {
                b->clearCellState();
                Cell* cell1 = this->getCell(col1 + x, row1);
                Cell* cell2 = this->getCell(col2 + x, row2);
                Cell* cell3 = this->getCell(col3 + x, row3);
                Cell* cell4 = this->getCell(col4 + x, row4);
                if (!(cell1->getiffilled()) && !(cell2->getiffilled()) && !(cell3->getiffilled()) && !(cell4->getiffilled())) {
                    b->eraseallcell(cell1, cell2, cell3, cell4);
                    b->updateCellState();
                } else {
                    b->updateCellState();
                }
            }
            x = 0;
        } else {
            b->clearCellState();
            Cell* cell1 = this->getCell(col1, row1 + 1);
            Cell* cell2 = this->getCell(col2, row2 + 1);
            Cell* cell3 = this->getCell(col3, row3 + 1);
            Cell* cell4 = this->getCell(col4, row4 + 1);
            if (!(cell1->getiffilled()) && !(cell2->getiffilled()) && !(cell3->getiffilled()) && !(cell4->getiffilled())) {
                b->eraseallcell(cell1, cell2, cell3, cell4);
                b->updateCellState();
            } else {
                b->updateCellState();
                return false;
            }
            sum -= 1;
        }
    }
    return true;  
}

Board::~Board() {}



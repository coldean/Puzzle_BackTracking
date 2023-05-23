//
//  main.cpp
//  Puzzle_BackTracking
//
//  Created by Jisung Yu on 2023/05/23.
//
// 아래일때는 index 0에 오게, 우측일때는 index 1에 오게 해야함
// 초기값 두번 들어가는거 방지 필요

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

void solvePuzzle(vector<vector<int>> &puzzle, int order, int chosenPiece);
bool isPromissing(int order, int chosenPiece, vector<int> piece);
bool checkNumbers(int order, int chosenPiece, vector<int> piece, int tempNum);
bool isPair(int n1, int n2);

vector<vector<int>> resultPuzzle;
vector<string> resultString;
bool remain_pieces[9] = {true};
int resultCount = 0;

int main(int argc, const char * argv[]) {
    
    vector<vector<int>> puzzle;
    vector<int> piece;
    string line;
    
    for(int i=0; i<9; i++){
        getline(cin, line);
        stringstream ss(line);
        
        for (int j=0; j<4; j++){
            int temp;
            ss >> temp;
            piece.push_back(temp);
        }
        puzzle.push_back(piece);
        piece.clear();
        
        remain_pieces[i] = true;
    }
    for (int i=0; i<9; i++){
        remain_pieces[i] = false;
        vector<int> newPiece;
        
        for ( int j=0; j<4; j++){
            for (int k=0; k<4; k++){
                newPiece.push_back(puzzle[i][(j + k + 4) % 4]);
            }
            resultPuzzle.push_back(newPiece);
            string r = to_string(i) + "(" + to_string(j) + ")";
            resultString.push_back(r);
            
            solvePuzzle(puzzle, 2, i);
            resultPuzzle.pop_back();
            resultString.pop_back();
            
            newPiece.clear();
        }
                
        remain_pieces[i] = true;
    }
    cout << resultCount;
    return 0;
}

void solvePuzzle(vector<vector<int>> &puzzle, int order, int chosenPiece){
    vector<int> piece = puzzle[chosenPiece];
    
    if(order > 10 || !isPromissing(order, chosenPiece, piece)){
        return;
    }
    
    if (resultPuzzle.size() == 9){  //다 찾음
        for (int i=0; i<9; i++){
            cout << resultString[i] << " ";
        }
        cout << endl;
        resultCount += 1;
    }
    
    for(int i=0; i<9; i++){
        if(remain_pieces[i]){
            solvePuzzle(puzzle, order + 1, i);
        }
    }
    remain_pieces[chosenPiece] = true;
    resultPuzzle.pop_back();
    resultString.pop_back();
}

// 각 사면이 맞닿아있는지 확인
// 여기서 퍼즐을 4방향으로 돌려서 확인
bool isPromissing(int order, int chosenPiece, vector<int> piece){
    bool check = false;
    
    if (order % 3 == 1){    // 좌측 라인일 때, order -3, (+1, +3) 체크  // +는 아직 안나왔으므로 체크 안해도 됨
        int tempNum = order - 3;
        check = checkNumbers(order, chosenPiece, piece, tempNum);
    }
    else if (order % 3 == 2){   // 중간 라인일 때, order -3, -1, (+1, +3) 체크
        int tempNum = order - 3;
        check = checkNumbers(order, chosenPiece, piece, tempNum);
        tempNum = order - 1;
        check = checkNumbers(order, chosenPiece, piece, tempNum);
    }
    else{   // 우측 라인일 때, order -3, -1, (+3) 체크
        int tempNum = order - 3;
        check = checkNumbers(order, chosenPiece, piece, tempNum);
        tempNum = order - 1;
        check = checkNumbers(order, chosenPiece, piece, tempNum);
    }
    
    return check;
}

bool checkNumbers(int order, int chosenPiece, vector<int> piece, int tempNum){
    bool check = false;             // 아래일때는 index 0에 오게, 우측일때는 index 1에 오게 해야함
    int indexNum = tempNum - 1;
    if (indexNum >= 0){
        int numberBefore;  // 이전 퍼즐의 맞물리는 수
        if (tempNum + 3 == order)   // 밑이랑 맞을때
            numberBefore = resultPuzzle[indexNum][2];
        else                        // 우측이랑 맞을 때
            numberBefore = resultPuzzle[indexNum][1];
        vector<int> newPiece;
        
        for (int i = 0; i<4; i++){
            if(isPair(numberBefore, piece[i])){
                for (int j = 0; j<4; j++){
                    newPiece.push_back(piece[(j + 4 - i) % 4]); // 순서 바꾸기
                }
                resultPuzzle.push_back(newPiece);               // 순서 바꾼 퍼즐 저장
                string r = to_string(chosenPiece) + "(" + to_string(i) + ")";
                resultString.push_back(r);
                remain_pieces[chosenPiece] = false; // 퍼즐 사용했다고 표시
                check = true;
            }
        }
    }
    return check;
}

bool isPair(int n1, int n2){
    bool check = false;
    
    if ((n1 == 1 && n2 == 2) || (n1 == 2 && n2 == 1))
        check = true;
    else if ((n1 == 3 && n2 == 4) || (n1 == 4 && n2 == 3))
        check = true;
    else if ((n1 == 5 && n2 == 6) || (n1 == 6 && n2 == 5))
        check = true;
    else if ((n1 == 7 && n2 == 8) || (n1 == 8 && n2 == 7))
        check = true;
    
    return check;
}

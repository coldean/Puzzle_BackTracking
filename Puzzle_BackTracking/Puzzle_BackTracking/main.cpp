//
//  main.cpp
//  Puzzle_BackTracking
//
//  Created by Jisung Yu on 2023/05/23.
//
// 아래일때는 index 0에 오게, 우측일때는 index 1에 오게 해야함
// 초기값 두번 들어가는거 방지 필요
// isPromissing에서 promissing 확인만(이전 퍼즐과 맞닿은 면이 유효한지)
// solvePuzzle에서 rotate도 해서 확인
//
// 얘가 중복되는걸 못걸러냄. 그것만 해결하면 될듯?

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

void solvePuzzle(vector<vector<int>> &puzzle, int order, int chosenPiece);
bool isPromissing(int order, int chosenPiece, vector<int> piece);
bool isPair(int n1, int n2);
void saveResult(vector<int> newPiece, int chosenPiece, int rotate);

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
    for (int i=0; i<9; i++){            // 첫번째 칸 9조각 다 확인
        remain_pieces[i] = false;       // remain_pieces 초기화
        vector<int> newPiece;
        
        for ( int j=0; j<4; j++){       // piece 방향 돌려가면서 다 확인해봄
            for (int k=0; k<4; k++){
                newPiece.push_back(puzzle[i][(j + k) % 4]);
            }
            saveResult(newPiece, i + 1, j);
            
            for (int l=0; l<9; l++){
                if (remain_pieces[l] == true)
                    solvePuzzle(puzzle, 2, l + 1);
            }
            resultPuzzle.pop_back();
            resultString.pop_back();
            
            newPiece.clear();
        }
                
        remain_pieces[i] = true;
    }
    cout << resultCount << endl;
    return 0;
}

void solvePuzzle(vector<vector<int>> &puzzle, int order, int chosenPiece){
    vector<int> piece = puzzle[chosenPiece - 1];
    
    if(!isPromissing(order, chosenPiece, piece)){
        return;
    }
    
    // 마지막 퍼즐까지 확인해서 다 채웠을 때
    if (order == 9){
        int count = 0;
        for (int i=0; i<9; i++){
            if (!remain_pieces[i])
                count++;
        }
        
        if (count == 9){
            for (int i=0; i<9; i++){
                cout << resultString[i] << " ";
            }
            cout << endl;
            resultCount ++;                     // 총 갯수 추가
            
            resultPuzzle.pop_back();            // 다시 위로 돌아가기 위해 뺴줌
            resultString.pop_back();
            remain_pieces[chosenPiece - 1] = true;
            
            return;
        }
    }
    
    for(int i=0; i<9; i++){
        if(remain_pieces[i]){
            solvePuzzle(puzzle, order + 1, i + 1);  // 남아있는 퍼즐 하나씩 확인
        }
    }
    resultPuzzle.pop_back();                    // 다시 위로 돌아가기 위해 빼줌
    resultString.pop_back();
    remain_pieces[chosenPiece - 1] = true;
}

// 각 사면이 맞닿아있는지 확인
// 여기서 퍼즐을 4방향으로 돌려서 확인
bool isPromissing(int order, int chosenPiece, vector<int> piece){
    bool check = false;
    
    vector<int> prePiece_up, prePiece_left;
    int prePiece_up_number = -1;
    int prePiece_left_number = -1;
    
    if (order % 3 == 1){    // 좌측 라인일 때, 인덱스[1](우측) 점검
        prePiece_up_number = order - 3;
        if (prePiece_up_number < 0)
            return true;
        prePiece_up = resultPuzzle[prePiece_up_number - 1];
    }
    else{   // 중앙, 우측 라인일 때, 인덱스[1](우측), 인덱스[2](하단) 점검
        prePiece_up_number = order - 3;
        prePiece_left_number = order - 1;
        
        if (prePiece_up_number > 0)
            prePiece_up = resultPuzzle[prePiece_up_number - 1];
        prePiece_left = resultPuzzle[prePiece_left_number - 1];
    }
    
    if (prePiece_up_number > 0 && (order % 3 == 1)){    // 좌측 라인일 때 (order 1번 제외)
        bool pair = false;
        int rotate = -1;
        
        for (int i=0; i<4; i++){                        // 위쪽 확인
            pair = isPair(prePiece_up[2], piece[i]);
            if (pair){
                rotate = i;
                break;
            }
        }
        if (pair){
            vector<int> newPiece;
            for (int i=0; i<4; i++){
                newPiece.push_back(piece[(i + rotate) % 4]);       // rotate 만큼 조각 돌려줌
            }
            saveResult(newPiece, chosenPiece, rotate % 4);
            remain_pieces[chosenPiece - 1] = false;
            return true;
        }
        else
            return false;
    }
    ///
    /// 위, 좌 확인
    ///
    if ((order % 3 == 2) || (order % 3 == 0)){      // 중앙과 우측 라인일 때
        bool pair = false;
        int rotate = -1;
        
        // 좌측 퍼즐
        for (int i=0; i<4; i++){                    // 좌측 확인
            pair = isPair(prePiece_left[1], piece[i]);
            if (pair){
                rotate = i + 1;
                break;
            }
        }
        if (pair){
            vector<int> newPiece;
            for (int i=0; i<4; i++){
                newPiece.push_back(piece[(i + rotate) % 4]);    // rotate 만큼 조각 돌려줌
            }
            if (prePiece_up_number > 0){    // 위에가 존재할 때, 위쪽도 확인
                pair = isPair(prePiece_up[2], newPiece[0]);
                if (pair){
                    saveResult(newPiece, chosenPiece, rotate % 4);
                    remain_pieces[chosenPiece - 1] = false;
                    return true;
                }
                else
                    return false;
            }
            else{                           // 위에가 존재 안하면, 좌측만 확인 후 추가
                saveResult(newPiece, chosenPiece, rotate % 4);
                remain_pieces[chosenPiece - 1] = false;
                return true;
            }
        }
        else
            return false;
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

void saveResult(vector<int> newPiece, int chosenPiece, int rotate){
    resultPuzzle.push_back(newPiece);
    string r = to_string(chosenPiece) + "(" + to_string(rotate) + ")";
    resultString.push_back(r);
}

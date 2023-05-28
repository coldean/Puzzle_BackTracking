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
                newPiece.push_back(puzzle[i][(j + k) % 4]);
            }
            resultPuzzle.push_back(newPiece);
            string r = to_string(i + 1) + "(" + to_string(j) + ")";
            resultString.push_back(r);
            
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
   
//    if (order == 10){
//        for (int i=0; i<9; i++){
//            cout << resultString[i] << " ";
//        }
//        cout << endl;
//        resultCount ++;
////        resultPuzzle.clear();
////        resultString.clear();
//        return;
//    }
//    if (order == 9){
//        //
//    }
    vector<int> piece = puzzle[chosenPiece - 1];
    
//    if(order > 10 || !isPromissing(order, chosenPiece, piece)){
//        return;
//    }
//
//    if (resultPuzzle.size() == 9){  //다 찾음
//        for (int i=0; i<9; i++){
//            cout << resultString[i] << " ";
//        }
//        cout << endl;
//        resultCount += 1;
//    }
//
//    for(int i=0; i<9; i++){
//        if(remain_pieces[i]){
//            solvePuzzle(puzzle, order + 1, i);
//        }
//    }
//    remain_pieces[chosenPiece] = true;
//    resultPuzzle.pop_back();
//    resultString.pop_back();
    
    if(!isPromissing(order, chosenPiece, piece)){
        return;
    }
    
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
            resultCount ++;
            
            resultPuzzle.pop_back();
            resultString.pop_back();
            remain_pieces[chosenPiece - 1] = true;
            
            return;
        }
    }
    
    for(int i=0; i<9; i++){
        if(remain_pieces[i]){
            solvePuzzle(puzzle, order + 1, i + 1);
        }
    }
    resultPuzzle.pop_back();
    resultString.pop_back();
    remain_pieces[chosenPiece - 1] = true;
}

// 각 사면이 맞닿아있는지 확인
// 여기서 퍼즐을 4방향으로 돌려서 확인
bool isPromissing(int order, int chosenPiece, vector<int> piece){
    bool check = false;
    
//    if (order % 3 == 1){    // 좌측 라인일 때, order -3, (+1, +3) 체크  // +는 아직 안나왔으므로 체크 안해도 됨
//        int tempNum = order - 3;
//        check = checkNumbers(order, chosenPiece, piece, tempNum);
//    }
//    else if (order % 3 == 2){   // 중간 라인일 때, order -3, -1, (+1, +3) 체크
//        int tempNum = order - 3;
//        check = checkNumbers(order, chosenPiece, piece, tempNum);
//        tempNum = order - 1;
//        check = checkNumbers(order, chosenPiece, piece, tempNum);
//    }
//    else{   // 우측 라인일 때, order -3, -1, (+3) 체크
//        int tempNum = order - 3;
//        check = checkNumbers(order, chosenPiece, piece, tempNum);
//        tempNum = order - 1;
//        check = checkNumbers(order, chosenPiece, piece, tempNum);
//    }
    
    vector<int> prePiece_up, prePiece_left;
    int prePiece_up_number = -1;
    int prePiece_left_number = -1;
    
    if (order % 3 == 1){    // 좌측 라인일 때, 1번 인덱스(우측) 점검
        prePiece_up_number = order - 3;
        if (prePiece_up_number < 0)
            return true;
        prePiece_up = resultPuzzle[prePiece_up_number - 1];
    }
    else{   // 우측 라인일 때, 1번 인덱스(우측), 2번 인덱스(하단) 점검
        prePiece_up_number = order - 3;
        prePiece_left_number = order - 1;
        
        if (prePiece_up_number > 0)
            prePiece_up = resultPuzzle[prePiece_up_number - 1];
        prePiece_left = resultPuzzle[prePiece_left_number - 1];
    }
    
    if (prePiece_up_number > 0 && (order % 3 == 1)){
        bool pair = false;
        int rotate = -1;
        
        for (int i=0; i<4; i++){
            pair = isPair(prePiece_up[2], piece[i]);
            if (pair){
                rotate = i;
                break;
            }
        }
        if (pair){
            vector<int> newPiece;
            for (int i=0; i<4; i++){
                newPiece.push_back(piece[(i + rotate) % 4]);
            }
            resultPuzzle.push_back(newPiece);
            string r = to_string(chosenPiece) + "(" + to_string(rotate % 4) + ")";
            resultString.push_back(r);
            remain_pieces[chosenPiece - 1] = false;
            return true;
        }
        else
            return false;
    }
    ///
    /// 위, 좌 확인
    ///
    if ((order % 3 == 2) || (order % 3 == 0)){
        bool pair = false;
        int rotate = -1;
        
        // 좌측 퍼즐
        for (int i=0; i<4; i++){
            pair = isPair(prePiece_left[1], piece[i]);
            if (pair){
                rotate = i + 1;
                break;
            }
        }
        if (pair){
            vector<int> newPiece;
            for (int i=0; i<4; i++){
                newPiece.push_back(piece[(i + rotate) % 4]);
            }
            if (prePiece_up_number > 0){    // 위에가 존재할 때
                pair = isPair(prePiece_up[2], newPiece[0]);
                if (pair){
                    resultPuzzle.push_back(newPiece);
                    string r = to_string(chosenPiece) + "(" + to_string(rotate % 4) + ")";
                    resultString.push_back(r);
                    remain_pieces[chosenPiece - 1] = false;
                    return true;
                }
                else
                    return false;
            }
            else{
                resultPuzzle.push_back(newPiece);
                string r = to_string(chosenPiece) + "(" + to_string(rotate % 4) + ")";
                resultString.push_back(r);
                remain_pieces[chosenPiece - 1] = false;
                return true;
            }
        }
        else
            return false;
    }
    
      
    
    return check;
}

//bool checkNumbers(int order, int chosenPiece, vector<int> piece, int tempNum){
//    bool check = false;             // 아래일때는 index 0에 오게, 우측일때는 index 1에 오게 해야함
//    int indexNum = tempNum - 1;
//    if (indexNum >= 0){
//        int numberBefore;  // 이전 퍼즐의 맞물리는 수
//        if (tempNum + 3 == order)   // 밑이랑 맞을때
//            numberBefore = resultPuzzle[indexNum][2];
//        else                        // 우측이랑 맞을 때
//            numberBefore = resultPuzzle[indexNum][1];
//        vector<int> newPiece;
//
//        for (int i = 0; i<4; i++){
//            if(isPair(numberBefore, piece[i])){
//                for (int j = 0; j<4; j++){
//                    newPiece.push_back(piece[(j + 4 - i) % 4]); // 순서 바꾸기
//                }
//                resultPuzzle.push_back(newPiece);               // 순서 바꾼 퍼즐 저장
//                string r = to_string(chosenPiece) + "(" + to_string(i) + ")";
//                resultString.push_back(r);
//                remain_pieces[chosenPiece] = false; // 퍼즐 사용했다고 표시
//                check = true;
//            }
//        }
//    }
//    return check;
//}

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

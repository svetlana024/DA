#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <ctime>
#include <fstream>

#define PASS 1
#define UNPASS 0

int height, width;

typedef struct Cell{
    int id;
    int h;
    int g;
    int f;
    int x, y;
    bool open;
    bool close;
    struct Cell * parent_cell;
} Cell;

struct GreaterThanByF{
  bool operator()(const Cell* left, const Cell* right) const{
    if(left-> f == right-> f)
        return (left-> g > right-> g);
    return (left-> f > right-> f);
  }
};

void A_Help_Search(std::vector<std::vector<Cell>>& Grill, std::vector<Cell*>& Changed, std::priority_queue<Cell*, std::vector<Cell*>, GreaterThanByF>& OpenQueue, Cell * current, int x1, int y1, int x2, int y2){
    if(Grill[x1][y1].id == PASS && Grill[x1][y1].close == false){
        if(Grill[x1][y1].open == false){
            Grill[x1][y1].open = true;
            Grill[x1][y1].parent_cell = current;
            Grill[x1][y1].g = current-> g + 1;
            Grill[x1][y1].h = abs(x2 - x1) + abs(y2 - y1);
            Grill[x1][y1].f = Grill[x1][y1].g + Grill[x1][y1].h;
            OpenQueue.push(&Grill[x1][y1]);
            Changed.push_back(&Grill[x1][y1]);
        }
        else if(Grill[x1][y1].g > current-> g + 1){
                Grill[x1][y1].g = current-> g + 1;
                Grill[x1][y1].f = Grill[x1][y1].g + Grill[x1][y1].h;
                Grill[x1][y1].parent_cell = current;
                Cell * check = OpenQueue.top();
                OpenQueue.push(check);
        }
    }
}

int A_Search(std::vector<std::vector<Cell>>& Grill, std::vector<int>& Num, std::vector<Cell*>& Changed){
    int x1 = Num[0], y1 = Num[1], x2 = Num[2], y2 = Num[3];
    int path = 0;
    int id = 1;
    int Ids[height][width] = {};
    std::ofstream file("map.txt", std::ios::out);
    
    if( Grill[x1][y1].id == UNPASS || Grill[x2][y2].id == UNPASS || x1 > (int)Grill.size() || x2 > (int)Grill.size() || y1 > (int)Grill[0].size() ||  y2 > (int)Grill[0].size() || x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
        return -1;
        
    std::priority_queue<Cell*, std::vector<Cell*>, GreaterThanByF> OpenQueue;
    std::queue<Cell*> CloseQueue;
    Cell * current = NULL;

    current = &Grill[x1][y1];
    OpenQueue.push(current);
    current-> open = true;
    Changed.push_back(&Grill[x1][y1]);

    while(Grill[x2][y2].open == false){
        if(OpenQueue.empty())
            return -1;
        CloseQueue.push(current);
        Ids[current-> x][current-> y] = id;
        id++;

        current-> close = true;
        OpenQueue.pop();

        if(current-> x - 1 >= 0)
            A_Help_Search(Grill, Changed, OpenQueue, current, current-> x - 1, current-> y, x2, y2);
        if(current-> x + 1 < Grill.size())
            A_Help_Search(Grill, Changed, OpenQueue, current, current-> x + 1, current-> y, x2, y2);
        if(current-> y - 1 >= 0)
            A_Help_Search(Grill, Changed, OpenQueue, current, current-> x, current-> y - 1, x2, y2);
        if(current-> y + 1 < Grill[0].size())
            A_Help_Search(Grill, Changed, OpenQueue, current, current-> x, current-> y + 1, x2, y2);

        current = OpenQueue.top();
    }
    current = &Grill[x2][y2];
    while(current-> parent_cell!= NULL){
        path++;
        current = current-> parent_cell;
    }
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            file << Ids[i][j];
            if(j != width - 1)
                file << " ";
        }
        file << std::endl;
    }

    return path;
}

void InitGrill(Cell * element){
    element-> f = 0;
    element-> h = 0;
    element-> g = 0;
    element-> open = false;
    element-> close = false;
    element-> parent_cell = NULL;
}

int main(){
	time_t start = clock();
    std::string str;
    std::vector<int> Num(4);
    std::vector<Cell*> Changed;
    Cell element;
    int q;

    std::getline(std::cin, str);
    std::getline(std::cin, str);

    for( int i{1}; i < str.size(); i++ ){
        try{
           height = std::stoi( str.substr(i));
            break;
        }
        catch(...)
        {}
    }
    std::vector< std::vector<Cell> > Grill(height);
    std::getline(std::cin, str);

    for(int i{1}; i < str.size(); i++ ){
        try{
           width = std::stoi( str.substr(i) );
            break;
        }
        catch(...)
        {}
    }
    std::getline(std::cin, str);
    for( int i = 0; i < height; i++){
        std::getline(std::cin, str);
        for( int j = 0; j < width; j++){
                if( str[j] == '.' || str[j] == 'G' )
                    element.id = PASS;
                else element.id = UNPASS;
                    InitGrill(&element);
                    element.x = i;
                    element.y = j;
                    Grill[i].push_back(element);
        }
    }
    std::cin >> q;
    while(q > 0){
        for(int i = 0; i < 4; i++)
            std::cin>> Num[i];
        std::cout << A_Search(Grill, Num, Changed) << std::endl;
        while(!Changed.empty()){
                InitGrill(Changed.back());
                Changed.pop_back();
        }
        q--;
    }
    time_t end = clock();
    std::cout << "TIME: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    return 0;
}

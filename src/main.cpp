#include "main.h"
#include <iostream>
#include <ctime>
#include <string>
#include <stdlib.h>
#include <cstdlib> 
#include <fstream>

void getMatrix(int row, int col,Matrix* mat){
    mat->col_length = col;
    mat->row_length = row;
    std::string word;
    mat->mat = (Token**)malloc(row*sizeof(Token*));
    for (int i=0;i<row;i++){
        mat->mat[i] = (Token*)malloc(col*sizeof(Token));
        for (int j=0;j<col;j++){
            std::cin>>word;
            mat->mat[i][j].a = word[0];
            mat->mat[i][j].b = word[1];
        }
    }
}

void getSeq(ListSeq* lis, int n, int buff){
    lis->Neff = n;
    lis->arr = (Sequence*)malloc(n*sizeof(Sequence));
    int j,count;
    for (int i =0;i<n;i++){
        j = 0;
        count = 0;
        lis->arr[i].taken = false;
        lis->arr[i].seq = (Token*)malloc(buff*sizeof(Token));
        std::string word;
        std::getline(std::cin>> std::ws, word);
        
        while (j < word.length()){
            if (word[j]==' '){
                j += 1;
            }
            lis->arr[i].seq[count].a = word[j];
            lis->arr[i].seq[count].b = word[j+1];
            j += 2;
            count += 1;
        }
        lis->arr[i].Neff = count;
        std::cin>>lis->arr[i].reward;
        
    }
}

void printMatrix(Matrix m){
    std::cout<<"Matrix: \n";
    for (int i=0;i< m.row_length;i++){
        for (int j=0;j<m.col_length;j++){
            std::cout<<m.mat[i][j].a<<m.mat[i][j].b<<" ";
        }
        std::cout<<"\n\n";
    }
    std::cout<<"\n\n";
}

void printSeq(ListSeq ls){
    std::cout<<"Sequences: \n";
    for (int i=0;i<ls.Neff;i++){
        std::cout<<i+1<<". ";
        for (int j=0;j<ls.arr[i].Neff;j++){
            std::cout<<ls.arr[i].seq[j].a<<ls.arr[i].seq[j].b<<" ";
        }
        std::cout<<"\nLength: "<<ls.arr[i].Neff;
        std::cout<<"\nReward: "<<ls.arr[i].reward<<"\n";
    }
    std::cout<<"\n";
}
void declareBuff(Buffer* buff, int n){
    buff->Neff = 0;
    buff->reward = 0;
    buff->point = (Point*)malloc(n*sizeof(Point));
}
void resetSeq(ListSeq L){
    for (int i=0;i<L.Neff;i++){
        L.arr[i].taken = false;
    }
}
void copyBuffer(Buffer *target,Buffer curr){
    target->Neff = curr.Neff;
    for (int i=0;i<target->Neff;i++){
        target->point[i].x = curr.point[i].x;
        target->point[i].y = curr.point[i].y;
    }
    target->reward  = curr.reward;
}

void addBuff(Buffer* curr, int x, int y){
    curr->point[curr->Neff].x = x;
    curr->point[curr->Neff].y = y;
    curr->Neff += 1;
}
void delBuff(Buffer* curr){
    curr->Neff -= 1;
}
void printBuffer(Buffer curr,Matrix mat){
    std::cout<<curr.reward<<"\n";
    int i;
    for (i=0;i<curr.Neff-1;i++){
        std::cout<<mat.mat[curr.point[i].y][curr.point[i].x].a<<mat.mat[curr.point[i].y][curr.point[i].x].b<<" ";
    }
    std::cout<<mat.mat[curr.point[i].y][curr.point[i].x].a<<mat.mat[curr.point[i].y][curr.point[i].x].b<<"\n";
    for (i=0;i<curr.Neff;i++){
        std::cout<<curr.point[i].x+1<<", "<<curr.point[i].y+1<<"\n";
    }

}
int calculateReward(Matrix mat,Buffer curr, ListSeq lseq){
    boolean isRight;
    int reward = 0;
    for (int i=0;i<lseq.Neff;i++){
        if (curr.Neff >= lseq.arr[i].Neff){
            for (int j=0;j<=curr.Neff - lseq.arr[i].Neff && !lseq.arr[i].taken;j++){
                isRight = true;
                for (int k=0;k<lseq.arr[i].Neff && isRight;k++){
                    if (mat.mat[curr.point[j+k].y][curr.point[j+k].x].a != lseq.arr[i].seq[k].a
                    || mat.mat[curr.point[j+k].y][curr.point[j+k].x].b != lseq.arr[i].seq[k].b){
                        isRight = false;
                    }
                }
                if (isRight){
                    lseq.arr[i].taken = true;
                    reward += lseq.arr[i].reward;
                }
            }
        }
    }
    resetSeq(lseq);
    return reward;
}
boolean isVThere(Buffer curr, int y){
    boolean result = false;
    for (int i=0;i<curr.Neff && !result;i++){
        if (curr.point[i].x == curr.point[curr.Neff-1].x && curr.point[i].y == y){
            result = true;
        }
    }
    return result;
}
boolean isHThere(Buffer curr, int x){
    boolean result = false;
    for (int i=0;i<curr.Neff && !result;i++){
        if (curr.point[i].y == curr.point[curr.Neff-1].y && curr.point[i].x == x){
            result = true;
        }
    }
    return result;
}
void exSearch(int buf,Matrix mat, boolean isVertical,Buffer* target, Buffer curr,ListSeq Lseq){
    if (curr.Neff == 0){
        curr.Neff = 1;
        curr.point[0].y = 0;
        for (int i=0;i<mat.col_length;i++){
            curr.point[0].x = i;
            curr.reward = calculateReward(mat,curr,Lseq);
            if (curr.reward > target->reward){
                        copyBuffer(target,curr);
            }else if(curr.reward == target->reward && curr.Neff < target->Neff){
                        copyBuffer(target,curr);
            }
            exSearch(buf,mat,true,target,curr,Lseq);    
        }

    }else if (curr.Neff==buf-1){
            if (isVertical){
            for (int i=0;i<mat.row_length;i++){
                if (!isVThere(curr,i)){
                    addBuff(&curr,curr.point[curr.Neff-1].x,i);
                    int prevReward = curr.reward;
                    curr.reward = calculateReward(mat,curr,Lseq);
                    if (curr.reward > target->reward){
                        copyBuffer(target,curr);
                    }else if(curr.reward == target->reward && curr.Neff < target->Neff){
                        copyBuffer(target,curr);
                    }
                    curr.reward = prevReward;
                    curr.Neff -= 1;
                }
            }
        }else{
            for (int i=0;i<mat.col_length;i++){
                 if (!isHThere(curr,i)){
                    int prevReward = curr.reward;
                    addBuff(&curr,i,curr.point[curr.Neff-1].y);
                    curr.reward = calculateReward(mat,curr,Lseq);
                    if (curr.reward > target->reward){
                        copyBuffer(target,curr);
                    }else if (curr.reward == target->reward && curr.Neff < target->Neff){
                        copyBuffer(target,curr);
                    }
                    curr.reward = prevReward;
                    curr.Neff -= 1;
                }
            }
        }
    }else{
        if (isVertical){
            for (int i=0;i<mat.row_length;i++){
                if (!isVThere(curr,i)){
                    int prevReward = curr.reward;
                    addBuff(&curr,curr.point[curr.Neff-1].x,i);
                    curr.reward = calculateReward(mat,curr,Lseq);
                    if (curr.reward > target->reward){
                        copyBuffer(target,curr);
                    }else if (curr.reward == target->reward && curr.Neff < target->Neff){
                        copyBuffer(target,curr);
                    }
                    exSearch(buf,mat,false,target,curr,Lseq);
                    curr.reward = prevReward;
                    curr.Neff -= 1;
                }
            }
        }else{
            for (int i=0;i<mat.col_length;i++){
                 if (!isHThere(curr,i)){
                    int prevReward = curr.reward;
                    addBuff(&curr,i,curr.point[curr.Neff-1].y);
                    curr.reward = calculateReward(mat,curr,Lseq);
                    if (curr.reward > target->reward){
                        copyBuffer(target,curr);
                    }else if(curr.reward == target->reward && curr.Neff < target->Neff){
                        copyBuffer(target,curr);
                    }
                    exSearch(buf,mat,true,target,curr,Lseq);
                    curr.reward = prevReward;
                    curr.Neff -= 1;
                }
            }
        }
    }

}

void getToken(ListToken * Lt){
    std::cin>>Lt->Neff;
    std::string word;
    std::getline(std::cin>> std::ws, word);
    Lt->token = (Token*)malloc(Lt->Neff*sizeof(Token));
    for (int i=0;i<Lt->Neff;i++){
        Lt->token[i].a = word[3*(i)];
        Lt->token[i].b = word[1+3*(i)];
    }
}

void generateMatrix(Matrix*  mat, int col, int row, ListToken Lt){
    mat->col_length = col;
    mat->row_length = row;
    int randomNum;
    mat->mat = (Token**)malloc(row*sizeof(Token*));
    for (int i=0;i<row;i++){
        mat->mat[i] = (Token*)malloc(col*sizeof(Token));
        for (int j=0;j<col;j++){
            randomNum = rand() % Lt.Neff;
            mat->mat[i][j].a = Lt.token[randomNum].a;
            mat->mat[i][j].b = Lt.token[randomNum].b;
        }
    }   
}
void generateSeq(ListSeq * Lseq,int buff,int nseq, int maxSeq, ListToken Lt){
    int ranNum;
    int ranToken;
    Lseq->Neff = nseq;
    Lseq->arr = (Sequence*)malloc(Lseq->Neff*(sizeof(Sequence)));
    for (int i=0;i<nseq;i++){
        Lseq->arr[i].taken = false;
        Lseq->arr[i].seq = (Token*)malloc(buff*sizeof(Token));
        ranNum = (rand() % maxSeq )+ 1;
        Lseq->arr[i].Neff = ranNum;
        for (int j=0;j<ranNum;j++){
            ranToken = rand() % Lt.Neff;
            Lseq->arr[i].seq[j].a = Lt.token[ranToken].a;
            Lseq->arr[i].seq[j].b = Lt.token[ranToken].b;
        }
        Lseq->arr[i].reward = (rand() % 20)+1;
    }
}
int getInput(Buffer* target, Matrix* mat){
    int buff, sizex,sizey, nseq;
    std::cin>>buff>>sizex>>sizey;
    getMatrix(sizey,sizex,mat);
    std::cin>>nseq;
    ListSeq Lseq;
    getSeq(&Lseq,nseq,buff);
    Buffer curr;
    declareBuff(target, buff);
    declareBuff(&curr, buff);
    const clock_t start = clock();
    exSearch(buff,*mat,false,target,curr,Lseq);
    const clock_t ends = clock();
    int time = ends - start;
    return time;
}
int getGenerator(Buffer* target, Matrix* mat){
    ListToken Lt;
    getToken(&Lt);
    int buff;
    int sizex,sizey;
    std::cin>>buff>>sizex>>sizey;
    int nseq, maxSeq;
    std::cin>>nseq>>maxSeq;
    generateMatrix(mat,sizex,sizey,Lt);
    printMatrix(*mat);
    ListSeq Lseq;
    generateSeq(&Lseq,buff,nseq,maxSeq,Lt);
    printSeq(Lseq);
    Buffer curr;
    declareBuff(&curr,buff);
    declareBuff(target,buff);
    const clock_t start = clock();
    exSearch(buff,*mat,false,target,curr,Lseq);
    const clock_t ends = clock();
    int time = ends-start;
    return time;
}
int FileInput(Buffer *target, Matrix* mat){
    std::string fileName;
    std::cout<<"(Nama file tidak perlu ditulis dengan ekstensi .txt)\nMasukkan Nama file : ";
    std::cin>>fileName;
    std::string folderName = "../test/";
    std::string extension = ".txt";
    std::string fullPath = folderName + fileName + extension;
    std::ifstream inFile(fullPath);
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file " << fullPath << std::endl;
    }
    std::string line;
    std::getline(inFile,line);
    int buff = line[0]-'0';
    int sizex,sizey;
    std::getline(inFile,line);
    sizex = line[0]-'0';
    sizey = line[2]-'0';
    
    mat->col_length = sizex;
    mat->row_length = sizey;
    std::string word;
    mat->mat = (Token**)malloc(sizey*sizeof(Token*));
    for (int i=0;i<sizey;i++){
        mat->mat[i] = (Token*)malloc(sizex*sizeof(Token));
        std::getline(inFile,word);
        for (int j=0;j<sizex;j++){
            mat->mat[i][j].a = word[0+(j)*3];
            mat->mat[i][j].b = word[1+(j)*3];
        }
    }
    std::getline(inFile,line);
    
    int n = line[0]-'0';

    ListSeq lis;
    lis.Neff = n;
    lis.arr = (Sequence*)malloc(n*sizeof(Sequence));
    int j,count;
    std::string wordSeq;

    for (int i =0;i<n;i++){
        j = 0;
        count = 0;
        lis.arr[i].taken = false;
        lis.arr[i].seq = (Token*)malloc(buff*sizeof(Token));
        std::getline(inFile,wordSeq);
        while (j < wordSeq.length()){
            if (wordSeq[j]==' '){
                j += 1;
            }
            lis.arr[i].seq[count].a = wordSeq[j];
            lis.arr[i].seq[count].b = wordSeq[j+1];
            j += 2;
            count += 1;
        }
        lis.arr[i].Neff = count;
        std::getline(inFile,wordSeq);
        lis.arr[i].reward = std::stoi(wordSeq);
    }
    Buffer curr;
    declareBuff(&curr,buff);
    declareBuff(target,buff);
    const clock_t start = clock();
    exSearch(buff,*mat,false,target,curr,lis);
    const clock_t ends = clock();
    int time = ends-start;
    inFile.close();
    return time;
    
}
int fileGenerate(Buffer* target, Matrix*mat){
   std::string fileName;
    std::cout<<"(Nama file tidak perlu ditulis dengan ekstensi .txt)\nMasukkan Nama file : ";
    std::cin>>fileName;
    std::string folderName = "../test/";
    std::string extension = ".txt";
    std::string fullPath = folderName + fileName + extension;
    std::ifstream inFile(fullPath);
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file " << fullPath << std::endl;
    }
    std::string line;
    ListToken Lt;
    std::getline(inFile,line);
    Lt.Neff = std::stoi(line);
    std::string word;
    std::getline(inFile,word);
    Lt.token = (Token*)malloc(Lt.Neff*sizeof(Token));
    for (int i=0;i<Lt.Neff;i++){
        Lt.token[i].a = word[3*(i)];
        Lt.token[i].b = word[1+3*(i)];
    }
    int buff;
    int sizex,sizey;
    int nseq, maxSeq;
    std::getline(inFile,line);
    buff = std::stoi(line);
    std::getline(inFile,line,' ');
    sizex = std::stoi(line);
    std::getline(inFile,line,' ');
    sizey = std::stoi(line);

    std::getline(inFile,line);
    nseq = std::stoi(line);
    std::getline(inFile,line);
    maxSeq = std::stoi(line);

    generateMatrix(mat,sizex,sizey,Lt);
    ListSeq Lseq;
    generateSeq(&Lseq,buff,nseq,maxSeq,Lt);
    printMatrix(*mat);
    printSeq(Lseq);
    Buffer curr;
    declareBuff(&curr,buff);
    declareBuff(target,buff);
    const clock_t start = clock();
    exSearch(buff,*mat,false,target,curr,Lseq);
    const clock_t ends = clock();
    int time = ends-start;
    inFile.close();
    return time;
}
void saveOutput(Buffer target, Matrix mat, int time){
    std::cout<<"Output akan berada dalam folder test/output\n";
    std::cout<<"(Nama file tidak perlu diberi extension .txt)\n";
    std::cout<<"Nama file output: ";
    std::string fileName;
    std::string filePath = "../test/output/";
    std::string extension = ".txt";
    std::cin>>fileName;
    std::string fullPath = filePath + fileName + extension;
    std::ofstream outFile(fullPath);

    if (!outFile) {
        std::cerr << "Error: Unable to open output file.\n";
    }else{
        outFile << target.reward<<"\n";
        int i;
        for (i=0;i<target.Neff-1;i++){
            outFile<<mat.mat[target.point[i].y][target.point[i].x].a<<mat.mat[target.point[i].y][target.point[i].x].b<<" ";
        }
        outFile<<mat.mat[target.point[i].y][target.point[i].x].a<<mat.mat[target.point[i].y][target.point[i].x].b<<"\n";
        for (i=0;i<target.Neff;i++){
            outFile<<target.point[i].x+1<<", "<<target.point[i].y+1<<"\n";
        }
        outFile<<"\n"<<time<<" ms\n";

    }
    outFile.close();

}
int main(){
    srand(time(0));
    Buffer target;
    Matrix mat;
    int time;
    std::cout<<"Silahkan pilih mode: \n1. Manual\n2. Auto\n";
    int mode;
    std::cin>>mode;
    if (mode==1){
        std::cout<<"Silahkan pilih cara input: \n1. Manual\n2. File\n";
        std::cin>>mode;
        if (mode ==1){
           time =  getInput(&target,&mat);
        }else{
            time = FileInput(&target,&mat);
        }

    }else{
        std::cout<<"Silahkan pilih cara input: \n1. Manual\n2. File\n";
        std::cin>>mode;
        if (mode==1){
            time = getGenerator(&target,&mat);
        }else{
            time = fileGenerate(&target,&mat);
        }
    }
    printBuffer(target,mat);
    std::cout<<"\ntime: "<<time<<" ms\n";
    char save;
    do{
        std::cout<<"Apakah ingini output disimpan (Y/N): ";
        std::cin>>save;
    }while(save !='Y' && save != 'y' && save != 'N' && save!='n');
    
    if (save=='Y' || save=='y'){
        saveOutput(target,mat,time);
    }
    return 0;
}
/*
9
4 4
7A 55 E9 E9
55 7A 1C 7A
55 1C 1C 55
BD 1C 7A 1C
2
55 1C E9
15
E9 1C
20
*/
/*
7
6 4
7A 55 E9 E9 1C 55
55 7A 1C 7A E9 55
55 1C 1C 55 E9 BD
BD 1C 7A 1C 55 BD
3
BD E9 1C
15
BD 7A BD
20
BD 1C BD 55
30
*/
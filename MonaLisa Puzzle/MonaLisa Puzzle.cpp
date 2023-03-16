#include <iostream>
#include <conio.h> /*for getch() function*/
#include <graphics.h> /*library to code the game*/
#include <mmsystem.h> /*library to play music*/
#pragma comment(lib, "Winmm.lib") /*link the Winmm library to this project*/
const int N = 3;//since we are working on 3x3 sliding puzzle

using namespace std;

void init();
void GameInit();
void DrawMap();
void play();
void Judg();
int inverseNumber();

IMAGE img[2], imgs[9]; //img to store a monalisa and a white backgroun jpg, imgs is to store the picture pieces that are cut
int map[3][3] = {0}; //create a matrix so locate the destination of all the pictures
int aim_r, aim_c;

int main() {
    
    initgraph(N * 300, N * 300);  //initialize a window of 900 x 900
    cleardevice();
    init();              //load the game files
    GameInit();            //formatting the game
    DrawMap();            //draw the actual graph
    while (1)
    {
        play();            //player controls
        Judg();            //check if win or lose
    }
    closegraph();          //terminate the window/game
    return 0;
}

void init() {

    loadimage(&img[0], _T("./mona_lisa.jpg"), 900, 900); //load the monalisa picture
    loadimage(&imgs[8], _T("./blank.jpg"), 900, 900); //load the white empty piece

}

void GameInit() {

    srand(time(NULL));
    putimage(0, 0, &img[0]); //put up the monalisa image

    SetWorkingImage(&img[0]); //set working image to monalisa.jpg
    for(int y = 0, n = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (n == 8) break; //since we are making sure the empty space at bottom right therefore we will break the loop when n == 8 instead of 9
            getimage(&imgs[n++], x * 300, y * 300, (x + 1) * 300, (y + 1) * 300); //split the images and store it inside imgs
        }
    }

    SetWorkingImage(); //set working image back to default window
    map[N - 1][N - 1] = 8;
    do {
        int index = 0; //array A's 
        int size = 8; //array A's number of element
        int A[9] = { 0, 1, 2, 3, 4, 5, 6, 7};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == 2 && j == 2) continue; //maintain the bottom right hand white space
                index = rand() % size; //randomly pick a number between the 8 number
                map[i][j] = A[index]; //put the randomly picked number into the 2d map
                for (int k = index; k < size; k++) { //move the position of the elements after the element picked forward(positinion number decreases) by 1
                    A[k] = A[k + 1];
                }
                size--; //reduce the size of the array to ensure no repeated puzzle is picked
            }
        }
    } while (inverseNumber() % 2); //execute the do action, when the returned sum is odd number

}

void DrawMap() {

    BeginBatchDraw();// starts to load image
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            putimage(x * 300, y * 300, &imgs[map[y][x]]);
        }
    }
    EndBatchDraw(); //end loading the image
}

void play() {

    int col, row; //the row and column of the mouse click position
    MOUSEMSG msg; //mouse click event
    msg = GetMouseMsg(); //get the mouse click event
    switch (msg.uMsg) {    //respond to mouse clicks events

    case WM_LBUTTONDOWN: //when left click
        col = msg.x / 300; //get the column position
        if (msg.x == 900) {
            col = 2;
        }
        row = msg.y / 300; //get the row position
        if (msg.y == 900) {
            row = 2;
        } // we need the if statement to make sure theres no bug when players click on the edge of the puzzle

        for (int i = 0; i < N; i++) { //locate the posiiton of mouse click
            for (int j = 0; j < N; j++) {
                if (map[i][j] == 8) { //locate the position of the white space puzzle
                    aim_r = i;
                    aim_c = j;
                }
            }
        }
        //check if the puzzle is nearby if yes perform swap
        if (row == aim_r && col == aim_c + 1 ||
            row == aim_r && col == aim_c - 1 ||
            row == aim_r + 1 && col == aim_c ||
            row == aim_r - 1 && col == aim_c) {

            //mouseclick the puzzle with empty space to swap
            PlaySound(TEXT("swoosh.wav"), NULL, SND_ASYNC);
            map[aim_r][aim_c] = map[row][col];
            map[row][col] = 8;
        }
        DrawMap();
        break;

    case WM_RBUTTONDOWN:  //when right click
        putimage(0, 0, &img[0]);  //put up the complete mona lisa picture up
        break;

    case WM_RBUTTONUP:    //when right click is let go
        DrawMap();      //restore the playing puzzle
        break;

    }   
}

int inverseNumber() {//this function is used to detects if player have completed the game, it assigns a number to each puzzle if they are all in order 
                     //their sum would be 0
    int sum = 0, k = 0;
    int arr[N * N];
    //Assign map's location data 0-8, into an integer array
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            arr[k++] = map[i][j];
        }
    }
    //to get the inversed sum
    for (int i = 1; i < N * N; i++)    //add from the second number
    {
        for (int j = 0; j < i; j++)    //iterate through the loop and add up all number that satisfied the condition
        {
            if (arr[i] < arr[j])
            {
                sum++;
            }
        }
    }
    return sum; 
}

void Judg() {
    //check if game done
    if (inverseNumber() == 0) { //if its 0 meaning the puzzle is sorted in ascending order 0 - 8
        putimage(0, 0, &img[0]); //put up the complete image without the white space 
        MessageBox(GetHWnd(), L"Puzzle Solved!", L"Victory", MB_OK);
        exit(0);
    }
}


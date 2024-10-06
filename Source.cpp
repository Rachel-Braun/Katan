#include <sstream>
#include <memory>
#include <fstream>
#include "Input.h"
#include <iostream>
#include <thread>
#include "json.hpp" 
#include "useJSON.h"
#include "World.h"
#include "Functions.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

void showWorld(World world)
{
    int border_width = 1;
    // Calculate the size of each image for the grid
    Mat temp = imread("D:/Users/User/Downloads/TILES/" + world.board[0][0].category + ".png", IMREAD_COLOR);
    int width = temp.cols + 2 * border_width, height = temp.rows + 2 * border_width;
    int sizeCell = width / 5;
    int gridWidth = world.board[0].size() * height;
    int gridHeight = world.board.size() * width;

    // Create a blank canvas to arrange images
    Mat gallery(gridHeight, gridWidth, CV_8UC3, Scalar(255, 255, 255));

    for (int i = 0; i < world.board.size(); i++)
    {
        for (int j = 0; j < world.board[i].size(); j++)
        {
            Mat img = imread("D:/Users/User/Downloads/TILES/"+ world.board[i][j].category+".png", IMREAD_COLOR);
            Mat bordered_image;
            cv::copyMakeBorder(img, bordered_image, border_width, border_width, border_width, border_width, BORDER_CONSTANT, Scalar(0, 0, 0));
            bordered_image.copyTo(gallery(Rect(j*width, i*height, bordered_image.cols, bordered_image.rows)));
        }
    }

    // Display the image gallery
    

    vector<Mat> objectsMat(world.itemCount["People"]+ world.itemCount["Truck"]+ world.itemCount["Car"]);
    vector<float>speeds(objectsMat.size());
    useJSON jsonData;
    int objectsMatIndex = 0;
    for(int i=0; i<world.cells.size(); i++)
        for (int j = 0; j < world.cells[i].size(); j++)
        {
            if (!world.itemEmpty(j, i))
            {
                Mat tempMat = imread("D:/Users/User/Downloads/WALKING PERSON 2/sprite_3.png", IMREAD_COLOR);
                tempMat.resize(sizeCell);
                objectsMat[objectsMatIndex] = tempMat;
                speeds[objectsMatIndex] = jsonData.getSpeedOf(world.cells[j][i].itemPtr->category);
                objectsMat[objectsMatIndex].copyTo(gallery(Rect(j*sizeCell,i*sizeCell,tempMat.cols,tempMat.rows)));
                objectsMatIndex++;
            }
        }
    namedWindow("Image Gallery", WINDOW_NORMAL);
    imshow("Image Gallery", gallery);
    waitKey(0);
   
        for(int i=0; i<objectsMat.size(); i++)
        { 
            //imshow("ObjectMat"+i, objectsMat[i]); // Display the image you want to move
            //objectsMat[i].copyTo(gallery(Rect(j * width, i * height, objectsMat[i].cols, objectsMat[i].rows)));

            int x = getWindowImageRect("ObjectMat" + i).tl().x;// Update image position
            int y = getWindowImageRect("ObjectMat" + i).tl().y;// Update image position

            moveWindow("ObjectMat" + i, x+1, y);
        }

        /*waitKey(1); 
        this_thread::sleep_for(chrono::seconds(2));*/
    //}


    destroyWindow("Image Gallery");
}

int main()
{
    Functions f;
    auto functions = f.functionsMap;
    Input i;
    i.parse_and_store();
    World board(i.world->data);

    for (shared_ptr<Command> c : i.start)
    {
        functions[c->name](board, *c);
    }

    f.isStart = false;

    for (shared_ptr<Command> c : i.steps)
    {
        functions[c->name](board, *c);
    }

    for (string name : i.asserts)
    {
        functions[name](board, Command(""));
    }

    showWorld(board);
    
    return 0;
}
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

#define INFO(X) cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " = " << X << endl;

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array (column) placed on the heap
  }
  delete [] image;
  image = nullptr;
}

int* createSeam(int length) {
    //INFO(length);

    int* newSeamPointer = new int [length];
    for (int i = 0; i < length; i += 1) {
      newSeamPointer[i] = 0;
    }
  return newSeamPointer;
}

void deleteSeam(int* seam) {
    //INFO(seam);
    delete[] seam;
    seam = nullptr;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
  /*INFO(filename);
  INFO(image);
  INFO(width);
  INFO(height);*/

  int color_check; // File whitespace checker for later in traversing the arrays of the PPM
  ifstream ifs (filename);
  if (!ifs.is_open()) {
    cout << "Error: failed to open input file - " << filename;
    return false;
  }

  char P3[3]; // String Array to check the P3 line
  ifs >> P3;
  if ((toupper(P3[0]) != 'P') || (P3[1] != '3')) { // check to see that it is actually P3
    cout << "Error: type is " << P3 << " instead of P3" << endl;
    return false;
  }

  int h;
  int w;
  int max255;
  ifs >> w >> h >> max255;
  if (ifs.fail()) { // Read an invaild int width/height/225
    cout << "Error: read non-integer value";
    return false;
  }

  if (w != width) { // If file and input widths do not match
    cout << "Error: input width (" << width << ") does not match value in file (" << w << ")";
    return false;
  }
  if (h != height) { // If file and input widths do not match
    cout << "Error: input height (" << height << ") does not match value in file (" << h << ")";
    return false;
  }

  for (int rows = 0; rows < height; rows += 1) { // Beginning of the [rows][columns] traversal to change to [columns][rows]
    for (int columns = 0; columns < width; columns += 1) {

      ifs >> image[columns][rows].r;
      if (ifs.fail()) { // Checks if r was not read (not a color value for it)
        if (!ifs.eof()) { // If the file still has info, it means it was a non-int
          cout << "Error: read non-integer value";
          return false;
        }
        cout << "Error: not enough color values"; // Otherwise, the ifs read an empty input
        return false;
      }
      ifs >> image[columns][rows].g;
      if (ifs.fail()) { // Checks if g was not read (not a color value for it)
        if (!ifs.eof()) { // If the file still has info, it means it was a non-int
          cout << "Error: read non-integer value";
          return false;
        }
        cout << "Error: not enough color values"; // Otherwise, the ifs read an empty input
        return false;
      }
      ifs >> image[columns][rows].b;
      if (ifs.fail()) { // Checks if b was not read (not a color value for it)
        if (!ifs.eof()) { // If the file still has info, it means it was a non-int
          cout << "Error: read non-integer value";
          return false;
        }
        cout << "Error: not enough color values"; // Otherwise, the ifs read an empty input
        return false;
      }
      // Other checks (invalid color value number)
      if (image[columns][rows].r < 0 || image[columns][rows].r > 255) { // Read an invaild r color value
        cout << "Error: invalid color value " << image[columns][rows].r;
        return false;
      }
      if (image[columns][rows].g < 0 || image[columns][rows].g > 255) { // Read an invaild g color value
        cout << "Error: invalid color value " << image[columns][rows].g;
        return false;
      }
      if (image[columns][rows].b < 0 || image[columns][rows].b > 255) { // Read an invaild b color value
        cout << "Error: invalid color value " << image[columns][rows].b;
        return false;
      }
    }
  }

  if (!ifs.eof()) { // If all the file hasn't been exhausted yet
    ifs >> color_check;
    if (!ifs.fail()) { // If it doesn't fail, an extra value was found
      cout << "Error: too many color values";
      return false;
    }
    // If ifs failed, the file just has whitespace
  }
  return true; // All is good
}

bool outputImage(string filename, Pixel** image, int width, int height) {
    /*INFO(filename);
    INFO(image);
    INFO(width);
    INFO(height);*/

    ofstream ofs (filename); // Output stream to write into the file
    if (!ofs.is_open()) {
      cout << "Error: failed to open output file - " << filename;
      return false;
    }

    ofs << "P3\n" << width << " " << height << "\n255\n"; // Preamble written
    for (int rows = 0; rows < height; rows += 1) { // Beginning of the [rows][columns] traversal to change to [columns][rows] (original config.)
      for (int columns = 0; columns < width; columns += 1) {
        ofs << image[columns][rows].r << " " << image[columns][rows].g << " " << image[columns][rows].b << " "; // Values spaces as: r g b r g b
      }
    }
  return true;
}

int energy(Pixel** image, int column, int row, int width, int height) { 
    /*INFO(image);
    INFO(column);
    INFO(row);
    INFO(width);
    INFO(height);*/

    int coord1x = column - 1; // X Coordinate 1 (The left point)
    if (coord1x < 0) { // If the column is the first column, the previous column is the last column
      coord1x = width - 1;
    }
    int coord2x = column + 1; // X Coordinate 2 (The right point)
    if (coord2x == width) { // If the column is the last column, the next column is the first column
      coord2x = 0;
    }
    int coord1y = row - 1; // Y Coordinate 1 (The point above)
    if (coord1y < 0) { // If the row is the first row, the previous row is the last row
      coord1y = height - 1;
    }
    int coord2y = row + 1; // Y Coordinate 2 (The point below)
    if (coord2y == height) { // If the row is the last row, the next row is the first row
      coord2y = 0;
    }

    // Only the x-coord changes to compute the deltaX energy (y-coord, row, is static)
    int Rx1 = image[coord1x][row].r; // X Coordinate 1's RGB values
    int Gx1 = image[coord1x][row].g;
    int Bx1 = image[coord1x][row].b;
    int Rx2 = image[coord2x][row].r; // X Coordinate 2's RGB values
    int Gx2 = image[coord2x][row].g;
    int Bx2 = image[coord2x][row].b;
    // Only the y-coord changes to compute the deltaY energy (x-coord, column, is static)
    int Ry1 = image[column][coord1y].r; // Y Coordinate 1's RGB values
    int Gy1 = image[column][coord1y].g;
    int By1 = image[column][coord1y].b;
    int Ry2 = image[column][coord2y].r; // Y Coordinate 2's RGB values
    int Gy2 = image[column][coord2y].g;
    int By2 = image[column][coord2y].b;

    int Rx = Rx1 - Rx2; // Computing all deltaRGBs in x and y terms
    int Gx = Gx1 - Gx2;
    int Bx = Bx1 - Bx2;
    int Ry = Ry1 - Ry2;
    int Gy = Gy1 - Gy2;
    int By = By1 - By2;

    int deltaXTotal = pow(Rx , 2.0) + pow(Gx , 2.0) + pow(Bx , 2.0); // Adding the squared deltas for total energy
    int deltaYTotal = pow(Ry , 2.0) + pow(Gy , 2.0) + pow(By , 2.0);
    int energy = deltaXTotal + deltaYTotal;
  return energy;
}

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
    /*INFO(image);
    INFO(start_col);
    INFO(width);
    INFO(height);
    INFO(seam);*/

    int totalEnergy = energy(image, start_col, 0, width, height); // Starting total sum is the starting column's (at row 0) energy
    int currentColumn = start_col;
    seam[0] = start_col;
    int possibleNextColumn;
    int minimumEnergy;

    for (int row = 1; row < height; row += 1) { // Where the currentColumn is the column chosen before the row in question of the loop.
      // Initialization
      minimumEnergy = energy(image, currentColumn, row, width, height); // Straight-down option
      possibleNextColumn = currentColumn;

      if ((currentColumn + 1 < width) && (energy(image, currentColumn + 1, row, width, height)) < minimumEnergy) { // LEFT
        possibleNextColumn = currentColumn + 1; // If the left option index exists and is smaller than the straight-down option.
        minimumEnergy = energy(image, currentColumn + 1, row, width, height);
      }
      if ((currentColumn - 1 >= 0) && (energy(image, currentColumn - 1, row, width, height)) < minimumEnergy) { // RIGHT
        possibleNextColumn = currentColumn - 1; // If the right option index exists and is smaller than the straight-down/left option.
        minimumEnergy = energy(image, currentColumn - 1, row, width, height);
      }

      currentColumn = possibleNextColumn;
      seam[row] = currentColumn;
      totalEnergy += minimumEnergy;
    }
  
  return totalEnergy;
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
    /*INFO(image);
    INFO(start_row);
    INFO(width);
    INFO(height);
    INFO(seam);*/

    int totalEnergy = energy(image, 0, start_row, width, height); // Starting total sum is the starting row's (at column 0) energy
    int currentRow = start_row;
    seam[0] = start_row;
    int possibleNextRow;
    int minimumEnergy;

    for (int column = 1; column < width; column += 1) { // Where the currentRow is the row chosen before the column in question of the loop.
      // Initialization
      minimumEnergy = energy(image, column, currentRow, width, height); // Forward option
      possibleNextRow = currentRow;

      if ((currentRow - 1 >= 0) && (energy(image, column, currentRow - 1, width, height)) < minimumEnergy) { // LEFT
        possibleNextRow = currentRow - 1; // If the left option index exists and is smaller than the forward option.
        minimumEnergy = energy(image, column, currentRow - 1, width, height);
      }
      if ((currentRow + 1 < height) && (energy(image, column, currentRow + 1, width, height)) < minimumEnergy) { // RIGHT
        possibleNextRow = currentRow + 1; // If the right option index exists and is smaller than the forward/left option.
        minimumEnergy = energy(image, column, currentRow + 1, width, height);
      }

      currentRow = possibleNextRow;
      seam[column] = currentRow;
      totalEnergy += minimumEnergy;
    }

  return totalEnergy;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
    /*INFO(image);
    INFO(width);
    INFO(height);*/

    int* workingSeam = createSeam(height);
    int minimumEnergy = loadVerticalSeam(image, 0, width, height, workingSeam); // Initialized to first vertical seam to compare
    int potentialMinimumSeamEnergy;
    int* seamToDelete = createSeam(height);
    for (int j = 0; j < height; j += 1) { // If column 0 is the column to be deleted, the seam is saved and returned later.
      seamToDelete[j] = workingSeam[j];
    }

    for (int column = 1; column < width; column += 1) {
      potentialMinimumSeamEnergy = loadVerticalSeam(image, column, width, height, workingSeam); // Each column's seam is compared against the minimum
      if (potentialMinimumSeamEnergy < minimumEnergy) {
        minimumEnergy = potentialMinimumSeamEnergy; // New minimum seam energy
        for (int j = 0; j < height; j += 1) { // The new column's minimum seam is updated to be deleted later.
          seamToDelete[j] = workingSeam[j];
        }
      }
    }
    delete[] workingSeam;
    workingSeam = nullptr;

  return seamToDelete;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
    /*INFO(image);
    INFO(width);
    INFO(height);*/

    int* workingSeam = createSeam(width);
    int minimumEnergy = loadHorizontalSeam(image, 0, width, height, workingSeam); // Initialized to first horizontal seam to compare
    int potentialMinimumSeamEnergy;
    int* seamToDelete = createSeam(width);
    for (int j = 0; j < width; j += 1) { // If row 0 is the row to be deleted, the seam is saved and returned later.
      seamToDelete[j] = workingSeam[j];
    }

    for (int row = 1; row < height; row += 1) {
      potentialMinimumSeamEnergy = loadHorizontalSeam(image, row, width, height, workingSeam); // Each row's seam is compared against the minimum
      if (potentialMinimumSeamEnergy < minimumEnergy) {
        minimumEnergy = potentialMinimumSeamEnergy; // New minimum seam energy
        for (int j = 0; j < width; j += 1) { // The new row's minimum seam is updated to be deleted later.
          seamToDelete[j] = workingSeam[j];
        }
      }
    }
    delete[] workingSeam;
    workingSeam = nullptr;

  return seamToDelete;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) { // +++++++++++ WIP ++++++++++++++++++++++++++++++++++++++++++++++m
    /*INFO(image);
    INFO(width);
    INFO(height);
    INFO(verticalSeam);*/

    for (int row = 0; row < height; row += 1) { // Finding the column where the pixel-to-be-deleted resides in each row
      for (int column = 0; column < width - 1; column += 1) {
        if (column == verticalSeam[row]) { // Found the column in the given row where we must remove the value (shifting the rest to the left)
          int workingColumn = column;
          while (workingColumn + 1 < width) { // Moves each cell (RGB) after the cell-to-be-deleted to the left
            image[workingColumn][row].r = image[workingColumn + 1][row].r;
            image[workingColumn][row].g = image[workingColumn + 1][row].g;
            image[workingColumn][row].b = image[workingColumn + 1][row].b;
            workingColumn += 1; // Next cell (if applicable) will be accessed in the next while-loop iteration to change as well
          }
          break; // Finished with this row, this breaks the column for-loop to go to the next row to modify
        }
      }
    } // At this point, all rows are modified and the width is ready to be shortened.
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
    /*INFO(image);
    INFO(width);
    INFO(height);
    INFO(horizontalSeam);*/

    for (int column = 0; column < width; column += 1) { // Finding the row where the pixel-to-be-deleted resides in each column
      for (int row = 0; row < height - 1; row += 1) {
        if (row == horizontalSeam[column]) { // Found the row in the given column where we must remove the value
          int workingRow = row;
          while (workingRow + 1 < height) { // Modifies each cell (RGB) after the cell-to-be-deleted
            image[column][workingRow].r = image[column][workingRow + 1].r;
            image[column][workingRow].g = image[column][workingRow + 1].g;
            image[column][workingRow].b = image[column][workingRow + 1].b;
            workingRow += 1; // Next cell (if applicable) will be accessed in the next while-loop iteration to change as well
          }
          break; // Finished with this column, this breaks the row for-loop to go to the next column to modify
        }
      }
    } // At this point, all columns are modified and the height is ready to be shortened.
}

#include <iostream>

#include "cdk.h"
#include <stdint.h>
#include <fstream>
#include <iomanip>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryFileHeader
{
public:

  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

const int maxRecordStringLength = 25;

class BinaryFileRecord
{
public:

  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
  };

int main()
{
    WINDOW*window;
    CDKSCREEN*cdkscreen;
    CDKMATRIX     *myMatrix;           // CDK Screen Matrix


    const char *rowTitles[] = {"R0", "a", "b", "c", "d", "e"}; //rows and columns
    const char *columnTitles[] = {"C0", "a", "b", "c", "d", "e"};
    int boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
    int boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

    window = initscr();
    cdkscreen = initCDKScreen(window);

    /* Start CDK Colors */
    initCDKColor();

    /*
     * Create the matrix.
     */
    myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			    MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
			    boxTypes, 1, 1, ' ', ROW, true, true, false);

    if (myMatrix ==NULL)
      {
	printf("Error creating Matrix\n");
	_exit(1);
      }

    /* Display the Matrix */
    drawCDKMatrix(myMatrix, true);


    BinaryFileHeader *myHeader  = new BinaryFileHeader(); //create classes and read in binary header and Record through ifstream
   BinaryFileRecord *myRecord = new BinaryFileRecord();
  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);
  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));
  
  char temp[25]; //convert to char* to use in matrix

  sprintf(temp,"0x%8X\n", myHeader->magicNumber);
  
  setCDKMatrixCell(myMatrix, 1, 1, temp);

  sprintf(temp, "%u", myHeader->versionNumber);
  setCDKMatrixCell(myMatrix, 1, 2,temp);

  sprintf(temp, "%lu", myHeader->numRecords);
  setCDKMatrixCell(myMatrix, 1, 3, temp);


  for (int i=2;i<6;i++)
  {
   binInfile.read((char *) myRecord, sizeof(BinaryFileRecord));
   
   sprintf(temp, "%u", myRecord->strLength);
   setCDKMatrixCell(myMatrix, i,1, temp);
   
   setCDKMatrixCell(myMatrix, i,2, myRecord->stringBuffer);

}

binInfile.close();

//display
 drawCDKMatrix(myMatrix, true);    

 //wait for input
  unsigned char x;
  cin >> x;

  // Cleanup
  endCDK();
}

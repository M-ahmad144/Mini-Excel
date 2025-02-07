#include <iostream>
#include <cctype>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;
//template <typename T>

class Excel
{
    class Cell
    {
    public:
        string data;
        Cell* up;
        Cell* down;
        Cell* left;
        Cell* right;
        Cell(string val)
        {
            data = val;
            up = nullptr;
            down = nullptr;
            left = nullptr;
            right = nullptr;
        }
    };

    //................. Iterator class

    class Iterator
    {
    public:
        Cell* iter;

        Iterator(Cell* curr)
        {
            iter = curr;
        }

        Iterator& operator++() // Pre increment to move right
        {
            iter = iter->right;
            return *this;
        }

        Iterator& operator--() // Pre decrement to move left
        {
            iter = iter->left;
            return *this;
        }

        Iterator& operator++(int) // Post increment to move down
        {
            iter = iter->down;
            return *this;
        }

        Iterator& operator--(int) // Post decrement to move up
        {
            iter = iter->up;
            return *this;
        }

        string& operator*() // Returns data in cell
        {
            return iter->data;
        }

        bool operator==(const Iterator& other) const
        {
            return iter == other.iter;
        }

        bool operator!=(const Iterator& other) const
        {
            return iter != other.iter;
        }
    };

    Iterator Start()
    {
        Cell* temp = current;
        while (temp->left)
        {
            temp = temp->left;
        }
        while (temp->up)
        {
            temp = temp->up;
        }

        return Iterator(temp);
    }

    Iterator End()
    {

        return Iterator(nullptr);
    }
    //........................................

public:
    Cell* current;
    vector<string> clipboardData; // to store copied or cut data
    bool isColumn;                // Rename 'check' to 'isColumn' for clarity
    Cell* firstCell;
    //.........................

    // Constructor
    Excel()
    {
        current = new Cell("---"); // Initializing 1st element
        firstCell = current;
        current->left = nullptr;

        // Initializing 1st row
        for (int i = 0; i < 4; i++)
        {
            Cell* Row1 = new Cell("---");
            current->right = Row1;
            Row1->left = current;
            Row1->up = nullptr;
            current = Row1;
        }
        current = firstCell; // Resetting the current pointer to the first cell of the 1st row

        // Initializing 2nd row
        Cell* row2Start = new Cell("---");
        current->down = row2Start;
        row2Start->up = current;

        for (int i = 0; i < 4; i++)
        {
            Cell* Row2 = new Cell("---");
            row2Start->right = Row2;
            Row2->left = row2Start;
            Row2->up = current;
            row2Start = Row2;
        }
        current = firstCell;

        // Initializing 3rd row
        Cell* row3Start = new Cell("---");
        current->down->down = row3Start;
        row3Start->up = current->down;

        for (int i = 0; i < 4; i++)
        {
            Cell* Row3 = new Cell("---");
            row3Start->right = Row3;
            Row3->left = row3Start;
            Row3->up = current;
            row3Start = Row3;
        }
        current = firstCell;

        // Initializing 4th row
        Cell* row4Start = new Cell("---");
        current->down->down->down = row4Start;
        row4Start->up = current->down->down;

        for (int i = 0; i < 4; i++)
        {
            Cell* Row4 = new Cell("---");
            row4Start->right = Row4;
            Row4->left = row4Start;
            Row4->up = current;
            row4Start = Row4;
        }
        current = firstCell;

        // Initializing 5th row
        Cell* row5Start = new Cell("---");
        current->down->down->down->down = row5Start;
        row5Start->up = current->down->down->down;

        for (int i = 0; i < 4; i++)
        {
            Cell* Row5 = new Cell("---");
            row5Start->right = Row5;
            Row5->left = row5Start;
            Row5->up = current;
            row5Start = Row5;
            row5Start->down = nullptr;
        }

        current = firstCell; // Resetting the current pointer to the first cell of the 1st row

        Cell* currentPtr = current;       // Pointer to the current cell
        Cell* nextRowPtr = current->down; // Pointer to the cell in the next row

        for (int i = 1; i <= 16; i++) // Making up and down connections between 1 to 4 columns
        {
            currentPtr = currentPtr->right;
            nextRowPtr = nextRowPtr->right;

            currentPtr->down = nextRowPtr;
            nextRowPtr->up = currentPtr;

            if (i % 4 == 0)
            {
                while (currentPtr->left && nextRowPtr->left)
                {
                    currentPtr = currentPtr->left;
                    nextRowPtr = nextRowPtr->left;
                }
                currentPtr = currentPtr->down;
                nextRowPtr = nextRowPtr->down;
            }
        }

        current = firstCell;
        //   checking Inserting row above below by changing the current
        // current = current->right;
    }

    //.........................
    // funtion1
    void InsertRowBelow()
    {
        Cell* temp = current;
        Cell* row = new Cell("0");

        // Find the last cell in the row below the current row
        while (temp->left)
        {
            temp = temp->left; // moving temporary to the left most
        }

        // If there is a row below the current row
        if (temp->down == nullptr)
        {
            // Insert the new row below the last cell in the row below
            row->up = temp;
            temp->down = row;
            row->down = nullptr;

            // Connect the cells in the new row horizontally
            while (temp->right)
            {
                Cell* NewRow = new Cell("0");
                NewRow->left = row;
                row->right = NewRow;
                temp = temp->right;
                temp->down = NewRow;
                NewRow->up = temp;
                row = NewRow;
                NewRow->down = nullptr;
            }
        }
        else
        {
            row->up = temp;
            row->down = temp->down;
            temp->down->up = row;
            temp->down = row;
            while (temp->right)
            {
                Cell* NewRow = new Cell("0");
                NewRow->left = row;
                row->right = NewRow;
                temp = temp->right;
                NewRow->up = temp;
                NewRow->down = temp->down;
                temp->down->up = NewRow;
                temp->down = NewRow;
                row = row->right;
            }
        }
    }
    //.........................
    // funtion2
    void InsertRowAbove()
    {
        Cell* temp = current;
        Cell* row = new Cell("0");
        while (temp->left)
        {
            temp = temp->left;
        }

        if (temp->up == nullptr)
        {
            row->down = temp;
            temp->up = row;
            row->up = nullptr;
            while (temp->right)
            {
                Cell* rowElement = new Cell("0");
                row->right = rowElement;
                rowElement->left = row;
                temp = temp->right;
                temp->up = rowElement;
                rowElement->down = temp;
                row = row->right;
                rowElement->up = nullptr;
            }
        }
        else
        {
            row->down = temp;
            row->up = temp->up;
            temp->up->down = row;
            temp->up = row;

            while (temp->right)
            {
                Cell* rowElement = new Cell("0");
                rowElement->left = row;
                row->right = rowElement;
                temp = temp->right;
                rowElement->up = temp->up;
                temp->up->down = rowElement;
                rowElement->down = temp;
                temp->up = rowElement;
                row = row->right;
            }
        }
    }
    //.........................
    // funtion3
    void insertColumnToLeft()
    {

        Cell* currentCell = current;

        // Create the first cell of the new columnSF
        Cell* newColumn = new Cell("0");

        // Move to the leftmost cell in the row
        while (currentCell->up)
        {
            currentCell = currentCell->up;
        }

        // If there is no cell to the left of the current cell, insert the new column at the beginning of the row
        if (currentCell->left == nullptr)
        {
            // Connect the new column to the right of the current cell
            newColumn->right = currentCell;

            // Connect the current cell to the left of the new column
            currentCell->left = newColumn;

            // Set the left side of the new column to null since it is at the beginning of the row
            newColumn->left = nullptr;

            // Insert remaining cells in the new column
            while (currentCell->down)
            {
                // Create a new cell for the new column
                Cell* newColumnElement = new Cell("0");

                // Connect the new cell to the bottom of the previous cell in the new column
                newColumn->down = newColumnElement;

                // Connect the previous cell in the new column to the top of the new cell
                newColumnElement->up = newColumn;

                // Move to the next cell in the row
                currentCell = currentCell->down;

                // Connect the new cell to the right of the next cell in the row
                newColumnElement->right = currentCell;

                // Connect the next cell in the row to the left of the new cell
                currentCell->left = newColumnElement;

                // Set the left side of the new cell to null since it is at the end of the column
                newColumnElement->left = nullptr;

                // Move the temporary column element to the next cell in the new column
                newColumn = newColumn->down;
            }
        }
        else
        {
            // Insert the new column to the right of the cell to the left of the current cell
            newColumn->right = currentCell;

            // Connect the cell to the left of the current cell to the left of the new column
            newColumn->left = currentCell->left;

            // Connect the cell to the left of the current cell to the right of the new column
            currentCell->left->right = newColumn;

            // Connect the current cell to the left of the new column
            currentCell->left = newColumn;

            // Insert remaining cells in the new column
            while (currentCell->down)
            {
                // Create a new cell for the new column
                Cell* newColumnElement = new Cell("0");

                // Connect the new cell to the bottom of the previous cell in the new column
                newColumn->down = newColumnElement;

                // Connect the previous cell in the new column to the top of the new cell
                newColumnElement->up = newColumn;

                // Move to the next cell in the row
                currentCell = currentCell->down;

                // Connect the new cell to the right of the next cell in the row
                newColumnElement->right = currentCell;

                // Connect the new cell to the left of the next cell in the row
                newColumnElement->left = currentCell->left;

                // Connect the cell to the left of the next cell in the row to the right of the new cell
                currentCell->left->right = newColumnElement;

                // Connect the next cell in the row to the left of the new cell
                currentCell->left = newColumnElement;

                // Move the temporary column element to the next cell in the new column
                newColumn = newColumn->down;
            }
        }
    }
    //.........................
    // funtion4
    void InsertColumnToRight()
    {
        // Get the current cell
        Cell* temp = current;
        // Create the new column
        Cell* col = new Cell("0");

        // Move to the top of the current cell's column
        while (temp->up)
        {
            temp = temp->up;
        }

        // If there is no cell to the right of the current cell, insert the new column at the end of the row
        if (temp->right == nullptr)
        {
            col->left = temp;
            temp->right = col;
            col->right = nullptr;

            // Loop till the row end
            while (temp->down)
            {
                Cell* NewColElement = new Cell("0");
                col->down = NewColElement;
                NewColElement->up = col;
                temp = temp->down;
                NewColElement->left = temp;
                temp->right = NewColElement;
                NewColElement->right = nullptr;
                col = col->down;
            }
        }
        else
        {
            // Insert the new column between the current cell and the cell to its right
            col->left = temp;
            col->right = temp->right;
            temp->right->left = col;
            temp->right = col;

            // Loop till the col end
            while (temp->down)
            {
                Cell* colElement = new Cell("0");
                col->down = colElement;
                colElement->up = col;
                temp = temp->down;
                colElement->left = temp;
                colElement->right = temp->right;
                temp->right->left = colElement;
                temp->right = colElement;
                col = col->down;
            }
        }
    }
    //.........................
    // funtion5
    void clearRow()
    {
        Cell* temp = current;
        while (temp->right)
        {

            temp = temp->right;
        }
        while (temp)
        {
            temp->data = "_";
            temp = temp->left;
        }
    }
    //.........................
    // funtion6
    void clearColumn()
    {
        Cell* temp = current;
        // cout << temp->down->data << endl;
        // cout << temp->down->down->data << endl;
        while (temp->up)
        {
            temp = temp->up;
        }
        while (temp)
        {
            temp->data = "_";
            temp = temp->down;
        }
    }
    //.........................

    // funtion7
    void PrintSheet()
    {
        // Bright cyan color
        cout << "\033[96m+\033[0m------------------------------------------\033[96m+\033[0m" << endl;

        // Find the top-left cell of the sheet
        Cell* topLeft = current;
        while (topLeft->up)
        {
            topLeft = topLeft->up;
        }
        while (topLeft->left)
        {
            topLeft = topLeft->left;
        }

        // Start printing the sheet from the top-left cell
        Cell* rowStart = topLeft;

        while (rowStart)
        {

            // Bright cyan color
            cout << "\033[96m|\033[0m";

            Cell* colStart = rowStart;

            while (colStart)
            {
                if (colStart == current)
                {
                    // Highlighted text in red and cyan background
                    cout << "\033[41;96m " << setw(4) << colStart->data << " \033[0m\033[96m|\033[0m";
                }
                else
                {
                    // Yellow text with cyan background
                    cout << "\033[93;46m|" << setw(4) << colStart->data << " \033[0m\033[96m|\033[0m";
                }

                colStart = colStart->right;
            }

            cout << endl;

            // Bright cyan color
            cout << "\033[96m+\033[0m------------------------------------------\033[96m+\033[0m" << endl;

            rowStart = rowStart->down;
        }
    }

    //.........................
    // funtion8
    void DeleteColumn()
    {
        Cell* topmostCellInColumn = current; // Pointer to the topmost cell in the current column
        Cell* tempCell = nullptr;            // Temporary pointer for traversal and link adjustments

        // Move to the topmost cell in the current column
        while (topmostCellInColumn->up)
        {
            topmostCellInColumn = topmostCellInColumn->up;
        }

        tempCell = topmostCellInColumn;

        // If it is the only column, nothing to delete
        if (topmostCellInColumn->left == nullptr && topmostCellInColumn->right == nullptr)
        {
            return;
        }

        // If it's the leftmost column
        else if (topmostCellInColumn->left == nullptr)
        {
            current = current->right; // Move current to the right
            while (topmostCellInColumn)
            {
                tempCell->right->left = nullptr;                 // Update left connection of the right pointer
                topmostCellInColumn = topmostCellInColumn->down; // Move down the column
                tempCell = topmostCellInColumn;
            }
        }

        // If it's the rightmost column
        else if (current->right == nullptr)
        {
            current = current->left; // Move current to the left
            while (topmostCellInColumn)
            {
                tempCell->left->right = nullptr;                 // Update left connection of the right pointer
                topmostCellInColumn = topmostCellInColumn->down; // Move down the column
                tempCell = topmostCellInColumn;
            }
        }

        // For other columns
        else
        {
            current = current->left; // Move current to the left
            while (topmostCellInColumn)
            {
                tempCell->left->right = tempCell->right;         // Update left connection of the right pointer
                tempCell->right->left = tempCell->left;          // Update right connection of the left pointer
                topmostCellInColumn = topmostCellInColumn->down; // Move down the column
                tempCell = topmostCellInColumn;
            }
        }
    }
    //.........................
    // funtion9
    void DeleteRow()
    {
        Cell* topmostCellInRow = current;
        Cell* tempCell;

        // Move to the topmost cell in the current row
        while (topmostCellInRow->left)
        {
            topmostCellInRow = topmostCellInRow->left;
        }

        tempCell = topmostCellInRow;

        // If it is the only row, nothing to delete
        if (topmostCellInRow->up == nullptr && topmostCellInRow->down == nullptr)
        {
            return;
        }

        // If it's the topmost row
        else if (topmostCellInRow->up == nullptr)
        {
            current = current->down; // Move current down
            while (topmostCellInRow)
            {
                topmostCellInRow->down->up = nullptr;       // Update up connection of the down pointer
                topmostCellInRow = topmostCellInRow->right; // Move across the row
                tempCell = topmostCellInRow;
            }
        }

        // If it's the bottommost row
        else if (current->down == nullptr)
        {
            current = current->up; // Move current up
            while (topmostCellInRow)
            {
                topmostCellInRow->up->down = nullptr;       // Update down connection of the up pointer
                topmostCellInRow = topmostCellInRow->right; // Move across the row
                tempCell = topmostCellInRow;
            }
        }

        // For other rows
        else
        {
            current = current->up; // Move current up
            while (topmostCellInRow)
            {
                tempCell->up->down = tempCell->down;        // Update down connection of the up pointer
                tempCell->down->up = tempCell->up;          // Update up connection of the down pointer
                topmostCellInRow = topmostCellInRow->right; // Move across the row
                tempCell = topmostCellInRow;
            }
        }
    }
    //.........................

    // funtion-9
    void InsertCellByRightShift()
    {
        Cell* temp = current;
        while (current->right)
        {
            current = current->right;
        }
        InsertColumnToRight();
        while (current != temp)
        {
            current->right->data = current->data;
            current = current->left;
        }

        current->data = "...";
    }
    //.........................
    // funtion-10
    void InsertCellByDownShift()
    {
        Cell* temp = current;
        while (current->down)
        {
            current = current->down;
        }
        InsertRowBelow();
        while (current != temp)
        {
            current->down->data = current->data;
            current = current->up;
        }

        current->data = "...";
    }

    //.........................
    // funtion-11
    void DeleteCellByLeftShift()
    {
        Cell* temp = this->current;

        // Check if the current cell is not the last cell in the row
        if (temp->right != nullptr)
        {
            Cell* tempNext = temp->right;

            // Shift the data to the left
            while (tempNext != nullptr)
            {
                temp->data = tempNext->data;
                temp = temp->right;
                tempNext = tempNext->right;
            }

            // Set the data of the last cell to indicate it's unused
            temp->data = "..."; // Replace with the desired placeholder value
        }
    }

    //...............................

    // Function to delete the current cell and shift cells below it up
    // funtion-12
    void DeleteCellByUpShift()
    {
        // Step 1: Initialization
        Cell* temp = this->current; // Start from the current cell

        // Check if the current cell has a cell below it
        if (temp->down != nullptr)
        {
            // Step 2: Shift the data upward
            Cell* tempNext = temp->down; // Initialize tempNext to the cell below temp
            while (tempNext != nullptr)  // Continue until there are no more cells below
            {
                temp->data = tempNext->data; // Shift data from tempNext to temp
                temp = temp->down;           // Move temp downward
                tempNext = tempNext->down;   // Move tempNext downward
            }

            temp->data = "...";
        }
    }

    //... funtion Check if digit or Not
    bool IsConvertibleToInt(string& str)
    {
        // Check if the string is convertible to an integer
        for (char c : str)
        {
            if (!isdigit(c))
            {
                return false;
            }
        }
        return true;
    }
    //....Funtion to Get Sum
    // funtion-13
    void GetRangeSum(Cell* start, Cell* end, Cell* destination)
    {
        int sum = 0;

        bool flag = checkTraversingLocation(end, start); // Finding the direction either to move right or down
        if (flag)                                        // When true move right
        {
            while (start != end->right) // Include the value at 'end' in the sum
            {
            
                    sum += stoi(start->data);
                    start = start->right;
               
            }

        }

        else // Move down
        {
            while (start != end->down) // Include the value at 'end' in the sum
            {
                
                    sum += stoi(start->data);
                
                start = start->down;
            }
        }
        current = destination;
        destination->data = to_string(sum);
    }

    // funtion-14
    void GetRangeAverage(Cell* start, Cell* end, Cell* location)
    {
        double sum = 0;
        int count = 0;

        bool flag = checkTraversingLocation(start, end);

        if (flag)
        {
            while (start != end->right)
            {
                sum += stoi(start->data);
                count++;
                start = start->right;
            }
        }

        else
        {
            while (start != end->down)
            {
                sum += stoi(start->data);
                count++;
                start = start->down;
            }
        }

        sum = sum / count;
        current = location;
        location->data = to_string(sum);
    }

    void MoveCurrentToLeft()
    {
        if (current->left)
        {
            current = current->left; // Moving left
        }
    }

    void MoveCurrentRight()
    {
        if (current->right)
        {
            current = current->right; // Moving right
        }
    }

    void MoveCurrentToUp()
    {
        if (current->up)
        {
            current = current->up; // Moving up
        }
    }

    void MoveCurrentToDown()
    {
        if (current->down)
        {
            current = current->down; // Moving down
        }
    }

    //... check traversing
    bool checkTraversingLocation(Cell* start, Cell* end)
    {
        while (start != nullptr)
        {
            if (start == end)
            {
                return true;
            }
            start = start->down;
        }
        return false;
    }

    void Validations(int& row, int& col, int& row2, int& col2)
    {
        if (row == row2)
        {

            while (col2 < col)
            {
                col2 = TakeInputForCell("Error! Enter ending cell column again: ");
            }
        }

        else
        {
            while (row2 < row)
            {
                row2 = TakeInputForCell("Error! Enter ending cell row again: ");
            }

            while (col2 != col)
            {
                col2 = TakeInputForCell("Error! Enter ending cell column again: ");
            }
        }
    }

    // Function to copy the data in a specified range
    void Copy(Cell* startCell, Cell* endCell)
    {
        clipboardData.clear();
        bool moveRight = checkTraversingLocation(startCell, endCell);
        isColumn = moveRight;

        if (moveRight)
        {
            while (startCell != endCell->right)
            {
                clipboardData.push_back(startCell->data);
                startCell = startCell->right;
            }
        }
        else
        {
            while (startCell != endCell->down)
            {
                clipboardData.push_back(startCell->data);
                startCell = startCell->down;
            }
        }
    }

    // Function to cut the data in a specified range
    void Cut(Cell* startCell, Cell* endCell)
    {
        clipboardData.clear();
        bool moveRight = checkTraversingLocation(startCell, endCell);
        isColumn = moveRight;

        if (moveRight)
        {
            while (startCell != endCell->right)
            {
                clipboardData.push_back(startCell->data);
                startCell->data = "-";
                startCell = startCell->right;
            }
        }

        else
        {
            while (startCell != endCell->down)
            {
                clipboardData.push_back(startCell->data);
                startCell->data = "-";
                startCell = startCell->down;
            }
        }
    }

    void Paste()
    {

        if (isColumn) // Finding if column or not , if it is true then row was copied
        {
            for (int i = 0; i < clipboardData.size(); i++)
            {
                current->data = clipboardData[i]; // copying data

                if (current->right == nullptr && i != clipboardData.size() - 1) // Checking if the columns are ended and still their are elements in vector
                {
                    Cell* newCell = new Cell("0"); // NewCell
                    newCell->left = current;       // Left Connection
                    current->right = newCell;      // right Connection
                }
                else if (current->right == nullptr && i == clipboardData.size() - 1) // If the last column and also the last row so do not create an extra cell
                {
                    current = current;
                }
                else
                {
                    current = current->right;
                }
            }
        }

        else // Checking if column was copied
        {
            for (int i = 0; i < clipboardData.size(); i++)
            {
                current->data = clipboardData[i];
                if (current->down == nullptr) // Checking if the columns are ended and still their are elements in vector
                {
                    Cell* newCell = new Cell("0");
                    newCell->up = current;
                    current->down = newCell;
                }
                else if (current->down == nullptr && i == clipboardData.size() - 1) // If the last column and also the last row so do not create an extra cell
                {
                    current = current;
                }
                else
                {
                    current = current->down;
                }
            }
        }
    }

    int TakeInputForCell(string input)
    {
        int cell;
        cout << input;
        while (_kbhit())
        {
            _getch();
        }
        cin >> cell;
        return cell - 1; // Adjust for 0-based indexing (if your Excel sheet is 0-indexed)
    }

    void ChageCellData()
    {
        cout << "Enter data of current: ";

        while (_kbhit()) // Clearing any echo
        {
            _getch();
        }
        cin >> current->data;
    }

    void gotoxy(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void printWelcomeBanner()
    {
        int x, y;
        // Assuming screen_size() is a function that sets the global variables columns_to_fit and rows_to_fit
        // screen_size();
        system("cls");

        x = 25;
        y = 3;
        gotoxy(x, y);
        cout << "\033[1;31m"
            << ".88b  d88.d 888888b  d8b"
            << "\033[1;32m"
            << "   db d888888b      d88888b db     db. o88b. d88888b db";

        gotoxy(x, y + 1);
        cout << " 88'YbdP`88   `88'   888o  88   `88'        88'     `8b  d8' d8P  Y8 88'     88 ";

        gotoxy(x, y + 2);
        cout << " 88  88  88   "
            << "\033[1;38m"
            << " 88    88V8o 88    88         88ooooo"
            << "\033[1;31m"
            << "  `8bd8'  8P      88ooooo 88";

        gotoxy(x, y + 3);
        cout << " 88  88  88    88    88 V8o88    88"
            << "\033[1;33m"
            << "         88~~~~~.  dPYb.  8b      88~~~~~ 88";

        gotoxy(x, y + 4);
        cout << " 88  88  88"
            << "\033[1;36m"
            << "   .88.   88  V888   .88.       "
            << "\033[1;38m"
            << " 88.     .8P  Y8.Y8b  d8 88.88bo  oo.";

        gotoxy(x, y + 5);
        cout << " YP  YP  YP Y888888P VP   V8P "
            << "\033[1;32m"
            << "Y888888P      Y88888P YP    YP"
            << "\033[1;31m"
            << "  `Y88P' Y88888P Y88888P";

        cout << "\n\n\n\n\n";
    }
    void DisplayMenu()
    {
        cout << "Excel Simulator Menu:" << endl;
        cout << "---------------------" << endl;
        cout << "Arrow Up        - Move Current Up" << endl;
        cout << "Arrow Down      - Move Current Down" << endl;
        cout << "Arrow Left      - Move Current Left" << endl;
        cout << "Arrow Right     - Move Current Right" << endl;
        cout << "Ctrl + Up       - Insert Row Above" << endl;
        cout << "Ctrl + Down     - Insert Row Below" << endl;
        cout << "Ctrl + Right    - Insert Column To Right" << endl;
        cout << "Ctrl + L        - Insert Column To Left" << endl;
        cout << "Shift + R       - Insert Cell By Right Shift" << endl;
        cout << "Shift + D       - Insert Cell By Down Shift" << endl;
        cout << "Delete + D      - Delete Cell By Left Shift" << endl;
        cout << "Delete + U      - Delete Cell By Up Shift" << endl;
        cout << "Delete + C      - Delete Column" << endl;
        cout << "Delete + R      - Delete Row" << endl;
        cout << "Tab + C         - Clear Column" << endl;
        cout << "Tab + R         - Clear Row" << endl;
        cout << "Shift + S       - Get Range Sum" << endl;
        cout << "Shift + V       - Get Range Average" << endl;
        cout << "Ctrl + W        - Copy Range" << endl;
        cout << "Ctrl + X        - Cut Range" << endl;
        cout << "Ctrl + V        - Paste Range" << endl;
        cout << "Ctrl + S        - Save Sheet" << endl;
        cout << "Insert          - Change Cell Data" << endl;
        cout << "Q               - Display Menu" << endl;
        cout << "Esc             - Exit" << endl;
    }

    Cell* GetCell(int row, int col)
    {
        Cell* temp = current;

        // Traverse to the specified row
        for (int i = 0; i < row && temp->down; i++)
        {
            if (temp->down != nullptr)
            {
                temp = temp->down;
            }

            else
            {
                break;
            }
        }

        // Traverse to the specified column
        for (int i = 0; i < col && temp->right; i++)
        {
            if (temp->right != nullptr)
            {
                temp = temp->right;
            }
            break;
        }

        return temp; // Return a pointer to the desired cell
    }

    // file handling

    // Function to save the sheet to a file
    void SaveSheet()
    {
        fstream file;
        file.open("sheet.txt", ios::out);

        for (Iterator it = Start(); it != End(); it++)
        {
            bool firstElement = true; // Flag to avoid a comma at very end of row

            for (Iterator t = it; t != End(); ++t)
            {
                if (!firstElement)
                {
                    file << ",";
                }
                else
                {
                    firstElement = false;
                }

                file << *t;
            }
            file << endl;
        }

        file.close();
    }
    // Function to load the sheet from a file
    void LoadSheet()
    {
        ifstream file("sheet.txt", ios::in);
        if (!file.is_open())
        {
            cerr << "Error opening file for reading" << endl;
            return;
        }

        Cell* tempCell = nullptr;
        Cell* tempRow = nullptr;

        string line;
        while (getline(file, line))
        {
            vector<string> cellValues;
            splitString(line, cellValues, ',');

            while (!cellValues.empty())
            {
                Cell* cellElement = new Cell(cellValues.front()); // Creating the first element of the row
                cellValues.erase(cellValues.begin());             // Erasing the first value

                if (tempCell == nullptr && tempRow == nullptr)
                {
                    tempCell = cellElement;
                    tempRow = tempCell;
                    current = tempCell;
                    firstCell = tempCell;
                }
                else if (tempCell == nullptr && tempRow != nullptr)
                {
                    tempCell = cellElement;
                    tempRow->down = cellElement;
                    cellElement->up = tempRow;
                    tempRow = tempRow->down;
                }
                else
                {
                    tempCell->right = cellElement;
                    cellElement->left = tempCell;
                    tempCell = tempCell->right;
                }
            }
            tempCell = nullptr;
        }

        // Connect rows and columns
        Cell* conec1 = firstCell;
        Cell* conec2 = firstCell->down;

        Cell* tempConec1 = conec1;
        Cell* tempConec2 = conec2;

        while (conec2 != nullptr)
        {
            tempConec1 = conec1;
            tempConec2 = conec2;
            while (tempConec2 != nullptr)
            {
                tempConec1->down = tempConec2;
                tempConec2->up = tempConec1;
                tempConec1 = tempConec1->right;
                tempConec2 = tempConec2->right;
            }
            conec1 = conec2;
            conec2 = conec2->down;
        }

        file.close();
    }

    // Function to split a string into a vector based on a delimiter
    void splitString(const string& inputString, vector<string>& outputVector, char delimiter)
    {
        string token;
        for (char c : inputString)
        {
            if (c == delimiter)
            {
                outputVector.push_back(token);
                token.clear();
            }
            else
            {
                token += c;
            }
        }
        if (!token.empty())
        {
            outputVector.push_back(token);
        }
    }

    // Function to parse a specific element from a comma-separated string
    int parsing(const string& line, int num)
    {
        int count = 1;
        string temp;
        for (char c : line)
        {
            if (c == ',')
            {
                count++;
            }
            else if (count == num)
            {
                temp += c;
            }
        }
        return stoi(temp);
        //try
        //{
        //    return stoi(temp);
        //}
        //catch (const invalid_argument& e)
        //{
        //    cerr << "Invalid argument: " << e.what() << endl;
        //    // Handle the error, possibly by returning a special value or throwing a different exception.
        //    return string(); // Return a default value for T
        //}
    }
};

int main()
{
    Excel excel;
    excel.LoadSheet();
    excel.printWelcomeBanner();
    _getch();
    system("cls");
    Sleep(500);
    excel.printWelcomeBanner();
    excel.DisplayMenu();
    Sleep(500);
    _getch();
    bool running = true;

    while (running)
    {
        Sleep(50);
        // Move current cell in the Excel sheet using arrow keys
        if (GetAsyncKeyState(VK_UP) && 1)
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.MoveCurrentToUp();
            excel.PrintSheet();
        }

        if (GetAsyncKeyState(VK_DOWN))
        {

            system("cls");
            excel.printWelcomeBanner();
            excel.MoveCurrentToDown();
            excel.PrintSheet();
        }

        if (GetAsyncKeyState(VK_LEFT))
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.MoveCurrentToLeft();
            excel.PrintSheet();
        }

        if (GetAsyncKeyState(VK_RIGHT))
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.MoveCurrentRight();
            excel.PrintSheet();
        }

        // Handle key combinations for different functions using Ctrl key

        if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_UP)) // funtion1
        {

            system("cls");
            excel.printWelcomeBanner();
            excel.InsertRowAbove();
            excel.PrintSheet();
        }
        // Funtion2
        if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_DOWN))
        {
            // Shift + Up Arrow: Insert row below
            system("cls");
            excel.printWelcomeBanner();
            excel.InsertRowBelow();
            excel.PrintSheet();
        }
        // funtion3
        if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_RIGHT))
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.InsertColumnToRight();
            excel.PrintSheet();
        }
        // funtion4
        if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_LEFT))
        {

            system("cls");
            excel.insertColumnToLeft();
            excel.printWelcomeBanner();
            excel.PrintSheet();
        }
        // funtion5
        if (GetAsyncKeyState(VK_SHIFT) & GetAsyncKeyState('R'))
        {

            system("cls");
            excel.printWelcomeBanner();
            excel.InsertCellByRightShift();
            excel.PrintSheet();
        }
        // funtion6
        if (GetAsyncKeyState(VK_SHIFT) & GetAsyncKeyState('D'))
        {

            system("cls");
            excel.printWelcomeBanner();
            excel.InsertCellByDownShift();
            excel.PrintSheet();
        }
        // funtion7
        if (GetAsyncKeyState(VK_DELETE) & GetAsyncKeyState('D'))
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.DeleteCellByLeftShift();
            excel.PrintSheet();
        }
        // funtion8
        if (GetAsyncKeyState(VK_DELETE) & GetAsyncKeyState('U'))
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.DeleteCellByUpShift();
            excel.PrintSheet();
        }
        // funtion9
        if (GetAsyncKeyState(VK_DELETE) & GetAsyncKeyState('C'))
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.DeleteColumn();
            excel.PrintSheet();
        }
        // funtion10
        if (GetAsyncKeyState(VK_DELETE) & GetAsyncKeyState('R'))
        {

            system("cls");
            excel.printWelcomeBanner();
            excel.DeleteRow();
            excel.PrintSheet();
        }
        // funtion11
        if (GetAsyncKeyState(VK_TAB) & GetAsyncKeyState('C'))
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.clearColumn();
            excel.PrintSheet();
        }
        // funtion12
        if (GetAsyncKeyState(VK_TAB) & GetAsyncKeyState('R'))
        {

            system("cls");
            excel.printWelcomeBanner();
            excel.clearRow();
            excel.PrintSheet();
        }

        // funtion13
        if (GetAsyncKeyState(VK_SHIFT) & GetAsyncKeyState('S'))
        {
            int startRow, startCol, endRow, endCol, destinationRow, destinationCol;

            // Input for the starting cell row and column
            cout << "Enter starting cell row: ";
            cin >> startRow;

            cout << "Enter starting cell column: ";
            cin >> startCol;

            // Input for the ending cell row and column
            cout << "Enter ending cell row: ";
            cin >> endRow;

            cout << "Enter ending cell column: ";
            cin >> endCol;

            // Input for the destination cell row and column
            cout << "Enter destination cell row: ";
            cin >> destinationRow;

            cout << "Enter destination cell column: ";
            cin >> destinationCol;

            // Call the GetRangeSum function with the specified cells
            excel.GetRangeSum(excel.GetCell(startRow, startCol), excel.GetCell(endRow, endCol), excel.GetCell(destinationRow, destinationCol));

            // Clear the console
            system("cls");

            // Print the updated sheet
            excel.PrintSheet();
        }

        // funtion14
        if (GetAsyncKeyState(VK_SHIFT) & GetAsyncKeyState('V'))
        { // Input for the starting cell row and column
            int startRow = excel.TakeInputForCell("Enter starting cell row: ");
            int startCol = excel.TakeInputForCell("Enter starting cell column: ");

            // Input for the ending cell row and column
            int endRow = excel.TakeInputForCell("Enter ending cell row: ");
            int endCol = excel.TakeInputForCell("Enter ending cell column: ");

            // Input for the destination cell row and column
            int destinationRow = excel.TakeInputForCell("Enter destination cell row: ");
            int destinationCol = excel.TakeInputForCell("Enter destination cell column: ");

            // Call the GetRangeSum function with the specified cells
            excel.GetRangeAverage(excel.GetCell(startRow, startCol), excel.GetCell(endRow, endCol), excel.GetCell(destinationRow, destinationCol));

            // Clear the console
            system("cls");

            // Print the updated sheet
            excel.PrintSheet();
        }
        // funtion15
        if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState('W'))
        {
            // copy funtion
            // Input for the starting cell row and column
            int startRow = excel.TakeInputForCell("Enter starting cell row: ");
            int startCol = excel.TakeInputForCell("Enter starting cell column: ");

            // Input for the ending cell row and column
            int endRow = excel.TakeInputForCell("Enter ending cell row: ");
            int endCol = excel.TakeInputForCell("Enter ending cell column: ");

            // Call the GetRangeSum function with the specified cells
            excel.Copy(excel.GetCell(startRow, startCol), excel.GetCell(endRow, endCol));
            // Clear the console
            system("cls");

            // Print the updated sheet
            excel.PrintSheet();
        }
        // funtion16
        if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState('X'))
        {

            // cut funtion

            // Input for the starting cell row and column
            int startRow = excel.TakeInputForCell("Enter starting cell row: ");
            int startCol = excel.TakeInputForCell("Enter starting cell column: ");

            // Input for the ending cell row and column
            int endRow = excel.TakeInputForCell("Enter ending cell row: ");
            int endCol = excel.TakeInputForCell("Enter ending cell column: ");

            // Call the GetRangeSum function with the specified cells
            excel.Cut(excel.GetCell(startRow, startCol), excel.GetCell(endRow, endCol));
            // Clear the console
            system("cls");

            // Print the updated sheet
            excel.PrintSheet();
        }
        // funtion17
        if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState('V'))
        {

            // paste funtion
            excel.Paste();
            system("cls");
            excel.PrintSheet();
        }

        if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState('S'))
        {

            excel.SaveSheet();
        }

        if (GetAsyncKeyState(VK_INSERT))
        {
            system("cls");
            excel.printWelcomeBanner();
            excel.ChageCellData();
            excel.PrintSheet();
        }

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            break;
        }

        if (GetAsyncKeyState('Q'))
        {
            system("cls");
            excel.DisplayMenu();
        }

        Sleep(200);
    }

    return 0;
}

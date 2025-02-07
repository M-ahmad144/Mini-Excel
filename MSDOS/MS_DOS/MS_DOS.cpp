
#include <iostream>
#include <string>
#include <queue>
#include <list>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <functional>
using namespace std;

class File
{
public:
    string FileName;
    string extension;
    string content;
    int size;
    string path;

    File(string name, string extension, string content, string path = "")
    {
        this->FileName = name;
        this->extension = extension;
        this->content = content;
        this->size = content.length();
    }
};
class Folder
{
public:
    string name; // Attribute for folder name
    string owner;
    /*
       >.storing objects directly within the lists (list<Folder> and list<File>) indicates that each Folder object can contain other Folder objects (subfolders) and File objects (files) directly as part of its structure.

       >.list<Folder*> subfolders; stores addresses (pointers) to subdirectories (folders). Similar to using node pointers in trees, when implementing functionality like the "change directory" command (cd), you'll navigate through subdirectories using these pointers. Following pointers in this list allows you to move into and manipulate the contents of subdirectories efficiently within the directory structure.

       >.In the context of list<Folder*>, the use of pointers (*) is related to storing memory addresses rather than the objects themselves.

       >.Insertion and Deletion: Lists perform better than vectors when it comes to frequent insertions and deletions at arbitrary positions within the container. Unlike vectors, lists don't require shifting elements after an insertion or deletion operation. If your application involves a lot of insertions and deletions in the middle of the container, a list might be more efficient.

    */
    list<Folder*> subFolders; // List to hold pointers to subfolders (folders within this folder)
    list<File*> files;        // List to hold pointers to files in this folder
    Folder* parent;            // Pointer to parent folder (if applicable)
    time_t creationTime;       // Timestamp indicating folder creation time
    string path;               // Path of the folder
    bool hidden;

public:
    // Constructor for the Folder class with default parameter values
    Folder(string name = "NULL",                     // Parameter: name of the folder (default: "NULL")
        string path = "",                         // Parameter: path of the folder (default: empty string)
        Folder* parent = nullptr,                 // Parameter: pointer to parent folder (default: nullptr)
        list<File*> files = list<File*>(),      // Parameter: list of files in the folder (default: empty list)
        list<Folder*> folders = list<Folder*>() // Parameter: list of subfolders (default: empty list)
    )
    {
        this->name = name;          // Initializing folder name
        this->subFolders = folders; // Initializing subfolders
        this->files = files;        // Initializing files
        this->path = path;          // Initializing folder path
        this->parent = parent;      // Initializing parent folder
        this->hidden = false;       // Setting hidden flag to false by default
    }

    void _removeFile(string& name)
    {
        for (auto it = files.begin(); it != files.end(); ++it)
        {
            if ((*it)->FileName == name)
            {
                delete* it;      // assuming File objects are dynamically allocated
                files.erase(it); // Remove the file from the list using the iterator
                break;           // Exit loop after the file is found and removed
            }
        }
    }

    // Function to remove a folder  from the 'subFolders' list in the Folder class
    void _removeFolder(string& name) // O(n)
    {
        for (auto it = this->subFolders.begin(); it != this->subFolders.end();)
        {
            // Check if the current folder's name matches the provided name
            if ((*it)->name == name)
            {
                delete* it;                      // Deallocate memory of the folder object (if dynamically allocated)
                it = this->subFolders.erase(it); // Remove the folder from the list and update the iterator
            }
            else
            {
                ++it; // Move to the next folder in the list
            }
        }
    }

    // Empty DIR
    void _EmptyDir() // O(1)
    {
        this->files.resize(0);
        this->subFolders.resize(0);
    }

    // funtion to find files
    File* _FindFile(string name) // O(n)
    {
        for (auto it = this->files.begin(); it != this->files.end(); ++it)
        {
            if ((*it)->FileName == name)
            {
                return *it; // Return the found File pointer
            }
        }
        return nullptr; // Return nullptr if the file with the specified name is not found
    }

    Folder* _FindFolder(string name) // O(n)
    {
        for (auto it = this->subFolders.begin(); it != this->subFolders.end(); ++it)
        {
            if ((*it)->name == name)
            {
                return *it; // Return the found File pointer
            }
        }
        return nullptr; // Return nullptr if the file with the specified name is not found
    }

    // funtion for insert files
    void _InsertFiles(File* file)
    {
        this->files.push_back(file);
    }

    // funtion for insert folders
    void _InsertFolders(Folder* folder)
    {
        this->subFolders.push_back(folder);
    }

    // funtion to print Directories
    void _displayDirectory()
    {
        cout << "Directory of " << this->path << "\\" << this->name << "\n\n";

        for (auto it = this->subFolders.begin(); it != this->subFolders.end(); ++it)
        {
            cout << "\t\t<DIR>\t" << (*it)->name << endl
                << endl;
        }

        for (auto it = this->files.begin(); it != this->files.end(); ++it)
        {
            cout << "\t\t\t\t\t" << (*it)->FileName + "." << (*it)->extension << "\t" << (*it)->content << endl
                << endl;
        }
    }

    string _GetPath()
    {
        return path;
    }
};
#include <iostream>
#include<conio.h>
#include<fstream>
#include<Windows.h>
#include<string>
#include<string.h>
#include<list>
#include<stack>
#include<deque>
#include<algorithm>
#include <vector>
using namespace std;




struct state
{
    int row;
    int column;
    list<list<char>> text;
    list<list<char>>::iterator rIt;
    list<char>::iterator colIt;
};

class TextEditor
{
public:
    int cRow;
    int cColumn;
    list<list<char>> text;
    list<list<char>>::iterator rIt;
    list<char>::iterator colIt;
    deque<state> undo;
    stack<state> redo;

    TextEditor()
    {
        text.push_back(list<char>());
        rIt = text.begin();
        (*rIt).push_back(' ');
        colIt = (*rIt).begin();
        cRow = 0;
        cColumn = 0;
    }

    state SaveState()
    {
        state* s = new state();
        s->text.push_back(list<char>());
        auto iterRow = s->text.begin();
        for (auto row = text.begin(); row != text.end(); row++, iterRow++)
        {
            s->text.push_back(list<char>());
            for (auto col = (*row).begin(); col != (*row).end(); col++)
            {
                char ch = *col;
                (*iterRow).push_back(ch);
            }
        }
        s->rIt = s->text.begin();
        s->colIt = (*s->rIt).begin();

        s->column = cColumn;
        s->row = cRow;

        return *s;
    }

    void LoadState(state s)
    {
        text = s.text;
        rIt = text.begin();
        cColumn = s.column;
        cRow = s.row;

        for (int i = 0; i < s.row; i++)
        {
            rIt++;
        }
        colIt = (*rIt).begin();

        for (int i = 0; i < s.column; i++)
        {
            colIt++;
        }

    }

    /*void Input(ifstream& rdr)
    {
        string fName;
        rdr >> fName;

        while (!rdr.eof())
        {
            files.push_back(fName);
            rdr >> fName;
        }
    }*/

    //void Create_File() // new file
    //{
    //    system("cls");
    //    string fN;
    //    cout << "Enter file name: ";
    //    cin >> fN;

    //    if (find(files.begin(), files.end(), fN) != files.end())
    //    {
    //        cout << "File already exist";
    //        return;
    //    }

    //    files.push_back(fN);


    //    ofstream wrt(fN.c_str());


    //    system("cls");
    //    EditFile(wrt);
    //    system("cls");
    //    system("color 09");
    //    wrt.close();

    //}

    void WriteToFile(ofstream& wrt)
    {
        for (auto r = text.begin(); r != text.end(); r++)
        {
            for (auto c = (*r).begin(); c != (*r).end(); c++)
            {
                wrt << *c;
            }
            wrt << '\n';
        }
    }

    //void Open_File() //ex file
    //{
    //    system("cls");
    //    string fname;
    //    cout << "Enter file name: ";
    //    cin >> fname;


    //    if (find(files.begin(), files.end(), fname) == files.end())
    //    {
    //        cout << "File does not exist\n";
    //        return;
    //    }

    //    ifstream rdr(fname.c_str());
    //    OpenSavedFile(rdr);
    //    rdr.close();

    //    ofstream write(fname.c_str());
    //    system("cls");
    //    print();
    //    gotoxy(cColumn, cRow);
    //    EditFile(write);
    //    system("cls");
    //    system("color 09");
    //}



    void OpenSavedFile(ifstream& rdr)
    {
        char ch;

        while (!rdr.eof())
        {
            rdr.get(ch);
            if (ch != '\n')
            {
                (*rIt).push_back(ch);
            }
            else
            {
                text.push_back(list<char>());
                rIt++;
            }
        }
        rIt = text.begin();
        colIt = (*rIt).begin();
        cRow = 0;
        cColumn = 0;
        gotoxy(cColumn, cRow);
    }

    void EditFile(ofstream& wrt)
    {
        system("color F0");
        char c;
        c = _getch();
        gotoxy(cColumn, cRow);
        cout << c;

        (*colIt) = c;
        cColumn++;

        while (true)
        {
            if (cRow == 0)
            {
                gotoxy(cColumn, cRow);
            }
            else
            {

                gotoxy(cColumn + 1, cRow);
            }


            c = _getch();

            if (c == -32)
            {
            A:
                c = _getch();

                if (c == 72) // up key
                {
                    if (cRow == 0)
                    {
                        continue;
                    }

                    rIt--;
                    colIt = (*rIt).begin();
                    cRow--;
                    cColumn = 0;
                }

                if (c == 80) // down key
                {

                    rIt++;
                    colIt = (*rIt).begin();
                    cRow++;
                    cColumn = 0;

                }

                if (c == 75)//Left key
                {
                    if (cColumn - 1 > 0)
                    {
                        colIt--;
                        cColumn--;
                    }
                }

                if (c == 77)//Right key
                {



                    colIt++;
                    cColumn++;

                }

                else if (c == 83)//Del key
                {
                    auto temp = colIt;
                    colIt++;
                    (*rIt).erase(colIt);
                    colIt = temp;
                    system("cls");
                    print();
                    UpdateUndo();
                }
                if (cColumn == 0)
                {
                    gotoxy(0, cRow);
                    c = _getch();
                    if (c == -32)
                    {
                        goto A;
                    }
                    (*rIt).push_front(c);
                    colIt = (*rIt).begin();
                    cColumn = 1;
                    system("cls");
                    print();
                    UpdateUndo();
                }
                continue;

            }

            else if (c == 13) // Enter
            {
                list<char>data;
                int size = 0;
                auto temp = colIt;
                if (++colIt != (*rIt).end())
                {
                    colIt = temp;
                    colIt++;
                    for (auto it = colIt; it != (*rIt).end(); it++)
                    {
                        if ((*it) == ' ' && ++it == (*rIt).end())
                        {
                            continue;
                        }
                        data.push_back(*it);
                        size++;
                    }
                    list<char>::iterator endIter = next(colIt, size);
                    (*rIt).erase(colIt, endIter);

                    auto temp = rIt;
                    rIt++;
                    text.insert(rIt, list<char>());
                    rIt = ++temp;
                    cRow++;
                    cColumn = 0;

                    (*rIt).push_back(data.front());
                    data.pop_front();
                    colIt = (*rIt).begin();
                    while (!data.empty())
                    {
                        auto temp = colIt;
                        colIt++;
                        (*rIt).insert(colIt, data.front());
                        colIt = ++temp;
                        cColumn++;

                        data.pop_front();
                    }
                }
                else
                {

                    auto temp = rIt;
                    rIt++;
                    text.insert(rIt, list<char>());
                    rIt = ++temp;
                    cRow++;
                    cColumn = 0;

                    c = _getch();
                    if (c == -32)
                    {
                        goto A;
                    }
                    (*rIt).push_back(c);

                    colIt = (*rIt).begin();
                    gotoxy(cColumn, cRow);
                    system("cls");
                    print();
                    UpdateUndo();
                    continue;
                }

            }

            else if (c == 8)// backspace
            {
                if (cColumn == 0)
                {
                    gotoxy(0, cRow);
                    c = _getch();
                    if (c == -32)
                    {
                        goto A;
                    }

                    if (c == 8)
                    {
                        continue;
                    }

                    (*colIt) = c;
                    colIt = (*rIt).begin();
                    cColumn = 1;
                    continue;
                }
                auto temp = --colIt;
                colIt++;
                (*rIt).erase(colIt);
                colIt = temp;
                cColumn--;
                system("cls");
                print();
                UpdateUndo();
                continue;
            }

            if (GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState('Z'))//Undo
            {
                if (!undo.empty())
                {
                    state s = undo.back();
                    LoadState(s);
                    redo.push(undo.back());
                    undo.pop_back();
                    system("cls");
                    print();
                }
                continue;
            }

            else if (GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState('Y'))//redo
            {
                if (!redo.empty())
                {
                    undo.push_back(redo.top());
                    state s = redo.top();
                    redo.pop();
                    LoadState(s);

                    system("cls");
                    print();
                }
                continue;

            }

            else if (c == 27)//esc to exit
            {
                WriteToFile(wrt);
                break;

            }

            if (cColumn == 100)
            {
                text.push_back(list<char>());
                rIt++;
                cColumn = 0;
                cRow++;
                (*rIt).push_back(c);
                colIt = (*rIt).begin();
            }
            else
            {
                auto temp = colIt;
                colIt++;
                (*rIt).insert(colIt, c);
                colIt = ++temp;
                cColumn++;
            }

            system("cls");
            print();
            UpdateUndo();
        }
        system("cls");

    }

    void UpdateUndo()
    {
        if (undo.size() > 5)
        {
            undo.erase(undo.begin());
        }

        state s = SaveState();
        undo.push_back(s);
    }

    void print()
    {
        for (auto r = text.begin(); r != text.end(); r++)
        {
            for (auto c = (*r).begin(); c != (*r).end(); c++)
            {
                cout << *c;
            }
            cout << endl;
        }
    }

   /* void Closing(ifstream& read)
    {

        read.close();
        ofstream write;
        write.open("SaveFile.txt");

        for (auto i = files.begin(); i != files.end(); i++)
        {
            write << (*i) << endl;
        }
        write.close();
    }*/

    /*int choice(ifstream& read)
    {
        system("cls");
        int op = -1;
        int r = 0;
        int c = 0;

        gotoxy(10, 6);
        cout << "New File" << endl;

        gotoxy(10, 9);
        cout << "Open File" << endl;

        gotoxy(10, 12);
        cout << "Exit" << endl;

        cin >> op;

        if (op == 1)
        {
            Create_File();
        }
        if (op == 2)
        {
            Open_File();
        }
        if (op == 3)
        {
            Closing(read);
        }
        return op;
    }*/

    void gotoxy(int x, int y)
    {
        COORD coordinates;
        coordinates.X = x;
        coordinates.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
    }
};



class Tree
{
public:
    Folder* root;
    Folder* currentFolder; // Represents the current folder
    // Used for the 'cd' command to navigate within the current directory
    // like we use current cell in mini excel

public:
    Tree()
    {
        root = currentFolder = new Folder("v");
        // Initially, both root and current folder point to the same folder'v'
        // Other parameters are set to default
    }

    // Function to print the path of the current folder with a specified prompt
    void _displayDirectoriesPath(string prompt = "\\>")
    {
        if (currentFolder->name == "v")
            cout << "v:" << prompt; // Output 'v:' followed by the prompt if the current folder name is 'v'
        else
            cout << "v:" << currentFolder->path.substr(2, currentFolder->path.length()) << "\\" << currentFolder->name << prompt;
        // Output 'v:' followed by the substring of the current folder's path starting from the third character
        // Then append the current folder's name and the specified prompt
    }

    // funtion to check if folder exists
    bool _IsFolderExists(string name)
    {
        for (auto it = this->currentFolder->subFolders.begin(); it != this->currentFolder->subFolders.end(); ++it)
        {
            if ((*it)->name == name)
            {
                return true;
            }
        }
        return false;
    }

    // funtion to check if File exists
    bool _IsFileExists(string name)
    {
        for (auto it = this->currentFolder->files.begin(); it != this->currentFolder->files.end(); ++it)
        {
            if ((*it)->FileName == name)
            {
                return true;
                break;

            }
        }
        return false;
    }
};
class DOS
{
public:

    bool prompt = false;
    Tree tree;
    TextEditor editor;
    string _user = "AHMAD";

    DOS() {}

    // Function to print the directories
    void print()
    {
        cout << "\t\t\t\t\t"
            << "MUHAMMAD AHMAD | 2022-CS-144 | Muhammad Tayyab| 2022-CS-135 \n"
            << endl;
        tree._displayDirectoriesPath();
    }



    // funtion to convert string in lower case
    string ConvertInLowerCase(string str)
    {
        for (int i = 0; i < str.length(); i++)
        {
            str[i] = tolower(str[i]);
        }
        return str;
    }

    // Function to handle the program's execution
    void _Run()
    {
        print();
        bool exit = false;
        // Loop runs until the 'exit' command becomes true
        while (!exit)
        {
            exit = _takeInput(); // Takes user input to check for the 'exit' command
        }
    }

    // Function to take user input
    bool _takeInput()
    {
        string command;
        getline(cin, command); // Reads the user input as a command (e.g., "copy v/f1/f2")

        // start from 0 index (c)  to until " "character
        // Extracts the operation (e.g., "copy") from the command until the first space character
        string opr = command.substr(0, command.find(' '));
        opr = ConvertInLowerCase(opr);

        if (opr == "attrib") // 1
        {

            // format attrib <filename>.<extension>
            string name = command.substr(opr.length() + 1);
            size_t pos_dot = name.find('.');

            if (pos_dot != string::npos)
            {
                string file_name = name.substr(0, pos_dot);
                string file_extension = name.substr(pos_dot + 1);

                attrib(file_name, file_extension, tree.currentFolder);
            }
            else
            {
                cout << "Invalid file name format." << endl;
            }
        }

        else if (opr == "cd") // 2
        {
            // format cd <foldername>
            string folder = command.substr(opr.length() + 1, command.length());
            if (tree._IsFolderExists(folder))
            {
                tree.currentFolder = tree.currentFolder->_FindFolder(folder);
            }
            else
            {
                cout << "No such directory exits !" << endl;
            }
        }

        else if (opr == "cd.") // 3
        {
            // format cd.
            //  Print the current directory path

            tree.currentFolder->_displayDirectory(); // Pass "\n" as the prompt to end the line after displaying the path
        }
        else if (opr == "cd..") // 4
        {
            // format cd..
            if (tree.currentFolder->parent != nullptr)
            {
                tree.currentFolder = tree.currentFolder->parent;
            }
        }

        else if (opr == "cd\\") // 5
        {
            // format cd
            if (tree.currentFolder->parent != nullptr)
            {
                tree.currentFolder = tree.root;
            }
        }

        else if (opr == "convert") // 6
        {
            // format convert <type1> <type2>

            size_t firstSpacePos = command.find(" ");
            if (firstSpacePos != string::npos) {
                string type1 = command.substr(opr.length() + 1, 4);

                size_t secondSpacePos = command.find(" ", firstSpacePos + 1);
                string type2 = (secondSpacePos != string::npos) ?
                    command.substr(secondSpacePos + 1) :
                    command.substr(firstSpacePos + 1);

                _CONVERT(type1, type2);
            }
            else {
                // Handle invalid command format
                cout << "Invalid command format for conversion." << endl;
            }
        }


        else if (opr == "edit")
        {
                string name = command.substr(opr.length() + 1);
           
                 bool flag = editFile(name);
                if (flag == true)
                {
                    system("cls");
                }
                if (flag == false)
                {
                    cout << "File does not exit" << endl;
                }

            

        }

            

            else if (opr == "copy")
            {

                // format copy <filename>.<extension> <directory>
                string name = command.substr(opr.length() + 1, command.find(".") - 5);
                command = command.substr(command.find(".") + 1);
                string extension = command.substr(0, command.find(" "));
                string directoryName = command.substr(command.find(" ") + 1);

                copy(name, extension, directoryName);
            }

            else if (opr == "create") // 8
            {
                // format create <filename>.<extension> <content>
                // Extracting the arguments from the command
                string args = command.substr(opr.length() + 1); // Extracting arguments after "create"

                // Find the space after the file name and extension
                size_t pos_space1 = args.find(' ');

                if (pos_space1 != string::npos)
                {
                    string fileDetails = args.substr(0, pos_space1); // Extracting file name and extension
                    string content = args.substr(pos_space1 + 1);    // Extracting file content

                    // Find the dot that separates file name and extension
                    size_t pos_dot = fileDetails.find('.');
                    if (pos_dot != string::npos)
                    {
                        string name = fileDetails.substr(0, pos_dot);       // Extracting file name
                        string extension = fileDetails.substr(pos_dot + 1); // Extracting file extension

                        // Create the file using the createFile function
                        createFile(name, extension, content);
                    }
                    else
                    {
                        cout << "Invalid file name format." << endl;
                    }
                }
                else
                {
                    cout << "Invalid command format for creating a file." << endl;
                }
            }

              else if (opr == "del")
             {
                // format del <filename>
                string name = command.substr(opr.length() + 1, command.find("."));
                tree.currentFolder->_removeFile(name);
              }

            else if (opr == "dir") // 10
            {
                // format dir
                tree.currentFolder->_displayDirectory();
            }

            // EDIT()//11

            else if (opr == "exit") // 12
            {
                // format exit
                return true;
            }

            else if (opr == "find") // 13
            {
                // format find <filename> 
                string name = command.substr(opr.length() + 1, command.length());
                bool find = tree._IsFileExists(name);
                if (find)
                {
                    cout << "File Present in the folder" << endl;
                }
                // File *file = tree.currentFolder->_FindFile(name);
                // cout << file->content;
            }

            else if (opr == "findf") // 14
            {
                // format findf <text> <filename>
                size_t textStart = command.find(" ") + 1;         // Find the start of the text
                size_t fileNameStart = command.find_last_of(" "); // Find the start of the filename
                string text = command.substr(textStart, fileNameStart - textStart);
                string fileName = command.substr(fileNameStart + 1);

                findStringInGivenFiles(text, fileName);
            }

            else if (opr == "findstr") // 15
            {
                // format findstr <text>
                string text = command.substr(opr.length() + 1); // Extract the text from the command
                findStringInFiles(text);
            }

            else if (opr == "format") // 16
            {
                // format format
                tree.currentFolder->_EmptyDir();

            }

            else if (opr == "help") // 17
            {
                // format help
                help();
            }

            // LoadTree();//18

            else if (opr == "mkdir") // 19
            {
                // format mkdir <foldername>
                mkdirCommand(command, tree.currentFolder);
            }

            else if (opr == "mov") // 20
            {
                // format mov <filename>.<extension> <directory>
                string name = command.substr(opr.length() + 1, command.find(".") - 4);
                command = command.substr(command.find(".") + 1);
                string extension = command.substr(0, command.find(" "));
                string directoryName = command.substr(command.find(" ") + 1);

                _move(name, extension, directoryName);
            }

            else if (command == "ppprint")
            {
                // displays the current priority queue.==> take size

                _ppprint();
            }

            else if (opr == "prompt") // 22
            {
                // format prompt
                prompt = !prompt;
            }

            // Adds a text file to the print queue, and displays the current queue.
            else if (opr == "print") // 23
            {
                size_t dotPosition = command.find(".");
                if (dotPosition != string::npos)
                {
                    string name = command.substr(opr.length() + 1, dotPosition - (opr.length() + 1));
                    string extension = command.substr(dotPosition + 1);
                    _print(name, extension);
                }
                else
                {
                    cout << "Invalid command format for printing a file." << endl;
                }
            }

            // Shows current state of the priority based print queue
            else if (command == "PQUEUE") // 24
            {
                _PQUEUE();
            }
            // Prints working directory//25
            else if (opr == "pwd")
            {
                _PWD();
            }
            else if (opr == "cls")
            {
                // format cls
                system("cls");
                print();
            }

            // QUEUE();//26 Shows current state of the print queue, with time left for each element
            else if (opr == "queue")
            {
                _QUEUE();
            }
            else if (opr == "rename") // 27
            {
                // format rename <currentname>.<extension> <newname>
                command = command.substr(opr.length() + 1); // extract rename

                // Find the first space to separate the current name and the new name
                size_t pos_space = command.find(' ');

                if (pos_space != string::npos) // means until the end of the string.
                {
                    string Currentname = command.substr(0, pos_space); // extract current name

                    // Extract the new name starting from the space after the current name
                    string newName = command.substr(pos_space + 1); // new name

                    // Find the dot that separates the file name and the extension
                    size_t pos_dot = Currentname.find('.');

                    if (pos_dot != string::npos) // until the end of the string
                    {
                        string file_name = Currentname.substr(0, pos_dot);  // Extracting file name
                        string extension = Currentname.substr(pos_dot + 1); // Extracting file extension

                        File* file = tree.currentFolder->_FindFile(file_name);
                        if (file != nullptr)
                        {
                            file->FileName = newName;
                        }
                        else
                        {
                            cout << "File not found." << endl;
                        }
                    }
                    else
                    {
                        cout << "Invalid file name format." << endl;
                    }
                }
                else
                {
                    cout << "Invalid command format for renaming a file." << endl;
                }
            }

            else if (opr == "rmdir") // 28
            {
                // format rmdir
                Folder* current = tree.currentFolder;
                tree.currentFolder = tree.currentFolder->parent;
                tree.currentFolder->_removeFolder(current->name);
            }

            else if (opr != "cls")
            {
                if (prompt)
                    tree._displayDirectoriesPath("$");
                else
                    tree._displayDirectoriesPath();
            }

            // SAVE(); Saves the currently open file to disk.//29

            // TREE Displays the complete directory structure.//30
            if (opr == "tree")
            {
                // cout << "TREE" << endl;
                TREE(tree.currentFolder);
            }

            // VER Displays the version of your program.
            if (opr == "ver") // 31
            {
                cout << "Muhammad Ahmad Windows 10";
            }

            return false;
            }

            // Funtions   ...................................................................................////
            bool mkdirCommand(string & command, Folder * currentFolder)
            {
                // Extract folder name from the command
                string folderName = command.substr(6, command.length()); // Assuming "mkdir " is 6 characters long

                // Implement folder creation logic
                currentFolder->_InsertFolders(
                    new Folder(
                        folderName,
                        currentFolder->path + "\\" + currentFolder->name,
                        currentFolder));

                return true; // Return appropriate boolean value indicating success/failure
            }

            void navigateToRoot(Tree & tree)
            {
                tree.currentFolder = tree.root;
            }

            // Funtion to copy file

            void copy(string name, string extension, string directoryName)
            {
                File* fileToCopy = nullptr;
                for (auto file : tree.currentFolder->files)
                {
                    if (file->FileName == name && file->extension == extension)
                    {
                        fileToCopy = file;
                        break;
                    }
                }
                if (fileToCopy != nullptr)
                {
                    for (auto dir : tree.currentFolder->subFolders)
                    {
                        if (dir->name == directoryName)
                        {
                            dir->files.push_back(fileToCopy);
                            break;
                        }
                    }
                }
            }

            // Function to create a file
            void createFile(string name, string extension, string content)
            {
                bool exists = false;

                for (auto f : tree.currentFolder->files)
                {
                    if (f->FileName == name && f->extension == extension)
                    {
                        exists = true;
                        break;
                    }
                }

                if (!exists)
                {
                    File* file = new File(name, extension, content, tree.currentFolder->_GetPath() + name + "." + extension);
                    tree.currentFolder->_InsertFiles(file);
                }
                else
                {
                    cout << "File with the same name and extension already exists." << endl;
                }
            }

            bool editFile(string fileName)
            {

                File* file = tree.currentFolder->_FindFile(fileName);
                if (file != nullptr)
                {
                    ofstream write(fileName.c_str());
                    editor.EditFile(write);

                }
                else
                {
                    return false;
                }

            }

            // Function to display file attributes
            void displayFileAttributes(File * file)
            {
                cout << file->FileName << "." << file->extension << endl;
                cout << "Size of File: " << file->size << endl;
                cout << "Content of File: " << file->content << endl;
            }

            // attrib function
            void attrib(string name, string extension, Folder * currentDirectory)
            {
                for (auto file : currentDirectory->files)
                {
                    if (file->FileName == name && file->extension == extension)
                    {
                        displayFileAttributes(file);
                        break;
                    }
                }
            }

            // Asks two types and converts extension of all files of one type to another type.
            void _CONVERT(string type1, string type2)
            {
                for (auto file : tree.currentFolder->files)
                {
                    if (file->extension == type1)
                    {
                        file->extension = type2;
                    }

                    if (file->extension == type2)
                    {
                        file->extension = type1;
                    }
                }
            }

            void help()
            {
                cout << "attrib <filename>.<extension>" << endl;
                cout << "cd <directory>" << endl;
                cout << "cd." << endl;
                cout << "cd.." << endl;
                cout << "cd\\" << endl;
                cout << "convert <type1> <type2>" << endl;
                cout << "copy <filename>.<extension> <directory>" << endl;
                cout << "create <filename>.<extension> <content>" << endl;
                cout << "del <filename>.<extension>" << endl;
                cout << "dir" << endl;
                cout << "edit <filename>.<extension> <content>" << endl;
                cout << "exit" << endl;
                cout << "find <filename>" << endl;
                cout << "findf <filename>.<extension> <text>" << endl;
                cout << "findSTR <text>" << endl;
                cout << "format" << endl;
                cout << "help" << endl;
                cout << "loadTree" << endl;
                cout << "mkdir <directory>" << endl;
                cout << "move <filename>.<extension> <directory>" << endl;
                cout << "PPPrint" << endl;
                cout << "prompt" << endl;
                cout << "print <filename>.<extension>" << endl;
                cout << "PQUEUE" << endl;
                cout << "PWD" << endl;
                cout << "QUEUE" << endl;
                cout << "rename <filename>.<extension> <newfilename>.<newextension>" << endl;
                cout << "RMDIR <directory>" << endl;
                cout << "SAVE" << endl;
                cout << "TREE" << endl;
                cout << "VER" << endl;
            }

            void findStringInFiles(const string & searchString)
            {
                bool found = false; // Flag to check if the string is found in any file

                for (auto file : tree.currentFolder->files)
                {
                    if (file->content.find(searchString) != string::npos)
                    {
                        cout << "STRING Found in: " << file->FileName << "." << file->extension << endl;
                        found = true;
                    }
                }

                if (!found)
                {
                    cout << "Text not found in any file." << endl;
                }
            }

            void findStringInGivenFiles(const string & searchString, const string & fileName)
            {
                bool found = false; // Flag to check if the string is found in the specified file

                for (auto file : tree.currentFolder->files)
                {
                    if (fileName == file->FileName)
                    {
                        if (file->content.find(searchString) != string::npos)
                        {
                            cout << "Yes! string present in: " << file->FileName << "." << file->extension << endl;
                            found = true;
                            break; // Exit loop once the string is found in the specified file
                        }
                    }
                }

                if (!found)
                {
                    cout << "Text not found in the specified file." << endl;
                }
            }

            void _move(string name, string extension, string folderName)
            {
                // Variable to hold the file to be moved
                File* fileToMove = nullptr;

                // Loop through each file in the current folder
                for (auto file : tree.currentFolder->files)
                {
                    // Check if the file matches the provided name and extension
                    if (file->FileName == name && file->extension == extension)
                    {
                        // Assign the file to the fileToMove variable
                        fileToMove = file;
                    }
                }

                // If a matching file is found
                if (fileToMove != nullptr)
                {
                    // Loop through each subfolder in the current folder
                    for (auto dir : tree.currentFolder->subFolders)
                    {
                        // Check if the subfolder name matches the provided folderName
                        if (dir->name == folderName)
                        {
                            // Move the file to the subfolder
                            dir->files.push_back(fileToMove);             // Add the file to the subfolder's files
                            tree.currentFolder->files.remove(fileToMove); // Remove the file from the current folder's files
                            break;                                        // Exit the loop after moving the file
                        }
                    }
                }
            }

            void _PWD()
            {

                cout << "    " << tree.currentFolder->name << endl;
            }

            // Function to add a file to the priority-based print queue based on file size
            void _ppprint()
            {
                cout << "File push in priority_queue(BASE onthe size of the file)" << endl;
                priority_queue<File*, vector<File*>, function<bool(File*, File*)>> pq([](File* a, File* b)
                    { return a->size < b->size; });

                for (auto file : tree.currentFolder->files)
                {
                    pq.push(file);
                }

                while (!pq.empty())
                {
                    File* file = pq.top();
                    pq.pop();
                    cout << file->FileName << "." << file->extension << endl;
                }
            }

            // Function to print a specific file by name and extension
            void _print(string name, string extension)
            {
                File* fileToPrint = nullptr;
                for (auto file : tree.currentFolder->files)
                {
                    if (file->FileName == name && file->extension == extension)
                    {
                        fileToPrint = file;
                        break;
                    }
                }
                if (fileToPrint != nullptr)
                {
                    cout << fileToPrint->FileName << "." << fileToPrint->extension << endl;
                }
            }

            // Function to display the current state of the priority-based print queue
            void _PQUEUE()
            {
                priority_queue<File*> pq;
                for (auto file : tree.currentFolder->files)
                {
                    pq.push(file);
                }
                while (!pq.empty())
                {
                    File* file = pq.top();
                    pq.pop();
                    cout << file->FileName << "." << file->extension << endl;
                }
            }

            void _QUEUE()
            {
                for (auto file : tree.currentFolder->files)
                {
                    cout << file->FileName << "." << file->extension << endl;
                }
            }

            void TREE(Folder * folder, int depth = 0)
            {
                if (folder == nullptr)
                    return;

                // Display the current folder name with appropriate indentation
                cout << string(depth * 4, ' ') << "|_ " << folder->name << endl;

                // Recursively display subfolders within this folder
                for (auto subFolder : folder->subFolders)
                {
                    TREE(subFolder, depth + 1);
                }
            }
        
};

int main()
{
    DOS dosSystem;
    dosSystem._Run();

    return 0;
}
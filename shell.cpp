#include <filesystem>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
// #include <curses.h>
#include <fstream>
#include <regex>

using namespace std;
namespace fs = filesystem;

const string BLUEB = "\033[1;34m";
const string BLUEE = "\033[0m";

const string REDB = "\033[1;31m";
const string REDE = "\033[0m";

const string GREENB = "\033[1;32m";
const string GREENE = "\033[0m";

const string parentPath = fs::current_path().u8string() + "/disk";

void cp(vector<string> &args) {
    string srcPath = fs::current_path().u8string();
    srcPath += "/" + args[1];
    string targetPath = fs::current_path().u8string();
    targetPath += "/" + args[2];
    cout << parentPath << endl;

    cout << targetPath << endl;
    if (args.size() < 3) {
        cout << "Too few arguments " << endl;
    } else if (srcPath.find(parentPath) == std::string::npos && targetPath.find(parentPath) == std::string::npos) {
        cout << "Error : unauthorized" << endl;
    } else if (args.size() > 2) {
        try {
            fs::copy(srcPath, targetPath, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
        } catch (std::exception &e) {
            std::cout << e.what();
            cout << "No such file or directory" << endl;
        }
    }
}

void mv(vector<string> &args) {
    if (args.size() < 3) {
        cout << "Too few arguments " << endl;
        return;
    }
    string srcPath = fs::current_path().u8string();
    srcPath += "/" + args[1];
    string targetPath = fs::current_path().u8string();
    targetPath += "/" + args[2];
    if (srcPath.find(parentPath) == std::string::npos && targetPath.find(parentPath) == std::string::npos) {
        cout << "Error : unauthorized" << endl;
    } else if (args.size() > 2) {
        try {
            fs::rename(srcPath, targetPath);
        } catch (std::exception &e) {
            std::cout << e.what();
            cout << "No such file or directory" << endl;
        }
    }
}

void split(vector<string> &args, string &token, char delimiter) {
    stringstream test(token);
    string segment;
    vector<string> seglist;

    while (getline(test, segment, delimiter)) {
        seglist.push_back(segment);
    }

    args = seglist;
}
void formatPathToUnix(string &pathToFormat) {
    pathToFormat = regex_replace(pathToFormat, regex(parentPath), "");
}
void touch(const vector<string> &args) {
    if (args.size() > 1) {
        string currentPath = fs::current_path().u8string();
        if (!fs::exists(currentPath + "/" + args[1])) {
            ofstream MyFile(currentPath + "/" + args[1]);
            MyFile << " ";
            MyFile.close();
        } else {
            cerr << "touch: cannot touch '" << args[1] << "': File exists" << endl;
        }

    } else {
        cerr << "touch: missing file operand" << endl;
    }
}

void ls() {
    string currentPath = fs::current_path().u8string();
    int i = 0;
    for (const auto &entry : fs::directory_iterator(currentPath)) {
        string elementToPrint = entry.path().filename().u8string();
        // elementToPrint = regex_replace(elementToPrint, regex(currentPath+"/"), "");
        if (fs::is_directory(entry.path()))
            cout << BLUEB << elementToPrint << BLUEE << " ";
        else
            cout << GREENB << elementToPrint << GREENE << " ";
        i++;
    }

    if (i > 0) cout << endl;
}

void mkdir(vector<string> &args) {
    if (args.size() > 1) {
        string currentPath = fs::current_path().u8string();

        fs::create_directories(currentPath + "/" + args[1] + "/");

    } else {
        cerr << "touch: missing file operand" << endl;
    }
}

int searchFile(const string &fileName, vector<vector<string>> &filesList) {
    int i = 0;
    bool found = false;

    while (i < filesList.size() && !found) {
        found = fileName == filesList[i][0];
        i++;
    }

    return (found ? i - 1 : -1);
}

void cat(const vector<string> &args) {
    if (args.size() > 1) {
        ifstream f(args[1]);
        string currentPath = fs::current_path().u8string();
        // check if file or directory
        if (!f.is_open())
            cout << "cat: " << args[1] << ": No such file or directory" << endl;
        else if (!std::filesystem::is_regular_file(currentPath + "/" + args[1]))
            cout << "cat: " << args[1] << ": Is a directory" << endl;
        else
            cout << f.rdbuf() << endl;

        f.close();

    } else {
        cerr << "nano: missing file operand" << endl;
    }
}

void wc(const vector<string> &args) {
    int i = 0;
    if (args.size() > 1) {
        string fileName;
        if (args[1] == "-l")
            fileName = args[2];
        else
            fileName = args[1];
        ifstream f(fileName);
        string currentPath = fs::current_path().u8string();
        // check if file or directory
        if (!f.is_open())
            cout << "cat: " << fileName << ": No such file or directory" << endl;
        else if (!std::filesystem::is_regular_file(currentPath + "/" + fileName))
            cout << "cat: " << fileName << ": Is a directory" << endl;
        else if (args.size() > 2 && args[1] == "-l") {
            string text;
            // Read data from the file object and put it into a string.
            while (getline(f, text)) {
                // Print the data of the string.
                i++;
            }
            cout << args[2] << " :" << i << " lines" << endl;
        } else {
            string text;
            // Read data from the file object and put it into a string.
            while (getline(f, text)) {
                // Print the data of the string.
                i += text.length();
            }
            cout << args[1] << " " << i << " words" << endl;
        }
        f.close();

    } else {
        cerr << "nano: missing file operand" << endl;
    }
}

void rm(const vector<string> &args) {
    if (args.size() > 1) {
        try {
            /* code */

            string currentPath = fs::current_path().u8string();

            fs::remove_all(currentPath + "/" + args[1]);

        } catch (const exception &e) {
            cerr << e.what() << '\n';
        }

    } else {
        cerr << "rm: missing file operand" << endl;
    }
}

void nanoCommand(const vector<string> &args) {
    if (args.size() > 1) {
        system("clear");
        cout << "NANO" << endl
             << endl;
        cout << "Enter :q to exit" << endl
             << endl;

        ifstream f(args[1]);

        if (f.is_open()) cout << f.rdbuf() << endl
                              << endl;
        f.close();
        ofstream fileToWrite(args[1], ofstream::app);

        // string fullString;
        string line;

        do {
            // fullString+=("\n"+line);
            fileToWrite << line << endl;

            getline(cin, line);

        } while (line != ":q");
        fileToWrite.close();
        // vector<string> fileTemp = {args[1], fullString};
        // filesList.push_back(fileTemp);

        // system(("echo '" + fullString + "' > disk/" + args[1]).c_str());
        // or directly write to the file using fstream
        // fstream("disk/" + args[1], ios::out | ios::trunc) << fullString;

    } else {
        cerr << "nano: missing file operand" << endl;
    }
}
string getVar(string varName) {
    bool found = false;
    ifstream f(parentPath + "/home/billal/.bashrc");

    if (f.is_open()) {
        string text;
        // Read data from the file object and put it into a string.
        while (getline(f, text)) {
            // Print the data of the string.
            vector<string> elements;
            split(elements, text, '=');

            // cout << elements[0] <<"="<<elements[1]<< endl;
            if (elements[0] == varName) return elements[1];
        }

        // Close the file object.
    }
    f.close();

    return ("");
}
void echo(vector<string> &args) {
    for (int i = 1; i < args.size(); i++) {
        if (args[i].substr(0, 1) == "$") {
            args[i] = args[i].substr(1);
            cout << getVar(args[i]) << " ";
        } else
            cout << args[i] << " ";
    }
    cout << endl;
}

void clearShell() {
    system("clear");
}

void date() {
    auto time = std::time(nullptr);
    cout
        // ISO 8601: %Y-%m-%d %H:%M:%S, e.g. 2017-07-31 00:42:00+0200.
        << put_time(gmtime(&time), "%F %T%z") << endl;
}

void createVar(vector<string> &varArgs) {
    if (varArgs[1].front() == '"') varArgs[1] = varArgs[1].substr(1);
    if (!varArgs[1].empty() && varArgs[1].back() == '"') varArgs[1].pop_back();

    // variables.push_back({varArgs[0], varArgs[1]});

    // ifstream f("/mnt/c/Users/billa/Desktop/shell/disk/home/billal/.bashrc");

    // check every line if the variable already exists
    // if it exists, replace it
    // if it doesn't, add it to the end of the file

    ifstream f(parentPath + "/home/billal/.bashrc");

    if (f.is_open()) {
        string text;
        string finalFile;
        // Read data from the file object and put it into a string.
        while (getline(f, text)) {
            // Print the data of the string.
            if (text.find(varArgs[0]) != string::npos) text = regex_replace(text, regex("^" + varArgs[0] + ".*$"), "");
            finalFile += text + (text != "" ? "\n" : "");
        }
        // Close the file object.
        ofstream fileToWrite(parentPath + "/home/billal/.bashrc");

        fileToWrite << finalFile << varArgs[0] << "=" << varArgs[1] << endl;

        fileToWrite.close();
    }
    f.close();
}

void printenv() {
    ifstream f(parentPath + "/home/billal/.bashrc");

    if (f.is_open()) {
        string text;
        // Read data from the file object and put it into a string.
        while (getline(f, text)) {
            // Print the data of the string.
            vector<string> elements;
            split(elements, text, '=');

            cout << elements[0] << "=" << elements[1] << endl;
        }

        // Close the file object.
    }
    f.close();
}

void whoami() {
    cout << getVar("USER") << endl;
}

void cd(vector<string> &args) {
    if (args.size() > 1) {
        auto currentPath = fs::current_path();  // getting path
        try

        {
            string currentPathFormatted = currentPath.u8string();
            // cout << currentPathFormatted << endl;
            // cout << currentPathFormatted << endl;
            formatPathToUnix(currentPathFormatted);
            // cout << currentPathFormatted << endl;
            // cout << currentPath.u8string()+"/"+args[1] << endl;

            if (args[1].find("../../") == std::string::npos && !((args[1].find("..") != std::string::npos && currentPathFormatted == ""))) fs::current_path(currentPath.u8string() + "/" + args[1]);  // setting path

        } catch (const exception &e) {
            cerr << e.what() << '\n';
        }

    } else {
        cerr << "rm: missing file operand" << endl;
    }
}

void pwd() {
    string currentPath = fs::current_path().u8string();  // getting path
    formatPathToUnix(currentPath);
    cout << currentPath << endl;
}

void addToHistory(string &input) {
    ofstream fileToWrite(parentPath + "/home/billal/.bash_history", ofstream::app);
    fileToWrite << input << endl;
    fileToWrite.close();
}

void history() {
    ifstream f(parentPath + "/home/billal/.bash_history");

    if (f.is_open()) {
        string text;
        // Read data from the file object and put it into a string.
        while (getline(f, text)) {
            // Print the data of the string.
            cout << text << endl;
        }

        // Close the file object.
    }
    f.close();
}

void neofetch(){

}

int main() {
    vector<vector<string>> files = {{"main.cpp", "printf('Hello world')"}};

    string input;
    string user = getVar("USER");
    string host = getVar("HOST");
    // string currentDir = getVar(variables, "CURRENTDIR");
    system("clear");

    auto currentPath = fs::current_path();          // getting path
    fs::current_path(parentPath + "/home/billal");  // setting path

    while (true) {
        addToHistory(input);
        currentPath = fs::current_path();
        string currentDirToPrint = currentPath.u8string();
        formatPathToUnix(currentDirToPrint);
        currentDirToPrint = regex_replace(currentDirToPrint, regex("/home/billal"), "~");
        cout << REDB << user << "@" << host << REDE << ":" << BLUEB << (currentDirToPrint != "" ? currentDirToPrint : "/") << BLUEE << "$ ";
        getline(cin, input);
        vector<string> args;
        split(args, input, ' ');

        if (args.size() == 0)
            ;
        else if (args[0] == "ls")
            ls();
        else if (args[0] == "touch")
            touch(args);
        else if (args[0] == "cd")
            cd(args);
        else if (args[0] == "rm")
            rm(args);
        else if (args[0] == "mkdir")
            mkdir(args);
        else if (args[0] == "echo")
            echo(args);
        else if (args[0] == "exit")
            return 1;
        else if (args[0] == "pwd")
            pwd();
        else if (args[0] == "cat")
            cat(args);
        else if (args[0] == "nano")
            nanoCommand(args);
        else if (args[0] == "whoami")
            whoami();
        else if (args[0] == "clear")
            clearShell();
        else if (args[0] == "help")
            cout << "ls, touch, cd, rm, echo, exit, pwd, cat, nano, whoami, clear, help, printenv, " << endl;
        else if (args[0] == "printenv")
            printenv();
        else if (args[0] == "alias")
            ;
        else if (args[0] == "cp")
            cp(args);
        else if (args[0] == "mv")
            mv(args);
        else if (args[0] == "unalias")
            ;
        else if (args[0] == "wc")
            wc(args);
        else if (args[0] == "g++") {
            string argGpp = "g++ " + args[1] + " -o " + args[1] + ".o && ./" + args[1] + ".o";
            system(argGpp.c_str());
        } else if (args[0] == "history")
            history();
        else if (args[0] == "date")
            date();
        else if (args[0] == "neofetch")
            neofetch();
            
        else if (args[0].find("=") != string::npos) {
            vector<string> varArgs;
            split(varArgs, input, '=');
            createVar(varArgs);
        }

        else
            cerr << args[0] << ": command not found" << endl;
        // add variables
        // add real files system and bashrc for variables and write to it if export is used
        // Like a real terminal, use system() to execute each command ? could be difficult because of the arguments but it's possible if we use a real file system.
    }

    return 0;
}

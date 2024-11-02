#include"./Header/Game.h"
std::string get_current_directory(char* arg);

int main(int argc, char *argv[])
{
    std::string dir = get_current_directory(argv[0]);
    Game g1(dir + "assets/config.txt");
    
    std::cout << "debug" << std::endl;
    g1.run();
    return 0;
}


std::string get_current_directory(char* arg){
    for (int i=strlen(arg)-1; i>=0; i--){
        if (arg[i] == '/') return std::string(arg).substr(0, i+1);
    }
    return "./";
}
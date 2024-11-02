There are a few things of note:
1. This game was developed on Mac os, Windows user may need to include the sfml files in the game folder to complie and run.
2. In main.cpp I use a function to obtain the current working directory, This is done because running the application on Mac os through double clicking the application executable does not set the wporking directory correctly, Windows can modify and remove that function as well as the string object of dir because windows does not face this issue.

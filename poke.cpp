/* Name: Anthony Moore
Date: 12/5/18
About this project: Pokemon Go Simulator
Assumptions: Assumes correct user input

All work below was performed by Anthony Moore */
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cctype>
using namespace std;
/*Function Declarations*/
void PrintMenu();
int PokeballChoice(int&,int&,int&);
void convertstring(string&);
void SeeResults(int,int);
void PrintInv(int,int,int);

struct Pokemon                                                                                                              //structure that stores the pokemon data
{
    int index;
    string name;
    string type;
    int catchpercent;
    int caught = 0;
    int seen = 0;
};
string GetTrainerName();
void PlayGame(Pokemon*,int,int&,int&,int&);
void PrintPokedexStats(Pokemon *, int);
void CheckBalls(int,int,int,int,int,Pokemon*);
void printSortedPoke(Pokemon*,int);
int main()
{
    srand(time(NULL));                                                                                                      //random seed
    /*variable declarations*/
    int size,pokeballs=10,greatballs=10,ultraballs=10, numoffires = 0;
    char selection;
    double totalseen = 0, totalcaught = 0;
    string trainer,filename = "poke.txt";
    ifstream in;
    do {
        in.open(filename);                                                                                                  //opens poke file
        if(!in)                                                                                                             //makes sure the file exists
            cout << "Error Opening File. Please Try Again." << endl;
    }while(!in);
    in >> size;                                                                                                             //reads in the size of the file, first integer in file
    Pokemon *  list = new Pokemon[size];                                                                                    //makes a structure array to store pokemon data
    for(int i = 0; i < size; i++)                                                                                           //reads in data from file
        in >> list[i].index >> list[i].name >> list[i].type >> list[i].catchpercent;
    in.close();                                                                                                             //closes file

    GetTrainerName();

    do {                                                                                                                    //do while loop the keep allowing entries until user quits
        PrintMenu();                                                                                                        //function to print menu to screen
        cin >> selection;

        switch (selection) {                                                                                                //switch statement that covers menu entries
            case '1': {
                /*variable declarations*/
                CheckBalls(pokeballs,greatballs,ultraballs,totalcaught,totalseen,list);
                PlayGame(list,size,pokeballs,greatballs,ultraballs);
                break;
            }
            case '2': {
                PrintPokedexStats(list,size);
                for(int i = 0; i < size; i++) {                                                                             //adds up the total seen and total caught
                    totalseen += list[i].seen;
                    totalcaught += list[i].caught;
                }
                /*prints results*/
                SeeResults(totalcaught,totalseen);
                break;
            }
            case '3':{
                /*prints remaining inventory*/
                PrintInv(pokeballs,greatballs,ultraballs);
                break;
            }
            case '4':{
                for(int i = 0; i < size; i++) {
                    convertstring(list[i].type);                                                                            //function that converts string to all lowercase
                }

                printSortedPoke(list,size);

                break;
            }
            case 'Q':
            case 'q':
                /*program quits and results are printed*/
                SeeResults(totalcaught,totalseen);
                delete [] list;                                                                                             //deletes pokemon list
                exit(0);
                break;
            default:
                cout << "Input a Valid Entry." << endl << endl;                                                             //apart of loop to enter a valid character
        }
    }while(selection != 'Q' || selection != 'q');
    return 0;
}
void PrintMenu()                                                                                                            //prints menu
{
    cout << "\n1 - Hunt for a Pokemon!" << endl;
    cout << "2 - Pokedex Statistics." << endl;
    cout << "3 - Print Inventory." << endl;
    cout << "4 - List Pokemon By Type" << endl;
    cout << "Q - Quit" << endl;
    cout << "Selection: ";
}

void convertstring(string& x)                                                                                               //converts string to all lowercase
{
    for(int i = 0; i < x.length(); i++) {
        if (isupper(x[i]))
            x[i] = tolower(x[i]);
    }
}

int PokeballChoice(int& pokeballs,int& greatballs,int& ultraballs)                                                          //takes in 3 integers by reference and allows user to choose what to return
{
    int value; bool ballsleft;
    cout << "Choose a Ball to Throw: " << endl;
    cout << "1 - Poke Ball  (You Have: " << pokeballs << ")" << endl;
    cout << "2 - Great Ball (You Have: " << greatballs << ")" << endl;
    cout << "3 - Ultra Ball (You Have: " << ultraballs << ")" << endl;
    do{
        do{
            cout << "Selection: "; cin >> value;
            if(value != 1 && value != 2 && value != 3)
                cout << "Enter Valid Choice." << endl;
        }while(value != 1 && value != 2 && value != 3);

        ballsleft = true;
        switch(value){
            case 1:
                pokeballs--;                                                                                                //if user picks pokeballs, the amount is decreased by 1
                if(pokeballs<0)
                {
                    cout << "No More Poke balls left." << endl;                                                             //if pokeballs were to go from 0 to -1, the pokeballs would be reset to zero and user picks again
                    ballsleft = false;
                    pokeballs = 0;
                    break;
                }
                else {
                    ballsleft = true;
                    return 1;
                }
            case 2:
                greatballs--;
                if(greatballs<0)
                {
                    cout << "No More Great balls left." << endl;                                                            //if greatballs were to go from 0 to -1, the pokeballs would be reset to zero and user picks again
                    ballsleft = false;
                    greatballs = 0;
                    break;
                }
                else {
                    ballsleft = true;
                    return 2;
                }
            case 3:
                ultraballs--;
                if(ultraballs<0)
                {
                    cout << "No More Ultra balls left." << endl;                                                            //if ultraballs were to go from 0 to -1, the pokeballs would be reset to zero and user picks again
                    ballsleft = false;
                    ultraballs = 0;
                    break;
                }
                else {
                    ballsleft = true;
                    return 3;
                }
        }
    }while(ballsleft == false);                                                                                             //while there are no balls left for that choice, the function will loop
}
void PlayGame(Pokemon * list, int size, int & pokeballs, int & greatballs, int & ultraballs) {
    int randpoke = rand() % size;
    int randcatch = rand() % 100 + 1;
    cout << "A wild " << list[randpoke].name << " has appeared!" << endl << endl;
    list[randpoke].seen++;                                                                                      //increases amount of seen pokemon by 1
    int choice = PokeballChoice(pokeballs, greatballs,
                                ultraballs);                                             //calls function to choose pokeball, and returns choice
    if (choice == 1) {
        cout << "Threw a PokeBall!" << endl;
        if (randcatch <=
            list[randpoke].catchpercent) {                                                         //if the number is less than the catch percentage, they caught the pokemon
            cout << "Congratulations! You Caught " << list[randpoke].name << "!" << endl;
            list[randpoke].caught++;
        } else
            cout << list[randpoke].name << " ran away."
                 << endl;                                                //else, it ran away
    } else if (choice == 2) {
        cout << "Threw a GreatBall!" << endl;
        if (randcatch <= list[randpoke].catchpercent + 20) {
            cout << "Congratulations! You Caught " << list[randpoke].name << "!" << endl;
            list[randpoke].caught++;
        } else
            cout << list[randpoke].name << " ran away." << endl;
    } else if (choice == 3) {
        cout << "Threw a UltraBall!" << endl;
        if (randcatch <= list[randpoke].catchpercent + 40) {
            cout << "Congratulations! You Caught " << list[randpoke].name << "!" << endl;
            list[randpoke].caught++;
        } else
            cout << list[randpoke].name << " ran away." << endl;
    }
    cout << endl;
}

void CheckBalls(int pokeballs, int greatballs, int ultraballs,int totalcaught, int totalseen, Pokemon * list) {
    if (pokeballs == 0 && greatballs == 0 && ultraballs == 0) {                                                          //if the user runs out of pokeballs, the program quits and the results are printed
        cout << "You Ran Out of Pokeballs." << endl;
        SeeResults(totalcaught,totalseen);
        delete [] list;
        exit(0);
    }
}

void SeeResults(int totalcaught, int totalseen)
{
    cout << "Here are your final results." << endl;
    cout << "Total Pokemon Caught: " << totalcaught << endl;
    cout << "Total Pokemon Seen: " << totalseen << endl;
    cout << fixed << showpoint << setprecision(2);
    if(totalseen == 0)
        cout << "Overall Catch Rate: " << 0 << "%" << endl << endl;
    else
        cout << "Overall Catch Rate: " << static_cast<double>(totalcaught) / static_cast<double>(totalseen) << "%" << endl << endl;
}

string GetTrainerName() {
    string trainer;
    cout << "What's your name, trainer? ";
    getline(cin, trainer);
    cout << "\nWelcome, " << trainer << ", to the COP3014 Safari Zone!" << endl;
    cout << "You'll have 30 chances to catch Pokemon, make them count!" << endl;
    cout << "The game will end once you run out of balls or if you choose to quit early." << endl << endl;
    return trainer;
}

void PrintPokedexStats(Pokemon * list, int size) {
    for (int i = 0; i <
                    size; i++) {                                                                            //prints pokemon name, number of times seen, and number of time caught for all pokemon
        cout.setf(ios::left);
        cout << "no. " << setw(4) << list[i].index << ' ' << setw(10) << list[i].name
             << '\t' << "Seen: " << list[i].seen << "\tCaught: " << list[i].caught << endl;
    }
}

void PrintInv(int pokeballs, int greatballs, int ultraballs) {
    cout << "You Have: " << endl;
    cout << pokeballs << " PokeBalls" << endl;
    cout << greatballs << " Great Balls" << endl;
    cout << ultraballs << " Ultra Balls" << endl << endl;
}
void printSortedPoke(Pokemon *list, int size)
{
    int count = 0;
    cout << "Fire: ";
        for(int i = 0; i < size; i++) {
            
            int firetype = list[i].type.compare("fire");
            if (firetype == 0){
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
            }
            
        }
        count = 0;
    cout << "\nGrass: ";
    for(int i = 0; i < size; i++) {
        int grasstype = list[i].type.compare("grass");
        if (grasstype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nWater: ";
    for(int i = 0; i < size; i++) {
        int watertype = list[i].type.compare("water");
        if (watertype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        } 
    }
    count = 0;
    cout << "\nBug: ";
    for(int i = 0; i < size; i++) {
        int bugtype = list[i].type.compare("bug");
        if (bugtype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nNormal: ";
    for(int i = 0; i < size; i++) {
        int normaltype = list[i].type.compare("normal");
        if (normaltype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nPoison: ";
    for(int i = 0; i < size; i++) {
        int poisontype = list[i].type.compare("poison");
        if (poisontype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nElectric: ";
    for(int i = 0; i < size; i++) {
        int electrictype = list[i].type.compare("electric");
        if (electrictype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nGround: ";
    for(int i = 0; i < size; i++) {
        int groundtype = list[i].type.compare("ground");
        if (groundtype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nFairy: ";
    for(int i = 0; i < size; i++) {
        int fairytype = list[i].type.compare("fairy");
        if (fairytype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nFighting: ";
    for(int i = 0; i < size; i++) {
        int fightingtype = list[i].type.compare("fighting");
        if (fightingtype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nGhost: ";
    for(int i = 0; i < size; i++) {
        int ghosttype = list[i].type.compare("ghost");
        if (ghosttype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nRock: ";
    for(int i = 0; i < size; i++) {
        int rocktype = list[i].type.compare("rock");
        if (rocktype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nPsychic: ";
    for(int i = 0; i < size; i++) {
        int psychictype = list[i].type.compare("psychic");
        if (psychictype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nIce: ";
    for(int i = 0; i < size; i++) {
        int icetype = list[i].type.compare("ice");
        if (icetype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
    count = 0;
    cout << "\nDragon: ";
    for(int i = 0; i < size; i++) {
        int dragontype = list[i].type.compare("dragon");
        if (dragontype == 0)
        {
            if(count == 0)
                cout << list[i].name;
            else    
                cout << ", "  << list[i].name;
            count++;
        }
    }
}
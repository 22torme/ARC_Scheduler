#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <time.h>
#include "Game.h"
#include "Schedule.h"
#include <set>
#include <limits>
#include <windows.h>


bool areEqualIgnoringOrder(const std::vector<int>& list1, const std::vector<int>& list2) {
    // Convert vectors to multisets
    std::multiset<int> multiset1(list1.begin(), list1.end());
    std::multiset<int> multiset2(list2.begin(), list2.end());

    // Compare multisets
    return multiset1 == multiset2;
}

void Solve(Schedule& ARC, std::vector<int>& refAssignments, const std::vector<int>& fairAssignment, int max, std::vector<Schedule>& solutions, int weekIndex = 0, int gameIndex = 0, bool allowCrews2WeeksOffInRow, bool allowCrewsSeeSameTeam2WeeksInRow, bool allowCrewsSeeSameTeamMoreThanTwicePerSeason, bool allowCrewsToGoToSameSiteMoreThanTwice, bool crewsMustSeeEachTeam) {
    // If all weeks are processed, check if one referee has 8 assignments and the rest 7 assignments
    if (weekIndex == ARC.getWeeks()) { //last week
    //checks to make sure assingment fair
        if (areEqualIgnoringOrder(refAssignments, fairAssignment)){  // Only one referee should have 8 assignments
            if(crewsMustSeeEachTeam == true && ARC.refsSeenEachTeam() == true){
                std::cout << "Solution Found" << std::endl;
                solutions.push_back(ARC); // Add the valid solution to our list
            }
            else if(crewsMustSeeEachTeam == false){
                std::cout << "Solution Found" << std::endl;
                solutions.push_back(ARC); // Add the valid solution to our list
            }  
            // //ADD IF TO SEE IF EACH REF HAS SEEN EACH TEAM
            // std::cout << "Solution Found" << std::endl;
            // solutions.push_back(ARC); // Add the valid solution to our list
        }
        return;  // Don't return true; continue searching for other solutions
    }

    // If all games for this week are processed, move to the next week
    if (gameIndex == ARC.getNumGamesPerWeek() + 1) {
        Solve(ARC, refAssignments, fairAssignment, max, solutions, weekIndex + 1, 0, allowCrews2WeeksOffInRow, allowCrewsSeeSameTeam2WeeksInRow, allowCrewsSeeSameTeamMoreThanTwicePerSeason, allowCrewsToGoToSameSiteMoreThanTwice, crewsMustSeeEachTeam);
        return;
    }

    // If this game already has a ref, skip to the next game
    if (ARC.getRef(weekIndex, gameIndex) != ' ') {
        Solve(ARC, refAssignments, fairAssignment, max, solutions, weekIndex, gameIndex + 1, allowCrews2WeeksOffInRow, allowCrewsSeeSameTeam2WeeksInRow, allowCrewsSeeSameTeamMoreThanTwicePerSeason, allowCrewsToGoToSameSiteMoreThanTwice, crewsMustSeeEachTeam);
        return;
    }

    // Try to assign each ref crew
    for (char c = 'A'; c <= 'A' + ARC.getNumRefs() - 1; c++) {
        if (refAssignments[c - 'A'] >= max) continue;  // If ref already has 8 assignments, skip to the next ref

        if (ARC.isValid(weekIndex, gameIndex, c, allowCrews2WeeksOffInRow, allowCrewsSeeSameTeam2WeeksInRow, allowCrewsSeeSameTeamMoreThanTwicePerSeason, allowCrewsToGoToSameSiteMoreThanTwice)) {
            ARC.assignRef(weekIndex, gameIndex, c);
            
            // Only increment the count for games 0-3
            if (gameIndex < ARC.getNumGamesPerWeek()) {
                refAssignments[c - 'A']++;  // Increment the count for the assigned ref
            }

            Solve(ARC, refAssignments, fairAssignment, max, solutions, weekIndex, gameIndex + 1, allowCrews2WeeksOffInRow, allowCrewsSeeSameTeam2WeeksInRow, allowCrewsSeeSameTeamMoreThanTwicePerSeason, allowCrewsToGoToSameSiteMoreThanTwice, crewsMustSeeEachTeam);
            
            // Backtrack
            ARC.assignRef(weekIndex, gameIndex, ' ');
            
            // Only decrement the count for games 0-3
            if (gameIndex < ARC.getNumGamesPerWeek()) {
                refAssignments[c - 'A']--;  // Decrement the count for the ref
            }
        }
    }
}

std::vector<int> distributeGames(int totalGames, int numRefs) {
    // Initialize the vector with the base number of games per ref
    int baseGames = totalGames / numRefs;
    std::vector<int> gamesPerRef(numRefs, baseGames);

    // Distribute the remaining games one by one to the refs
    int remainingGames = totalGames % numRefs;
    for (int i = 0; i < remainingGames; ++i) {
        gamesPerRef[i]++;
    }

    return gamesPerRef;
}

// Function to enable ANSI escape codes in Windows command prompt
void enableWindowsTerminalColors(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Error getting console handle" << std::endl;
        return;
    }

    DWORD consoleMode;
    if (!GetConsoleMode(hConsole, &consoleMode)) {
        std::cerr << "Error getting console mode" << std::endl;
        return;
    }

    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hConsole, consoleMode)) {
        std::cerr << "Error setting console mode" << std::endl;
        return;
    }
}


int main() {

    enableWindowsTerminalColors();

    // Define some ANSI color codes
    const std::string RED = "\033[31m";      // Red
    const std::string GREEN = "\033[32m";    // Green
    const std::string YELLOW = "\033[33m";   // Yellow
    const std::string BLUE = "\033[34m";     // Blue
    const std::string MAGENTA = "\033[35m";  // Magenta
    const std::string CYAN = "\033[36m";     // Cyan
    const std::string RESET = "\033[0m";     // Reset to default color

  //initialize weeks. 9 weeks(0-8), 4 games per week(0-3)
    // std::vector<std::vector<Game>> weeks = {
    //     {Game("BVU","LOR"), Game("SIM","DBQ"), Game("WAR","CEN"), Game("COE","LUT"),Game("BYE","BYE")},

    //     {Game("LOR","SIM"), Game("CEN","BVU"), Game("DBQ","COE"), Game("NWU","WAR"),Game("BYE","BYE")},

    //     {Game("COE","LOR"), Game("BVU","NWU"), Game("LUT","DBQ"), Game("SIM","CEN"),Game("BYE","BYE")},
    
    //     {Game("LOR","LUT"), Game("WAR","BVU"), Game("NWU","SIM"), Game("CEN","COE"),Game("BYE","BYE")},
    
    //     {Game("DBQ","LOR"), Game("SIM","WAR"), Game("LUT","CEN"), Game("COE","NWU"),Game("BYE","BYE")},
    
    //     {Game("BVU","SIM"), Game("CEN","DBQ"), Game("WAR","COE"), Game("NWU","LUT"),Game("BYE","BYE")},
    
    //     {Game("LOR","CEN"), Game("COE","BVU"), Game("DBQ","NWU"), Game("LUT","WAR"),Game("BYE","BYE")},
    
    //     {Game("NWU","LOR"), Game("BVU","LUT"), Game("WAR","DBQ"), Game("SIM","COE"),Game("BYE","BYE")},
    
    //     {Game("LOR","WAR"), Game("DBQ","BVU"), Game("CEN","NWU"), Game("LUT","SIM"),Game("BYE","BYE")},
    //     };

    // int numWeeks = 9; //when click add week increments
    // int gamesPerWeek = 4; //when click add game increments
    // int numTeams = 9; 

    // int numRefs = 5; //gonna have to manually input

    // std::vector<std::string> teams = {"BVU", "LOR", "SIM", "DBQ", "WAR", "CEN", "COE", "LUT", "NWU"};

    // std::vector<std::unordered_set<std::string>> teamEachRefSeen = {{},{},{},{},{}}; // (A=0, B=1, C=2, D=3, E=4)

    //stores solutions
    // std::vector<Schedule> solutions;

    //Initialize schedule
    // Schedule ARC(weeks, teams, numWeeks, gamesPerWeek, numTeams, numRefs);

    //counts # of assignments for each ref
    // std::vector<int> refAssignments(ARC.getNumRefs(), 0);  // Initialize with 0 for all refs (A=0, B=1, C=2, D=3, E=4)

    // std::vector<int> fairAssignment = distributeGames(numWeeks*gamesPerWeek, numRefs);

    // int maxAssignment = 0;
    // if (!fairAssignment.empty()) {
    //     maxAssignment = *std::max_element(fairAssignment.begin(), fairAssignment.end());
    // }

    // Solve(ARC, refAssignments, fairAssignment, maxAssignment, solutions); //eventually add customizable constraints





    //================================================================================================
    // program start
 
  
    // ask for number of weeks
    int numWeeks;
    std::cout << "Enter the number of weeks: ";
    std::cin >> numWeeks;

    // ask for number of games per week
    int games;
    std::cout << "Enter the number of games per week: ";
    std::cin >> games;

    // ask for number of teams
    int numTeams;
    std::cout << "Enter the number of teams: ";
    std::cin >> numTeams;

    // ask for teams
    std::vector<std::string> teamNames;
    for (int i = 0; i < numTeams; i++) {
        std::string team;
        std::cout << "Enter team " << i + 1 << ": ";
        std::cin >> team;
        teamNames.push_back(team);
    }

    //turn teamNames into string
    std::string teamString = "{";
    for (int i = 0; i < teamNames.size(); i++) {
        teamString += std::to_string(i+1) + ":" + teamNames[i];
        if (i != teamNames.size() - 1) {
            teamString += ", ";
        }
    }
    teamString += "}";

    // ask for number of refs
    int refs;
    std::cout << "Enter the number of ref crews: ";
    std::cin >> refs;


    // enter games per week by entering number
    std::vector<std::vector<Game>> weeks;
    for (int i = 0; i < numWeeks; i++) {
        std::vector<Game> week;
        for (int j = 0; j < games; j++) {
            std::string awayTeam;
            std::string homeTeam;
            // int userInput1 = 0;
            // int userInput2 = 0;
            // do{
            //     std::cout << "Enter integer corresponding to away team for game " << j + 1 << " of week " << i + 1 << " " << CYAN << teamString << RESET << ": ";
            //     std::cin >> userInput1;
            // } while(userInput1 > numTeams && userInput1 <= 0);
            
            // do{
            //     std::cout << "Enter integer corresponding to home team for game " << j + 1 << " of week " << i + 1 << " " << CYAN << teamString << RESET << ": ";
            //     std::cin >> userInput2;
            // } while(userInput2 > numTeams && userInput2 <= 0 && userInput2 == userInput1);

            // awayTeam = teamNames[userInput1 - 1];
            // homeTeam = teamNames[userInput2 - 1];

            std::cout << "Enter away team for game " << j + 1 << " of week " << i + 1 << ": ";
            std::cin >> awayTeam;
            std::cout << "Enter home team for game " << j + 1 << " of week " << i + 1 << ": ";
            std::cin >> homeTeam;

            week.push_back(Game(awayTeam, homeTeam));
        }
        week.push_back(Game("BYE", "BYE"));
        weeks.push_back(week);
    }

    Schedule ARC(weeks, teamNames, numWeeks, games, numTeams, refs);

    int userInput = 0;
    //print schedule to check and have loop to change if needed
    while(userInput != 1){
        ARC.printGameSchedule();
        std::cout << "Verify Schedule (Press 1 to continue, Press 2 to edit): ";
        std::cin >> userInput;
        if(userInput == 2){
            int week;
            int game;
            std::cout << "Enter week to edit: ";
            std::cin >> week;
            
            std::cout << "Enter game to edit: ";
            std::cin >> game;

            std::string awayTeam;
            std::string homeTeam;
            
            std::cout << "Enter away team for game " << game << " of week " << week << ": ";
            std::cin >> awayTeam;
            std::cout << "Enter home team for game " << game << " of week " << week << ": ";
            std::cin >> homeTeam;

            ARC.getGame(week-1, game-1).setAwayTeam(awayTeam);
            ARC.getGame(week-1, game-1).setHomeTeam(homeTeam);
        }
    }



    //counts # of assignments for each ref
    std::vector<int> refAssignments(ARC.getNumRefs(), 0);  // Initialize with 0 for all refs (A=0, B=1, C=2, D=3, E=4)

    std::vector<int> fairAssignment = distributeGames(numWeeks*games, refs);

    int maxAssignment = 0;
    if (!fairAssignment.empty()) {
        maxAssignment = *std::max_element(fairAssignment.begin(), fairAssignment.end());
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // press enter to solve
    std::cout << "Press Enter to Generate Solutions..." << std::endl;

    char c;
    do {
        c = std::cin.get();  // Read a single character from input
    } while (c != '\n');  // Continue looping until Enter (newline) is detected

    std::cout << "Enter Key was Pressed. Generating Solutions..." << std::endl;
    
   // ask which constraints to apply 


    // solve
    std::vector<Schedule> solutions;

    while(solutions.size() == 0){
        
        //constraints
        bool allowCrews2WeeksOffInRow;
        bool allowCrewsSeeSameTeam2WeeksInRow;
        bool allowCrewsSeeSameTeamMoreThanTwicePerSeason;
        bool allowCrewsToGoToSameSiteMoreThanTwice;
        bool crewsMustSeeEachTeam;

        std::cout << "Allow crews to have 2 weeks off in a row? (1 for yes, 0 for no): ";
        std::cin >> allowCrews2WeeksOffInRow;

        std::cout << "Allow crews to see the same team 2 weeks in a row? (1 for yes, 0 for no): ";
        std::cin >> allowCrewsSeeSameTeam2WeeksInRow;
        
        std::cout << "Allow crews to see the same team more than twice per season? (1 for yes, 0 for no): ";
        std::cin >> allowCrewsSeeSameTeamMoreThanTwicePerSeason;

        std::cout << "Allow crews to go to the same site more than twice? (1 for yes, 0 for no): ";
        std::cin >> allowCrewsToGoToSameSiteMoreThanTwice;

        std::cout << "Crews must see each team at least once? (1 for yes, 0 for no): ";
        std::cin >> crewsMustSeeEachTeam;

        time_t start,end;
        time(&start);
        
        Solve(ARC, refAssignments, fairAssignment, maxAssignment, solutions,0,0, allowCrews2WeeksOffInRow, allowCrewsSeeSameTeam2WeeksInRow, allowCrewsSeeSameTeamMoreThanTwicePerSeason, allowCrewsToGoToSameSiteMoreThanTwice, crewsMustSeeEachTeam); //eventually add customizable constraints

        
        time(&end);

        std::cout << "runtime: " << end-start << std::endl;

        if(solutions.size() == 0){
            std::cout << "No solutions found. Reconfigure constraints" << std::endl;
        }
    }
    // get path from user
    std::string path;
    std::cout << "Enter the full path to the file (e.g. C:\\Users\\YourUsername\\Desktop\\[yourfilename].csv): ";
    std::cin >> path;

    // open file
    std::ofstream
        file(path);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file" << std::endl;
        return -1;
    }

    std::ofstream output(path);

    output << "Number of solutions: " << solutions.size() << std::endl;
    output << std::endl;

    for (const Schedule& sol : solutions) {
        sol.outputFile(output);
        output << std::endl;
    }
    
    output.close();

    std::cout << "solutions generated at " << path << std::endl; 

    return 0;
}



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



bool areEqualIgnoringOrder(const std::vector<int>& list1, const std::vector<int>& list2) {
    // Convert vectors to multisets
    std::multiset<int> multiset1(list1.begin(), list1.end());
    std::multiset<int> multiset2(list2.begin(), list2.end());

    // Compare multisets
    return multiset1 == multiset2;
}

void Solve(Schedule& ARC, std::vector<int>& refAssignments, const std::vector<int>& fairAssignment, int max, std::vector<Schedule>& solutions, int weekIndex = 0, int gameIndex = 0) {
    // If all weeks are processed, check if one referee has 8 assignments and the rest 7 assignments
    if (weekIndex == ARC.getWeeks()) { //last week
    //checks to make sure assingment fair
        if (areEqualIgnoringOrder(refAssignments, fairAssignment) && ARC.refsSeenEachTeam()){  // Only one referee should have 8 assignments
            //ADD IF TO SEE IF EACH REF HAS SEEN EACH TEAM
            std::cout << "Solution Found" << std::endl;
            solutions.push_back(ARC); // Add the valid solution to our list

        }
        return;  // Don't return true; continue searching for other solutions
    }

    // If all games for this week are processed, move to the next week
    if (gameIndex == ARC.getNumGamesPerWeek() + 1) {
        Solve(ARC, refAssignments, fairAssignment, max, solutions, weekIndex + 1, 0);
        return;
    }

    // If this game already has a ref, skip to the next game
    if (ARC.getRef(weekIndex, gameIndex) != ' ') {
        Solve(ARC, refAssignments, fairAssignment, max, solutions, weekIndex, gameIndex + 1);
        return;
    }

    // Try to assign each ref crew
    for (char c = 'A'; c <= 'A' + ARC.getNumRefs() - 1; c++) {
        if (refAssignments[c - 'A'] >= max) continue;  // If ref already has 8 assignments, skip to the next ref

        if (ARC.isValid(weekIndex, gameIndex, c)) {
            ARC.assignRef(weekIndex, gameIndex, c);
            
            // Only increment the count for games 0-3
            if (gameIndex < ARC.getNumGamesPerWeek()) {
                refAssignments[c - 'A']++;  // Increment the count for the assigned ref
            }

            Solve(ARC, refAssignments, fairAssignment, max, solutions, weekIndex, gameIndex + 1);
            
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

int main() {

    time_t start,end;
    time(&start);

  //initialize weeks. 9 weeks(0-8), 4 games per week(0-3)
    std::vector<std::vector<Game>> weeks = {
        {Game("BVU","LOR"), Game("SIM","DBQ"), Game("WAR","CEN"), Game("COE","LUT"),Game("BYE","BYE")},
        {Game("LOR","SIM"), Game("CEN","BVU"), Game("DBQ","COE"), Game("NWU","WAR"),Game("BYE","BYE")},
        {Game("COE","LOR"), Game("BVU","NWU"), Game("LUT","DBQ"), Game("SIM","CEN"),Game("BYE","BYE")},
        {Game("LOR","LUT"), Game("WAR","BVU"), Game("NWU","SIM"), Game("CEN","COE"),Game("BYE","BYE")},
        {Game("DBQ","LOR"), Game("SIM","WAR"), Game("LUT","CEN"), Game("COE","NWU"),Game("BYE","BYE")},
        {Game("BVU","SIM"), Game("CEN","DBQ"), Game("WAR","COE"), Game("NWU","LUT"),Game("BYE","BYE")},
        {Game("LOR","CEN"), Game("COE","BVU"), Game("DBQ","NWU"), Game("LUT","WAR"),Game("BYE","BYE")},
        {Game("NWU","LOR"), Game("BVU","LUT"), Game("WAR","DBQ"), Game("SIM","COE"),Game("BYE","BYE")},
        {Game("LOR","WAR"), Game("DBQ","BVU"), Game("CEN","NWU"), Game("LUT","SIM"),Game("BYE","BYE")},
        };

    int numWeeks = 9; //when click add week increments
    int gamesPerWeek = 4; //when click add game increments
    int numTeams = 9; 
    int numRefs = 5; //gonna have to manually input

    std::vector<std::string> teams = {"BVU", "LOR", "SIM", "DBQ", "WAR", "CEN", "COE", "LUT", "NWU"};

    // std::vector<std::unordered_set<std::string>> teamEachRefSeen = {{},{},{},{},{}}; // (A=0, B=1, C=2, D=3, E=4)

    //stores solutions
    std::vector<Schedule> solutions;

    //Initialize schedule
    Schedule ARC(weeks, teams, numWeeks, gamesPerWeek, numTeams, numRefs);

    //counts # of assignments for each ref
    std::vector<int> refAssignments(ARC.getNumRefs(), 0);  // Initialize with 0 for all refs (A=0, B=1, C=2, D=3, E=4)

    std::vector<int> fairAssignment = distributeGames(numWeeks*gamesPerWeek, numRefs);

    int maxAssignment = 0;
    if (!fairAssignment.empty()) {
        maxAssignment = *std::max_element(fairAssignment.begin(), fairAssignment.end());
    }

    Solve(ARC, refAssignments, fairAssignment, maxAssignment, solutions); //eventually add customizable constraints

    std::ofstream output("Solutions.csv");

    output << "Number of solutions: " << solutions.size() << std::endl;
    output << std::endl;

    for (const Schedule& sol : solutions) {
        sol.outputFile(output);
        output << std::endl;
    }
    
    output.close();

    time(&end);
    std::cout << "runtime: " << end-start << std::endl; 

    return 0;
}



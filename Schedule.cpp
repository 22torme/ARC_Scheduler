#include "Schedule.h"
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

Schedule::Schedule(const std::vector<std::vector<Game>>& weeks, const std::vector<std::string>& teams, int numWeeks, int gamesPerWeek, int numTeams, int numRefs) : weeks(weeks), teams(teams), numWeeks(numWeeks), gamesPerWeek(gamesPerWeek), numTeams(numTeams), numRefs(numRefs) {}

void Schedule::assignRef(int week, int game, char refCrew) { weeks[week][game].setRefCrew(refCrew); }

Game& Schedule::getGame(int week, int game) { return weeks[week][game]; }

std::vector<Game> Schedule::getWeek(int week) const { return weeks[week]; }

char Schedule::getRef(int week, int game) const { return weeks[week][game].getRefCrew(); }

int Schedule::getWeeks() {return numWeeks;}

bool Schedule::refsSeenEachTeam() {
    std::unordered_set<std::string> teams = {"BVU", "LOR", "SIM", "DBQ", "WAR", "CEN", "COE", "LUT", "NWU"};

    for (char c = 'A'; c <= 'A' + numRefs - 1; c++) {
        std::unordered_set<std::string> refSeen;
        for (int i = 0; i < numWeeks; i++) { //9 == num weeks
            for (int j = 0; j < gamesPerWeek; j++) { //4 games per week
                Game game = getGame(i, j);
                if (game.getRefCrew() == c) {
                    refSeen.insert(game.getAwayTeam());
                    refSeen.insert(game.getHomeTeam());
                }
            }
        }
        // Check if the ref has seen all teams
        if (refSeen != teams) {
            return false;
        }
    }

    return true;
} 

void Schedule::printSchedule() const {
    for (int i = 0; i < numWeeks; i++) {
        std::cout << "Week " << i + 3 << std::endl;
        for (int j = 0; j < gamesPerWeek; j++) {
            std::cout << weeks[i][j].getAwayTeam() << " @ " << weeks[i][j].getHomeTeam() << " Refs: " << weeks[i][j].getRefCrew() << std::endl;
        }
        std::cout << std::endl;
    }
}

bool Schedule::isComplete() const {
    for (const auto& week : weeks) {
        for (const auto& game : week) {
            if (game.getRefCrew() == ' ') {
                return false;
            }
        }
    }
    return true;
}

bool Schedule::isValid(int week, int game, char refCrew) const {

    if(game == gamesPerWeek && week > 0){
        if(weeks[week-1][gamesPerWeek].getRefCrew() == refCrew){
            return false;
        }
    }
    
    //check if ref is already assigned for week
    for(int i = 0; i<gamesPerWeek; i++){
        if(weeks[week][i].getRefCrew() == refCrew){
            return false;
        }
    }

    //check if crew saw same team last week
    if(week > 0){
        for(int i = 0; i<gamesPerWeek; i++){
            if(weeks[week-1][i].getRefCrew() == refCrew){
                if(weeks[week-1][i].getAwayTeam() == weeks[week][game].getAwayTeam() || weeks[week-1][i].getAwayTeam() == weeks[week][game].getHomeTeam() || weeks[week-1][i].getHomeTeam() == weeks[week][game].getAwayTeam() || weeks[week-1][i].getHomeTeam() == weeks[week][game].getHomeTeam()){
                    return false;
                }
            }
        }
    }

    //check if crew reffed home team at all
    for(int i = 0; i<numWeeks; i++){
        for(int j = 0; j<gamesPerWeek; j++){
            if(weeks[i][j].getRefCrew() == refCrew){
                if(weeks[i][j].getHomeTeam() == weeks[week][game].getHomeTeam()){
                    return false;
                }
            }
        }
    }

    // check if ref has seen same team more than twice
    int countAwayTeam = 0;
    int countHomeTeam = 0;
    int assignmentCount = 0;
    for(int i = 0; i < numWeeks; i++){ 
        for(int j = 0; j < gamesPerWeek; j++){
            if(weeks[i][j].getRefCrew() == refCrew){
                assignmentCount++;
                if(weeks[i][j].getAwayTeam() == weeks[week][game].getAwayTeam() || weeks[i][j].getHomeTeam() == weeks[week][game].getAwayTeam()){
                    countAwayTeam++;
                }
                if(weeks[i][j].getAwayTeam() == weeks[week][game].getHomeTeam() || weeks[i][j].getHomeTeam() == weeks[week][game].getHomeTeam()){
                    countHomeTeam++;
                }
            }
        }
    }
    if(countAwayTeam == 2 || countHomeTeam == 2){
        return false;
    }

    //ref cannot exceed 8 assignments
    if(assignmentCount == 8){
        return false;
    }

    return true;
}       

void Schedule::outputFile(std::ofstream& output) const {
    output << " , ";
    for(int i = 0; i<gamesPerWeek; i++){
        output << "Game " << i+1 << ", " << "Ref Crew, ";
    }
    output << std::endl;
    for (int i = 0; i < numWeeks; i++) {
        output << "Week: " << i << ", ";
        for (int j = 0; j < gamesPerWeek; j++) {
            output << weeks[i][j].getAwayTeam() << " @ " << weeks[i][j].getHomeTeam() << ", " << weeks[i][j].getRefCrew() << ", ";
        }
        
        output << std::endl;
    }
}

int Schedule::getNumRefs() const { return numRefs; }

int Schedule::getNumGamesPerWeek() const { return gamesPerWeek; }
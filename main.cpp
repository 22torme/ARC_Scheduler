#include <string>
#include <iostream>
#include <fstream> 
#include <vector>

class Game{
    public:
        Game(int homeTeamToSet, int awayTeamToSet){
            homeTeam = homeTeamToSet;
            awayTeam = awayTeamToSet;
        };
        void setRefCrew(char refCrewToSet);
        void setHomeTeam(int homeTeamToSet);
        void setAwayTeam(int awayTeamToSet);
        char getRefCrew();
        int getHomeTeam();
        int getAwayTeam();

    private:
        char refCrew; // A-E
        int homeTeam;
        int awayTeam;
};

void Game::setRefCrew(char refCrewToSet){
    refCrew = refCrewToSet;
}

void Game::setHomeTeam(int homeTeamToSet){
    homeTeam = homeTeamToSet;
}

void Game::setAwayTeam(int awayTeamToSet){
    awayTeam = awayTeamToSet;
}

char Game::getRefCrew(){
    return refCrew;
}

int Game::getHomeTeam(){
    return homeTeam;
}

int Game::getAwayTeam(){
    return awayTeam;
}


// FUNCTION: INITIALIZE SCHEDULE
std::vector<std::vector<Game>> initializeSched(std::vector<int> teams, int numWeeks, int numGamesPerWeek){

}

int main(){

    //Initialize teams
    // LOR = 1, BVU = 2, DBQ = 3, WAR = 4, LUT = 5, NWU = 6, COE = 7, CEN = 8, SIM = 9;
    std::vector<int> teams = {1,2,3,4,5,6,7,8,9};
















    return 0;
}



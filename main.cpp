#include <string>
#include <iostream>
#include <fstream> 
#include <vector>

class Game{
    public:
        Game(std::string awayTeamToSet, std::string homeTeamToSet){
            awayTeam = awayTeamToSet;
            homeTeam = homeTeamToSet;
        }
        void setRefCrew(char refCrewToSet);
        void setHomeTeam(std::string homeTeamToSet);
        void setAwayTeam(std::string awayTeamToSet);
        char getRefCrew();
        std::string getHomeTeam();
        std::string getAwayTeam();

    private:
        char refCrew; // A-E
        std::string awayTeam;
        std::string homeTeam;
};

void Game::setRefCrew(char refCrewToSet){
    refCrew = refCrewToSet;
}

void Game::setHomeTeam(std::string homeTeamToSet){
    homeTeam = homeTeamToSet;
}

void Game::setAwayTeam(std::string awayTeamToSet){
    awayTeam = awayTeamToSet;
}

char Game::getRefCrew(){
    return refCrew;
}

std::string Game::getHomeTeam(){
    return homeTeam;
}

std::string Game::getAwayTeam(){
    return awayTeam;
}

int main(){

    //Initialize list of teams
    std::vector<std::string> teams = {"LOR", "BVU", "DBQ","WAR","LUT","NWU","COE","CEN","SIM"};

    //Initialize list of referees
    std::vector<char> refs = {'A','B','C','D','E'};

    //Initialize week 1 BVU vs. LOR, SIM vs DBQ, WAR vs CEN, COE vs LUT, the right team is home
    Game game1("LOR","BVU");
    Game game2("SIM","DBQ");















    return 0;
}



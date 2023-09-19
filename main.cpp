#include <string>
#include <iostream>
#include <fstream> 

class Game{
    public:
        void setRefCrew(char refCrewToSet);
        void setHomeTeam(std::string homeTeamToSet);
        void setAwayTeam(std::string awayTeamToSet);
        char getRefCrew();
        std::string getHomeTeam();
        std::string getAwayTeam();

    private:
        char refCrew; // A-E
        std::string homeTeam;
        std::string awayTeam;
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



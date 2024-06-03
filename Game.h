#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

class Game {
public:
    Game(const std::string& awayTeamToSet, const std::string& homeTeamToSet);
    void setRefCrew(char refCrewToSet);
    char getRefCrew() const;
    std::string getAwayTeam() const;
    std::string getHomeTeam() const;\
    void setAwayTeam(std::string awayTeamToSet);
    void setHomeTeam(std::string homeTeamToSet);

private:
    char refCrew = ' '; // A-E
    std::string awayTeam;
    std::string homeTeam;
};

#endif // GAME_H
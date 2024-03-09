#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include <iostream>
#include <fstream>
#include "Game.h"
#include <unordered_set>
#include <string>


class Schedule {
public:
    explicit Schedule(const std::vector<std::vector<Game>>& weeks, const std::vector<std::string>& teams, int numWeeks = 9, int gamesPerWeek = 4, int numTeams = 9, int numRefs = 5);
    void assignRef(int week, int game, char refCrew);
    Game& getGame(int week, int game);
    std::vector<Game> getWeek(int week) const;
    void printSchedule() const;
    char getRef(int week, int game) const;
    bool isComplete() const;
    bool isValid(int week, int game, char refCrew) const;
    void outputFile(std::ofstream& output) const;
    int getWeeks();
    bool refsSeenEachTeam();
    int getNumRefs() const;
    int getNumGamesPerWeek() const;

private:
    std::vector<std::vector<Game>> weeks;
    const int numWeeks;
    const int gamesPerWeek;
    const int numTeams;
    const int numRefs;
    std::vector<std::string> teams;
};

#endif // SCHEDULE_H


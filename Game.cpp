#include "Game.h"
#include <vector>
#include <string>



Game::Game(const std::string& awayTeamToSet, const std::string& homeTeamToSet): awayTeam(awayTeamToSet), homeTeam(homeTeamToSet) {}
void Game::setRefCrew(char refCrewToSet) { refCrew = refCrewToSet; }
char Game::getRefCrew() const { return refCrew; }
std::string Game::getAwayTeam() const { return awayTeam; }
std::string Game::getHomeTeam() const { return homeTeam; }
void Game::setAwayTeam(std::string awayTeamToSet) { awayTeam = awayTeamToSet; }
void Game::setHomeTeam(std::string homeTeamToSet) { homeTeam = homeTeamToSet; }

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
        void setRefCrew(char refCrewToSet){refCrew = refCrewToSet;};
        // void setHomeTeam(std::string homeTeamToSet){homeTeam = homeTeamToSet;};
        // void setAwayTeam(std::string awayTeamToSet){awayTeam = awayTeamToSet;};
        char getRefCrew(){return refCrew;};
        std::string getHomeTeam(){return homeTeam;};
        std::string getAwayTeam(){return awayTeam;};
        std::vector<std::string> getTeams(){return {awayTeam, homeTeam};};

    private:
        char refCrew; // A-E
        std::string awayTeam;
        std::string homeTeam;
};


class Schedule{
    public:
        //assign refs
        void assignRefs(int week, int game, char refCrew){weeks[week][game].setRefCrew(refCrew);};
        char getRef(int week, int game){return weeks[week][game].getRefCrew();};
        
        //return game
        Game getGame(int week, int game){return weeks[week][game];};


    private:
        //Initialize weeks 3-11 the right team is home
        std::vector<std::vector<Game>> weeks = {
        {Game("LOR","SIM"), Game("CEN","BVU"), Game("DBQ","COE"), Game("NWU","WAR")},
        {Game("COE","LOR"), Game("BVU","NWU"), Game("LUT","DBQ"), Game("SIM","CEN")},
        {Game("LOR","LUT"), Game("WAR","BVU"), Game("NWU","SIM"), Game("CEN","COE")},
        {Game("DBQ","LOR"), Game("SIM","WAR"), Game("LUT","CEN"), Game("COE","NWU")},
        {Game("BVU","SIM"), Game("CEN","DBQ"), Game("WAR","COE"), Game("NWU","LUT")},
        {Game("LOR","CEN"), Game("COE","BVU"), Game("DBQ","NWU"), Game("LUT","WAR")},
        {Game("NWU","LOR"), Game("BVU","LUT"), Game("WAR","DBQ"), Game("SIM","COE")},
        {Game("LOR","WAR"), Game("DBQ","BVU"), Game("CEN","NWU"), Game("LUT","SIM")},
        };

        //refs
        std::vector<char> refs = {'A','B','C','D','E'};

        //teams
        std::vector<std::string> teams = {"LOR", "BVU", "DBQ","WAR","LUT","NWU","COE","CEN","SIM"};
};

//ALL FALSE FOR 1st week
//bool reffedLastWeek
//bool reffedTeamLastWeek
//bool reffedTeamTwice
//bool reffedTeamWhileBefore
//is valid function]


//helper functions
bool isValid(Schedule ARC, int week, int game, char refCrew);

// bool reffedLastWeek(Schedule ARC, int week, int game, char refCrew){
//     if(week == 0){
//         return false;
//     }
//     else{
//         for (int i = 0; i < 4; i++){
//             if(ARC.getRef(week-1,i) == refCrew){
//                 if(){
//                     return true;
//                 }
//             }
//         }
//         return false;
//         }
// };

bool reffedTeamLastWeek(Schedule ARC, int week, int game, char refCrew){
    if(week == 0){
        return false;
    }
    else{
        for (int i = 0; i < 4; i++){
            if(ARC.getRef(week-1,i) == refCrew){
                return true;
            }
        }
        return false;
        }
}





//solution return list[8] of strings ex:
// ["ABCD","ABCE","ABDE","ACDE","BCDE", "CDEA", "ABCD", "ABCD"]

// std::vector<std::string> solve(Schedule ARC, int week, int game, std::vector<std::string> &solution);


int main(){


    //using a backtracking algorithm, assign referees to games under the following hard constraints:
    // Each week, there are 4 conference games, so one crew will have the week off. 
    // No crew is allowed to have 2 weeks off in a row. 
    // No crew is allowed to see the same team 2 weeks in a row. 
    // No crew is allowed to see the same team more than 2 times in a season. 
    // No crew is allowed to go to the same site more than once in a season. 
    // Crews will be assigned to 7 games minimum, one crew will be assigned to 8 games.

    Schedule ARC;

    ARC.assignRefs(0,0,'A');
    std::cout << ARC.getRef(0,0) << std::endl;
    std::cout << ARC.getGame(0,0).getHomeTeam() << std::endl;

    return 0;
}



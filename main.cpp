#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <time.h>

class Game {
public:
    Game(const std::string& awayTeamToSet, const std::string& homeTeamToSet)
        : awayTeam(awayTeamToSet), homeTeam(homeTeamToSet) {}

    void setRefCrew(char refCrewToSet) { refCrew = refCrewToSet; }
    char getRefCrew() const { return refCrew; }
    std::string getAwayTeam() const { return awayTeam; }
    std::string getHomeTeam() const { return homeTeam; }

private:
    char refCrew = ' '; // A-E
    std::string awayTeam;
    std::string homeTeam;
};

class Schedule {
public:
    explicit Schedule(const std::vector<std::vector<Game>>& weeks) : weeks(weeks), numWeeks(9) {}

    void assignRef(int week, int game, char refCrew) { weeks[week][game].setRefCrew(refCrew); }
    Game& getGame(int week, int game) { return weeks[week][game]; }
    std::vector<Game> getWeek(int week) const { return weeks[week]; }
    void printSchedule() const;
    char getRef(int week, int game) const { return weeks[week][game].getRefCrew(); }
    bool isComplete() const;
    bool isValid(int week, int game, char refCrew) const;
    void outputFile(std::ofstream& output) const;
    int getWeeks() {return numWeeks;}
    bool refsSeenEachTeam();

private:
    std::vector<std::vector<Game>> weeks;
    int numWeeks;
};

bool Schedule::refsSeenEachTeam() {
    std::unordered_set<std::string> teams = {"BVU", "LOR", "SIM", "DBQ", "WAR", "CEN", "COE", "LUT", "NWU"};

    for (char c = 'A'; c <= 'E'; c++) {
        std::unordered_set<std::string> refSeen;
        for (int i = 0; i < 9; i++) { //9 == num weeks
            for (int j = 0; j < 4; j++) { //4 games per week
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


void Solve(Schedule& ARC, std::vector<int>& refAssignments, std::vector<Schedule>& solutions, int weekIndex = 0, int gameIndex = 0) {
    // If all weeks are processed, check if one referee has 8 assignments and the rest 7 assignments
    if (weekIndex == 9) { //last week
        int countEight = 0;
        for (int count : refAssignments) {
            if (count < 7) return; // Less than 7 assignments is invalid
            if (count == 8) countEight++;
        }
        if (countEight == 1 && ARC.refsSeenEachTeam()){  // Only one referee should have 8 assignments
            //ADD IF TO SEE IF EACH REF HAS SEEN EACH TEAM
            std::cout << "Solution Found" << std::endl;
            solutions.push_back(ARC); // Add the valid solution to our list

        }
        return;  // Don't return true; continue searching for other solutions
    }

    // If all games for this week are processed, move to the next week
    if (gameIndex == 5) {
        Solve(ARC, refAssignments, solutions, weekIndex + 1, 0);
        return;
    }

    // If this game already has a ref, skip to the next game
    if (ARC.getRef(weekIndex, gameIndex) != ' ') {
        Solve(ARC, refAssignments, solutions, weekIndex, gameIndex + 1);
        return;
    }

    // Try to assign each ref crew
    for (char c = 'A'; c <= 'E'; c++) {
        if (refAssignments[c - 'A'] >= 8) continue;  // If ref already has 8 assignments, skip to the next ref

        if (ARC.isValid(weekIndex, gameIndex, c)) {
            ARC.assignRef(weekIndex, gameIndex, c);
            
            // Only increment the count for games 0-3
            if (gameIndex < 4) {
                refAssignments[c - 'A']++;  // Increment the count for the assigned ref
            }

            Solve(ARC, refAssignments, solutions, weekIndex, gameIndex + 1);
            
            // Backtrack
            ARC.assignRef(weekIndex, gameIndex, ' ');
            
            // Only decrement the count for games 0-3
            if (gameIndex < 4) {
                refAssignments[c - 'A']--;  // Decrement the count for the ref
            }
        }
    }
}

//--------------------------------------------------------//

int main() {
    time_t start,end;
    time(&start);

  //initialize weeks. 9 weeks(0-8), 4 games per week(0-3)
    std::vector<std::vector<Game>> weeks = {
        {Game("BVU","LOR"), Game("SIM","DBQ"), Game("WAR","CEN"), Game("COE","LUT"), Game("BYE","BYE")},
        {Game("LOR","SIM"), Game("CEN","BVU"), Game("DBQ","COE"), Game("NWU","WAR"), Game("BYE","BYE")},
        {Game("COE","LOR"), Game("BVU","NWU"), Game("LUT","DBQ"), Game("SIM","CEN"), Game("BYE","BYE")},
        {Game("LOR","LUT"), Game("WAR","BVU"), Game("NWU","SIM"), Game("CEN","COE"), Game("BYE","BYE")},
        {Game("DBQ","LOR"), Game("SIM","WAR"), Game("LUT","CEN"), Game("COE","NWU"), Game("BYE","BYE")},
        {Game("BVU","SIM"), Game("CEN","DBQ"), Game("WAR","COE"), Game("NWU","LUT"), Game("BYE","BYE")},
        {Game("LOR","CEN"), Game("COE","BVU"), Game("DBQ","NWU"), Game("LUT","WAR"), Game("BYE","BYE")},
        {Game("NWU","LOR"), Game("BVU","LUT"), Game("WAR","DBQ"), Game("SIM","COE"), Game("BYE","BYE")},
        {Game("LOR","WAR"), Game("DBQ","BVU"), Game("CEN","NWU"), Game("LUT","SIM"), Game("BYE","BYE")},
        };

    //counts # of assignments for each ref
    std::vector<int> refAssignments(5, 0);  // Initialize with 0 for all refs (A=0, B=1, C=2, D=3, E=4)
    
    // std::vector<std::unordered_set<std::string>> teamEachRefSeen = {{},{},{},{},{}}; // (A=0, B=1, C=2, D=3, E=4)

    //stores solutions
    std::vector<Schedule> solutions;

    //Initialize schedule
    Schedule ARC(weeks);


    Solve(ARC, refAssignments, solutions); //eventually add customizable constraints

    std::ofstream output("Solutions.txt");

    output << "Number of solutions: " << solutions.size() << std::endl;
 

    for (const Schedule& sol : solutions) {
        sol.outputFile(output);
        output << "------------------\n";
    }

    time(&end);
    std::cout << "runtime: " << end-start << std::endl; 

    return 0;
}

//--------------------------------------------------//

void Schedule::printSchedule() const {
    for (int i = 0; i < 9; i++) {
        std::cout << "Week " << i + 3 << std::endl;
        for (int j = 0; j < 4; j++) {
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

    if(game == 4 && week > 0){
        if(weeks[week-1][4].getRefCrew() == refCrew){
            return false;
        }
    }
    
    //check if ref is already assigned for week
    for(int i = 0; i<4; i++){
        if(weeks[week][i].getRefCrew() == refCrew){
            return false;
        }
    }

    //check if crew saw same team last week
    if(week > 0){
        for(int i = 0; i<4; i++){
            if(weeks[week-1][i].getRefCrew() == refCrew){
                if(weeks[week-1][i].getAwayTeam() == weeks[week][game].getAwayTeam() || weeks[week-1][i].getAwayTeam() == weeks[week][game].getHomeTeam() || weeks[week-1][i].getHomeTeam() == weeks[week][game].getAwayTeam() || weeks[week-1][i].getHomeTeam() == weeks[week][game].getHomeTeam()){
                    return false;
                }
            }
        }
    }

    //check if crew reffed home team at all
    for(int i = 0; i<9; i++){
        for(int j = 0; j<4; j++){
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
    for(int i = 0; i < 9; i++){ 
        for(int j = 0; j < 4; j++){
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

    for (int i = 0; i < 9; i++) {
        output << "Week " << i + 3 << std::endl;
        for (int j = 0; j < 4; j++) {
            output << weeks[i][j].getAwayTeam() << " @ " << weeks[i][j].getHomeTeam() << " Refs: " << weeks[i][j].getRefCrew() << std::endl;
        }
        output << std::endl;
    }
}
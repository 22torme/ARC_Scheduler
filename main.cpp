#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

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
    explicit Schedule(const std::vector<std::vector<Game>>& weeks) : weeks(weeks) {}

    void assignRef(int week, int game, char refCrew) { weeks[week][game].setRefCrew(refCrew); }
    Game& getGame(int week, int game) { return weeks[week][game]; }
    std::vector<Game> getWeek(int week) const { return weeks[week]; }
    void printSchedule() const;
    char getRef(int week, int game) const { return weeks[week][game].getRefCrew(); }
    bool isComplete() const;
    bool isValid(int week, int game, char refCrew) const;
    void outputFile(std::ofstream& output) const;

private:
    std::vector<std::vector<Game>> weeks;
};

// bool Solve(Schedule& ARC, std::vector<int>& refAssignments, int weekIndex = 0, int gameIndex = 0){
//     // If all weeks are processed, check if every referee has been assigned to at least 7 games
//     if (weekIndex == 9) {
//         for (int count : refAssignments) {
//             if (count < 7) {
//                 return false;  // Not all referees have the minimum assignments
//             }
//         }
//         return true;  // Every referee has been assigned to at least 7 games, and the schedule is complete
//     }

//     // If all games for this week are processed, move to the next week
//     if (gameIndex == 5) {
//         return Solve(ARC, refAssignments, weekIndex + 1, 0);
//     }

//     // If this game already has a ref, skip to the next game
//     if (ARC.getRef(weekIndex, gameIndex) != ' ') {
//         return Solve(ARC, refAssignments, weekIndex, gameIndex + 1);
//     }

//     // Try to assign each ref crew
//     for (char c = 'A'; c <= 'E'; c++) {
//         if (ARC.isValid(weekIndex, gameIndex, c)) {
//             ARC.assignRef(weekIndex, gameIndex, c);
//             refAssignments[c - 'A']++;  // Increment the count for the assigned ref
//             if (Solve(ARC, refAssignments, weekIndex, gameIndex + 1)) {
//                 return true;  // If further assignments are successful, return true
//             }
//             // Backtrack
//             ARC.assignRef(weekIndex, gameIndex, ' ');
//             refAssignments[c - 'A']--;  // Decrement the count for the ref

//         }
//     }

//     return false;  // No valid assignment for this game
// }

// bool Solve(Schedule& ARC, std::vector<int>& refAssignments, int weekIndex = 0, int gameIndex = 0) {
//     // If all weeks are processed, check if one referee has 8 assignments and the rest 7 assignments
//     if (weekIndex == 9) {
//         int countEight = 0;
//         for (int count : refAssignments) {
//             if (count < 7) return false; // Less than 7 assignments is invalid
//             if (count == 8) countEight++;
//         }
//         return countEight == 1;  // Only one referee should have 8 assignments
//     }

//     // If all games for this week are processed, move to the next week
//     if (gameIndex == 5) {
//         return Solve(ARC, refAssignments, weekIndex + 1, 0);
//     }

//     // If this game already has a ref, skip to the next game
//     if (ARC.getRef(weekIndex, gameIndex) != ' ') {
//         return Solve(ARC, refAssignments, weekIndex, gameIndex + 1);
//     }

//     // Try to assign each ref crew
//     for (char c = 'A'; c <= 'E'; c++) {
//         if (refAssignments[c - 'A'] >= 8) continue;  // If ref already has 8 assignments, skip to the next ref

//         if (ARC.isValid(weekIndex, gameIndex, c)) {
//             ARC.assignRef(weekIndex, gameIndex, c);
            
//             // Only increment the count for games 0-3
//             if (gameIndex < 4) {
//                 refAssignments[c - 'A']++;  // Increment the count for the assigned ref
//             }

//             if (Solve(ARC, refAssignments, weekIndex, gameIndex + 1)) {
//                 return true;  // If further assignments are successful, return true
//             }
            
//             // Backtrack
//             ARC.assignRef(weekIndex, gameIndex, ' ');
            
//             // Only decrement the count for games 0-3
//             if (gameIndex < 4) {
//                 refAssignments[c - 'A']--;  // Decrement the count for the ref
//             }
//         }
//     }

//     return false;  // No valid assignment for this game
// }

void Solve(Schedule& ARC, std::vector<int>& refAssignments, std::vector<Schedule>& solutions, int weekIndex = 0, int gameIndex = 0) {
    // If all weeks are processed, check if one referee has 8 assignments and the rest 7 assignments
    if (weekIndex == 9) {
        int countEight = 0;
        for (int count : refAssignments) {
            if (count < 7) return; // Less than 7 assignments is invalid
            if (count == 8) countEight++;
        }
        if (countEight == 1) {  // Only one referee should have 8 assignments
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

int main() {

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

    std::vector<int> refAssignments(5, 0);  // Initialize with 0 for all refs (A=0, B=1, C=2, D=3, E=4)
    
    std::vector<Schedule> solutions;

    Schedule ARC(weeks);


    Solve(ARC, refAssignments, solutions);

    std::cout << solutions.size() << std::endl;

    std::ofstream output("Solutions.txt");

    for (const Schedule& sol : solutions) {
        sol.outputFile(output);
        output << "------------------\n";
    }

    // if (Solve(ARC, refAssignments, 0, 0)){
    //     ARC.printSchedule();
    // } else {
    //     std::cout << "No solution found!" << std::endl;
    // }

    return 0;
}

// bool Solve(Schedule& ARC) {

//     for (int i = 0; i < 9; i++) {
//         for (int j = 0; j < 5; j++) {

//             if (ARC.getRef(i, j) == ' ') {
//                 for (char c = 'A'; c <= 'E'; c++) {
//                     if (ARC.isValid(i, j, c)) {

//                         ARC.assignRef(i, j, c);

//                         if (Solve(ARC)) {
//                             return true;

//                         } else{
//                         ARC.assignRef(i, j, ' ');
//                         }
//                     } else {
//                         ARC.assignRef(i, j, ' ');

//                     }
//                 }
//                 return false;
//             }
//         }
//     }
//     return true;
// }


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
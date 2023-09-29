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
        std::string getAwayTeam(){return awayTeam;};
        std::string getHomeTeam(){return homeTeam;};

    private:
        char refCrew = ' '; // A-E
        std::string awayTeam;
        std::string homeTeam;
};


class Schedule{
    public:
        //data structure
        std::vector<std::vector<Game>> weeks;

        //constructor
        Schedule(std::vector<std::vector<Game>> weeks){
            this->weeks = weeks;
        }

        //assign refs
        void assignRef(int week, int game, char refCrew){weeks[week][game].setRefCrew(refCrew);};

        //return game
        Game getGame(int week, int game){return weeks[week][game];};

        //return week
        std::vector<Game> getWeek(int week){return weeks[week];};

        //print schedule
        void printSchedule(){
            for(int i = 0; i < 9; i++){
                std::cout << "Week " << i+3 << std::endl;
                for(int j = 0; j < 4; j++){
                    std::cout << weeks[i][j].getAwayTeam() << " @ " << weeks[i][j].getHomeTeam() << " Refs: " << weeks[i][j].getRefCrew() << std::endl;
                }
                std::cout << std::endl;
            }
        }

        bool isComplete(){
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 4; j++){
                    if(weeks[i][j].getRefCrew() == ' '){
                        return false;
                    }
                }
            }
            return true;
        }

        bool isValid(int week, int game, char refCrew){

            //check if ref has NOT reffed this week and last week... return false if true
            int counter = 0;
            if(week > 0){
                for(int i = 0; i<4; i++){
                    if(weeks[week-1][i].getRefCrew() == refCrew || weeks[week][i].getRefCrew() == refCrew){
                        counter++;
                    }
                }
                if(counter == 0){
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

            //check if ref has seen same team more than twice
            int count = 0;
            for(int i = 0; i<9; i++){
                for(int j = 0; j<4; j++){
                    if(weeks[i][j].getRefCrew() == refCrew){
                        if(weeks[i][j].getAwayTeam() == weeks[week][game].getAwayTeam() || weeks[i][j].getAwayTeam() == weeks[week][game].getHomeTeam() || weeks[i][j].getHomeTeam() == weeks[week][game].getAwayTeam() || weeks[i][j].getHomeTeam() == weeks[week][game].getHomeTeam()){
                            count++;
                        }
                    }
                }
            }
            if(count > 2){
                return false;
            }


            return true;
        }

};


    
bool Solve(Schedule &ARC){
    
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 4; j++){

            if(ARC.getGame(i,j).getRefCrew() == ' '){

                for(char c = 'A'; c <= 'E'; c++){
                    if(ARC.isValid(i,j,c)){
                        ARC.assignRef(i,j,c);

                        if(Solve(ARC) == true){
                            return true;
                        } else{
                            ARC.assignRef(i,j,' ');
                        }
                    }
                }
                return false;
            }
        }
    }     
    return true;
}


int main(){

    //initialize weeks. 9 weeks(0-8), 4 games per week(0-3)
    std::vector<std::vector<Game>> weeks = {
        {Game("BVU","LOR"), Game("SIM","DBQ"), Game("WAR","CEN"), Game("COE","LUT")},
        {Game("LOR","SIM"), Game("CEN","BVU"), Game("DBQ","COE"), Game("NWU","WAR")},
        {Game("COE","LOR"), Game("BVU","NWU"), Game("LUT","DBQ"), Game("SIM","CEN")},
        {Game("LOR","LUT"), Game("WAR","BVU"), Game("NWU","SIM"), Game("CEN","COE")},
        {Game("DBQ","LOR"), Game("SIM","WAR"), Game("LUT","CEN"), Game("COE","NWU")},
        {Game("BVU","SIM"), Game("CEN","DBQ"), Game("WAR","COE"), Game("NWU","LUT")},
        {Game("LOR","CEN"), Game("COE","BVU"), Game("DBQ","NWU"), Game("LUT","WAR")},
        {Game("NWU","LOR"), Game("BVU","LUT"), Game("WAR","DBQ"), Game("SIM","COE")},
        {Game("LOR","WAR"), Game("DBQ","BVU"), Game("CEN","NWU"), Game("LUT","SIM")},
        };


    Schedule ARC(weeks);
    Solve(ARC);
    ARC.printSchedule();

    return 0;
}



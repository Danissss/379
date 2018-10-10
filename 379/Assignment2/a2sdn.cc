#include <iostream> 

#define MAX_NSW 7



using namespace std; 

void controller();
void switches(int argc, char **arguments);


// Print table like this:
// [srcIP lo, srcIP hi, destIP lo, destIP hi, actionType, actionVal, pri, pktCount]





static int nSwitch = 0;

void controller(){




}

void switches(int argc, char **arguments){




}















int main(int argc, char** argv) 
{ 
    
    if(argc < 3){
    	cout << "Too little input!" << endl;
    	return 0;
    }


    if (strcmp(argv[1],"cont") == 0){
    	nSwitch = stoi(argv[2]);
    	if (nSwitch > MAX_NSW){
    		cout << "Most 7 switches!" << endl;
    		return 0;
    	}else{
    		// printf("%d\n", nSwitch);
    		controller();
    	}
 
    }
    else if(strcmp(argv[1],"swi") == 0){
    	switches(argc,argv);
    }
    else{
    	cout << "Invalid arguments!" << endl;
   		return 0;
    }

} 

























































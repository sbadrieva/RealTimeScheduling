//
//  main.cpp
//  CSC767TeamProj
//
//  Created by Shokhina Badrieva on 4/18/22.
//
//
//  main.cpp
//  CSC767TeamProj
//
//  Created by Shokhina Badrieva on 4/18/22.
//

#include <iostream>
#include <queue>


using namespace std;

struct processInfo{
    
    int processID;
    int arrival_time;
    double period;
    int deadline;
    int upcoming_deadline;
    int upcoming_period;
    double execution_time;
    double remaining_execution_time;
    
    //The < operator decides which process is placed into the queue based on priority requirement. In the case of DF, the priority of a process is based on the deadline.
    bool operator<(const processInfo &o) const
        {
            return deadline> o.deadline;
        }
};


processInfo minPeriodProcess(processInfo processes[],int num_processes){
    //Since priority is decided on the inverse of the period for RMA, we use minPeriodProcess to return the process with the minimum period.
    
    int time=processes[0].upcoming_period;
    int chosenProcess=0;
    
    for(int i=1;i<num_processes;i++){
        if (processes[i].upcoming_period<time){
            chosenProcess=i;
            time=processes[i].upcoming_period;
        }
    }
    
    return processes[chosenProcess];
}


int gcd(int a, int b)
{
    if (b == 0)
        return a;
    
    return gcd(b, a % b);
}
 
int findlcm(int arr[], int n)
{
    int ans = arr[0];

    for (int i = 1; i < n; i++)
        ans = (((arr[i] * ans)) /
                (gcd(arr[i], ans)));
 
    return ans;
}



int main(int argc, const char * argv[]) {
    
    int num_processes,process_switch;
    
    //The following block of code will read in the necessary information from the input file
    cin >> num_processes >> process_switch;
    processInfo processes[num_processes];
    int pID;
    int arrival;
    int period;
    int deadline;
    double execution;
    for(int i=0;i<num_processes;i++){
        cin>>pID;
        processes[i].processID=pID-1;
        cin>>arrival;
        processes[i].arrival_time=arrival;
        cin>>execution;
        processes[i].execution_time=execution;
        cin>>deadline;
        processes[i].deadline=deadline;
        cin>>period;
        processes[i].period=period;
        processes[i].upcoming_deadline=processes[i].arrival_time;
        processes[i].upcoming_period=processes[i].arrival_time;
        //processes[i].execution_time=0.5;
        processes[i].remaining_execution_time=0.0;
    }


    //The following block of code will read in the argument from command line to decide which output the user is requesting
    char mode='o';
    //default mode
    if (argv[1]==NULL){
        mode='o';
    }
    else if(strcmp( argv[1], "d") == 0){
        mode='d';
    }
    else if (strcmp( argv[1], "v") == 0){
        mode='v';
    }
    else if (strcmp( argv[1], "a") == 0){
        mode='a';
        //put var for which algorithm using argv[2]
    }
    

    //In the 2D array , eventInfo, each row is a new event. First column is process number, second column is current time, third column is finish time,fourth column will take a value of 0 or -1 (0 means finished executing and -1 means not finished executing)
    double  eventInfo[10000][4];
    int eventID=0;
    
    
    //initialize the first process coming into the system and the time. Any process that has arrived (can be more than one process if arrival times are equal) is then added into the priority queue pq
    processInfo nextProcessToPreempt=minPeriodProcess(processes,num_processes);
    double time=nextProcessToPreempt.arrival_time;
    priority_queue <processInfo> pq;
    
    for(int i=0;i<num_processes;i++){
        if(time==processes[i].upcoming_period){
            
            //update next period of these processes
            processes[i].upcoming_period=processes[i].upcoming_period+processes[i].period;
            //update the deadline of these processes
            processes[i].upcoming_deadline=time+processes[i].deadline;

            pq.push(processes[i]);

        }
    }
    
    
    //In this part, end of simulation time is calculated using the lcm of all the processes. Some variables are also declared which will be used in the main simulation.
    int processPeriodArray[num_processes];
    for (int i=0;i<num_processes;i++){
        processPeriodArray[i]=processes[i].period;
    }
    
    int end_of_sim=findlcm(processPeriodArray, num_processes);
    int upcoming_period;
    double idle_time=0;
    bool isFeasible=true;
    processInfo curr_process;
    
    while((time<end_of_sim) && isFeasible){
        
        //if queue is empty
        if(pq.empty()){
            //Find smallest of next periods and add to queue
            nextProcessToPreempt=minPeriodProcess(processes,num_processes);
            upcoming_period=nextProcessToPreempt.upcoming_period;\
            //Code to find idle time
            double current_time=time;
            time=upcoming_period;
            idle_time+=time-current_time;
            
            //Add next processe/s entering the queue into pq
            for(int i=0;i<num_processes;i++){
                if(time==processes[i].upcoming_period){

                    //update next period of these processes
                    processes[i].upcoming_period=processes[i].upcoming_period+processes[i].period;
                    //update the deadline of these processes
                    processes[i].upcoming_deadline=time+processes[i].deadline;
                    pq.push(processes[i]);
                    
                }
            }
            
            //go to next iteration of while
            continue;
            
        }
        
        
        //If queue is not empty, get process in queue with highest priority
        curr_process = pq.top();
        pq.pop();
        
        //Update current event's process ID (which processes will execute)
        eventInfo[eventID][0]=curr_process.processID;
        //Update currect time process enetrs the system
        eventInfo[eventID][1]=time;
        
        
        //Get smallest period to check if pre-emption will be necessary
        nextProcessToPreempt=minPeriodProcess(processes,num_processes);
        upcoming_period=nextProcessToPreempt.upcoming_period;

        
        //Check if process will be able to complete before preemption. If not, update its remaining time.
        double timeOfProcessCompletion;
        if(curr_process.remaining_execution_time==0.0){
            timeOfProcessCompletion=curr_process.execution_time+time;
        }
        else{
            timeOfProcessCompletion=curr_process.remaining_execution_time+time;
            curr_process.remaining_execution_time=0.0;
        }
        
        
        
        //First case is current process will finish executing before next period, no need for pre-emption
        if(timeOfProcessCompletion<upcoming_period){
            if(timeOfProcessCompletion<=curr_process.upcoming_deadline){
                //process finished execution and time is updated
                time=timeOfProcessCompletion;
                
                //update end of process completion in eventInfo
                eventInfo[eventID][2]=timeOfProcessCompletion;
                //event finished completing so we will make it 0
                eventInfo[eventID][3]=0;
            }
            
            else{
                //If process doesn't complete by deadline, algorithm is not feasible
                isFeasible=false;
            }
        }
        
        //Second case is current process will finish executing after next period, so pre-emption is necessary
        else if(timeOfProcessCompletion>upcoming_period){
            
            if(timeOfProcessCompletion<=curr_process.upcoming_deadline){

                curr_process.remaining_execution_time=timeOfProcessCompletion-upcoming_period;
                
                for(int i=0;i<num_processes;i++){
                    if(upcoming_period==processes[i].upcoming_period){

                        //update next period of these processes
                        processes[i].upcoming_period=processes[i].upcoming_period+processes[i].period;
                        //update the deadline of these processes
                        processes[i].upcoming_deadline=timeOfProcessCompletion+processes[i].deadline;
                        pq.push(processes[i]);
        
                    }
                }
                
                pq.push(curr_process);
                time=upcoming_period;
                
                nextProcessToPreempt.upcoming_period=nextProcessToPreempt.upcoming_period+nextProcessToPreempt.period;
                //update time this event finished executing
                eventInfo[eventID][2]=time;
                //event didnt fully finish executing so mark it -1
                eventInfo[eventID][3]=-1;
            }
            
            else{
                //If process doesn't complete by deadline, algorithm is not feasible
                isFeasible=false;
            }
            
        }
        
        
        //Third case is current process will finish executing at the same time as next period, so pre-emption is necessary, but the current process finishes executing
        else if(timeOfProcessCompletion==upcoming_period){
            if(timeOfProcessCompletion<=curr_process.upcoming_deadline){

                //Since time is equal to period add those processes with the upcoming period into the queue
                for(int i=0;i<num_processes;i++){
                    if(timeOfProcessCompletion==processes[i].upcoming_period){

                        //update next period of these processes
                        processes[i].upcoming_period=processes[i].upcoming_period+processes[i].period;
                        //update the deadline of these processes
                        processes[i].upcoming_deadline=timeOfProcessCompletion+processes[i].deadline;
                        pq.push(processes[i]);
                        
                    }
                }
                
                
                time=timeOfProcessCompletion;

                //update end of process completion in eventInfo
                eventInfo[eventID][2]=timeOfProcessCompletion;
                //event finished completing so we will mark it as 0
                eventInfo[eventID][3]=0;
            }
            else{
                //If process doesn't complete by deadline, algorithm is not feasible
                isFeasible=false;
            }
        
        }
    
        //update time considering the process switch time
        time=time+process_switch;
        eventID++;
        
    }
    
    
    //based on user specified arguments, the following block of code will decide which output format to provide the user with
    if(mode=='o'){
        cout<<endl<<endl;
        cout<<"Rate Monotonic Scheduling Algorithm(RM):"<<endl;
        if(isFeasible==true){
            cout<<"There is a feasible schedule produced"<<endl;
            cout<<"Total Time required is "<<time<<endl;
            double cpu_utilization=(time-idle_time)/time;
            cout<<"CPU Utilization is "<<cpu_utilization<<endl;
            
        }
        else{
            cout<<"There is no feasible schedule produced"<<endl;

            
        }
    }
    
    if(mode=='a'){
        cout<<endl<<endl;
        cout<<"Rate Monotonic Scheduling Algorithm(RM):"<<endl;
        if(isFeasible==true){
            cout<<"There is a feasible schedule produced"<<endl;
            cout<<"Total Time required is "<<time<<endl;
            double cpu_utilization=(time-idle_time)/time;
            cout<<"CPU Utilization is "<<cpu_utilization<<endl;
            
        }
        else{
            cout<<"There is no feasible schedule produced"<<endl;
            
        }
    }


    if(mode=='d'){
        cout<<endl<<endl;
        cout<<"Rate Monotonic Scheduling Algorithm(RM):"<<endl;
        if(isFeasible==true){
            cout<<"There is a feasible schedule produced"<<endl;
            cout<<"Total Time required is "<<time<<endl;
            double cpu_utilization=(time-idle_time)/time;
            cout<<"CPU Utilization is "<<cpu_utilization<<endl;
            
            //print process details
            cout<<endl;
            for(int i=0;i<num_processes;i++){
                cout<<"Process "<<i+1<<endl;
                cout<<"Arrival time: "<<processes[i].arrival_time<<endl;;
                cout<<"Service time: "<<processes[i].execution_time<<endl;;
                cout<<"Relative deadline: "<<processes[i].deadline<<endl;;
                cout<<"Period: "<<processes[i].period<<endl;
                cout<<"Finish time: ";
                for(int j=0;j<eventID;j++){
                    
                    if(eventInfo[j][0]==i){
                        if(eventInfo[j][3]==0){
                            
                            cout<<eventInfo[j][2]<<"   ";
                            
                            
                        }
                    }
                
                }
                cout<<endl<<endl;
            }
    
        }
        else{
            double cpu_utilization=(time-idle_time)/time;
            cout<<"CPU Utilization is "<<cpu_utilization<<endl;
            cout<<"There is no feasible schedule produced. Schedule can be feasible from time 0 to time "<<time<<" units. At time "<<time<<" process "<<curr_process.processID+1<<" missed the deadline. From time 0 to "<<time<<" CPU Utilization is "<< cpu_utilization<<endl;
            
            //print process details
            cout<<endl;
            for(int i=0;i<num_processes;i++){
                cout<<"Process "<<i+1<<endl;
                cout<<"Arrival time: "<<processes[i].arrival_time<<endl;;
                cout<<"Service time: "<<processes[i].execution_time<<endl;;
                cout<<"Relative deadline: "<<processes[i].deadline<<endl;;
                cout<<"Period: "<<processes[i].period<<endl;
                cout<<"Finish time: ";
                for(int j=0;j<eventID;j++){
                    
                    if(eventInfo[j][0]==i){
                        if(eventInfo[j][3]==0){
                            if(eventInfo[j][2]!=0){
                                cout<<eventInfo[j][2]<<"   ";
                            }
                        }
                    }
                }
                cout<<endl<<endl;
            }
        }
        
    }
    if(mode=='v'){
        cout<<endl<<endl;
        cout<<"Rate Monotonic Scheduling Algorithm(RM):"<<endl;

        for(int i=0;i<eventID;i++){
            if(eventInfo[i][3]==0){
                if(eventInfo[i][2]!=0){
                    cout<<"At time: "<<eventInfo[i][1]<<", process "<<eventInfo[i][0]+1<<" is scheduled until "<<eventInfo[i][2]<<endl;
                }

            }
            else{
                if(eventInfo[i][2]!=0){
                    cout<<"At time: "<<eventInfo[i][1]<<", process "<<eventInfo[i][0]+1<<" is scheduled until "<<eventInfo[i+1][1]<<endl;
                    cout<<"At time: "<<eventInfo[i+1][1]<<", process "<<eventInfo[i][0]+1<<" is preempted by process "<<eventInfo[i+1][0]+1<<endl;
                }
                    
            }
        }
    }
    
    
    return 0;
}






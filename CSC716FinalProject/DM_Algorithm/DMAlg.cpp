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
    
    bool operator<(const processInfo &o) const
        {
            return deadline> o.deadline;
        }
};


processInfo minPeriodProcess(processInfo processes[],int num_processes){
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
 
// Returns LCM of array elements
int findlcm(int arr[], int n)
{
    // Initialize result
    int ans = arr[0];
 
    // ans contains LCM of arr[0], ..arr[i]
    // after i'th iteration,
    for (int i = 1; i < n; i++)
        ans = (((arr[i] * ans)) /
                (gcd(arr[i], ans)));
 
    return ans;
}



int main(int argc, const char * argv[]) {
    
    int num_processes,process_switch;
    
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
    

    //cout << argv[1] << endl;
    //in event info, each row is a new event. First column is process number, second column is current time, third column is finish time,fourth column will take a value of 0 or -1 (0 means finished executing and -1 means not finished executing)
    double  eventInfo[10000][4];
    int eventID=0;
    
    
    
    processInfo nextProcessToPreempt=minPeriodProcess(processes,num_processes);
    double time=nextProcessToPreempt.arrival_time;
    
    priority_queue <processInfo> pq;
    //priority_queue<processInfo, vector<processInfo>, ComparePriority> pq;
    
    for(int i=0;i<num_processes;i++){
        if(time==processes[i].upcoming_period){
            //update next period of these processes
            processes[i].upcoming_period=processes[i].upcoming_period+processes[i].period;
            //update the deadline of these processes
            processes[i].upcoming_deadline=time+processes[i].deadline;

            pq.push(processes[i]);
            //cout<<"The following process is in the queue: "<<processes[i].processID<<endl;
            //cout<<"Its upcoming period is : "<<processes[i].upcoming_period<<endl;
            //cout<<"Its upcoming deadline is : "<<processes[i].upcoming_deadline<<endl;

        }
    }
    
    
    //used to find lcm
    int processPeriodArray[num_processes];
    for (int i=0;i<num_processes;i++){
        processPeriodArray[i]=processes[i].period;
    }
    
    int end_of_sim=findlcm(processPeriodArray, num_processes);
    //cout<<endl<<endl<<"Lcm is "<<end_of_sim<<endl;
    int upcoming_period;
    double idle_time=0;
    bool isFeasible=true;
    processInfo curr_process;
    
    //end of sim should be lcm of all periods
    while((time<end_of_sim) && isFeasible){
        
        //cout<<"Time is "<<time<<endl;
        //if queue is empty
        if(pq.empty()){
            //cout<<"Queue is empty. Idle time"<<endl;
            //find smallest of next periods and add to queue
            nextProcessToPreempt=minPeriodProcess(processes,num_processes);
            upcoming_period=nextProcessToPreempt.upcoming_period;\
            //cout<<"upcoming period is: "<<upcoming_period<<endl;
            //code to find idle time
            double current_time=time;
            time=upcoming_period;
            idle_time+=time-current_time;
            //cout<<"Idle time so far is "<<idle_time<<endl;
            
            
            for(int i=0;i<num_processes;i++){
                if(time==processes[i].upcoming_period){

                    //update next period of these processes
                    processes[i].upcoming_period=processes[i].upcoming_period+processes[i].period;
                    //update the deadline of these processes
                    processes[i].upcoming_deadline=time+processes[i].deadline;
                    pq.push(processes[i]);
                    //cout<<"The following process is added to the queue: "<<processes[i].processID<<endl;
                    //cout<<"Its upcoming period is : "<<processes[i].upcoming_period<<endl;
                    //cout<<"Its upcoming deadline is : "<<processes[i].upcoming_deadline<<endl;

                    
                }
            }
            
            //cout<<endl<<endl;

            continue;
            
        }
        
        
        //get process in queue with highest priority
        curr_process = pq.top();
        pq.pop();
        //cout<<"current process is "<<curr_process.processID<<endl;
        //cout<<"current process upcoming period is "<<curr_process.upcoming_period<<endl;
        //cout<<"current process upcoming deadline is "<<curr_process.upcoming_deadline<<endl;
        
        
        //update current event's process ID (which processes will execute)
        eventInfo[eventID][0]=curr_process.processID;
        //update currect time process eneters the system
        eventInfo[eventID][1]=time;
        
        
        //otherwise get smallest period to check if pre-emption will be necessary
        nextProcessToPreempt=minPeriodProcess(processes,num_processes);
        upcoming_period=nextProcessToPreempt.upcoming_period;
        //cout<<"Next process to preempt is "<<nextProcessToPreempt.processID<<" at time "<<upcoming_period<<endl;
        
        
        double timeOfProcessCompletion;
        if(curr_process.remaining_execution_time==0.0){
            timeOfProcessCompletion=curr_process.execution_time+time;
        }
        else{
            timeOfProcessCompletion=curr_process.remaining_execution_time+time;
            curr_process.remaining_execution_time=0.0;
        }
        //cout<<"Current process will finish executing at "<<timeOfProcessCompletion<<endl;
        
        
        
        
        
        //first case is current process will finish executing before next period, no need for pre-emption
        if(timeOfProcessCompletion<upcoming_period){
            if(timeOfProcessCompletion<=curr_process.upcoming_deadline){
                //process finished execution and time is updated
                time=timeOfProcessCompletion;
                //cout<<"current process will finish executing before next preemption at time "<<timeOfProcessCompletion<<endl;
                
                //update end of process completion in eventInfo
                eventInfo[eventID][2]=timeOfProcessCompletion;
                //event finished completing so we will make it 0
                eventInfo[eventID][3]=0;
            }
            
            else{
                isFeasible=false;
                //cout<<"not feasible!"<<endl;
            }
        }
        
        
        
        
        else if(timeOfProcessCompletion>upcoming_period){
            
            if(timeOfProcessCompletion<=curr_process.upcoming_deadline){

                curr_process.remaining_execution_time=timeOfProcessCompletion-upcoming_period;
                //cout<<"Remaining execution time is "<<curr_process.remaining_execution_time<<endl;
                
                
                
                
                for(int i=0;i<num_processes;i++){
                    if(upcoming_period==processes[i].upcoming_period){

                        //update next period of these processes
                        processes[i].upcoming_period=processes[i].upcoming_period+processes[i].period;
                        //update the deadline of these processes
                        processes[i].upcoming_deadline=timeOfProcessCompletion+processes[i].deadline;
                        pq.push(processes[i]);
                        //cout<<"The following process is added to the queue: "<<processes[i].processID<<endl;
                        //cout<<"Its upcoming period is : "<<processes[i].upcoming_period<<endl;
                        //cout<<"Its upcoming deadline is : "<<processes[i].upcoming_deadline<<endl;


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
                isFeasible=false;
                //cout<<"not feasible!"<<endl;
            }
            
        }
        
        
        
        
        else if(timeOfProcessCompletion==upcoming_period){
            if(timeOfProcessCompletion<=curr_process.upcoming_deadline){
                //cout<<"equal!"<<endl;
                //addToQueue(processes, pq, upcoming_period, num_processes);
                //for clarification
                
                //since time is equal to period add those processes with the upcoming period into the queue
                for(int i=0;i<num_processes;i++){
                    if(timeOfProcessCompletion==processes[i].upcoming_period){

                        //update next period of these processes
                        processes[i].upcoming_period=processes[i].upcoming_period+processes[i].period;
                        //update the deadline of these processes
                        processes[i].upcoming_deadline=timeOfProcessCompletion+processes[i].deadline;
                        pq.push(processes[i]);
                        //cout<<"The following process is added to the queue: "<<processes[i].processID<<endl;
                        //cout<<"Its upcoming period is : "<<processes[i].upcoming_period<<endl;
                        //cout<<"Its upcoming deadline is : "<<processes[i].upcoming_deadline<<endl;
                        //cout<<"top of pq is "<<pq.top().processID<<endl;

                    }
                }
                
                
                time=timeOfProcessCompletion;

                //update end of process completion in eventInfo
                eventInfo[eventID][2]=timeOfProcessCompletion;
                //event finished completing so we will mark it as 0
                eventInfo[eventID][3]=0;
            }
            else{
                isFeasible=false;
                //cout<<"not feasible!"<<endl;
            }
        
        }
    
        //update time considering the process switch time
        time=time+process_switch;
        
        //cout<<"Process: "<<eventInfo[eventID][0]<<" , arrived at "<<eventInfo[eventID][1]<<" ,finished at "<<eventInfo[eventID][2]<<endl;
        eventID++;
        //cout<<endl<<endl;
        
    }
    /*
    cout<<"Process info table"<<endl;
    for(int i=0;i<eventID;i++){
        cout<<"Process: "<<eventInfo[i][0]<<" , arrived at "<<eventInfo[i][1]<<" ,finished at "<<eventInfo[i][2]<<" and flag is "<<eventInfo[i][3]<<endl;
    }
    */
    if(mode=='o'){
        cout<<endl<<endl;
        cout<<"DM Scheduling Algorithm:"<<endl;
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
        cout<<"DM Scheduling Algorithm:"<<endl;
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
        cout<<"DM Scheduling Algorithm:"<<endl;
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
        cout<<"DM Scheduling Algorithm:"<<endl;

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

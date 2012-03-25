#ifndef PROGRESS_METER_H
#define PROGRESS_METER_H

/**
 *  Progress meter class that records how far along the 
 *  rendering process is and how effeciently we are tracing rays.
 * 
 *  TODO: Implement this class
 *
 */
class ProgressMeter{
public:

    ProgressMeter(int totalWork){}

    void startMeter(int totalWork){}

    void addProgress(int prog){}

    void setProgress(int amt){}

    void finished(){}

private:
    int Ntotal; //Number of "things" to do total
    int Ndone ; //Numer of "things" that have been done so far
};


#endif //PROGRESS_METER_H

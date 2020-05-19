//
// Created by geier on 18/01/2020.
//

#ifndef LIVEVIDEO10MS_TIMEHELPER_HPP
#define LIVEVIDEO10MS_TIMEHELPER_HPP

#include <AndroidLogger.hpp>
#include <chrono>

class AvgCalculator{
private:
    long sum=0;
    long sumCount=0;
public:
    AvgCalculator() = default;

    void add(long x){
        sum+=x;
        sumCount++;
    }
    long getAvg(){
        if(sumCount==0)return 0;
        return sum/sumCount;
    }
    void reset(){
        sum=0;
        sumCount=0;
    }
};

class RelativeCalculator{
private:
    long sum=0;
    long sumAtLastCall=0;
public:
    RelativeCalculator() = default;
    void add(unsigned long x){
        sum+=x;
    }
    long getDeltaSinceLastCall() {
        long ret = sum - sumAtLastCall;
        sumAtLastCall = sum;
        return ret;
    }
    long getAbsolute(){
        return sum;
    }
};

class MeasureExecutionTime{
private:
    const std::chrono::steady_clock::time_point begin;
    const std::string functionName;
    const std::string tag;
public:
    MeasureExecutionTime(const std::string& tag,const std::string& functionName):functionName(functionName),tag(tag),begin(std::chrono::steady_clock::now()){}
    ~MeasureExecutionTime(){
        const auto duration=std::chrono::steady_clock::now()-begin;
        LOGD(tag)<<"Execution time for "<<functionName<<" is "<<std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()<<"ms";
    }
};

// Macro to measure execution time of a specific function.
// See https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c/61886741#61886741
// Example output: ExecutionTime: For DecodeMJPEGtoANativeWindowBuffer is 54ms
// Resolve: use __PRETTY_FUNCTION__ or __FUNCTION__
// __CLASS_NAME__ comes from AndroidLogger
#ifndef MEASURE_FUNCTION_EXECUTION_TIME
#define MEASURE_FUNCTION_EXECUTION_TIME const MeasureExecutionTime measureExecutionTime(__CLASS_NAME__,__FUNCTION__);
#endif


#endif //LIVEVIDEO10MS_TIMEHELPER_HPP
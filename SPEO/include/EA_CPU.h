#ifndef __CPU_EA_LIBRARY_H__
#define __CPU_EA_LIBRARY_H__
#include <stdio.h>
#include "config.h"
#include "CEC2014.h"
#include "random.h"
#include <sstream>

struct DEInfo
{
    double CR;
    double F;
    int strategy_ID;
};


class EA_CPU
{
protected:
    ProblemInfo             problem_info_;
    int                     pop_size_;
    CEC2014                 cec2014_;
    Random                  random_;
public:
                            EA_CPU();
                            ~EA_CPU();
    virtual int             InitilizePopulation(Population & population);
    virtual int             Initilize(ProblemInfo problem_info, IslandInfo island_info);
    virtual int             Unitilize();


    Individual              FindBestIndividual(Population & population);
    virtual string          GetParameters() = 0;
    virtual int             Run(Population & population) = 0;

    real                    CheckBound(real to_check_elements, real min_bound, real max_bound);
#ifdef DEBUG
    void                    FprintPopulation(Population &population);
#endif
};

class DE_CPU : public EA_CPU
{
private:
    DEInfo                  DE_info_;
    Population              candidate_;
    int                     Reproduce(Population & population);
    int                     SelectSurvival(Population & population);
    virtual int             InitilizePopulation(Population & population);

public:
                            DE_CPU();
                            ~DE_CPU();
    virtual int             Initilize(ProblemInfo problem_info, IslandInfo island_info);
    virtual int             Unitilize();
    virtual int             Run(Population & population);
    virtual string          GetParameters();
};


#endif

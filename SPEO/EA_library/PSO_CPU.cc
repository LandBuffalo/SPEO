#include "EA_CPU.h"

PSO_CPU::PSO_CPU()
{

}

PSO_CPU::~PSO_CPU()
{
    EA_CPU::Unitilize();
}

string PSO_CPU::GetParameters()
{
    string str;
    ostringstream temp1, temp2, temp3;
    string parameters = "C1/C2/W=";
    double C1 = PSO_info_.C1;
    temp1 << C1;
    str = temp1.str();
    parameters.append(str);

    parameters.append("/");
    double C2 = PSO_info_.C2;
    temp2 << C2;
    str = temp2.str();
    parameters.append(str);

    parameters.append("/");
    double W = PSO_info_.W;
    temp3 << W;
    str = temp3.str();
    parameters.append(str);

    return parameters;
}

int PSO_CPU::Initilize(ProblemInfo problem_info, int pop_size)
{
	EA_CPU::Initilize(problem_info, pop_size);
    PSO_info_.C1 = 1.49445;
	PSO_info_.C2 = 1.49445;
	PSO_info_.W = 0.729;

	double VRmax = problem_info.max_bound;
	double VRmin = problem_info.min_bound;
	PSO_info_.Vmax = 0.2 * (VRmax - VRmin);
}
int PSO_CPU::InitilizePopulation(Population & population)
{
    EA_CPU::InitilizePopulation(population);

    pbest_ = population;
    gbest_ = pbest_[0];
    velocity_ = population;
    return 0;
}

int PSO_CPU::Unitilize()
{
    return 0;
}

int PSO_CPU::Optimizer(Population & population)
{
    for (int i = 0; i < pop_size_; i++)
    {
        for (int j = 0; j < problem_info_.dim; j++)
        {
            double rand1 = random_.RandDoubleUnif(0, 1);
            double rand2 = random_.RandDoubleUnif(0, 1);
            real tmp_element = population[i].elements[j];
            real tmp_velocity = PSO_info_.W * velocity_[i].elements[j] + \
            rand1 * PSO_info_.C1 * (gbest_.elements[j] - tmp_element) + rand2 * PSO_info_.C2 * (pbest_[i].elements[j] - tmp_element);

            if (tmp_velocity > PSO_info_.Vmax)
                tmp_velocity = PSO_info_.Vmax;
            else if(tmp_velocity < -PSO_info_.Vmax)
                tmp_velocity = -PSO_info_.Vmax;

            velocity_[i].elements[j] = tmp_velocity;
            population[i].elements[j] += tmp_velocity;
            population[i].elements[j] = CheckBound(population[i].elements[j], problem_info_.min_bound, problem_info_.max_bound);
        }
        population[i].fitness_value = cec2014_.EvaluateFitness(population[i].elements);
    }
    return 0;
}

int PSO_CPU::UpdateGbest()
{
    gbest_ = pbest_[0];
    for (int i = 1; i < pop_size_; i++)
    {
        if (gbest_.fitness_value < pbest_[i].fitness_value)
            gbest_ = pbest_[i];
    }

    return 0;
}

int PSO_CPU::UpdatePbest(Population & population)
{
    for (int i = 0; i < pop_size_; i++)
    {
        if (population[i].fitness_value < pbest_[i].fitness_value)
            pbest_[i] = population[i];
    }

    return 0;
}


int PSO_CPU::Run(Population & population)
{
    Optimizer(population);
    UpdatePbest(population);
    UpdateGbest();

    return 0;
}

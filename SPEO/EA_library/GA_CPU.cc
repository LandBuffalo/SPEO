#include "EA_CPU.h"

GA_CPU::GA_CPU()
{

}

GA_CPU::~GA_CPU()
{
    EA_CPU::Unitilize();
}

string GA_CPU::GetParameters()
{
    string str;
    ostringstream temp1, temp2, temp3;
    string parameters = "CR/MR/Pool=";

    double CR = GA_info_.CR;
    temp1<<CR;
    str=temp1.str();
    parameters.append(str);

    parameters.append("/");
    double MR = GA_info_.MR;
    temp2 << MR;
    str=temp2.str();
    parameters.append(str);

    parameters.append("/");
    double pool = GA_info_.pool_size;
    temp3<<pool;
    str=temp3.str();
    parameters.append(str);
    if(GA_info_.selection_strategy == 0)
        parameters.append("_tournament");
    else if(GA_info_.selection_strategy == 1)
        parameters.append("_roulette");

    return parameters;
}

int GA_CPU::Initilize(ProblemInfo problem_info, int pop_size)
{
	EA_CPU::Initilize(problem_info, pop_size);
    GA_info_.CR = 0.6;
	GA_info_.MR = 0.001;
	GA_info_.selection_strategy = 0;
	GA_info_.pool_size = 2;
}
int GA_CPU::InitilizePopulation(Population & population)
{
    EA_CPU::InitilizePopulation(population);
    candidate_ = population;
    return 0;
}
int GA_CPU::Unitilize()
{
    return 0;
}

vector<int> GA_CPU::SelectParents(Population & population, vector<int> parents_index_pool)
{
    vector<int> parent_index;
    vector<real> parents_fitness_value_pool;
    for (int i = 0; i < GA_info_.pool_size; i++)
        parents_fitness_value_pool.push_back(population[parents_index_pool[i]].fitness_value);

    for(int parents_num = 0; parents_num < 2; parents_num++)
    {
        real best_fitness_value = parents_fitness_value_pool[0];
        int local_best_index = 0;

        for (int i = 1; i < GA_info_.pool_size; i++)
        {
            if (best_fitness_value > parents_fitness_value_pool[i])
            {
                best_fitness_value = parents_fitness_value_pool[i];
                local_best_index = i;
            }
        }
        parent_index.push_back(parents_index_pool[local_best_index]);
        parents_fitness_value_pool[local_best_index] = 1e20;
    }

    return parent_index;
}

int GA_CPU::Reproduce(Population & population)
{
    Individual best_individual = FindBestIndividual(population);

    for (int i = 0; i < pop_size_; i = i + 2)
    {
        vector<int> parents_index_pool = random_.Permutate(pop_size_, GA_info_.pool_size);
        vector<int> parent_index = SelectParents(population, parents_index_pool);
        for (int j = 0; j < problem_info_.dim; j++)
        {
            if (random_.RandDoubleUnif(0, 1) < GA_info_.CR)
            {
                real lambda = random_.RandDoubleUnif(0, 1);
                candidate_[i].elements[j] = lambda * population[parent_index[0]].elements[j] + (1 - lambda) * population[parent_index[1]].elements[j];
                candidate_[i + 1].elements[j] = lambda * population[parent_index[1]].elements[j] + (1 - lambda) * population[parent_index[0]].elements[j];
            }
            else
            {
                candidate_[i].elements[j] = population[parent_index[0]].elements[j];
                candidate_[i + 1].elements[j] = population[parent_index[1]].elements[j];
            }

            if (random_.RandDoubleUnif(0, 1) < GA_info_.MR)
                candidate_[i].elements[j] = random_.RandDoubleUnif(problem_info_.min_bound, problem_info_.max_bound);
            if (random_.RandDoubleUnif(0, 1) < GA_info_.MR)
                candidate_[i + 1].elements[j] = random_.RandDoubleUnif(problem_info_.min_bound, problem_info_.max_bound);
            candidate_[i].elements[j] = CheckBound(candidate_[i].elements[j], problem_info_.min_bound, problem_info_.max_bound);
            candidate_[i + 1].elements[j] = CheckBound(candidate_[i + 1].elements[j], problem_info_.min_bound, problem_info_.max_bound);
        }
    }

    candidate_[random_.RandDoubleUnif(0, pop_size_ - 1)] = best_individual;
    for (int i = 0; i < pop_size_; i++)
        candidate_[i].fitness_value = cec2014_.EvaluateFitness(candidate_[i].elements);
    return 0;
}


int GA_CPU::Run(Population & population)
{
    Reproduce(population);
    population = candidate_;

    return 0;
}

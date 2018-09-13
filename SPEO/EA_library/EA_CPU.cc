#include "EA_CPU.h"
EA_CPU::EA_CPU()
{

}

EA_CPU::~EA_CPU()
{

}

int EA_CPU::InitilizePopulation(Population & population)
{
    for(int i = 0; i < pop_size_; i++)
    {
        Individual tmp_individual;

        for (int j = 0; j < problem_info_.dim; j++)
            tmp_individual.elements.push_back(random_.RandRealUnif(problem_info_.min_bound, problem_info_.max_bound));

        tmp_individual.fitness_value = cec2014_.EvaluateFitness(tmp_individual.elements);
        population.push_back(tmp_individual);
    }
    return 0;
}

int EA_CPU::Initilize(ProblemInfo problem_info, IslandInfo island_info)
{
    problem_info_ = problem_info;
    pop_size_ = island_info.island_size;

    cec2014_.Initilize(problem_info_.function_ID, problem_info_.dim);
    return 0;
}

int EA_CPU::Unitilize()
{
    cec2014_.Unitilize();
    return 0;
}
real EA_CPU::CheckBound(real to_check_elements, real min_bound, real max_bound)
{
	while ((to_check_elements < min_bound) || (to_check_elements > max_bound))
	{
		if (to_check_elements < min_bound)
			to_check_elements = min_bound + (min_bound - to_check_elements);
		if (to_check_elements > max_bound)
			to_check_elements = max_bound - (to_check_elements - max_bound);
	}
	return to_check_elements;
}

Individual EA_CPU::FindBestIndividual(Population & population)
{
    int best_individual_ind = 0;
    double best_individual_fitness_value = population[0].fitness_value;
    for(int i = 1; i < pop_size_; i++)
    {
        if(population[i].fitness_value < best_individual_fitness_value)
        {
            best_individual_ind = i;
            best_individual_fitness_value = population[i].fitness_value;
        }
    }
	return population[best_individual_ind];
}


#ifdef DEBUG
void EA_CPU::FprintPopulation(Population &population)
{
    FILE *file;
    char file_name[100];
    sprintf(file_name, "Population_CUDA:Node=%d.dat", device_id_);
    file = fopen(file_name,"w");

    for(int i = 0; i < total_individual_num_; i++)
    {
        fprintf(file, "%d\t%lf\n", i, population[i].fitness_value);
        for(int j = 0; j < problem_info_.dim; j++)
        {
            fprintf(file, "%lf\t", population[i].elements[j]);
        }
        fprintf(file, "\n");
    }
}
#endif

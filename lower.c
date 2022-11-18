#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<limits.h>
#include<gsl/gsl_randist.h>

#define ITERATIONS 1000





double average(int r[ITERATIONS])
{
	double average_1 = 0.0;

	int i;

	for(i=0; i <ITERATIONS; i++)
	{
		average_1 += ((double)r[i]) / ((double)ITERATIONS);
	}

	return average_1;
}


int main()
{
	//allocates a random number generator "skill", used for gaussian skill generation
	gsl_rng * skill = gsl_rng_alloc (gsl_rng_taus);
	
	//allocates a random number generator "victory", used for uniform victory generation
	gsl_rng * victory = gsl_rng_alloc (gsl_rng_taus);


	//allocates a random number generator "seed", used for uniform seed generation for the other RNGs
	gsl_rng * seed = gsl_rng_alloc (gsl_rng_taus);

	//seeds the random number generator
	gsl_rng_set(seed, time(NULL));
	

	int i;

	//supposedly, MM was undefeated for 61 duels

	//to get max number of victories with 95% confidence level, run 1000 iterations
	//sort, check number at index 50
	//i.e., such that 95% of runs at >= than that number

	//control variables
	double skill_musashi = 100.0;
	
	double mean = 49.5;

	//variance up to ((musashi - mean)^2)/2
	#define INCREASE_VARIANCE (0.1 * 49.5)
	double variance = 0.0;

	//alg variables
	
	double skill_opponent = 0.0;

	int defeat = 0;
	


	//iteration over variance values
	for(variance = 4.95; variance <= mean; variance += INCREASE_VARIANCE)
	{
		int longest = 0;
		int victories = 0;

		int results[ITERATIONS];

		

		int done = 0;
		int found = 0;

		for(skill_musashi = 100.0; !done; skill_musashi += 10.0)
		{

			//printf("Attempting skill level %f\n",skill_musashi);
			for(i=0; i <ITERATIONS; i++)
			{
				results[i] = 0;			
			}

			for(i=0; i <ITERATIONS; i++)
			{
				//seeds the random number generator
				gsl_rng_set(skill, gsl_rng_uniform_int(seed, INT_MAX));
				//seeds the random number generator
				gsl_rng_set(victory, gsl_rng_uniform_int(seed, INT_MAX));

				while(1)
				{

					while(skill_opponent < 1.0)
					{
						skill_opponent = gsl_ran_gaussian(skill, variance) + mean;
						if(skill_opponent > skill_musashi)
							skill_opponent = skill_musashi;
					}

					double chance_of_victory = gsl_rng_uniform_pos(victory) * (skill_opponent + skill_musashi);

					if(chance_of_victory <= skill_musashi)
					{
						victories++;
					}
					else
					{
						break;
					}
					skill_opponent = 0.0;
				}

				//updated longest streak
				if(victories > longest)
					longest = victories;

				results[i] = victories;

				if(victories >= 61 && found == 0)
				{
					printf("61 streak at skill %f\n",skill_musashi);
					found = 1;
				}

				//reset
				victories = 0;

			}

			int j, k;
			for(j=0; j< (ITERATIONS-1); j++)
			{
				for(k=0;k<(ITERATIONS-1);k++)
				{
					if(results[k] > results[k+1])
					{
						int tmp = results[k];
						results[k] = results[k+1];
						results[k+1] = tmp;
					}
				}
			}

			if(results[(unsigned int)(ITERATIONS / 20.0)] >= 61)
			{
				done = 1;
			}

		}
		found = 0;
		

		/*printf("Variance %f\n",variance);
		printf("Longest victory chain was %d\n",longest);
		printf("Average number of victories: %f\n",average(results));
		
		printf("Skill for Guaranteed victories with 95%% confidence: %f\n",skill_musashi);*/
	}


	


	
	
	return 0;
}
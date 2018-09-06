#include<iostream>
#include<math.h>
#include<time.h>
#include<omp.h>

using namespace std;

#define MAX 100000000
long counter = MAX-1;
long number_of_primes = 0;
long primesum = 0;
long isprime(long counter);
void decrementCounter();

//decrement the counter by factor of 2(only consider odd numbers)
void decrementCounter()
{
	#pragma omp critical
	counter-=2;
}

//function returns 0 if the number is not prime else returns the prime number
//this function also keeps a track of the total number of prime numbers.
long isprime(long counter)
{
	int i;
	//exclude the multiples of 3,5,7,11,13 to reduce the number of elements checked for prime.
	if((counter%3 != 0 && counter%5 != 0 && counter%7 != 0 && counter%11 != 0)  || counter == 11 || counter == 7 || counter == 5 || counter == 3)
	{
		
		for (i=3;i<=sqrt(counter);i+=2){
			if (counter%i==0)
				return 0;
		}
		#pragma omp critical
		number_of_primes++;
		return counter;	
	}
	else
	{
		return 0;
	}
}
int main()
{

	long i;
	int count = 10;
	long arr[10];
	double start = omp_get_wtime();
	while(count != 0)
	{
		long num = isprime(counter);
		if(num)
		{
			decrementCounter();
			count--;
			arr[count] = num;
			primesum += num;
		}
		else
		{
			decrementCounter();
		}
	}
	//cout<<"Counter start"<<counter;
 	
	#pragma omp parallel for num_threads(8) 
	for(long c = counter; c >= 3; c-=2)//while(counter >= 3)
	{
		long num = isprime(c);
		if(num)
		{
			//decrementCounter();
			#pragma omp critical
			primesum += num;
		}
		else
		{
			//decrementCounter();
		}
	}

	primesum += 2; //add 2 as only odd numbers were iterated over.
        number_of_primes++;
	double stop = omp_get_wtime(); 
	cout<<"\nExecution time in seconds:"<<stop-start;
        cout<<"\nNumber of primes found:"<<number_of_primes;
	cout<<"\nSum of primes:"<<primesum<<endl;
	cout<<"\nTop 10 Prime numbers:\n";
	for(int j = 0; j < 10; j++)
	{
		cout<<arr[j]<<endl;
	}
		

	return 1; 
}

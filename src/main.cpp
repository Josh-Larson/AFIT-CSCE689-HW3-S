/****************************************************************************************
 * prime_calc - program that uses PCalc_T and PCalc_SP to calculate all the prime
 *              numbers up to the max number represented by an unsigned int. 
 *
 *              **Students should not modify this code! Or at least you can to test your
 *                code, but your code should work with the unmodified version
 *
 ****************************************************************************************/  

#include <stdio.h>
#include <strings.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <cmath>
#include <getopt.h>
#include <unistd.h>

#include "PCalc_SP.h"
#include "PCalc_T.h"

using namespace std::chrono;

// Default of how high should we get prime numbers up to:
const unsigned int default_max_range = 50000;

// Default maximum number of threads that should be used to calculate
const unsigned int default_num_threads = 4;

void displayHelp(const char *execname) {
   std::cout << execname << " [-t <num_threads>] [-n <max_range>] [-s] [-m]\n";
   std::cout << "   t: maximum number of threads to use\n";
   std::cout << "   n: calculate primes up to the given range\n";
   std::cout << "   s: only run in single process mode\n";
   std::cout << "   m: only run in multithreaded mode\n";
   std::cout << "   w: skip writing to disk\n";

}

int main(int argc, char *argv[]) {


   bool use_singlethread = true;
   bool use_multithread = true;
   bool write_to_disk = true;
   unsigned int count_to = default_max_range;
   unsigned int num_threads = default_num_threads;


   // Get the command line arguments and set params appropriately
   int c = 0;
   long int max_range, n_threads;
   while ((c = getopt(argc, argv, "n:t:smw")) != -1) {
      switch (c) {
  
      // Set the max number to count up to	    
      case 'n':
         max_range = strtol(optarg, NULL, 10);
	      if ((max_range < 3) || (max_range > std::pow(2, 8*sizeof(unsigned int)))) {
            std::cout << "Invalid max range. Value must be between 3 and " <<
                                        std::pow(2, 8*sizeof(unsigned int)) << "\n";
            std::cout << "Format: " << argv[0] << " [<max_range>] [<max_threads>]\n";
            exit(0);
	      }
	      count_to = (unsigned int) max_range;
	      break;

      // Set the max number of threads to use	 
      case 't':
         n_threads = strtol(optarg, NULL, 10);
         if ((n_threads < 1) && (n_threads <= 10000)) {
            std::cout << "Invalid max threads. Value must be between 1 and 10000\n";
            std::cout << "Format: " << argv[0] << " [<max_range>] [<max_threads>]\n";
            exit(0);
         }
         num_threads = (unsigned int) n_threads;
         break;
   
      // Just run the singlethreaded version of this code
      case 's':
         use_multithread = false;
	 break;

      // Just run the multithreaded version of this code
      case 'm':
	 use_singlethread = false;
	 break;

      // Skip writing the results to disk (useful for testing)
      case 'w':
	 write_to_disk = false;
	 break;

      case '?':
	 displayHelp(argv[0]);
	 break;

      default:
	 std::cout << "Unknown command line option '" << c << "'\n";
	 displayHelp(argv[0]);
	 break;
      }

   }

   // Find primes using a single thread with nexted loops
   if (use_singlethread) {

      // Initialize student's class for the single process calculator where count_to is the
      // max possible prime to calculate
      std::cout << "Initializing single process prime calculator\n";
      PCalc_SP primes(count_to);
      std::cout << "Calculating " << count_to << " possible primes with a single process.\n";

      auto start = high_resolution_clock::now();

      // Student's function to calculate single-process primes
      primes.markNonPrimes();
      auto stop = high_resolution_clock::now();

      auto duration = duration_cast<microseconds>(stop - start);
   
      std::cout << "Function took: " << duration.count() << " microseconds\n";

      if (write_to_disk) {
         std::cout << "Writing primes to file\n"; 
         primes.printPrimes("sp_primes.txt");
      }

      std::cout << "Complete.\n";
      primes.cleanup();
   }

   // Find primes using multiple threads
   if (use_multithread) {

      // Initialize student's class with our max possible prime (count_to) and the max number of threads
      // to use in the calculator (num_threads)
      PCalc_T tprimes(count_to, num_threads);
      std::cout << "Calculating " << count_to << " possible primes using " << num_threads << 
	   					" max threads.\n";

      // Student's function to manage multiple threads to calculate primes
      auto start = high_resolution_clock::now();
      tprimes.markNonPrimes();
      auto stop = high_resolution_clock::now();

      auto duration = duration_cast<microseconds>(stop - start);
      std::cout << "Function took: " << duration.count() << " microseconds\n";

      if (write_to_disk) {
         std::cout << "Writing primes to file\n";
         tprimes.printPrimes("threaded_primes.txt");
      }

      std::cout << "Complete.\n";

   }
}

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "prelude.h"

typedef long long int Integer;

using namespace prelude;

/********************************************************************************
 * Non Functional Section                                             *
 * Needs improvments                                                  *
 ********************************************************************************/

std::vector<std::pair<Integer, Integer> > generateRationals(Integer qmin, Integer qmax){
	Integer minV = (Integer)std::pow(3,qmin);
	Integer maxV = (Integer)std::pow(3,qmax);
	std::vector<std::pair<Integer, Integer> > out;
	int c=0;
	for(int q=0; q < (Integer)std::pow(3,qmax) - minV; q++){
		out.resize(out.size()+(q+minV)/3);
		for(Integer p=1; p < (q+minV)/3; p++){
			out[c++] = std::make_pair(p, q + minV);
		}
	}
	return out;
}

/********************************************************************************
 * End Non Functional Section                                         *
 ********************************************************************************/

bool testCantorSec(Integer pstart, Integer p, Integer q, bool start){
	if(p == pstart && !start) return true;
	else{
		if(3*p/q == 1) return false;
		else return testCantorSec(pstart,3*p % q, q, false);
	}
}

bool testCantor(std::pair<Integer, Integer> pq){
	const Integer p = pq.first;
	const Integer q = pq.second;
	if(q % 3 == 0){
		if(p/(q/3) == 1) return p==q && gcd(p,q);
		else return testCantor(std::make_pair(p % (q/3), q/3));
	}
	else return testCantorSec(p,p,q,true) && gcd(p,q);
}

/**
 args <- getArgs
		 number <- return (foldl1 (+) (map fromEnum (map testCantor (generateRationals (read (head (tail args))) (read (head (tail (tail args))))))))
		 putStrLn (show (2*number))
 **/

int main(int argc, char* argv[]){
	putStrLn(
	 show(
	  generateRationals(
	      read<Integer>(
	       std::string(argv[1])
	      ),
	      read<Integer>(
		   std::string(argv[2])
		  )
		 )
		)
	);
	putStrLn(
	 show(
	  2*foldl(
	   std::plus<Integer>(),
	   0,
	   map(
	    [](bool v)->Integer{return v ? 1 : 0;},
	    map(
	     testCantor,
	     generateRationals(
	      read<Integer>(
	       std::string(argv[1])
	      ),
	      read<Integer>(
		   std::string(argv[2])
		  )
		 )
		)
	   )
	  )
	 )
	);
	return 0;
}

#include "PopulationFile.cpp"
#include <ctime>

LocusVec parseFile(char* fileName) {
    LocusVec retVec;
    std::ifstream inFile(fileName, std::ios::binary);
    std::string line;
    std::cout << "Importing Population Statistics..." << std::endl;

    while(getline(inFile, line, '\n')) {
        std::string currStr;
        std::istringstream lineStream(line);
        getline(lineStream, currStr, ',');

        Locus curr(currStr);
        while(getline(lineStream, currStr, ',')) {
            curr.addAlleleProb(std::stod(currStr));
        }

        retVec.push_back(curr);
    }
   
    return retVec;
}

Sample makeSample(const LocusVec& locuses) {
    Sample curr(10);
    for (Locus l : locuses) {
        curr.addLocusPeak(l);
    }
    return curr;
}

Population genPopulation(const LocusVec& locuses, const int n) {
    Population pop;
    for (int i = 0; i < n; i++) {
        pop.push_back(makeSample(locuses));
    }
    return pop;
}


std::vector<int> easyList(const Sample& sample){
  std::vector<int> profile;
  std::vector<LocusPeak> peaks = sample.getLocusPeaks();
  for (LocusPeak p: peaks) {
    profile.push_back(p.second.first);
    profile.push_back(p.second.second);
  }
  return profile;
}


int parentsCarlo(const Population& pop){
  std::vector<int> parent1,parent2;
  //std::cout << pop[0] << std::endl;
  parent1 = easyList(pop[0]);
  parent2 = easyList(pop[1]);
  int samecount=0;
  for(size_t i=0; i < parent1.size();i++){
    if(parent1[i] == parent2[i])
      samecount++;
  }
  return samecount;
}

int listCarlo(std::vector<int>& list1, std::vector<int>& list2){
  int samecount = 0;
  for(size_t i =0; i < list1.size();i++){
    if(list1[i]==list2[i])
      samecount++;
  }
  return samecount;
}

std::vector<int> makeKid(const Population& pop){
  std::vector<int> parent1,parent2,kid;
  parent1 = easyList(pop[0]);
  parent2 = easyList(pop[1]);
  for(size_t i = 0; i < parent1.size();i++){
    double randVal = (double) rand()/RAND_MAX;
    if(randVal > .5)
      kid.push_back(parent1[i]);
    else
      kid.push_back(parent2[i]);
  }
  return kid;
}

std::vector<int> makeCousin(const Sample& person, std::vector<int>& sib){
  std::vector<int> person1,cousin;
  person1 = easyList(person);
    for(size_t i = 0; i < sib.size();i++){
    double randVal = (double) rand()/RAND_MAX;
    if(randVal > .5)
      cousin.push_back(sib[i]);
    else
      cousin.push_back(person1[i]);
  }
  return cousin;
}

int main(int argc, char* argv[]) {
  int parentsSameSeen=0,kidsSameSeen=0,cousinSameSeen=0;
  std::vector<int> kid1,kid2,cousin1,cousin2;
  int monteRunNum = atoi(argv[1]);
  srand(time(NULL));
  if (argc < 3) {
    std::cerr << "Two arguments required [ n loci_file ]" << std::endl;
    return 1;
  }
  LocusVec locuses = parseFile(argv[2]);
  for(int i = 0; i< monteRunNum; i++){
    Population population = genPopulation(locuses,4);
    parentsSameSeen += parentsCarlo(population);
    kid1 = makeKid(population);
    kid2 = makeKid(population);
    kidsSameSeen += listCarlo(kid1,kid2);
    cousin1 = makeCousin(population[2],kid1);
    cousin2 = makeCousin(population[3],kid2);
    cousinSameSeen += listCarlo(cousin1,cousin2);
  }
  std::cout << "For two random strangers, with 26 alleles (at 13 loci) we expect about " 
	    << (double)parentsSameSeen/monteRunNum << " alleles to be the same" << std::endl;

 std::cout << "For two childern of the same parents, with 26 alleles (at 13 loci) we expect about " 
	   << (double)kidsSameSeen/monteRunNum << " alleles to be the same" << std::endl;

 std::cout << "For two cousins, with 26 alleles (at 13 loci) we expect about " 
	   << (double)cousinSameSeen/monteRunNum << " alleles to be the same" << std::endl;
  return 0;
}



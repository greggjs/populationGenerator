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

int main(int argc, char* argv[]) {
    srand(time(NULL));
    if (argc < 3) {
        std::cerr << "Two arguments required [ n loci_file ]" << std::endl;
        return 1;
    }
    LocusVec locuses = parseFile(argv[2]);
    Population population = genPopulation(locuses, atoi(argv[1]));
    PopulationFile popFile(locuses, population);

    return 0;
}

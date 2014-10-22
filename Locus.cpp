#include <unordered_map>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

typedef std::vector<double> ProbVec;

class Locus {
    friend std::ostream& operator<<(std::ostream& os, const Locus& l) {
        os << l.name << ": " << std::endl;
        for (int i = 0; i < l.locusProbVec.size(); i++) {
            os << "[ " << (i + 1) << ": " << l.locusProbVec[i] << " ], ";
        }
        os << std::endl;
        return os;
    }
    public:
        Locus(const std::string& name) : name(name) {}

        double getAlleleProb(const int allele) {
            return locusProbVec[allele];
        }

        const double getAlleleProb(const int allele) const {
            return locusProbVec[allele];
        }

        int numAlleles() const {
            return locusProbVec.size();
        }

        void addAlleleProb(const double prob) {
            locusProbVec.push_back(prob);
            alleleCount++;
        }

        const std::string getName() const { return name; }

    private:
        int alleleCount = 0;
        std::string name;
        ProbVec locusProbVec;
};

#include <cmath>
#include <cstdlib>
#include "Locus.cpp"

static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

struct Key {
    double first;
    double second;
};

typedef std::pair<double, double> Peaks;
typedef std::pair<std::string, Key> LocusPeak;
typedef std::unordered_map<std::string, Key> LocusPeaks;

class Sample {
    friend std::ostream& operator<<(std::ostream& os, const Sample& sample) {
        os << sample.name << ": ";
        std::vector<LocusPeak> peaks(sample.allelePeaks.begin(), sample.allelePeaks.end());
        std::sort(peaks.begin(), peaks.end(),
            [](const LocusPeak& a, const LocusPeak& b){ return a.first < b.first; });
        for (LocusPeak peak : peaks) {
            os << "[" << peak.first << ": " << peak.second.first
                << ", " << peak.second.second << "], ";
        }
        os << "\n";
        return os;
    }

    public:
        Sample(const int len) {
            name = makeRandId(len);
        }

        void addLocusPeak(const Locus& locus) {
            double prob1 = randomDouble();
            double prob2 = randomDouble();
            double allele1 = determineAllele(prob1, locus);
            double allele2 = determineAllele(prob2, locus);
            Key allelePair = { allele1, allele2 };
            allelePeaks[locus.getName()] = allelePair;
        }

        const std::string getName() const { return name; }

        std::vector<LocusPeak> getLocusPeaks() const {
            std::vector<LocusPeak> peaks(allelePeaks.begin(), allelePeaks.end());
            std::sort(peaks.begin(), peaks.end(),
                [](const LocusPeak& a, const LocusPeak& b){ return a.first < b.first; });
            return peaks;
        }

    private:
        std::string makeRandId(const int len) {
            std::string retStr;

            for (int i = 0; i < len; ++i) {
                retStr += alphanum[rand() % (sizeof(alphanum) - 1)];
            }

            return retStr;
        }

        double randomDouble() {
            double f = (double) rand() / RAND_MAX;
            return f;
        }

        double determineAllele(const double prob, const Locus& locus) {
            double sum = 0;
            for (int i = 0; i < locus.numAlleles(); i++) {
                sum += locus.getAlleleProb(i);
                if (prob <= sum) {
                    return i + 1;
                }
            }
            return -1;
        }

        std::string name;
        LocusPeaks allelePeaks;
};

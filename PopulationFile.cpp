#include "Sample.h"
#include <fstream>
#include <iterator>
#include <sstream>

typedef std::vector<Locus> LocusVec;
typedef std::vector<Sample> Population;

class PopulationFile {
    public:
        PopulationFile(const LocusVec& locuses, const Population& pop) {
            filename = "ouput_pop.csv";
            makeHeader(locuses);
            for (Sample s : pop) {
                addSample(s);
            }
            std::ofstream output(filename);
            std::copy(fileContents.begin(), fileContents.end(),
                std::ostream_iterator<std::string>(output));
        }

    private:
        void addSample(const Sample& sample) {
            std::vector<LocusPeak> peaks = sample.getLocusPeaks();
            std::ostringstream s;
            s << sample.getName() << ",";
            int count = 0;
            for (LocusPeak p : peaks) {
                s << p.second.first << "," << p.second.second;
                count++;
                if (count < peaks.size()) {
                    s << ",";
                }
            }
            s << "\n";
            fileContents.push_back(s.str());
        }

        void makeHeader(const LocusVec& locuses) {
            std::ostringstream s;
            s << "SampleCode,";
            int count = 0;
            for (Locus l : locuses) {
                s << l.getName() << "," << l.getName();
                count++;
                if (count < locuses.size()) {
                    s << ",";
                }
            }
            s << "\n";
            fileContents.push_back(s.str());

        }

        std::string filename;
        std::vector<std::string> fileContents;
};

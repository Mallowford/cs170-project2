#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <climits>

class DATA {
    public:
        std::vector<int> originalClass;
        std::vector<std::vector<double>> row;
        int features() const {
            return features_d;
        };
        int size() const {
            return size_d;
        };
        void init() {
            size_d = originalClass.size();
            features_d = row.at(0).size();
        };
        DATA() {};
        ~DATA() {};
        DATA(const DATA& other) = delete;
        DATA& operator=(const DATA& other) {
            if (this != &other) {
                originalClass = other.originalClass;
                row = other.row;
                size_d = other.size_d;
                features_d = other.features_d;
            };
            return *this;
        };
    private:
        int size_d;
        int features_d;
};

double leave_one_out_cross_validation(const DATA& data, std::vector<int>& features, int feature_to_add);
void feature_search_demo(DATA);
void read(const std::string& filename);
void forwardSelection();
void backwardsElimination();

int main() {
    DATA data;
    std::string filename;

    filename = "CS170_Large_DataSet__28.txt";
    read(filename, data);

    data.init();

    return 0;
};

/* Read Function
https://youtu.be/Rc61KnQArGI
*/
void read(const std::string& filename, DATA& data) {
    std::ifstream openedFile;
    openedFile.open(filename);
    
    if (!openedFile) {
        std::cout << "Failed to open file\n";
        return;
    };

    std::string currLine;
    while (std::getline(openedFile, currLine)) {
        std::istringstream ss(currLine);
        double value;

        ss >> value;
        data.originalClass.push_back(value);
        std::vector<double> entry;
        while(ss >> value) {
            entry.push_back(value);
        };

        data.row.push_back(entry);
    };

    openedFile.close();
};
void modifyData(const DATA& data, DATA& newData, const std::vector<int>& features, int feature_to_add) {
    newData = data;

    // For Every Data Entry, For All of The Data Entry's Row, 0 Features that are not in features
    for (int i = 0; i < data.size(); i++) {

        for (int j = 0; j < data.features(); j++) {
            bool is_a_feature = false;

            for (int k = 0; k < features.size(); k++) {
                if ((j + 1) == features.at(j)) {
                    is_a_feature = true;
                };
            };

            if (!is_a_feature) {
                newData.row.at(i).at(j) = 0;
            };
        };
    };
};
// From Slides
double leave_one_out_cross_validation(const DATA& data, std::vector<int>& features, int feature_to_add) {

    DATA newData;
    modifyData(data, newData, features, feature_to_add);

    int number_correctly_classified = 0;

    for (int i = 0; i < newData.size(); i++) {
        std::vector<double> object_to_classify = newData.row.at(i);
        int label_object_to_classify = newData.originalClass.at(i);

        double nearest_neighbor_distance = DBL_MAX;
        int nearest_neighbor_location = INT_MAX;
        int nearest_neighbor_label = 0; // 1 or 2

        for (int k = 1 ; k < data.size(); k++) {
            if (k != i) {
                double num = 0;
                for (int a = 0; a < data.features(); a++) {
                    double sum = (object_to_classify.at(a) - data.row.at(k).at(a));
                    pow(sum, 2);
                    num =+ sum;
                };

                double distance = sqrt(num);

                if (distance < nearest_neighbor_distance) {
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = k;
                    nearest_neighbor_label = newData.originalClass.at(nearest_neighbor_location);
                };
            };
        };

        if (label_object_to_classify == nearest_neighbor_label) {
            number_correctly_classified++;
        };
    };

    double accuracy = number_correctly_classified / data.size();

    return accuracy;
};

// From Slides
void feature_search_demo(DATA& data) {

    std::vector<int> current_set_of_features;

    for (int i = 1; data.size(); i++) {
        std::cout << "On the " << i << "th level of the search tree\n";
        int feature_to_add_at_this_level = 1;
        double best_so_far_accuracy = 0;

        for (int j = 1; data.size(); j++) {
            std::cout << "--Considering adding the " << j << " feature\n";
            
            double accuracy = leave_one_out_cross_validation(data, current_set_of_features, j);

            if (accuracy > best_so_far_accuracy) {
                best_so_far_accuracy = accuracy;
                feature_to_add_at_this_level = j;
            };


        };

        current_set_of_features.push_back(feature_to_add_at_this_level);

    };
};
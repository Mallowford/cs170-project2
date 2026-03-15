#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <climits>
#include <chrono>

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
void feature_search_demo(DATA& data);
void read(const std::string& filename, DATA& data);
void backwardsElimination(DATA& data);
double backward_leave_one_out_cross_validation(const DATA& data, std::vector<int>& features, int feature_to_remove);

int main() {
    DATA data;
    std::string filename;

    std::cout <<    "Hello! This is Ryan Hoang's Feature Selection Algorithm.\n" <<
                    "Type in the name of the file (with extension) you want to test: ";
    std::cin >> filename;
    
    if (filename == "で") {
        filename = "CS170_Large_DataSet__28.txt";
    }
    else if (filename == "ち") {
        filename = "CS170_Small_DataSet__15.txt";
    };
    std::cin.clear();
    std::cin.ignore();
    std::cout << "\n" << "Type the number of the algorithm you want to run:\n"
            << "1. Forward Selection\n"
            << "2. Backward Selection\n";
    int option = INT_MAX;
    
    do {
        if (option != INT_MAX) {
            std::cout << "Invalid Option\n";
        };
        std::cin >> option;
        std::cin.clear();
        std::cin.ignore();
    } while (option != 1 && option != 2);

    read(filename, data);
    data.init();

    std::cout << "\n====Beginning Search====\n";

    auto start = std::chrono::steady_clock::now();
    switch (option) {
        case 1:
            feature_search_demo(data);
            break;
        case 2:
            backwardsElimination(data);
            break;
        default:
            feature_search_demo(data);
            break;
    };
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "Took " << (double)(duration.count()) << " seconds to finish search\n"; 
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

    for (int i = 0; i < data.size(); i++) {

        for (int j = 0; j < data.features(); j++) {
            bool is_a_feature = false;

            for (int k = 0; k < features.size(); k++) {
                if (j == features.at(k)) {
                    is_a_feature = true;
                };
            };
            if (j == feature_to_add) {
                is_a_feature = true;
            };

            if (!is_a_feature) {
                newData.row.at(i).at(j) = 0;
            };
        };
    };
};
void modifyData_remove(const DATA& data, DATA& newData, const std::vector<int>& features, int feature_to_remove) {
    newData = data;

    for (int i = 0; i < data.size(); i++) {

        for (int j = 0; j < data.features(); j++) {
            bool to_remove = true;

            for (int k = 0; k < features.size(); k++) {
                if (j == features.at(k)) {
                    to_remove = false;
                };
            };

            if (j == feature_to_remove) {
                to_remove = true;
            };

            if (to_remove) {
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
        const std::vector<double>& object_to_classify = newData.row.at(i);
        int label_object_to_classify = newData.originalClass.at(i);

        double nearest_neighbor_distance = DBL_MAX;
        int nearest_neighbor_location = INT_MAX;
        int nearest_neighbor_label = 0; // 1 or 2

        for (int k = 0 ; k < newData.size(); k++) {
            if (k != i) {
                double num = 0;
                for (int a = 0; a < newData.features(); a++) {
                    double sum = (object_to_classify.at(a) - newData.row.at(k).at(a));
                    num += pow(sum, 2.0);
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

    double accuracy = (double)number_correctly_classified / data.size();

    return accuracy;
};
// From Slides
bool isempty(const std::vector<int>& overlap) {
    return overlap.empty();
};
std::vector<int> intersect(const std::vector<int>& features, int k) {
    std::vector<int> results;
    for (int i = 0; i < features.size(); i++) {
        if (k == features.at(i)) {
            results.push_back(features.at(i));
        };
    };

    return results;
};
void feature_search_demo(DATA& data) {
    std::vector<int> current_set_of_features;
    std::vector<int> best_set_of_features;
    double best_accuracy = 0;

    for (int i = 0; i < data.features(); i++) {
        // std::cout << "On the " << (i + 1) << "th level of the search tree\n";
        int feature_to_add_at_this_level = -1;
        double best_so_far_accuracy = 0;

        for (int k = 0; k < data.features(); k++) {
            if (isempty(intersect(current_set_of_features, k))) {
                // std::cout << "--Considering adding the " << (k + 1) << " feature\n";
                
                double accuracy = leave_one_out_cross_validation(data, current_set_of_features, k);

                std:: cout << "\tUsing feature(s) {";
                for (int a = 0; a < current_set_of_features.size(); a++) {
                    if (a == 0) {
                        std::cout << (current_set_of_features.at(a) + 1);
                    }
                    else {
                        std::cout << ", " << (current_set_of_features.at(a) + 1);
                    };
                };

                if (current_set_of_features.size() > 0) {
                    std::cout << ", " << (k + 1);
                }
                else {
                    std::cout << (k + 1);
                };
                
                std::cout << "} accuracy is " << (accuracy * 100) << "%\n";

                if (accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    feature_to_add_at_this_level = k;
                };
            };
        };

        if (feature_to_add_at_this_level != -1) {
            current_set_of_features.push_back(feature_to_add_at_this_level);
            std::cout << "Feature set {";
            for (int i = 0; i < current_set_of_features.size(); i++) {
                if (i == 0) {
                    std::cout << (current_set_of_features.at(i));
                }
                else {
                    std::cout << ", " << (current_set_of_features.at(i) + 1);
                };
            };
            std::cout << "} was best, accuracy is " << (best_so_far_accuracy * 100) << "%\n";
        };

        if (best_so_far_accuracy > best_accuracy) {
            best_accuracy = best_so_far_accuracy;
            best_set_of_features = current_set_of_features;
        };
    };

    std::cout << "\nFinished Search. Best Feature Subset is {";
    for (int a = 0; a < best_set_of_features.size(); a++) {
        if (a == 0) {
            std::cout << (best_set_of_features.at(a) + 1);
        }
        else {
            std::cout << ", " << (best_set_of_features.at(a) + 1);
        };
    };
    std::cout << "}, which had an accuracy of " << (best_accuracy * 100) << "%\n";
};
void backwardsElimination(DATA& data) {

    std::vector<int> current_set_of_features;
    for (int i = 0; i < data.features(); i++) {
        current_set_of_features.push_back(i);
    };

    std::vector<int> best_set_of_features;
    double best_accuracy = 0;

    for (int i = 0; i < data.features(); i++) {
        // std::cout << "On the " << (i + 1) << "th level of the search tree\n";
        int feature_to_remove_at_this_level = -1;
        double best_so_far_accuracy = 0;

        for (int k = 0; k < data.features(); k++) {
            if (!isempty(intersect(current_set_of_features, k))) {
                // std::cout << "--Considering removing the " << (k + 1) << " feature\n";
                
                double accuracy = backward_leave_one_out_cross_validation(data, current_set_of_features, k);

                std:: cout << "\tUsing feature(s) {";
                bool first = true;
                for (int a = 0; a < current_set_of_features.size(); a++) {
                    if (current_set_of_features.at(a) != k) {
                        if (first) {
                            std::cout << (current_set_of_features.at(a) + 1);
                            first = false;
                        }
                        else {
                            std::cout << ", " << (current_set_of_features.at(a) + 1);
                        };
                    };
                };
                
                std::cout << "} accuracy is " << (accuracy * 100) << "%\n";

                if (accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    feature_to_remove_at_this_level = k;
                };
            };
        };

        if (feature_to_remove_at_this_level != -1) {
            int eraseIndex;
            for (int a = 0; a < current_set_of_features.size(); a++) {
                if (current_set_of_features.at(a) == feature_to_remove_at_this_level) {
                    eraseIndex = a;
                };
            };
            current_set_of_features.erase(current_set_of_features.begin() + eraseIndex);
            std::cout << "Feature set {";
            for (int i = 0; i < current_set_of_features.size(); i++) {
                if (i == 0) {
                    std::cout << (current_set_of_features.at(i));
                }
                else {
                    std::cout << ", " << (current_set_of_features.at(i) + 1);
                };
            };
            std::cout << "} was best, accuracy is " << (best_so_far_accuracy * 100) << "%\n";
        };
        // else {
        //     // std::cout << "On level " << (i + 1) << " i didn't remove a feature\n"; 
        // };

        if (best_so_far_accuracy > best_accuracy) {
            best_accuracy = best_so_far_accuracy;
            best_set_of_features = current_set_of_features;
        };
    };

    std::cout << "Finished Search. Best Feature Subset is {";
    for (int a = 0; a < best_set_of_features.size(); a++) {
        if (a == 0) {
            std::cout << (best_set_of_features.at(a) + 1);
        }
        else {
            std::cout << ", " << (best_set_of_features.at(a) + 1);
        };
    };
    std::cout << "}, which had an accuracy of " << (best_accuracy * 100) << "%\n";
};
double backward_leave_one_out_cross_validation(const DATA& data, std::vector<int>& features, int feature_to_remove) {
    DATA newData;
    modifyData_remove(data, newData, features, feature_to_remove);

    int number_correctly_classified = 0;

    for (int i = 0; i < newData.size(); i++) {
        const std::vector<double>& object_to_classify = newData.row.at(i);
        int label_object_to_classify = newData.originalClass.at(i);

        double nearest_neighbor_distance = DBL_MAX;
        int nearest_neighbor_location = INT_MAX;
        int nearest_neighbor_label = 0; // 1 or 2

        for (int k = 0 ; k < newData.size(); k++) {
            if (k != i) {
                double num = 0;
                for (int a = 0; a < newData.features(); a++) {
                    double sum = (object_to_classify.at(a) - newData.row.at(k).at(a));
                    num += pow(sum, 2.0);
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

    double accuracy = (double)number_correctly_classified / data.size();

    return accuracy;
};
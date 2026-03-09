#include <iostream>
#include <string>
#include <vector>
#include <fstream>

double leave_one_out_cross_validation(/*DATA data,*/ /*current_set/path of features,*/ int feature_to_add);
void feature_search_demo(/*DATA*/);

void read(const std::string& filename);

int main() {
    read("CS170_Large_DataSet__28.txt");

    return 0;
};

void read(const std::string& filename) {
    std::ifstream openedFile;
    openedFile.open(filename);
    
    double currLine;
    openedFile >> currLine;
    openedFile >> currLine;

    std::cout << currLine << "\n";

};

// From Slides
// double leave_one_out_cross_validation(/*DATA data,*/ /*current_set/path of features,*/ int feature_to_add) {
//     int number_correctly_classified = 0;

//     for (int i = 0; i < data.size(); i++) {
//         object_to_classify = data(i, 2:end);
//         label_object_to_classify = data(i, 1);

//         nearest_neighbor_distance = inf;
//         nearest_neighbor_location = inf;

//         for (int k = 1 ; k < data.size(); k++) {
//             if (k != i) {
//                 distance = sqrt(sum(object_to_classify^^2 - data(k, 2:end)^^2));

//                 if (distance < nearest_neighbor_distance) {
//                     nearest_neighbor_distance = distance;
//                     nearest_neighbor_location = k;
//                     nearest_neighbor_label = data(nearest_neighbor_location, 1)
//                 };
//             };
//         };

//         if (label_object_to_classify == nearest_neighbor_label) {
//             number_correctly_classified++;
//         };
//     };

//     double accuracy = number_correctly_classified / size(data, 1);

//     return accuracy;
// };
// // From Slides
// void feature_search_demo(/*DATA*/) {

//     int current_set_of_features = 0;

//     for (int i = 1; 3/*data.size()*/; i++) {
//         std::cout << "On the " << i << "th level of the search tree\n";
//         int feature_to_add_at_this_level = 1;
//         double best_so_far_accuracy = 0;

//         for (int j = 1; 3 /*data.size()*/; j++) {
//             std::cout << "--Considering adding the " << j << " feature\n";
            
//             double accuracy = leave_one_out_cross_validation(j);

//             if (accuracy > best_so_far_accuracy) {
//                 best_so_far_accuracy = accuracy;
//                 feature_to_add_at_this_level = j;
//             };


//         };

//     };
// };
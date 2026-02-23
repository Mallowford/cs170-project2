#include <iostream>
double leave_one_out_cross_validation(/*DATA data,*/ /*current_set/path of features,*/ int feature_to_add) {

};

void feature_search_demo(/*DATA*/) {

    int current_set_of_features = 0;

    for (int i = 1; 3/*data.size()*/; i++) {
        std::cout << "On the " << i << "th level of the search tree\n";
        int feature_to_add_at_this_level = 1;
        double best_so_far_accuracy = 0;

        for (int j = 1; 3 /*data.size()*/; j++) {
            std::cout << "--Considering adding the " << j << " feature\n";
            
            double accuracy = leave_one_out_cross_validation(j);

            if (accuracy > best_so_far_accuracy) {
                best_so_far_accuracy = accuracy;
                feature_to_add_at_this_level = j;
            };


        };

    };
};
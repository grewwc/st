#include <iostream>
#include <string>
#include "mock_data.h"

#include <future>

using namespace std;


void test_st(){
    auto images = get_images();
    auto res = fit(images, {90, 110, 21}, {90, 110, 21}, {1.5, 2.5, 10}, {1.5, 4.5, 20});
    cout << res.i0 << "  " << res.j0 << "  " << res.vi << "  " << res.vj << endl;
}

void what(const FittingSpace& a){}

void test_st_multi_core(){
    vector<Image> images = get_images();
    CombinedSpace space {{90, 111, 15}, {90, 111, 15}, {1.5, 2.5, 12}, {2.5, 3.5, 12}};
    vector<future<FittingResult>> res_fu;
    vector<CombinedSpace> sub_spaces = space.split(3);
    for(auto c : sub_spaces){
        cout << c << endl;
    }
    for(auto sub_space : sub_spaces){
        res_fu.push_back(async(launch::async, fit, images, sub_space[0].to_int(), sub_space[1].to_int(), sub_space[2], sub_space[3]));
    }

    vector<FittingResult> res;
    for(int i=0; i<res_fu.size(); i++){
        res.push_back(res_fu[i].get());
    }

    sort(res.begin(), res.end(), [](const FittingResult& r1, const FittingResult& r2){return r1.cost < r2.cost;});
    cout << res[0] << endl;
}
#include "api.h"
#include <cmath>
#include <climits>
#include "lsfit.h"
#include <climits>

#include <mutex>
#include <future>
#include <iomanip>

using namespace std;

/**  
 * Using bfs method to construct the sub-fitting spaces recursively
 * this function is initiated by "CombinedSpace::split"
*/
void bfs(const CombinedSpace &s, int n_split, int curr_idx, deque<CombinedSpace> &dq)
{
    if (curr_idx >= (int)s.size())
    {
        return;
    }

    const int n = (int)dq.size();
    for (int i = 0; i < n; i++)
    {
        CombinedSpace front = dq.front();
        dq.pop_front();

        vector<FittingSpace> subs = front[curr_idx].split(n_split);
        for (FittingSpace sub : subs)
        {
            CombinedSpace new_combined{front};
            new_combined[curr_idx] = sub;
            dq.emplace_back(new_combined);
        }
    }
    bfs(s, n_split, curr_idx + 1, dq);
}

vector<CombinedSpace> CombinedSpace::split(int n) const
{
    if (n == 1)
    {
        return {*this};
    }
    vector<CombinedSpace> res;
    res.reserve(pow(m_size, n));
    deque<CombinedSpace> dq;

    dq.push_back(*this);

    bfs(*this, n, 0, dq);
    copy(dq.cbegin(), dq.cend(), back_inserter(res));
    return res;
}

Image add_all_images(const std::vector<Image> &images, int row_shift, int col_shift)
{
    return Image::add_all(images, row_shift, col_shift);
}

/** the main function for finding the best params */
FittingResult fit(const std::vector<Image> &images, const FittingSpace &i0_space,
                  const FittingSpace &j0_space, const FittingSpace &vi_space, const FittingSpace &vj_space,
                  bool verbose)
{
    const auto i0s = static_cast<vector<double>>(i0_space);
    const auto j0s = static_cast<vector<double>>(j0_space);
    const auto vis = static_cast<vector<double>>(vi_space);
    const auto vjs = static_cast<vector<double>>(vj_space);

    double cost = numeric_limits<double>::max();
    int res_i0, res_j0;
    double res_vi, res_vj;

    const auto total_count = i0s.size() * j0s.size() * vis.size() * vjs.size();
    size_t count = 0l;

    for (auto i0 : i0s)
    {
        for (auto j0 : j0s)
        {
            for (auto vi : vis)
            {
                for (auto vj : vjs)
                {
                    if (verbose)
                    {
                        cout << setprecision(5) << "Progress "
                             << "( " << (double)count / total_count * 100 << "% )  " << count++ << "/" << total_count << "\n";
                    }
                    double curr_cost = Cost(images, i0, j0, vi, vj);
                    if (cost > curr_cost)
                    {
                        res_i0 = i0;
                        res_j0 = j0;
                        res_vi = vi;
                        res_vj = vj;
                        cost = curr_cost;
                    }
                }
            }
        }
    }

    FittingResult res{res_i0, res_j0, res_vi, res_vj};
    res.cost = cost;
    return res;
}

FittingResult fit_multi(const std::vector<Image> &images, const FittingSpace &x0,
                        const FittingSpace &y0, const FittingSpace &vi, const FittingSpace &vj, int cores, bool verbose)
{
    CombinedSpace space{x0, y0, vi, vj};
    vector<future<FittingResult>> res_fu;
    vector<CombinedSpace> sub_spaces = space.split(cores);
    // if (verbose)
    // {
    //     for (auto c : sub_spaces)
    //     {
    //         cout << c << endl;
    //     }
    // }
    for (auto sub_space : sub_spaces)
    {
        res_fu.push_back(async(launch::async, fit, images, sub_space[0].to_int(), sub_space[1].to_int(), sub_space[2], sub_space[3], verbose));
    }

    vector<FittingResult> res;
    res.reserve(res_fu.size());
    for (int i = 0; i < res_fu.size(); i++)
    {
        res.push_back(res_fu[i].get());
    }

    sort(res.begin(), res.end(), [](const FittingResult &r1, const FittingResult &r2) { return r1.cost < r2.cost; });
    return res[0];
}

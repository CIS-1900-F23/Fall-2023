#include <execution>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> v = {10,9,8,7,6,5,4,3,2,1};
    std::sort(std::execution::seq, v.begin(), v.end());

}
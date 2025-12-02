#include <emscripten/bind.h>
#include "rho_filter/rhoFilter.hpp"
#include <vector>

using namespace emscripten;

// Wrapper class to handle std::vector <-> Eigen conversion for Wasm
class RhoFilterWasm {
public:
    rhoFilter* filter;
    int dim;

    RhoFilterWasm(double dt, int state_dim, double alpha, double k1, double k2, double k3) {
        dim = state_dim;
        filter = new rhoFilter(dt, dim, alpha, k1, k2, k3);
    }

    ~RhoFilterWasm() { delete filter; }

    std::vector<double> update(const std::vector<double>& input) {
        // 1. Convert JS Array (std::vector) to Eigen Input
        Eigen::MatrixXd u(dim, 1);
        for(int i=0; i<dim; ++i) {
            u(i,0) = input[i];
        }

        // 2. Call Core Math (Returns p_hat)
        Eigen::MatrixXd res = filter->propagate_filter(u);

        // 3. Convert Eigen Result back to JS Array
        // Since propogate_filter now returns ONLY p_hat, we copy exactly that.
        std::vector<double> out;
        for(int i=0; i<res.size(); ++i) {
            out.push_back(res(i));
        }
        
        // Result: [p_hat_0, p_hat_1, ...] 
        // For dim=1, this is just [p_hat]
        return out; 
    }
};

EMSCRIPTEN_BINDINGS(rho_module) {
    register_vector<double>("VectorDouble");
    
    class_<RhoFilterWasm>("RhoFilter")
        .constructor<double, int, double, double, double, double>()
        .function("update", &RhoFilterWasm::update);
}
#include <iostream>
#include <cmath>
#include <unsupported/Eigen/MatrixFunctions>
#include <Eigen/Dense>

using namespace std;

double main(){
    double sampling_time = 0.01; // seconds
    int n = 3; // State space dim
    int dim = 4;
    double alpha = 0.0f;
    double k1 = 0.0f;
    double k2 = 0.0f;
    double k3 = 0.0f;

    double m_12 = -(3 - pow(k1, 2) + (2*k1 + k2 + k3) * (k1 + k2));
    double m_13 = -(k1 + k2);
    double m_14 = -(1 + (k1 + k2)*(k3 - k1));
    double m_32 = -((2 * k1 + k2 + k3)*(k1 + k2) + 1);
    double m_33 = -(2 * k1 + k2);
    double m_42 = -(2 * k1 + k2 + k3);
    double m_43 = -(k1 + k2)*(k3 - k1);

    Eigen::MatrixXd M(dim,dim);
    M << 0,    1,   0 ,   0,
        m_12,  0,  m_32,  m_42,
        m_13,  0,  m_33,  -1,
        m_14,  0,  m_43,  -k3;

    double n_12 = 3 - pow(k1, 2) + (2*k1 + k2 + k3)*(k1 + k2);
    double n_13 = k1 + k2;
    double n_14 = 1 + (k1 + k2)*(k3 - k1);

    Eigen::MatrixXd N(dim,1);
    N << 0,
        n_12,
        n_13,
        n_14;
        
    Eigen::MatrixXd E(dim,1);
    E << 0,
        1,
        0,
        0;
    
    Eigen::MatrixXd S(1,dim);
    S << -1, 0, -1, 0;

    double propagate_filter(double last_position){
        // Initial filter state
        Eigen::MatrixXd zeta(4*n,1) // zeta = [ q_hat, p_hat, m, sigma]

        double  = 0; // q_k
        Eigen::MatrixXd M_inv = M.colPivHouseholderQr().inverse();
        Eigen::MatrixXd A_d = (M * sampling_time).exp();
        Eigen::MatrixXd I_n = Eigen::MatrixXd::Identity(4, 4);
        Eigen::MatrixXd B_q = M_inv * (A_d - I_n) * N;
        Eigen::MatrixXd B_s = M_inv * (A_d - I_n) * E;

        zeta = A_d * zeta + B_q * last_position + alpha * B_s * matrix_signum_elementwise(S * zeta + last_position)
    }
    
    return 0;
}

template <typename T>
int signum(T x){
    return (x < 0) ? -1 : (x > 0);
}

template <typename T>
T matrix_signum_elementwise(const T& A) {
    return A.unaryExpr([](double x) {
        return (double)signum(x); 
    });
}
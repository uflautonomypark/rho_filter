#include "rho_filter/rhoFilter.hpp"
#include <unsupported/Eigen/MatrixFunctions>
#include <unsupported/Eigen/KroneckerProduct>
#include <cmath>

rhoFilter::rhoFilter(
    double sampling_time,
    int state_space_dim,
    double alpha,
    double k1,
    double k2,
    double k3
) : 
    sampling_time(sampling_time),
    alpha(alpha),
    k1(k1),
    k2(k2),
    k3(k3)
{   
    zeta_dim = 4 * state_space_dim;
    num_inputs = state_space_dim;

    m.resize(4, 4);
    n.resize(4, 1);
    e.resize(4, 1);
    s.resize(1, 4);

    I_n = Eigen::MatrixXd::Identity(num_inputs, num_inputs);
    I_4 = Eigen::MatrixXd::Identity(4, 4);

    m_12 = -(3 - pow(k1,2) + (2*k1 + k2 + k3)*(k1 + k2));
    m_13 = -(k1 + k2);
    m_14 = -(1 + (k1 + k2)*(k3 - k1));
    m_32 = -((2*k1 + k2 + k3)*(k1 + k2) + 1);
    m_33 = -(2*k1 + k2);
    m_42 = -(2*k1 + k2 + k3);
    m_43 = -(k1 + k2)*(k3 - k1);

    m << 0,     1,      0,      0,
         m_12,  0,      m_32,   m_42,
         m_13,  0,      m_33,  -1,
         m_14,  0,      m_43,  -k3;

    n_12 = 3 - pow(k1,2) + (2*k1 + k2 + k3)*(k1 + k2);
    n_13 = k1 + k2;
    n_14 = 1 + (k1 + k2)*(k3 - k1);

    n << 0,
         n_12,
         n_13,
         n_14;
    e << 0,
         1,
         0,
         0;
    s << -1, 0, -1, 0;
    
    m_inv = m.colPivHouseholderQr().inverse();
    a_d = (m * sampling_time).exp();
    b_q = m_inv * (a_d - I_4) * n;
    b_s = m_inv * (a_d - I_4) * e;

    A_d = Eigen::kroneckerProduct(a_d, I_n);
    B_q = Eigen::kroneckerProduct(b_q, I_n);
    B_s = Eigen::kroneckerProduct(b_s, I_n);
    S = Eigen::kroneckerProduct(s, I_n);

    v.resize(num_inputs, 1);
    v.setZero();
    zeta.resize(zeta_dim, 1);
    zeta.setZero();
    next_zeta.resize(zeta_dim, 1);
    next_zeta.setZero();
}

void rhoFilter::propagate_filter(const Eigen::MatrixXd& last_position) // q_k, which must be a (n,1)
{
    // S is (n,4n)
    // N is (4n,n)
    // E is (4n,n)
    // M is (4n,4n)
    // zeta is (4n,1)
    // A_d is (4n,4n)
    // B_q is (4n,n)
    // B_s is (4n,n)

    v.noalias() = S * zeta;
    v += last_position;
    next_zeta.noalias() = A_d * zeta;
    next_zeta.noalias() += B_q * last_position;
    next_zeta.noalias() += alpha * B_s * v.cwiseSign();
    zeta = next_zeta;
}

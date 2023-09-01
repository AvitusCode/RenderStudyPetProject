#pragma once
#include "ECS/components/AssimpModel.h"
#include <glm/glm.hpp>
#include <array>
#include <cmath>
#include <utility>

namespace jd::math
{
	/*
	* Finding eigenvals to estimating eigenvectors
	* return R, S, T vectors fo |l1| >= |l2| >= |l3| where is li are eigenvals, sorted by not decreasing order
	*/
	std::array<glm::vec3, 3> findEigenVectors(const Model& model);

    namespace // for memory managment
    {
        template<typename Entry>
        void AllocMatrix(size_t nrows, size_t ncols, Entry*** paaX);
        template<typename Entry>
        void DeallocMatrix(Entry*** paaX);
    }

    /// @class Jacobi
    /// @brief Calculate the eigenvalues and eigevectors of a symmetric matrix
    ///        using the Jacobi eigenvalue algorithm.
    /// @note  The "Vector" and "Matrix" type arguments can be any 
    ///        C or C++ object that support indexing, including pointers or vectors.
    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix = Matrix>
    class Jacobi
    {
    private:
        size_t n_;            
        Scalar** M_;     

        // Precomputed cosine, sine, and tangent of the most recent rotation angle:
        Scalar c;        
        Scalar s;         
        Scalar t; // (note |t|<=1)
        size_t* max_idx_row; // for row i, the index j of the maximum element where j>i

    public:
        Jacobi(const Jacobi<Scalar, Vector, Matrix, ConstMatrix>& source);
        Jacobi(Jacobi<Scalar, Vector, Matrix, ConstMatrix>&& other);
        void swap(Jacobi<Scalar, Vector, Matrix, ConstMatrix>& other) noexcept;
        Jacobi<Scalar, Vector, Matrix, ConstMatrix>& operator=(const Jacobi<Scalar, Vector, Matrix, ConstMatrix>& source);

        void SetSize(size_t n);

        Jacobi(size_t n = 0) {
            Init();
            SetSize(n);
        }
        ~Jacobi() noexcept {
            Dealloc();
        }

        // @typedef choose the criteria for sorting eigenvalues and eigenvectors
        enum class SortCriteria {
            DEFAULT,
            DECREASING_EVALS,
            INCREASING_EVALS,
            DECREASING_ABS_EVALS,
            INCREASING_ABS_EVALS
        };

        /// @brief Calculate all the eigenvalues and eigevectors of a symmetric matrix
        ///        using the Jacobi eigenvalue algorithm:
        ///        https://en.wikipedia.org/wiki/Jacobi_eigenvalue_algorithm
        /// @returns The number of Jacobi iterations attempted, which should be > 0.
        ///          If the return value is not strictly > 0 then convergence failed.
        /// @note  To reduce the computation time further, set calc_evecs=false.
        size_t makeEval(ConstMatrix mat, Vector eval, Matrix evec, 
            SortCriteria sort_criteria = SortCriteria::DECREASING_EVALS,
            bool calc_evecs = true,      
            size_t max_iterations = 50); 

    private:
        void CalcRot(Scalar const* const* M,size_t i, size_t j);
        void ApplyRot(Scalar** M, size_t i, size_t j);
        void ApplyRotLeft(Matrix E, size_t i, size_t j);   
        size_t MaxEntryRow(Scalar const* const* M, size_t i) const;

        /// @brief Find the indices (i_max, j_max) marking the location of the
        ///        entry in the matrix with the largest absolute value.  This
        ///        uses the max_idx_row[] array to find the answer in O(n) time.
        /// @returns pair of i_max and j_max
        std::pair<size_t, size_t> MaxEntry(Scalar const* const* M) const;
        void SortRows(Vector v, Matrix M, size_t n, SortCriteria s = SortCriteria::DECREASING_EVALS) const;

        // memory management:
        void Alloc(size_t N);
        void Init();
        void Dealloc() noexcept;  
    };

    namespace
    {
        template<typename Entry>
        void AllocMatrix(size_t nrows, size_t ncols, Entry*** paaX)
        {
            if (paaX) {
                throw std::bad_alloc{};
            }

            *paaX = new Entry * [nrows];  
            (*paaX)[0] = new Entry[nrows * ncols];

            for (size_t iy = 0; iy < nrows; iy++) {
                (*paaX)[iy] = (*paaX)[0] + iy * ncols;
            }
        }

        template<typename Entry>
        void DeallocMatrix(Entry*** paaX) 
        {
            if (paaX && *paaX) 
            {
                delete[](*paaX)[0];
                delete[](*paaX);
                *paaX = nullptr;
            }
        }
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    size_t Jacobi<Scalar, Vector, Matrix, ConstMatrix>::makeEval(ConstMatrix mat, Vector eval, Matrix evec,
            SortCriteria sort_criteria, 
            bool calc_evec,    
            size_t max_iterations)
    {
        for (size_t i = 0; i < n_; i++)
            for (size_t j = i; j < n_; j++)          
                M_[i][j] = mat[i][j];     

        if (calc_evec) {
            for (size_t i = 0; i < n_; i++)
                for (size_t j = 0; j < n_; j++)
                    evec[i][j] = (i == j) ? 1.0 : 0.0; //Set evec equal to the identity matrix
        }

        for (size_t i = 0; i < n_ - 1; i++) {
            max_idx_row[i] = MaxEntryRow(M_, i);
        }

        // -- Iteration --
        size_t n_iters;
        size_t max_num_iters = max_iterations * n_ * (n_ - 1) / 2;
        for (n_iters = 1; n_iters <= max_num_iters; n_iters++) 
        {
            auto [i, j] = MaxEntry(M_); // Find the maximum entry in the matrix. Store in i,j

            // If M[i][j] is small compared to M[i][i] and M[j][j], set it to 0.
            if ((M_[i][i] + M_[i][j] == M_[i][i]) && (M_[j][j] + M_[i][j] == M_[j][j])) {
                M_[i][j] = 0.0;
                max_idx_row[i] = MaxEntryRow(M_, i); //update max_idx_row[i]
            }

            if (M_[i][j] == 0.0) {
                break;
            }

            CalcRot(M_, i, j); 
            ApplyRot(M_, i, j); 
            if (calc_evec) {
                ApplyRotLeft(evec, i, j);
            }

        }

        // -- Post-processing --
        for (size_t i = 0; i < n_; i++) {
            eval[i] = M_[i][i];
        }
        SortRows(eval, evec, n_, sort_criteria);

        if ((n_iters > max_num_iters) && (n_ > 1)) {
            return 0; // indicate an error occured.
        }

        return n_iters;
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::CalcRot(Scalar const* const* M, size_t i, size_t j)
    {
        t = 1.0; // = tan(θ)
        Scalar M_jj_ii = (M[j][j] - M[i][i]);

        if (M_jj_ii != 0.0) 
        {
            // kappa = (M[j][j] - M[i][i]) / (2*M[i][j])
            Scalar kappa = M_jj_ii;
            t = 0.0;
            Scalar M_ij = M[i][j];
            if (M_ij != 0.0) 
            {
                kappa /= (2.0 * M_ij);
                t = 1.0 / (std::sqrt(1 + kappa * kappa) + std::abs(kappa));
                if (kappa < 0.0) {
                    t = -t;
                }
            }
        }
        c = 1.0 / std::sqrt(1 + t * t);
        s = c * t;
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::ApplyRot(Scalar** M, size_t i, size_t j)    
    {
        // Compute the diagonal elements of M which have changed:
        M[i][i] -= t * M[i][j];
        M[j][j] += t * M[i][j];
        // Note: This is algebraically equivalent to:
        // M[i][i] = c*c*M[i][i] + s*s*M[j][j] - 2*s*c*M[i][j]
        // M[j][j] = s*s*M[i][i] + c*c*M[j][j] + 2*s*c*M[i][j]

        //Update the off-diagonal elements of M which will change (above the diagonal)

        M[i][j] = 0.0;

        for (size_t w = 0; w < i; w++) {        // 0 <= w <  i  <  j < n
            M[i][w] = M[w][i]; 
            M[w][i] = c * M[w][i] - s * M[w][j]; 
            if (i == max_idx_row[w]) {
                max_idx_row[w] = MaxEntryRow(M, w);
            }
            else if (std::abs(M[w][i]) > std::abs(M[w][max_idx_row[w]])) {
                max_idx_row[w] = i;
            }
        }

        for (size_t w = i + 1; w < j; w++) {      // 0 <= i <  w  <  j < n
            M[w][i] = M[i][w]; 
            M[i][w] = c * M[i][w] - s * M[w][j]; 
        }

        for (size_t w = j + 1; w < n_; w++) {      // 0 <= i < j+1 <= w < n
            M[w][i] = M[i][w]; 
            M[i][w] = c * M[i][w] - s * M[j][w];
        }
        max_idx_row[i] = MaxEntryRow(M, i);

        for (size_t w = 0; w < i; w++) {        // 0 <=  w  <  i <  j < n
            M[w][j] = s * M[i][w] + c * M[w][j];
            if (j == max_idx_row[w]) {
                max_idx_row[w] = MaxEntryRow(M, w);
            }
            else if (std::abs(M[w][j]) > std::abs(M[w][max_idx_row[w]])) {
                max_idx_row[w] = j;
            }
        }

        for (size_t w = i + 1; w < j; w++) {      // 0 <= i+1 <= w <  j < n
            M[w][j] = s * M[w][i] + c * M[w][j]; 
            if (j == max_idx_row[w]) {
                max_idx_row[w] = MaxEntryRow(M, w);
            }
            else if (std::abs(M[w][j]) > std::abs(M[w][max_idx_row[w]])) {
                max_idx_row[w] = j;
            }
        }

        for (size_t w = j + 1; w < n_; w++) {      // 0 <=  i  <  j <  w < n
            M[j][w] = s * M[w][i] + c * M[j][w];
        }
   
        max_idx_row[j] = MaxEntryRow(M, j);
    } 

    ///@brief Multiply matrix M on the LEFT side by a transposed rotation matrix R^T
    ///       This matrix performs a rotation in the i,j plane by angle θ  (where
    ///       the arguments "s" and "c" refer to cos(θ) and sin(θ), respectively).
    /// @verbatim
    ///   E'_uv = Σ_w  R_wu * E_wv
    /// @endverbatim

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::ApplyRotLeft(Matrix E, size_t i, size_t j)
    {
        for (size_t v = 0; v < n_; v++)
        {
            Scalar Eiv = E[i][v];
            E[i][v] = c * E[i][v] - s * E[j][v];
            E[j][v] = s * Eiv + c * E[j][v];
        }
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    size_t Jacobi<Scalar, Vector, Matrix, ConstMatrix>::MaxEntryRow(Scalar const* const* M, size_t i) const {
        size_t j_max = i + 1;
        for (size_t j = i + 2; j < n_; j++) {
            if (std::abs(M[i][j]) > std::abs(M[i][j_max])) {
                j_max = j;
            }
        }
        return j_max;
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    std::pair<size_t, size_t> Jacobi<Scalar, Vector, Matrix, ConstMatrix>::MaxEntry(Scalar const* const* M) const 
    {
        size_t i_max = 0;
        size_t j_max = max_idx_row[i_max];
        Scalar max_entry = std::abs(M[i_max][j_max]);
        size_t nm1 = n_ - 1;
        for (size_t i = 1; i < nm1; i++) {
            size_t j = max_idx_row[i];
            if (std::abs(M[i][j]) > max_entry) {
                max_entry = std::abs(M[i][j]);
                i_max = i;
                j_max = j;
            }
        }

        return { i_max, j_max };
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::SortRows(Vector eval, Matrix evec, size_t n, SortCriteria sort_criteria) const
    {
        for (size_t i = 0; i < n - 1; i++) 
        {
            size_t i_max = i;
            for (size_t j = i + 1; j < n; j++) 
            {
                switch (sort_criteria) 
                {
                case SortCriteria::DECREASING_EVALS:
                    if (eval[j] > eval[i_max])
                        i_max = j;
                    break;
                case SortCriteria::INCREASING_EVALS:
                    if (eval[j] < eval[i_max])
                        i_max = j;
                    break;
                case SortCriteria::DECREASING_ABS_EVALS:
                    if (std::abs(eval[j]) > std::abs(eval[i_max]))
                        i_max = j;
                    break;
                case SortCriteria::INCREASING_ABS_EVALS:
                    if (std::abs(eval[j]) < std::abs(eval[i_max]))
                        i_max = j;
                    break;
                default:
                    break;
                }
            }

            std::swap(eval[i], eval[i_max]);
            for (size_t k = 0; k < n; k++) {
                std::swap(evec[i][k], evec[i_max][k]);
            }
        }
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::Init() {
        n_ = 0;
        M_ = nullptr;
        max_idx_row = nullptr;
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::SetSize(size_t n) 
    {
        Dealloc();
        Alloc(n);
    }

    // memory management:
    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::Alloc(size_t N) {
        n_ = N;
        if (n_ > 0) {
            max_idx_row = new size_t[n_];
            AllocMatrix(n_, n_, &M_);
        }
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::Dealloc() noexcept 
    {
        if (max_idx_row) {
            delete[] max_idx_row;
            max_idx_row = nullptr;
        }
        DeallocMatrix(&M_);
        Init();
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    Jacobi<Scalar, Vector, Matrix, ConstMatrix>::Jacobi(const Jacobi<Scalar, Vector, Matrix, ConstMatrix>& source)
    {
        Init();
        SetSize(source.n_);
        std::copy(source.max_idx_row, source.max_idx_row + n_, max_idx_row);
        for (size_t i = 0; i < n_; i++) {
            std::copy(source.M_[i], source.M_[i] + n_, M_[i]);
        }
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    void Jacobi<Scalar, Vector, Matrix, ConstMatrix>::swap(Jacobi<Scalar, Vector, Matrix, ConstMatrix>& other) noexcept 
    {
        std::swap(n_, other.n_);
        std::swap(max_idx_row, other.max_idx_row);
        std::swap(M_, other.M_);
    }

    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    Jacobi<Scalar, Vector, Matrix, ConstMatrix>::Jacobi(Jacobi<Scalar, Vector, Matrix, ConstMatrix>&& other)
        : Jacobi()
    {
        this->swap(other);
    }
    template<typename Scalar, typename Vector, typename Matrix, typename ConstMatrix>
    Jacobi<Scalar, Vector, Matrix, ConstMatrix>&
        Jacobi<Scalar, Vector, Matrix, ConstMatrix>::operator=(const Jacobi<Scalar, Vector, Matrix, ConstMatrix>& source) 
    {
        Jacobi<Scalar, Vector, Matrix, ConstMatrix> jacoby{source};
        this->swap(jacoby);
        return *this;
    }
}
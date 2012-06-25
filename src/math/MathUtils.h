#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cstring>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstdlib>
//--
#include "utils/Constants.h"


namespace MathUtils{

    template<typename T>
    inline T checkClose(const T a, const T b, const T eps){
        return std::abs(a-b) <= eps;
    }

    /// \brief Check if float is NaN (not a number)
    inline bool isNaN(float val){ return val != val; }

    /// \brief Check if double is NaN (not a number)
    inline bool isNaN(double val){ return val != val; }

    template<typename T>
    inline T max(T a, T b){
        return a >= b ? a : b;
    }

    template<typename T>
    inline T min(T a, T b){
        return a <= b ? a : b;
    }

    /**
     *  Check if a given value is in a range (inclusive).
     *  @param val is the value to test.
     *  @param min is the lower end of the range.
     *  @param max is te upper end of the range.
     *  @return true if val is in [min, max] false otherwise.
     */
    template<typename T>
    inline bool inRangeInclusive(T val, T min, T max){
        return val >= min && val <= max;
    }

    /**
     *  Check if a given value is in a range (exclusive).
     *  @param val is the value to test.
     *  @param min is the lower end of the range.
     *  @param max is te upper end of the range.
     *  @return true if val is in (min, max) false otherwise.
     */
    template<typename T>
    inline bool inRangeExclusive(T val, T min, T max){
        return val > min && val < max;
    }


    /**
     *  Check if a given value is between two numbers (inclusive).
     *  @param val is the value to test.
     *  @param a is the first range value.
     *  @param b is the second range value.
     *  @return true if val is in [min(a,b),max(a,b)], false otherwise.
     */
    template<typename T>
    inline bool betweenInclusive(T val, T a, T b){
        const T min = MathUtils::min<T>(a,b);
        const T max = MathUtils::max<T>(a,b);
        return MathUtils::inRangeInclusive<T>(val, min, max);
    }

    /**
     *  Check if a given value is between two numbers (exclusive).
     *  @param val is the value to test.
     *  @param a is the first range value.
     *  @param b is the second range value.
     *  @return true if val is in (min(a,b),max(a,b)), false otherwise.
     */
    template<typename T>
    inline bool betweenExclusive(T val, T a, T b){
        const T min = MathUtils::min<T>(a,b);
        const T max = MathUtils::max<T>(a,b);
        return MathUtils::inRangeExclusive<T>(val, min, max);
    }

    /*
    template<typename T>
    inline T roundDownToNearest(T round, T val){
        assert(round > ((T)0.0));
        int temp = (int)(val / round);
        return ((T)temp) * round;
    }

    template<typename T>
    inline T roundUpToNearest(T round, T val){
        assert(round > ((T)0.0));
        int temp = (int)(val / round);
        return ((T)(temp+1)) * round;
    }
    */

    inline float degreesToRads(float degrees){
        return degrees * (Constants::PI_FLOAT / 180.0f);
    }

    inline float radsToDegrees(float rads){
        return rads * (180.0f / Constants::PI_FLOAT);
    }

    template<typename T>
    struct QuadraticEqnRes{
        T sol1, sol2; //Solutions.  Can be undefined for solCount <= 1.
        int solCount; //Number of solutions (may be 0,1,2)

        //No solutions c-tor
        QuadraticEqnRes() : sol1(T(-1.0)), sol2(T(-1.0)), solCount(0) {}

        //1 Solution c-tor
        QuadraticEqnRes(const T a) : sol1(a), sol2(a), solCount(1) {}

        //2 Solution c-tor
        QuadraticEqnRes(const T a, const T b) : sol1(a), sol2(b), solCount(2) {}

        friend std::ostream& operator<<(std::ostream& os, const struct QuadraticEqnRes& res){
            os << res.solCount << " solutions {";
            if(res.solCount == 1){
                os << res.sol1;
            }else if(res.solCount == 2){
                os << res.sol1 << ", " << res.sol2;
            }
            os << "}";
            return os;
        }
    };

    /**
     *  Solve a quadratic equation and return only NON-IMAGINARY
     *  solutions.
     */
    template<typename T>
    inline struct QuadraticEqnRes<T> solveQuadratic(const T a, const T b, const T c){
        //Solve ax^2 + bx + c = 0
        const double EPSILON_DISCRIMINANT = 0.0000001;
        //std::cout << a << "x^2 + " << b << "x + " << c << std::endl;
        const T disc = (b*b) - T(4.0) * a * c;
        if(disc < T(0.0)){ //Miss
            return QuadraticEqnRes<T>();
        }else if(MathUtils::checkClose<T>(disc, T(0.0), T(EPSILON_DISCRIMINANT))){ 
            //std::cout << "We have one solution, discrim = " << disc << std::endl;
            return QuadraticEqnRes<T>(-b / (T(2.0) * a));
        }else{//Hit
            const T t1 = (-b + sqrt(disc)) / (T(2.0) * a);
            const T t2 = (-b - sqrt(disc)) / (T(2.0) * a);
            //std::cout << " hits @ " << t1 << " , " << t2 << std::endl;
            //std::cout << "We have two solution!" << std::endl;
            return QuadraticEqnRes<T>(t1,t2);
        }
    }


    /**
     *  Clamp val to be in the range [minVal, maxVa].
     *
     *  Returns a clamped version of val.
     */
    template<typename T>
    inline T clamp(T val, T minVal, T maxVal){
        return MathUtils::min<T>( MathUtils::max<T>(val, minVal), maxVal);
    }

    //TODO: Add an abs<T> function to avoid issues
    //like accidentally calling abs for integers


    //Note: this is specialized but for obscure C++ reasons the specialization must occur outside the
    //class brackets (whyyyy?) :(
    /*
    template<typename T>
    inline T lerp(T in, T inMin, T inMax, T outMin, T outMax){
        double inD     = static_cast<double>(in    );
        double inMinD  = static_cast<double>(inMin );
        double inMaxD  = static_cast<double>(inMax );
        double outMinD = static_cast<double>(outMin);
        double outMaxD = static_cast<double>(outMax);;
        double t = (inD - inMin)/(inMaxD - inMinD);
        return (1.0 - t) * outMinD + t * outMaxD;

        //TODO: Why won't the below code work wtf?
        //return MathUtils::lerp<double>(inD, inMinD, inMaxD, outMaxD);

        //T t = (inMax - inMin)/(inMax - inMin);
        //return (1.0 - t) * outMin + t * outMax;


        //________
    }


    //Specialization for doubles
    template<>
    inline double lerp(double in, double inMin, double inMax, double outMin, double outMax){
        double t = (in - inMin)/(inMax - inMin);
        assert(t >= 0.0 && t <= 1.0);
        return (1.0 - t) * outMin + t * outMax;
    }

    //Specialization for floats
    template<>
    inline float lerp(float in, float inMin, float inMax, float outMin, float outMax){
        float t = (in - inMin)/(inMax - inMin);
        assert(t >= 0.0f && t <= 1.0f);
        return (1.0 - t) * outMin + t * outMax;
    }




    template<typename T>
    inline T lerpGeneric(T in, T inA, T inB, T outA, T outB){

        //Check that in is in the range [inA, inB] or [inB, inA]
        assert((in >= inA && in <= inB) || (in >= inB && in <= inA));
        //std::cout << "Error in lerp generic! not implemented!" << std::endl;
        //assert(false); //TODO: Fix this stuff

        // ********************
        //T dFromGoal = fabsf(in - inB);
        //T dGap = fabsf(inA - inB);
        //T t = dFromGoal / dGap;
        //if(t < 0.0f || t > 1.0f){
        //    std::cout << t << std::endl;
        //}
        //if(outA <= outB){
        //    return MathUtils::lerp<T>(t, (T)0.0, (T)1.0, outA, outB);
        //}else{
        //    return MathUtils::lerp<T>(t, (T)0.0, (T)1.0, (T)0.0, outA - outB) + outB;
        //}
        // ******************

        if(in == inA){
            return outA;
        }else if(in == inB){
            return outB;
        }

        T percFromAtoB = fabs(in - inA)/fabs(inB - inA);
        T dir = outA < outB ? static_cast<T>(1.0) : static_cast<T>(-1.0);
        return ((fabs(outB - outA) * percFromAtoB) * dir) + outA;
    }
    */

    //Invert a 4x4 matrix
    //TODO: Copied from PBRT, write my own
    /*
    float* gaussJordanInvert4x4(const float mat[4][4]){
        int indxc[4], indxr[4];
        int ipiv[4] = { 0, 0, 0, 0 };
        float minv[4][4]; //return me
        memcpy(minv, mat, 4*4*sizeof(float));
        for (int i = 0; i < 4; i++) {
            int irow = -1, icol = -1;
            float big = 0.;
            // Choose pivot
            for (int j = 0; j < 4; j++) {
                if (ipiv[j] != 1) {
                    for (int k = 0; k < 4; k++) {
                        if (ipiv[k] == 0) {
                            if (fabsf(minv[j][k]) >= big) {
                                big = float(fabsf(minv[j][k]));
                                irow = j;
                                icol = k;
                            }
                        }
                        else if (ipiv[k] > 1){
                            std::cerr << "Singular matrix passed to invert." << std::endl;
                            assert(false);
                        }
                    }
                }
            }
            ++ipiv[icol];
            // Swap rows _irow_ and _icol_ for pivot
            if (irow != icol) {
                for (int k = 0; k < 4; ++k)
                    std::swap<float>(minv[irow][k], minv[icol][k]);
            }
            indxr[i] = irow;
            indxc[i] = icol;
            if (minv[icol][icol] == 0.){
                std::cerr << "Singular matrix passed to invert." << std::endl;
                assert(false);
            }

            // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
            float pivinv = 1.f / minv[icol][icol];
            minv[icol][icol] = 1.f;
            for (int j = 0; j < 4; j++)
                minv[icol][j] *= pivinv;

            // Subtract this row from others to zero out their columns
            for (int j = 0; j < 4; j++) {
                if (j != icol) {
                    float save = minv[j][icol];
                    minv[j][icol] = 0;
                    for (int k = 0; k < 4; k++)
                    minv[j][k] -= minv[icol][k]*save;
                }
            }
        }
        // Swap columns to reflect permutation
        for (int j = 3; j >= 0; j--) {
            if (indxr[j] != indxc[j]) {
                for (int k = 0; k < 4; k++)
                std::swap<float>(minv[k][indxr[j]], minv[k][indxc[j]]);
            }
        }

        //Malloc for return
        float* ret = (float*) malloc(4 * 4 * sizeof(float));
        memcpy(ret, minv, 4 * 4 * sizeof(float));
        return ret;
    }
    */


    //----------

    //Invert a NxN matrix
    //TODO: Copied from PBRT, write my own
    /*
    template<typename T, int N>
    T* gaussJordanInvertNxN(const T mat[N][N]){
        int indxc[N], indxr[N];
        int ipiv[N];
        for(int f = 0; f < N; f++){
            ipiv[f] = 0;
        }

        T minv[N][N]; //return me
        memcpy(minv, mat, N*N*sizeof(T));
        for (int i = 0; i < N; i++) {
            int irow = -1, icol = -1;
            T big = 0.;
            // Choose pivot
            for (int j = 0; j < N; j++) {
                if (ipiv[j] != 1) {
                    for (int k = 0; k < N; k++) {
                        if (ipiv[k] == 0) {
                            if (fabsf(minv[j][k]) >= big) {
                                big = T(fabsf(minv[j][k]));
                                irow = j;
                                icol = k;
                            }
                        }
                        else if (ipiv[k] > 1){
                            std::cerr << "Singular matrix passed to invert." << std::endl;
                            assert(false);
                        }
                    }
                }
            }
            ++ipiv[icol];
            // Swap rows _irow_ and _icol_ for pivot
            if (irow != icol) {
                for (int k = 0; k < N; ++k)
                    std::swap<T>(minv[irow][k], minv[icol][k]);
            }
            indxr[i] = irow;
            indxc[i] = icol;
            if (minv[icol][icol] == 0.){
                std::cerr << "Singular matrix passed to invert." << std::endl;
                assert(false);
            }

            // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
            T pivinv = ((T)1.0) / minv[icol][icol];
            minv[icol][icol] = ((T)1.0);
            for (int j = 0; j < N; j++)
                minv[icol][j] *= pivinv;

            // Subtract this row from others to zero out their columns
            for (int j = 0; j < N; j++) {
                if (j != icol) {
                    T save = minv[j][icol];
                    minv[j][icol] = 0;
                    for (int k = 0; k < N; k++)
                    minv[j][k] -= minv[icol][k]*save;
                }
            }
        }
        // Swap columns to reflect permutation
        for (int j = N-1; j >= 0; j--) {
            if (indxr[j] != indxc[j]) {
                for (int k = 0; k < N; k++)
                std::swap<T>(minv[k][indxr[j]], minv[k][indxc[j]]);
            }
        }

        //Malloc for return
        T* ret = (T*) malloc(N * N * sizeof(T));
        memcpy(ret, minv, N * N * sizeof(T));
        return ret;
    }
*/
}



#endif //MATH_UTILS_H


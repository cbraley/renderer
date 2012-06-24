#ifndef TEST_BRDF_GEOM_H
#define TEST_BRDF_GEOM_H

#include <UnitTest++.h>
//--
#include "materials/BRDFGeom.h"
#include "math/Vector.h"

using namespace BRDFGeom;

SUITE(TestBRDFGeom){

    //TODO: USE RANDOM VECTOR TESTS AND FIGURE OUT THE b thing
    static Vector randomVector(bool makeUnit = false){
        float rx =  ((float)rand()/(float)RAND_MAX) - 0.5f;
        float ry =  ((float)rand()/(float)RAND_MAX) - 0.5f;
        float rz =  ((float)rand()/(float)RAND_MAX) - 0.5f;
        
        Vector v(rx,ry,rz);
        return makeUnit ? v.getNormalized() : v;
    }

    /// Check if (theta, phi) is close to (thetaExp, phiExp)
    /// degEps = degree epsilon for comparison
    static bool anglesClose(float theta, float phi, float thetaExp, float phiExp, float degEps = 0.1f){
        const float epsRadians = (degEps / 360.0f) / (PI_FLOAT * 2.0f);
        return
            fabs(theta - thetaExp) < epsRadians &&
            fabs(phi - phiExp    ) < epsRadians;
    }


    /// Check if two vectors are close and both are normalized
    static bool vecsClose(const Vector& a, const Vector& b, float epsDeg = 0.1f, const float epsNorm = 1e-6){
        const float angleRads = a.angle(b);
        const float angleDegrees = angleRads * (180.0f/PI_FLOAT);
        return ((a == b) ||
            (angleDegrees < epsDeg)) && //vectors are close
            fabs(a.magnitude() - 1.0f) < epsNorm &&
            fabs(b.magnitude() - 1.0f) < epsNorm;
    }

    static bool vecsCloseNoNorm(const Vector& a, const Vector& b, float epsDeg = 0.001f){
        const float angleRads = a.angle(b);
        const float angleDegrees = angleRads * (180.0f/PI_FLOAT);
        return ((a == b) ||
            (angleDegrees < epsDeg));//vectors are close

    }

    static bool isOrthoNormal(const Vector& a, const Vector& b, const Vector& c){
        const float EPSILON_VAL = 1e-7;

        const float la = a.magnitude();
        const float lb = b.magnitude();
        const float lc = c.magnitude();

        const float dab = a.dot(b);
        const float dac = a.dot(c);
        const float dbc = b.dot(c);

        return
            fabs(la - 1.0f) < EPSILON_VAL &&
            fabs(lb - 1.0f) < EPSILON_VAL &&
            fabs(lc - 1.0f) < EPSILON_VAL &&
            fabs(dab) < EPSILON_VAL &&
            fabs(dac) < EPSILON_VAL &&
            fabs(dbc) < EPSILON_VAL;
    }

    TEST(TestAnsioIsoEquivalence){

    }


    TEST(TestAnisoCoords){

        //Degree measurements can be off by ths much
        const float DEGREE_EPS = 0.1f;

        //Start with a simple test with the normal aligned to the z axis
        //    normal along z
        //    in and out vectors are the same(along x)
        Vector n (0.0f, 0.0f, 1.0f);
        Vector wo(1.0f, 0.0f, 0.0f);
        Vector wi(1.0f, 0.0f, 0.0f);
        float theta_in, theta_out, phi_in, phi_out, phi_diff;
        theta_in = theta_out = phi_in = phi_out = phi_diff = -999.0f;
        genAnisotropicBRDFAngularCoords(n, wo, wi, theta_in, phi_in, theta_out, phi_out);
        phi_diff = abs(phi_in - phi_out);

        CHECK_CLOSE(90.0f , theta_in ,  DEGREE_EPS);
        CHECK_CLOSE(90.0f , theta_out,  DEGREE_EPS);
        CHECK_CLOSE(0.0f  , phi_diff ,  DEGREE_EPS);


        //    normal along -x
        //    wo and wi along y and -y
        n  = Vector(-1.0f, 0.0f, 0.0f);
        wi = Vector(0.0f,  1.0f, 0.0f);
        wo = Vector(0.0f, -1.0f, 0.0f);

        theta_in = theta_out = phi_in = phi_out = phi_diff = -999.0f;
        genAnisotropicBRDFAngularCoords(n, wo, wi, theta_in, phi_in, theta_out, phi_out);
        phi_diff = abs(phi_in - phi_out);

        CHECK_CLOSE(90.0f   , theta_in ,  DEGREE_EPS);
        CHECK_CLOSE(90.0f   , theta_out,  DEGREE_EPS);
        std::cout << "phis are " << phi_in << "\t" << phi_out << std::endl;
        CHECK_CLOSE(180.0f  , MathUtils::radsToDegrees(wi.angle(wo)),  DEGREE_EPS);
        CHECK_CLOSE(180.0f  , phi_diff ,  DEGREE_EPS);
    }

    TEST(TestGenTangentFrameForIsotropicBRDF){

        //Check results for basic i,j,k unit vectoes
        Vector n(1.0f, 0.0f, 0.0f);
        Vector t1, t2;
        genTangentFrameForIsotropicBRDF(n, t1, t2);
        CHECK(isOrthoNormal(n, t1, t2) );
        n = Vector(0.0f, 1.0f, 0.0f);
        genTangentFrameForIsotropicBRDF(n, t1, t2);
        CHECK(isOrthoNormal(n, t1, t2) );
        n = Vector(0.0f, 0.0f, 1.0f);
        genTangentFrameForIsotropicBRDF(n, t1, t2);
        CHECK(isOrthoNormal(n, t1, t2) );

        //Now check a few more challenging situations
        //For any random unit vectors, we know that the CS generated should always be orthonormal
        //TODO

        //Try a few CS generated by hand to make sure things are correct
        Vector a(1.0f, 1.0f, 0.0f);
        a = a.getNormalized();
        Vector b,c;
        genTangentFrameForIsotropicBRDF(a,b,c);
        CHECK(isOrthoNormal(a, c, b) );
        CHECK( vecsClose(c, Vector(0.0f,0.0f,1.0f)) || vecsClose(c, Vector(0.0f, 0.0f, -1.0f)) ||
               vecsClose(b, Vector(0.0f,0.0f,1.0f)) || vecsClose(b, Vector(0.0f, 0.0f, -1.0f))
        );


    }

    TEST(TestProjectVectorOntoPlane){

        Vector i(1.0f, 0.0f, 0.0f);
        Vector j(0.0f, 1.0f, 0.0f);
        Vector k(0.0f, 0.0f, 1.0f);

        //Project onto the XY plane
        Vector temp(10.0f, 120.0f, 0.0f);
        CHECK( vecsCloseNoNorm(temp, projVectorOntoPlane(temp, k)  ) );
        temp = Vector(11.0f, 00.0f, 0.0f);
        CHECK( vecsCloseNoNorm(temp, projVectorOntoPlane(temp, k)  ) );
        temp = Vector(0.0f, 820.0f, 0.0f);
        CHECK( vecsCloseNoNorm(temp, projVectorOntoPlane(temp, k)  ) );
        temp = Vector(-10.0f, -120.0f, 0.0f);
        CHECK( vecsCloseNoNorm(temp, projVectorOntoPlane(temp, k)  ) );

        //Project onto the XZ plane
        temp = Vector(-10.0f, 0.0f, -21.0f);
        CHECK( vecsCloseNoNorm(temp, projVectorOntoPlane(temp, j)  ) );

        //Project onto the YZ plane
        temp = Vector(0.0f, 66.0f, -21.0f);
        CHECK( vecsCloseNoNorm(temp, projVectorOntoPlane(temp, i)  ) );

        //Project onto a plane not that is not ont of {XY, XZ, or YZ}
        //Here we are simply testing that the project vector should be 90 degreed with the normal
        Vector n(-1.0f,-0.8f,0.0f);
        n = n.getNormalized();

        temp = Vector(0.0f, 66.0f, -21.0f);
        Vector o = projVectorOntoPlane(temp, n);
        float angleDeg = o.angle(n) * (180.0f / M_PI);
        CHECK(abs(angleDeg - 90.0f) <= 1e-5);
    }

    /// Make sure conversion from spherical coords to BRDF vector works
    TEST(TestSphericalCoordsToVector){
        //Check that the angles map to axes correctly
        CHECK(vecsClose( //X Axis
            sphericalCoordToVector(PI_FLOAT/2.0f, 0.0f), //(theta, phi) aka (elevation, azimuth)
            Vector(1.0f, 0.0f, 0.0f)
        ));
        CHECK(vecsClose( //Y Axis
            sphericalCoordToVector(PI_FLOAT/2.0f, PI_FLOAT/2.0f),
            Vector(0.0f, 1.0f, 0.0f)
        ));
        float a = PI_FLOAT; //theta can be arbitrary for the z axis mapping
        CHECK(vecsClose( //Z Axis
            sphericalCoordToVector(0.0f, a),
            Vector(0.0f, 0.0f, 1.0f)
        ));
        a = 0.0f;
        CHECK(vecsClose( //Z Axis
            sphericalCoordToVector(0.0f, a),
            Vector(0.0f, 0.0f, 1.0f)
        ));
        a = .2f;
        CHECK(vecsClose( //Z Axis
            sphericalCoordToVector(0.0f, a),
            Vector(0.0f, 0.0f, 1.0f)
        ));

        //Test an off axis mapping with multiples of 2PI passed in a few times
        for(int i = 0; i < 20; i++){
            const float piMult = 2.0f * PI_FLOAT * float(i);
            CHECK(vecsClose( //Z Axis
                sphericalCoordToVector(
                    (PI_FLOAT/2.0f) + piMult, //xy plane
                    PI_FLOAT/4.0f),           //halfway from x to y
                Vector(sinf(PI_FLOAT/4.0f), sinf(PI_FLOAT/4.0f), 0.0f)
            ));
        }
    }

    TEST(BRDFWorldAndLocalTransforms){

        /*
        while(true){
            std::cout << "Local shading test." << std::endl;
            Vector n,t,b,w;
            std::cout << "Enter n, t, b(world space)" << std::endl;
            std::cin >> n;
            std::cin >> t;
            std::cin >> b;
            std::cout << std::endl;
            std::cout << "\tn = " << n << std::endl;
            std::cout << "\tt = " << t << std::endl;
            std::cout << "\tb = " << b << std::endl;
            std::cout << "Enter wo" << std::endl;
            std::cin >> w;
            std::cout << std::endl;
            std::cout << "\tw = " << w << std::endl;
            //--
            Vector wLocal  = BRDFGeom::vectorToBRDFSpace (w     ,n,t,b);
            Vector wWorld2 = BRDFGeom::vectorToWorldSpace(wLocal,n,t,b);
            //--
            std::cout << "w = " << w << std::endl;
            std::cout << "brdf_space(" << w << ") = " << wLocal << std::endl;
            std::cout << "world(brdf_space(" << w << ")) = " << wWorld2 << std::endl;
            std::cout << std::endl;
        }
        */
    }

    TEST(VectorToSphericalCoords){

        float theta, phi;
        theta = phi = 0.0f;

        Vector x(1.0f, 0.0f, 0.0f);
        Vector y(0.0f, 1.0f, 0.0f);
        Vector z(0.0f, 0.0f, 1.0f);

        vectorToSphericalCoord(x, theta, phi);
        CHECK( anglesClose(theta, phi, PI_FLOAT / 2.0f, 0.0f) );

        vectorToSphericalCoord(y, theta, phi);
        CHECK( anglesClose(theta, phi, PI_FLOAT / 2.0f, PI_FLOAT / 2.0f) );

        vectorToSphericalCoord(z, theta, phi);
        CHECK( fabs(theta) < 0.001f );
    }

    TEST(TestGenIsotropicBRDFAngularCoords){

        /*
        while(true){
            Vector n,t,b,wi,wo;
            std::cout << "Enter n, t, b" << std::endl;
            std::cin >> n;
            std::cin >> t;
            std::cin >> b;
            std::cout << std::endl;
            std::cout << "\tn = " << n << std::endl;
            std::cout << "\tt = " << t << std::endl;
            std::cout << "\tb = " << b << std::endl;
            std::cout << "Enter wo, wi" << std::endl;
            std::cin >> wo;
            std::cin >> wi;
            std::cout << std::endl;
            std::cout << "\two = " << wo << std::endl;
            std::cout << "\twi = " << wi << std::endl;
            float theta_in, theta_out, phi_diff;
            theta_in = theta_out = phi_diff = -720.0f;
            BRDFGeom::genIsotropicBRDFAngularCoords(n,
                wo,wi,
                theta_in, theta_out, phi_diff);

            std::cout << std::endl << "(theta_i, theta_o, phi_d) = (" <<
                theta_in << ", " << theta_out << ", " << phi_diff << ")" << std::endl;
            std::cout << std::endl;

        }
        */

    }

}

#endif //TEST_BRDF_GEOM_H
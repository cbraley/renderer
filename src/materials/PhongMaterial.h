#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

#include "Material.h"
#include "Spectrum.h"

class PhongMaterial : public Material{
private:
    Spectrum kd, ks;        //Diffuse and specular color
    float ka;               //Ambient
    int alpha;              //Shininess exponent

public:


    /**
     *  Construct a perfect white lambertian reflector akin to Spectralon.
     */
    PhongMaterial() : 
        kd(1.0f, 400.0f, 720.0f), ks(1.0f, 400.0f, 720.0f), ka(0.0f), alpha(0.1f)
    {}

    PhongMaterial(const Spectrum& diffuseColor, const Spectrum& specularColor, const int shininess, const float ambient = 0.0f) : 
        kd(diffuseColor), ks(specularColor), ka(ambient), alpha(shininess)
    {}


    PhongMaterial(const PhongMaterial& other) :
        kd(other.kd), ks(other.ks), ka(other.ka), alpha(other.alpha)
    {}

    void setSpectralRefl(const Spectrum& s){
        kd = s;
    }

    void setParams(const Spectrum& kdNew, const Spectrum& ksNew, int shininessNew){
        kd = kdNew;
        ks = ksNew;
        alpha = shininessNew;
    }

    void setAmbient(float ambient){ ka = ambient; }

    virtual Spectrum getApproxSpectralReflectance()const{
        return kd;
    }


    virtual Spectrum evaluate(const Vector& L, const Vector& N, const Vector& H,
        const float visAmt, const Spectrum& lightColor)const
    {
        Assert(visAmt >= 0.0f && visAmt <= 1.0f);
        //TODO: Assert normalized vecs
        const Spectrum reflSpec =
            (lightColor * visAmt) *
            (
                kd * std::max<float>(L.dot(N), 0.0f) +
                ks * pow((N.dot(H)), alpha)
            );
        return (lightColor * (ka)) + reflSpec;
    }

};

#endif //PHONG_MATERIAL_H

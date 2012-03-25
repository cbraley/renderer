#ifndef COLOR_CONSTANTS_H
#define COlOR_CONSTANTS_H

namespace ColorConstants{

    /**
     *  All spectral integration and sampling routines operate
     *  in the wavelength range [MIN_NM_VIS, MAX_NM_VIS] nm.
     *
     *  Visible light really occurs over the band [380, 730]nm,
     *  but light near either edge of that band is much less visible.
     *  For a plot of this, see the "luminous effeciency functions"
     *  Spectrum(CIE_LUMINOUS_EFFICIENCY_PHOTOPIC_1951) and
     *  Spectrum(CIE_LUMINOUS_EFFICIENCY_SCOPTIC_1951).
     *  Note that photopic is "normal" vision, whereas scoptic is
     *  vision in dark conditions.
     *
     *  Another reason that we don't integrate near the edges of
     *  the visible spectrum is because measured data is hard to
     *  find in these ranges due to problems with instrument
     *  sensitivity.
     */
    const float MIN_NM_VIS = 400.0f;
    const float MAX_NM_VIS = 720.0f;

}

#endif //COLOR_CONSTANTS_H

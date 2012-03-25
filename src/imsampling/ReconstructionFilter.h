#ifndef RECONSTRUCTION_FILTER_H
#define RECONSTRUCTION_FILTER_H

/**
 *  Pure virtual interface that subclasses can implement.
 *  Specifies the interface for filters that allow reconstruction of images
 *  from scattered point sampled radiance values in the image plane.
 *
 *  @author Colin Braley
 */
class ReconstructionFilter{
public:

    /**
     *  Evaluate the filter at a given location (xLoc, yLoc), where xLoc and yLoc
     *  is the sampled position RELATIVE TO THE CENTER OF THE RECONSTRUCTION FILTER.
     *
     *  @param xLoc is the sample's x location, relative to the center point.
     *  @param yLoc is the sample's y location, relative to the center point.
     */
    virtual float evalFilter(float xLoc, float yLoc)const = 0;

    /**
     *  Get the width of the filter in X.  We assume that when some value x
     *  such that x > filter->extentX() is passed into evalFilter(x, ...)
     *  that evalFilter returns a number "close enough" to 0 that the filter's
     *  contribution should be negligable at this distance.
     *
     *  Note that many practical filters have an "infinite extent" and that the
     *  typical practice is to say that once the distance is far enough away that
     *  the filter only returns 1% of its maximum value that this is the filter's
     *  extent.
     *
     *  @return the extent of the filter in the x direction.
     */
    virtual float extentX()const = 0;

    /**
     *  Get the width of the filter in X.  We assume that when some value x
     *  such that x > filter->extentX() is passed into evalFilter(x, ...)
     *  that evalFilter returns a number "close enough" to 0 that the filter's
     *  contribution should be negligable at this distance.
     *
     *  Note that many practical filters have an "infinite extent" and that the
     *  typical practice is to say that once the distance is far enough away that
     *  the filter only returns 1% of its maximum value that this is the filter's
     *  extent.
     *
     *  @return the extent of the filter in the x direction.
     */
    virtual float extentY()const = 0;

};

#endif //RECONSTRUCTION_FILTER_H

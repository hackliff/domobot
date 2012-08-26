/*******************************************************************************
*                                                                              *
*   PrimeSense NITE 1.3 - Point Viewer Sample                                  *
*   Copyright (C) 2010 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

#ifndef XNV_POINT_DRAWER_H_
#define XNV_POINT_DRAWER_H_
 
#include <cmath>
#include <map>
#include <list>
#include <vector>
#include <XnCppWrapper.h>
#include <XnVPointControl.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;
using namespace xn;

typedef enum
{
	IN_SESSION,
	NOT_IN_SESSION,
	QUICK_REFOCUS
} SessionState;

typedef struct 
{
    int hand;
    int id;
    bool contact;
    XnPoint3D coordinates;
    float discriminantAngle;
    float length;
    float lastGap;
    float nextGap;
    float angle;
    float lastAngle;
    float nextAngle;
} Blob;

std::string PrintSessionState(SessionState eState);
/**
 * This is a point control, which stores the history of every point
 * It can draw all the points as well as the depth map.
 */
class XnVHandTracker : public XnVPointControl
{
    friend class XnCommunication;

public:
	XnVHandTracker(xn::DepthGenerator depthGenerator);
	virtual ~XnVHandTracker();

	/**
	 * Handle a new message.
	 * Calls other callbacks for each point, then draw the depth map (if needed) and the points
	 */
	void Update(XnVMessage* pMessage);

	/**
	 * Handle creation of a new point
	 */
	void OnPointCreate(const XnVHandPointContext* cxt);
	/**
	 * Handle new position of an existing point
	 */
	void OnPointUpdate(const XnVHandPointContext* cxt);
	/**
	 * Handle destruction of an existing point
	 */
	void OnPointDestroy(XnUInt32 nID);

	/**
	 * Change mode - should draw the depth map?
	 */
	void SetTouchingFOVEdge(XnUInt32 nID);

    /*
     *Retrieve Hands 3D position
     */
    void getPosition(float *v, XnUInt32 Id);

    /*
     *Compute  a vector describing hand contour
     */
    bool getContour(const Mat mat, const float *v, vector<Point> &handContour, bool debug, const double epsilon, const int maxHandRadius, int distance, float maxDepth);

    /*
     *Process a convex algo to improve segmentation
     */
    double computeConvex(const vector<Point> &contour, Mat *debugFrame);

    /*
     *Store each finger coordinates
     */
    void detectFingerTips(const vector<Point> &handContour, vector<Point> &fingerTips, Mat *debugFrame, float angleMax, float cutoffCoeff);

    /*
     * Classification des doigts détectés
     */
    int fingerTipsIdentification(vector<Point> &fingerTips, Mat *debugFrame);

    /*
     * Estimation de l'identité du doigt à partir de la structure remplie
     */
    int getFingerId(Blob blob, int lastId, int probableId);

protected:
	XnBool IsTouching(XnUInt32 nID) const;
	std::list<XnUInt32> m_TouchingFOVEdge;
	xn::DepthGenerator m_DepthGenerator;
    std::map<XnUInt32, XnPoint3D> m_handsPosition;
    Scalar m_color;

    XnPoint3D m_pointTracked;
    std::vector<Point> m_poignet;
    std::vector<Blob> Fingers;
};

#endif

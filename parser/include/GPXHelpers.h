#ifndef GPX_HELPERS_H
#define GPX_HELPERS_H

TrackSegment* trackSegmentConstructor();

void updateTrackName(Track* trk, char name []);

Track* trackConstructor();

void updateRouteName(Route* rte, char name []);

Route* routeConstructor();

GPXData* gpxDataConstructor(char name [], char content []);

void updateWaypointName(Waypoint* wpt, char name []);

Waypoint* waypointConstructor(double latitude, double longitude);

GPXdoc* GPXdocConstructor(char namespace[256], double version, char* creator);

GPXdoc* build_gpx_doc(xmlNode * a_node, GPXdoc* gpx);

//A2
int validateTree(xmlDoc* doc, char* gpxSchemaFile);

void addGPXDataToTree(List* otherData, xmlNode* node);

void addWptGPXDataToTree(List* otherData, xmlNode* node, char* name);

void addWaypointsToTree(List* waypoints, xmlNode* node);

xmlDoc* gpxToTree(GPXdoc * doc);

int checkGPXDataConstraints(List* otherData);

int checkWaypointConstraints(List* waypoints);

int checkGPXConstraints(GPXdoc* doc);

void fakeDelete(void* data);


#endif

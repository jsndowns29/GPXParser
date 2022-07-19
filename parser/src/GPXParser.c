#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <libxml/xmlschemastypes.h>
#include "GPXParser.h"
#include "LinkedListAPI.h"
#include "GPXHelpers.h"

#ifndef LIBXML_SCHEMAS_ENABLED
  #define LIBXML_SCHEMAS_ENABLED
#endif

#ifndef R
  #define R 6371
#endif

#ifndef TO_RAD
  #define TO_RAD (M_PI / 180)
#endif

//GPX data helpers
void deleteGpxData(void* data)
{
  GPXData* tmpGPXData = NULL;

  if(data == NULL){
    return;
  }

  tmpGPXData = (GPXData*)data;

  free(tmpGPXData);
}

char* gpxDataToString(void* data)
{
  GPXData* tmpGPXData = (GPXData*)data;
  if(data == NULL){
    return NULL;
  }

  int len;
  char* tmpStr;

  len = strlen(tmpGPXData->name) + strlen(tmpGPXData->value) + 256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "\n   Element: GPX data.  name: %s  value: %s", tmpGPXData->name, tmpGPXData->value);

  return tmpStr;
}

int compareGpxData(const void* first, const void* second)
{
  return 0;
}

//waypoint helpers
void deleteWaypoint(void* data)
{
    Waypoint* tmpWpt = NULL;

    if(data == NULL){
      return;
    }

    tmpWpt = (Waypoint*)data;
    if(tmpWpt->name != NULL){
      free(tmpWpt->name);
    }
    if(tmpWpt->otherData != NULL){
      freeList(tmpWpt->otherData);
    }

    free(tmpWpt);
}

char* waypointToString(void* data)
{
  Waypoint* wpt = (Waypoint*)data;
  if(wpt == NULL){
    return NULL;
  }

  int len;
  char* tmpStr;

  len = strlen(wpt->name) +256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "\nElement: wpt.  name: %s  latitude: %f  longitude: %f", wpt->name, wpt->latitude, wpt->longitude);

  char* tmpGPXData = toString(wpt->otherData);
  if(tmpGPXData != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpGPXData) + 2));
    strcat(tmpStr, tmpGPXData);
    free(tmpGPXData);
  }

  return tmpStr;
}

int compareWaypoints(const void* first, const void* second)
{
  return 0;
}

//route helpers
void deleteRoute(void* data)
{
  Route* tmpRte = NULL;

  if(data == NULL){
    return;
  }

  tmpRte = (Route*)data;
  if(tmpRte->name != NULL){
    free(tmpRte->name);
  }
  if(tmpRte->waypoints != NULL){
    freeList(tmpRte->waypoints);
  }
  if(tmpRte->otherData != NULL){
    freeList(tmpRte->otherData);
  }

  free(tmpRte);
}

char* routeToString(void* data)
{
  Route* rte = (Route*)data;
  if(rte == NULL){
    return NULL;
  }

  int len;
  char* tmpStr;

  len = strlen(rte->name) +256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "\n\nElement: rte.  name: %s", rte->name);

  char* tmpGPXData = toString(rte->otherData);
  if(tmpGPXData != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpGPXData) + 2));
    strcat(tmpStr, tmpGPXData);
    free(tmpGPXData);
  }

  char* tmpWaypoints = toString(rte->waypoints);
  if(tmpWaypoints != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpWaypoints) + 2));
    strcat(tmpStr, tmpWaypoints);
    free(tmpWaypoints);
  }

    return tmpStr;
}

int compareRoutes(const void* first, const void* second)
{
  return 0;
}

//track helpers
void deleteTrack(void* data)
{
  Track* tmpTrack = NULL;

  if(data == NULL){
    return;
  }

  tmpTrack = (Track*)data;
  if(tmpTrack->name != NULL){
    free(tmpTrack->name);
  }
  if(tmpTrack->segments != NULL){
    freeList(tmpTrack->segments);
  }
  if(tmpTrack->otherData != NULL){
    freeList(tmpTrack->otherData);
  }

  free(tmpTrack);
}

char* trackToString(void* data)
{
  Track* trk = (Track*)data;
  if(trk == NULL){
    return NULL;
  }

  int len;
  char* tmpStr;

  len = strlen(trk->name) +256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "\n\nElement: trk.  name: %s", trk->name);

  char* tmpGPXData = toString(trk->otherData);
  if(tmpGPXData != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpGPXData) + 2));
    strcat(tmpStr, tmpGPXData);
    free(tmpGPXData);
  }

  char* tmpSegments = toString(trk->segments);
  if(tmpSegments != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpSegments) + 2));
    strcat(tmpStr, tmpSegments);
    free(tmpSegments);
  }

    return tmpStr;
}

int compareTracks(const void* first, const void* second)
{
  return 0;
}

//track Segment helpers
void deleteTrackSegment(void* data)
{
  TrackSegment* tmpTrkSeg = NULL;

  if(data == NULL){
    return;
  }

  tmpTrkSeg = (TrackSegment*)data;
  if(tmpTrkSeg->waypoints){
    freeList(tmpTrkSeg->waypoints);
  }

  free(tmpTrkSeg);
}

char* trackSegmentToString(void* data)
{
  TrackSegment* trkseg = (TrackSegment*)data;
  if(trkseg == NULL){
    return NULL;
  }

  int len;
  char* tmpStr;

  len = 256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "   \n\nElement: trkseg.");

  char* tmpWaypoints = toString(trkseg->waypoints);
  if(tmpWaypoints != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpWaypoints) + 1));
    strcat(tmpStr, tmpWaypoints);
    free(tmpWaypoints);
  }

    return tmpStr;
}

int compareTrackSegments(const void* first, const void* second)
{
  return 0;
}

int getNumWaypoints(const GPXdoc* doc)
{
  if(doc == NULL){
    return 0;
  }
  if(!doc){
    return 0;
  }

  int totalWpts = 0;

  //get waypoints in waypoints listHead
  if(doc->waypoints != NULL){
    totalWpts += getLength(doc->waypoints);
  }

  //get waypoints in routes
  //iter through all routes
  /*if(doc->routes != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->routes);
    while((elem = nextElement(&iter)) != NULL){
      Route* tmpRoute = (Route*)elem;
      if(tmpRoute != NULL && tmpRoute->waypoints != NULL){
        totalWpts += getLength(tmpRoute->waypoints);
      }
    }
  }*/

  //get waypoints in trackSegmentToString
  //first iter through tracks
  /*if(doc->tracks != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->tracks);
    while((elem = nextElement(&iter)) != NULL){
      Track* tmpTrack = (Track*)elem;
      if(tmpTrack != NULL && tmpTrack->segments != NULL){
        //now iter through track segments
          void* elem2;
          ListIterator iter2 = createIterator(tmpTrack->segments);
          while((elem2 = nextElement(&iter2)) != NULL){
            TrackSegment* tmpSeg = (TrackSegment*)elem2;
            if(tmpSeg != NULL && tmpSeg->waypoints != NULL){
              totalWpts += getLength(tmpSeg->waypoints);
            }
          }
      }
    }
  }*/

  return totalWpts;
}

int getNumRoutes(const GPXdoc* doc)
{
  if(doc == NULL){
    return 0;
  }
  if(!doc){
    return 0;
  }

  int totalRoutes = 0;

  if(doc->routes != NULL){
    totalRoutes += getLength(doc->routes);
  }

  return totalRoutes;
}

int getNumTracks(const GPXdoc* doc)
{
  if(doc == NULL){
    return 0;
  }
  if(!doc){
    return 0;
  }

  int totalTracks = 0;

  if(doc->tracks != NULL){
    totalTracks += getLength(doc->tracks);
  }

  return totalTracks;
}

int getNumSegments(const GPXdoc* doc)
{
  if(doc == NULL){
    return 0;
  }
  if(!doc){
    return 0;
  }

  int totalSegs = 0;

  if(doc->tracks != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->tracks);
    while((elem = nextElement(&iter)) != NULL){
      Track* tmpTrack = (Track*)elem;
      if(tmpTrack != NULL && tmpTrack->segments != NULL){
        totalSegs += getLength(tmpTrack->segments);
      }
    }
  }

  return totalSegs;
}

int getNumGPXData(const GPXdoc* doc)
{
  if(doc == NULL){
    return 0;
  }
  if(!doc){
    return 0;
  }

  int totalGPXData = 0;

  //get gpx data from waypoints
  if(doc->waypoints != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->waypoints);
    while((elem = nextElement(&iter)) != NULL){
      Waypoint* tmpWpt = (Waypoint*)elem;
      //count valid names
      if((strcmp(tmpWpt->name, "") != 0) && tmpWpt->name != NULL){
        totalGPXData++;
      }
      //count GPXData
      if(tmpWpt != NULL && tmpWpt->otherData != NULL){
        totalGPXData += getLength(tmpWpt->otherData);
      }
    }
  }

  //get GPXData from routes and routepoints
  //start with routes
  if(doc->routes != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->routes);
    while((elem = nextElement(&iter)) != NULL){
      Route* tmpRoute = (Route*)elem;
      if(tmpRoute != NULL){
        //count valid names
        if((strcmp(tmpRoute->name, "") != 0) && tmpRoute->name != NULL){
          totalGPXData++;
        }
        //count GPXData in routes
        if(tmpRoute->otherData != NULL){
          totalGPXData += getLength(tmpRoute->otherData);
        }
        //now iter through routepoints
        if(tmpRoute->waypoints != NULL){
          void* elem2;
          ListIterator iter2 = createIterator(tmpRoute->waypoints);
          while((elem2 = nextElement(&iter2)) != NULL){
            Waypoint* tmpWpt = (Waypoint*)elem2;
            //count valid names
            if((strcmp(tmpWpt->name, "") != 0) && tmpWpt->name != NULL){
              totalGPXData++;
            }
            //countGPXData
            if(tmpWpt != NULL && tmpWpt->otherData != NULL){
              totalGPXData += getLength(tmpWpt->otherData);
            }
          }
        }
      }
    }
  }

  //get GPXData from tracks and trackpoints
  //start with tracks
  if(doc->tracks != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->tracks);
    while((elem = nextElement(&iter)) != NULL){
      Track* tmpTrack = (Track*)elem;
      if(tmpTrack != NULL){
        //count valid names
        if((strcmp(tmpTrack->name, "") != 0) && tmpTrack->name != NULL){
          totalGPXData++;
        }
        //count GPXData in tracks
        if(tmpTrack->otherData != NULL){
          totalGPXData += getLength(tmpTrack->otherData);
        }
        //now iter through track Segments
        if(tmpTrack->segments != NULL){
          void* elem2;
          ListIterator iter2 = createIterator(tmpTrack->segments);
          while((elem2 = nextElement(&iter2)) != NULL){
            TrackSegment* tmpSeg = (TrackSegment*)elem2;
            if(tmpSeg != NULL && tmpSeg->waypoints != NULL){
              //now iter thought trackpoints
              void* elem3;
              ListIterator iter3 = createIterator(tmpSeg->waypoints);
              while((elem3 = nextElement(&iter3)) != NULL){
                Waypoint* tmpWpt = (Waypoint*)elem3;
                //count valid names
                if((strcmp(tmpWpt->name, "") != 0) && tmpWpt->name != NULL){
                  totalGPXData++;
                }
                //count GPXData in trkpts
                if(tmpWpt != NULL && tmpWpt->otherData != NULL){
                  totalGPXData += getLength(tmpWpt->otherData);
                }
              }
            }

          }
        }
      }
    }
  }

  return totalGPXData;
}

Waypoint* getWaypoint(const GPXdoc* doc, char* name)
{
  if(doc == NULL || name == NULL){
    return NULL;
  }
  if(!doc){
    return NULL;
  }

  //search for waypoint
  if(doc->waypoints != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->waypoints);
    while((elem = nextElement(&iter)) != NULL){
      Waypoint* tmpWpt = (Waypoint*)elem;
      //return if wpt name if found
      if((strcmp(tmpWpt->name, name) == 0) && tmpWpt->name != NULL){
        return tmpWpt;
      }
    }
  }

  return NULL;
}

Track* getTrack(const GPXdoc* doc, char* name)
{
  if(doc == NULL || name == NULL){
    return NULL;
  }
  if(!doc){
    return NULL;
  }

  //search for track
  if(doc->tracks != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->tracks);
    while((elem = nextElement(&iter)) != NULL){
      Track* tmpTrk = (Track*)elem;
      //return track name if found
      if((strcmp(tmpTrk->name, name) == 0) && tmpTrk->name != NULL){
        return tmpTrk;
      }
    }
  }

  return NULL;
}

Route* getRoute(const GPXdoc* doc, char* name)
{
  if(doc == NULL || name == NULL){
    return NULL;
  }
  if(!doc){
    return NULL;
  }

  //search for track
  if(doc->routes != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->routes);
    while((elem = nextElement(&iter)) != NULL){
      Route* tmpRte = (Route*)elem;
      //return track name if found
      if((strcmp(tmpRte->name, name) == 0) && tmpRte->name != NULL){
        return tmpRte;
      }
    }
  }

  return NULL;
}

char* GPXdocToString(GPXdoc* doc)
{
  if(doc == NULL){
    return NULL;
  }
  if(!doc){
    return NULL;
  }
  //GPX doc
  int len;
  //char* tmpStr = "";

  len = strlen(doc->namespace)+strlen(doc->creator) + 400;
  char* tmpStr;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "Element name gpx.\n   Version: %f\n    Creator: %s\n    Namespace: %s", doc->version, doc->creator, doc->namespace);

  //Waypoints
  char* tmpWpt = toString(doc->waypoints);
  if(tmpWpt != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpWpt)) + 2);
    strcat(tmpStr, tmpWpt);
    free(tmpWpt);
  }

  //Routes
  char* tmpRoutes = toString(doc->routes);
  if(tmpRoutes != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpRoutes)) + 2);
    strcat(tmpStr, tmpRoutes);
    free(tmpRoutes);
  }

  //Tracks
  char* tmpTracks = toString(doc->tracks);
  /*printf("track size %ld\n", strlen(tmpTracks));
  printf("string size %ld\n", strlen(tmpStr));
  int tot = (int)sizeof(char) * (strlen(tmpStr) + strlen(tmpTracks));
  printf("realloc size %d\n", tot);*/
  if(tmpTracks != NULL){
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(tmpTracks)) + 2);
    strcat(tmpStr, tmpTracks);
    free(tmpTracks);
  }

  return tmpStr;
}

void deleteGPXdoc(GPXdoc* doc)
{
  if(doc == NULL){
    return;
  }
  if(!doc){
    return;
  }

  if(doc->tracks != NULL){
    freeList(doc->tracks);
  }
  if(doc->routes != NULL){
    freeList(doc->routes);
  }
  if(doc->waypoints != NULL){
    freeList(doc->waypoints);
  }
  if(doc->creator != NULL){
    free(doc->creator);
  }

  free(doc);
}

GPXdoc* createGPXdoc(char* fileName)
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    GPXdoc* gpx = NULL;

    if(fileName == NULL){
      return NULL;
    }else if(strcmp(fileName, "") == 0){
      return NULL;
    }

    //init xml lib
    LIBXML_TEST_VERSION

    //parse file and get document
    doc = xmlReadFile(fileName, NULL, 0);

    if(doc == NULL) {
        //printf("error: could not parse file %s\n", fileName);
        //free the document
        xmlFreeDoc(doc);

        //free global variables that may have been allocated by the parser
        xmlCleanupParser();

        return NULL;
    }

    //Get root element node
    root_element = xmlDocGetRootElement(doc);

    gpx = build_gpx_doc(root_element, gpx);

    //free the document
    xmlFreeDoc(doc);

    //free global variables that may have been allocated by the parser
    xmlCleanupParser();

    //printf("kjskhfsa\n");
    return gpx;
}

GPXdoc* createValidGPXdoc(char* fileName, char* gpxSchemaFile){
  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;
  GPXdoc* gpx = NULL;

  if(fileName == NULL){
    return NULL;
  }else if(strcmp(fileName, "") == 0){
    return NULL;
  }

  if(gpxSchemaFile == NULL){
    return NULL;
  }else if(strcmp(gpxSchemaFile, "") == 0){
    return NULL;
  }

  //init xml lib
  LIBXML_TEST_VERSION

  //parse file and get document
  doc = xmlReadFile(fileName, NULL, 0);

  if(doc == NULL) {
      //printf("error: could not parse file %s\n", fileName);
      //free the document
      xmlFreeDoc(doc);

      //free global variables that may have been allocated by the parser
      xmlCleanupParser();

      return NULL;
  }

  int validator = 0;

  //validate gpx against schemaLocation
  validator = validateTree(doc, gpxSchemaFile);

  if(validator == 0){
    //printf("error: could not parse file %s\n", fileName);
    //free the document
    xmlFreeDoc(doc);

    //free global variables that may have been allocated by the parser
    xmlCleanupParser();

    return NULL;
  }

  //Get root element node
  root_element = xmlDocGetRootElement(doc);

  gpx = build_gpx_doc(root_element, gpx);

  //free the document
  xmlFreeDoc(doc);

  //free global variables that may have been allocated by the parser
  xmlCleanupParser();

  //printf("kjskhfsa\n");
  return gpx;
}

bool writeGPXdoc(GPXdoc* doc, char* fileName){

  if(fileName == NULL){
    return FALSE;
  }else if(strcmp(fileName, "") == 0){
    return FALSE;
  }

  if(doc == NULL){
    return FALSE;
  }

  xmlDoc* newXML = gpxToTree(doc);

  if(newXML == NULL){
    return FALSE;
  }

  //write to file
  int writeSucess;
  writeSucess = xmlSaveFormatFileEnc(fileName, newXML, "UTF-8", 1);

  if (writeSucess == -1){
    return FALSE;
  }

  xmlFreeDoc(newXML);

  xmlCleanupParser();


  return TRUE;
}

bool validateGPXDoc(GPXdoc* gpxDoc, char* gpxSchemaFile){

  if(gpxDoc == NULL){
    return FALSE;
  }

  if(gpxSchemaFile == NULL){
    return NULL;
  }else if(strcmp(gpxSchemaFile, "") == 0){
    return NULL;
  }

  int gpxConstraints = checkGPXConstraints(gpxDoc);

  if(gpxConstraints == 0){
    return FALSE;
  }

  xmlDoc* newXML = gpxToTree(gpxDoc);

  if(newXML == NULL){
    return FALSE;
  }

  int validator;
  //validate gpx against schemaLocation
  validator = validateTree(newXML, gpxSchemaFile);

  if(validator == 0){
    //printf("error: could not parse file %s\n", fileName);
    //free the document
    xmlFreeDoc(newXML);

    //free global variables that may have been allocated by the parser
    xmlCleanupParser();

    return FALSE;
  }


  xmlFreeDoc(newXML);

  xmlCleanupParser();

  return TRUE;
}

float round10(float len){

  int roundNum = (int)len;
  int lastDigit = len;
  lastDigit = lastDigit%10;

  if(lastDigit < 5){
    roundNum = roundNum - lastDigit;
  }
  else{
    roundNum = roundNum + (10 - lastDigit);
  }

  float returnVal = (float)roundNum;

  return returnVal;
}

//haversine formula Source: https://rosettacode.org/wiki/Haversine_formula#C
double haversine(double th1, double ph1, double th2, double ph2)
{
	double dx, dy, dz;
	ph1 -= ph2;
	ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;

	dz = sin(th1) - sin(th2);
	dx = cos(ph1) * cos(th1) - cos(th2);
	dy = sin(ph1) * cos(th1);
	return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R;
}

float getRouteLen(const Route* rt)
{

  if(rt == NULL){
    return 0;
  }

  double lat1 = 0;
  double long1 = 0;
  double lat2 = 0;
  double long2 = 0;

  float dist = 0;

  int i = 0;

  if(rt->waypoints != NULL){
    void* elem;
    ListIterator iter = createIterator(rt->waypoints);
    while((elem = nextElement(&iter)) != NULL){
      Waypoint* tmpWpt = (Waypoint*)elem;

      if(i == 0){
        lat1 = tmpWpt->latitude;
        long1 = tmpWpt->longitude;
      }
      else{
        lat2 = tmpWpt->latitude;
        long2 = tmpWpt->longitude;

        dist += (float)haversine(lat1, long1, lat2, long2);

        lat1 = lat2;
        long1 = long2;
      }

      i++;
    }
  }else{
    return 0;
  }

  //computes distance in km, convert to meters
   return dist * 1000;
}

float getTrackLen(const Track* tr)
{

  if(tr == NULL){
    return 0;
  }

  double lat1 = 0;
  double long1 = 0;
  double lat2 = 0;
  double long2 = 0;

  double lastLat = 0;
  double lastLong = 0;

  float dist = 0;
  //count segment iterations
  int i = 0;
  //count waypoint iterations
  int j = 0;

  if(tr->segments != NULL){
    void* elem;
    ListIterator iter = createIterator(tr->segments);
    while((elem = nextElement(&iter)) != NULL){
      TrackSegment* tmpSeg = (TrackSegment*)elem;



      if(tmpSeg->waypoints != NULL){
        void* elem2;
        ListIterator iter2 = createIterator(tmpSeg->waypoints);
        while ((elem2 = nextElement(&iter2)) != NULL) {
          Waypoint* tmpWpt = (Waypoint*)elem2;

          if(j == 0){
            lat1 = tmpWpt->latitude;
            long1 = tmpWpt->longitude;
            //if not first segment compute dist between last segment and current
            if(i > 0){
              dist += (float)haversine(lastLat, lastLong, lat1, long1);
            }
          }
          else{
            lat2 = tmpWpt->latitude;
            long2 = tmpWpt->longitude;

            dist += (float)haversine(lat1, long1, lat2, long2);

            lat1 = lat2;
            long1 = long2;
          }

          j++;


        }
        j = 0;
        i++;
        lastLat = lat2;
        lastLong = long2;

      }
    }
  }else{
    return 0;
  }


  return dist * 1000;
}

int numRoutesWithLength(const GPXdoc* doc, float len, float delta)
{
  int count = 0;
  float checkLen = 0;

  if(doc == NULL){
    return 0;
  }

  if(delta < 0 || len < 0){
    return 0;
  }

  if(doc->routes != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->routes);
    while((elem = nextElement(&iter)) != NULL){
      Route* tmpRte = (Route*)elem;

      checkLen = getRouteLen(tmpRte);

      //check if len is between acceptable range
      if((checkLen <= (len + delta)) && (checkLen >= (len - delta))){
        count++;
      }
    }
  }else{
    return 0;
  }

  return count;
}

int numTracksWithLength(const GPXdoc* doc, float len, float delta)
{
  int count = 0;
  float checkLen = 0;

  if(doc == NULL){
    return 0;
  }

  if(delta < 0 || len < 0){
    return 0;
  }

  if(doc->tracks != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->tracks);
    while((elem = nextElement(&iter)) != NULL){
      Track* tmpTrk = (Track*)elem;

      checkLen = getTrackLen(tmpTrk);

      //check if len is between acceptable range
      if((checkLen <= (len + delta)) && (checkLen >= (len - delta))){
        count++;
      }
    }
  }else{
    return 0;
  }

  return count;
}

bool isLoopRoute(const Route* route, float delta)
{
  if(route == NULL){
    return FALSE;
  }

  if(delta < 0){
    return FALSE;
  }

  double firstLat = 0;
  double firstLong = 0;
  double lastLat = 0;
  double lastLong = 0;

  float dist = 0;

  int i = 0;

  if(route->waypoints != NULL){
    void* elem;
    ListIterator iter = createIterator(route->waypoints);
    while((elem = nextElement(&iter)) != NULL){
      Waypoint* tmpWpt = (Waypoint*)elem;

      if(i == 0){
        firstLat = tmpWpt->latitude;
        firstLong = tmpWpt->longitude;
      }
      else{
        lastLat = tmpWpt->latitude;
        lastLong = tmpWpt->longitude;
      }

      i++;
    }
  }else{
    return FALSE;
  }

  if(i < 4){
    return FALSE;
  }

  dist = (float)haversine(firstLat, firstLong, lastLat, lastLong);
  dist = dist * 1000;

  if(dist <= delta){
    return TRUE;
  }else{
    return FALSE;
  }
}


bool isLoopTrack(const Track* tr, float delta)
{
  if(tr == NULL){
    return FALSE;
  }

  if(delta < 0){
    return FALSE;
  }

  double lat1 = 0;
  double long1 = 0;
  double lat2 = 0;
  double long2 = 0;

  double lastLat = 0;
  double lastLong = 0;

  float dist = 0;
  //count segment iterations
  int i = 0;
  //count waypoint iterations
  int j = 0;

  int numWpts = 0;

  if(tr->segments != NULL){
    void* elem;
    ListIterator iter = createIterator(tr->segments);
    while((elem = nextElement(&iter)) != NULL){
      TrackSegment* tmpSeg = (TrackSegment*)elem;



      if(tmpSeg->waypoints != NULL){
        void* elem2;
        ListIterator iter2 = createIterator(tmpSeg->waypoints);
        while ((elem2 = nextElement(&iter2)) != NULL) {
          Waypoint* tmpWpt = (Waypoint*)elem2;

          if(j == 0 && i == 0){
            lat1 = tmpWpt->latitude;
            long1 = tmpWpt->longitude;
          }
          else{
            lat2 = tmpWpt->latitude;
            long2 = tmpWpt->longitude;
          }

          j++;
          numWpts++;

        }
        j = 0;
        i++;
        lastLat = lat2;
        lastLong = long2;
      }
    }
  }else{
    return FALSE;
  }

  if(numWpts < 4){
    return FALSE;
  }

  dist = (float)haversine(lat1, long1, lastLat, lastLong);
  dist = dist * 1000;

  if(dist <= delta){
    return TRUE;
  }else{
    return FALSE;
  }

}

List* getRoutesBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta)
{
  if(doc == NULL){
    return NULL;
  }
  if(delta < 0){
    return NULL;
  }

  double firstLat = 0;
  double firstLong = 0;
  double lastLat = 0;
  double lastLong = 0;

  float dist = 0;

  int numFound = 0;

  Route* route = NULL;

  int i = 0;

  List* list = initializeList(&routeToString, &fakeDelete, &compareRoutes);

  if(doc->routes != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->routes);
    while((elem = nextElement(&iter)) != NULL){
      route = (Route*)elem;

    /*  if(getLength(route->waypoints) == 1 ){
        freeList(list)
        return NULL;
      }*/

      if(route->waypoints != NULL){

        void* elem;
        ListIterator iter = createIterator(route->waypoints);
        while((elem = nextElement(&iter)) != NULL){
          Waypoint* tmpWpt = (Waypoint*)elem;

          if(i == 0){
            firstLat = tmpWpt->latitude;
            firstLong = tmpWpt->longitude;

          }
          else{
            lastLat = tmpWpt->latitude;
            lastLong = tmpWpt->longitude;
          }


          i++;
        }
      }else{
        freeList(list);
        return NULL;
      }

      dist = (float)haversine(firstLat, firstLong, sourceLat, sourceLong);
      dist = dist * 1000;

      if(dist <= delta){
        dist = (float)haversine(lastLat, lastLong, destLat, destLong);
        dist = dist * 1000;

        if(dist <= delta){
          numFound++;
          insertBack(list, route);
        }
      }


    }
  }else{
    freeList(list);
    return NULL;
  }

  if(numFound == 0){
    freeList(list);
    return NULL;

  }

  return list;
}

List* getTracksBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta)
{
  if(doc == NULL){
    return NULL;
  }

  if(delta < 0){
    return NULL;
  }

  double lat1 = 0;
  double long1 = 0;
  double lat2 = 0;
  double long2 = 0;

  double lastLat = 0;
  double lastLong = 0;

  float dist = 0;
  //count segment iterations
  int i = 0;
  //count waypoint iterations
  int j = 0;

  int numWpts = 0;

  int numFound = 0;

  Track* tr = NULL;

  List* list = initializeList(&trackToString, &fakeDelete, &compareTracks);


  if(doc->tracks != NULL){
    void* elem0;
    ListIterator iter = createIterator(doc->tracks);
    while((elem0 = nextElement(&iter)) != NULL){
      tr = (Track*)elem0;

      if(tr->segments != NULL){
        void* elem;
        ListIterator iter = createIterator(tr->segments);
        while((elem = nextElement(&iter)) != NULL){
          TrackSegment* tmpSeg = (TrackSegment*)elem;



          if(tmpSeg->waypoints != NULL){
            void* elem2;
            ListIterator iter2 = createIterator(tmpSeg->waypoints);
            while ((elem2 = nextElement(&iter2)) != NULL) {
              Waypoint* tmpWpt = (Waypoint*)elem2;

              if(j == 0 && i == 0){
                lat1 = tmpWpt->latitude;
                long1 = tmpWpt->longitude;
              }
              else{
                lat2 = tmpWpt->latitude;
                long2 = tmpWpt->longitude;
              }

              j++;
              numWpts++;

            }
            j = 0;
            i++;
            lastLat = lat2;
            lastLong = long2;
          }
        }
      }else{
        freeList(list);
        return NULL;
      }

      dist = (float)haversine(lat1, long1, sourceLat, sourceLong);
      dist = dist * 1000;

      if(dist <= delta){
        dist = (float)haversine(lastLat, lastLong, destLat, destLong);
        dist = dist * 1000;

        if(dist <= delta){
          numFound++;
          insertBack(list, tr);
        }
      }
    }
  }else{
    freeList(list);
    return NULL;
  }

  if(numFound == 0){
    freeList(list);
    return NULL;

  }

  return list;
}

char* routeToJSON(const Route *rt){

  if(rt == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"{}");
    return errorChar;
  }

  if(rt->name == NULL){
    return NULL;
  }


  char* tmpStr = NULL;
  int len = 0;
  char* rtName = NULL;
  int numPoints = 0;
  float rtLength = 0;
  bool isLoop = 0;
  char* isLoopChar = NULL;

  //get name or set to None
  if(strcmp(rt->name, "") == 0){
    rtName = (char*)malloc(sizeof(char)*5);
    strcpy(rtName,"None");
  }else{
    rtName = (char*)malloc(sizeof(char)*(strlen(rt->name) + 1));
    strcpy(rtName, rt->name);
  }

  //get get num pointers
  if(rt->waypoints == NULL){
    return NULL;
  }

  numPoints = getLength(rt->waypoints);

  //get length and round
  rtLength = getRouteLen(rt);
  rtLength = round10(rtLength);

  //check if loop
  isLoop = isLoopRoute(rt, 10);

  if(isLoop == 0){
    isLoopChar = (char*)malloc(sizeof(char)*6);
    strcpy(isLoopChar,"false");
  }else if(isLoop == 1){
    isLoopChar = (char*)malloc(sizeof(char)*5);
    strcpy(isLoopChar,"true");
  }

  //allocate memory for the json string
  len = strlen(rtName) + strlen(isLoopChar) + 256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "{\"name\":\"%s\",\"numPoints\":%d,\"len\":%.1f,\"loop\":%s}", rtName, numPoints, rtLength, isLoopChar);

  free(rtName);
  free(isLoopChar);

  return tmpStr;
}

int countTrackPoints(const Track *tr)
{
  int numPts = 0;

  void* elem;
  ListIterator iter = createIterator(tr->segments);
  while ((elem = nextElement(&iter)) != NULL) {
    /* code */
    TrackSegment* tmpSeg = (TrackSegment*)elem;

    void* elem2;
    ListIterator iter2 = createIterator(tmpSeg->waypoints);
    while((elem2 = nextElement(&iter2)) != NULL){
      numPts++;
    }
  }
  return numPts;
}

char* trackToJSON(const Track *tr)
{
  if(tr == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"{}");
    return errorChar;
  }

  if(tr->name == NULL){
    return NULL;
  }

  char* tmpStr = NULL;
  int len = 0;
  char* trName = NULL;
  float trLength = 0;
  bool isLoop = 0;
  char* isLoopChar = NULL;

  //get name or set to None
  if(strcmp(tr->name, "") == 0){
    trName = (char*)malloc(sizeof(char)*5);
    strcpy(trName,"None");
  }else{
    trName = (char*)malloc(sizeof(char)*(strlen(tr->name) + 1));
    strcpy(trName, tr->name);
  }

  //get length and round
  trLength = getTrackLen(tr);
  trLength = round10(trLength);

  //check if loop
  isLoop = isLoopTrack(tr, 10);

  if(isLoop == 0){
    isLoopChar = (char*)malloc(sizeof(char)*6);
    strcpy(isLoopChar,"false");
  }else if(isLoop == 1){
    isLoopChar = (char*)malloc(sizeof(char)*5);
    strcpy(isLoopChar,"true");
  }

  int numPts = countTrackPoints(tr);

  //allocate memory for the json string
  len = strlen(trName) + strlen(isLoopChar) + 256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "{\"name\":\"%s\",\"points\":%d,\"len\":%.1f,\"loop\":%s}", trName, numPts, trLength, isLoopChar);

  free(trName);
  free(isLoopChar);

  return tmpStr;
}

char* routeListToJSON(const List *routeList)
{
  if(routeList == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"[]");
    return errorChar;
  }

  int len;
  len = getLength((List*)routeList);

  if(len == 0){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"[]");
    return errorChar;
  }

  char* tmpStr = NULL;
  char* addStr = NULL;
  int i = 0;

  if(len == 1){
    char* singleStr = NULL;
    singleStr = routeToJSON(getFromFront((List*)routeList));
    tmpStr = (char*)malloc(sizeof(char) *(strlen(singleStr)) + 4);
    sprintf(tmpStr,"[%s]", singleStr);
    free(singleStr);
    return tmpStr;
  }


  tmpStr = (char*)malloc(sizeof(char)*2);
  strcpy(tmpStr,"[");

  void* elem;
  ListIterator iter = createIterator((List*)routeList);
  while((elem = nextElement(&iter)) != NULL){
    Route* tmpRte = (Route*)elem;
    i++;

    addStr = routeToJSON(tmpRte);
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(addStr)) + 2);
    strcat(tmpStr, addStr);
    free(addStr);

    if(i != len){
      tmpStr = (char*)realloc(tmpStr, sizeof(char) * strlen(tmpStr) + 2);
      strcat(tmpStr, ",");
    }
  }

  tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen("]")) + 2);
  strcat(tmpStr, "]");

  return tmpStr;
}

char* trackListToJSON(const List * trackList)
{
  if(trackList == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"[]");
    return errorChar;
  }

  int len;
  len = getLength((List*)trackList);

  if(len == 0){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"[]");
    return errorChar;
  }

  char* tmpStr = NULL;
  char* addStr = NULL;
  int i = 0;

  if(len == 1){
    char* singleStr = NULL;
    singleStr = trackToJSON(getFromFront((List*)trackList));
    tmpStr = (char*)malloc(sizeof(char) *(strlen(singleStr)) + 4);
    sprintf(tmpStr,"[%s]", singleStr);
    free(singleStr);
    return tmpStr;
  }


  tmpStr = (char*)malloc(sizeof(char)*2);
  strcpy(tmpStr,"[");

  void* elem;
  ListIterator iter = createIterator((List*)trackList);
  while((elem = nextElement(&iter)) != NULL){
    Track* tmpTrk = (Track*)elem;
    i++;

    addStr = trackToJSON(tmpTrk);
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(addStr)) + 2);
    strcat(tmpStr, addStr);
    free(addStr);

    if(i != len){
      tmpStr = (char*)realloc(tmpStr, sizeof(char) * strlen(tmpStr) + 2);
      strcat(tmpStr, ",");
    }
  }

  tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen("]")) + 2);
  strcat(tmpStr, "]");

  return tmpStr;
}

char* waypointToJSON(const Waypoint *dt){

  if(dt == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"{}");
    return errorChar;
  }


  char* tmpStr = NULL;
  char* dtName = NULL;

  int len = 0;

  dtName = (char*)malloc(sizeof(char)*(strlen(dt->name) + 1));
  strcpy(dtName, dt->name);

  double lat = dt->latitude;
  double lon = dt->longitude;

  //allocate memory for the json string
  len = strlen(dtName) + 256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "{\"name\":\"%s\",\"lat\":\"%f\",\"lon\":\"%f\"}", dtName, lat, lon);

  free(dtName);

  return tmpStr;
}

char* WaypointListToJSON(const List *routeList)
{
  if(routeList == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"[]");
    return errorChar;
  }

  int len;
  len = getLength((List*)routeList);

  if(len == 0){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"[]");
    return errorChar;
  }

  char* tmpStr = NULL;
  char* addStr = NULL;
  int i = 0;

  if(len == 1){
    char* singleStr = NULL;
    singleStr = waypointToJSON(getFromFront((List*)routeList));
    tmpStr = (char*)malloc(sizeof(char) *(strlen(singleStr)) + 4);
    sprintf(tmpStr,"[%s]", singleStr);
    free(singleStr);
    return tmpStr;
  }


  tmpStr = (char*)malloc(sizeof(char)*2);
  strcpy(tmpStr,"[");

  void* elem;
  ListIterator iter = createIterator((List*)routeList);
  while((elem = nextElement(&iter)) != NULL){
    Waypoint* tmpRte = (Waypoint*)elem;
    i++;

    addStr = waypointToJSON(tmpRte);
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(addStr)) + 2);
    strcat(tmpStr, addStr);
    free(addStr);

    if(i != len){
      tmpStr = (char*)realloc(tmpStr, sizeof(char) * strlen(tmpStr) + 2);
      strcat(tmpStr, ",");
    }
  }

  tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen("]")) + 2);
  strcat(tmpStr, "]");

  return tmpStr;
}


char* otherDataToJSON(const GPXData *dt){

  if(dt == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"{}");
    return errorChar;
  }


  char* tmpStr = NULL;
  char* dtName = NULL;
  char* dtValue = NULL;
  int len = 0;

  dtName = (char*)malloc(sizeof(char)*(strlen(dt->name) + 1));
  strcpy(dtName, dt->name);

  dtValue = (char*)malloc(sizeof(char)*(strlen(dt->value) + 1));
  strcpy(dtValue, dt->value);

  //allocate memory for the json string
  len = strlen(dtName) + strlen(dtValue) + 256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "{\"name\":\"%s\",\"value\":\"%s\"}", dtName, dtValue);

  free(dtName);
  free(dtValue);

  return tmpStr;
}



char* otherDataListToJSON(const List *routeList)
{
  if(routeList == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"[]");
    return errorChar;
  }

  int len;
  len = getLength((List*)routeList);

  if(len == 0){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"[]");
    return errorChar;
  }

  char* tmpStr = NULL;
  char* addStr = NULL;
  int i = 0;

  if(len == 1){
    char* singleStr = NULL;
    singleStr = otherDataToJSON(getFromFront((List*)routeList));
    tmpStr = (char*)malloc(sizeof(char) *(strlen(singleStr)) + 4);
    sprintf(tmpStr,"[%s]", singleStr);
    free(singleStr);
    return tmpStr;
  }


  tmpStr = (char*)malloc(sizeof(char)*2);
  strcpy(tmpStr,"[");

  void* elem;
  ListIterator iter = createIterator((List*)routeList);
  while((elem = nextElement(&iter)) != NULL){
    GPXData* tmpRte = (GPXData*)elem;
    i++;

    addStr = otherDataToJSON(tmpRte);
    tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen(addStr)) + 2);
    strcat(tmpStr, addStr);
    free(addStr);

    if(i != len){
      tmpStr = (char*)realloc(tmpStr, sizeof(char) * strlen(tmpStr) + 2);
      strcat(tmpStr, ",");
    }
  }

  tmpStr = (char*)realloc(tmpStr, sizeof(char) * (strlen(tmpStr) + strlen("]")) + 2);
  strcat(tmpStr, "]");

  return tmpStr;
}


char* GPXtoJSON(const GPXdoc* gpx)
{
  if(gpx == NULL){
    char* errorChar = NULL;
    errorChar = (char*)malloc(sizeof(char)*3);
    strcpy(errorChar,"{}");
    return errorChar;
  }

  if(gpx->creator == NULL){
    return NULL;
  }


  char* tmpStr = NULL;
  int len = 0;

  double version = 0.0;
  char* creator;
  int numWaypoints = 0;
  int numRoutes = 0;
  int numTracks = 0;


  version = gpx->version;

  //get creator
  creator = (char*)malloc(sizeof(char)*(strlen(gpx->creator) + 1));
  strcpy(creator, gpx->creator);


  numWaypoints = getNumWaypoints(gpx);
  numRoutes = getNumRoutes(gpx);
  numTracks = getNumTracks(gpx);

  //allocate memory for the json string
  len = strlen(creator) + 256;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr, "{\"version\":%.1f,\"creator\":\"%s\",\"numWaypoints\":%d,\"numRoutes\":%d,\"numTracks\":%d}", version, creator, numWaypoints, numRoutes, numTracks);

  free(creator);


  return tmpStr;
}

void addWaypoint(Route *rt, Waypoint *pt)
{
  if(rt == NULL){
    return;
  }

  if(pt == NULL){
    return;
  }

  insertBack(rt->waypoints, pt);
}

void addRoute(GPXdoc* doc, Route* rt)
{
  if(doc == NULL){
    return;
  }

  if(rt == NULL){
    return;
  }

  insertBack(doc->routes, rt);
}

GPXdoc* JSONtoGPX(const char* gpxString)
{
  if(gpxString == NULL){
    return NULL;
  }

  char temp[256];
  char* subString;
  GPXdoc* doc = NULL;
  char* creator;
  double version;
  char namespace[256];
  int i = 0;

  strcpy(namespace, "http://www.topografix.com/GPX/1/1");

  strcpy(temp, gpxString);
  subString = strtok(temp, "{}\",:");

  while(subString != NULL){
    subString = strtok(NULL, "{}\",:");

    //get version
    if(i == 0){
      version = atof(subString);
    }
    //get creator
    if(i == 2){
      creator = subString;
    }

    i++;
  }

  doc = GPXdocConstructor(namespace, version, creator);

  return doc;
}

Waypoint* JSONtoWaypoint(const char* gpxString)
{
  if(gpxString == NULL){
    return NULL;
  }

  char temp[256];
  char* subString;
  Waypoint* wpt = NULL;
  double lat;
  double lon;

  int i = 0;


  strcpy(temp, gpxString);
  subString = strtok(temp, "{}\",:");

  while(subString != NULL){
    subString = strtok(NULL, "{}\",:");

    //get lat
    if(i == 0){
      lat = atof(subString);
    }
    //get long
    if(i == 2){
      lon = atof(subString);
    }

    i++;
  }

  wpt = waypointConstructor(lat,lon);

  return wpt;
}

Route* JSONtoRoute(const char* gpxString)
{
  if(gpxString == NULL){
    return NULL;
  }

  char temp[256];
  char* subString;
  Route* rte = NULL;
  char name[256];

  int i = 0;


  strcpy(temp, gpxString);
  subString = strtok(temp, "{}\",:");

  while(subString != NULL){
    subString = strtok(NULL, "{}\",:");

    //get name
    if(i == 0){
      strcpy(name, subString);
    }

    i++;
  }

  rte = routeConstructor();

  updateRouteName(rte, name);

  return rte;
}

char* getGPXdescription (char* fileName){

  GPXdoc* gpx = NULL;

  gpx = createGPXdoc(fileName);

  char* string;
  string = GPXtoJSON(gpx);

  bool b = validateGPXDoc(gpx, "./parser/gpx.xsd");

  if(b == FALSE){
    deleteGPXdoc(gpx);
    char* error = NULL;
    error = (char*)malloc(sizeof(char)*2);
    strcpy(error,"");
    return error;
  }

  if(string != NULL){
    deleteGPXdoc(gpx);
    return string;
  }
  else{
    deleteGPXdoc(gpx);
    char* error = NULL;
    error = (char*)malloc(sizeof(char)*2);
    strcpy(error,"");
    return error;
  }
}

char* getGPXroutes (char* fileName){

  GPXdoc* gpx = NULL;

  gpx = createGPXdoc(fileName);

  char* string;
  string = routeListToJSON(gpx->routes);

  if(string != NULL){
    deleteGPXdoc(gpx);
    return string;
  }
  else{
    deleteGPXdoc(gpx);
    char* error = NULL;
    error = (char*)malloc(sizeof(char)*2);
    strcpy(error,"");
    return error;
  }
}

char* getGPXtracks (char* fileName)
{

  GPXdoc* gpx = NULL;

  gpx = createGPXdoc(fileName);

  char* string;
  string = trackListToJSON(gpx->tracks);

  if(string != NULL){
    deleteGPXdoc(gpx);
    return string;
  }
  else{
    deleteGPXdoc(gpx);
    char* error = NULL;
    error = (char*)malloc(sizeof(char)*2);
    strcpy(error,"");
    return error;
  }
}

char* getRouteData(char* fileName, int index)
{
  GPXdoc* gpx = NULL;

  gpx = createGPXdoc(fileName);

  char* string;

  int searchIndex = 0;

  void* elem;
  ListIterator iter = createIterator(gpx->routes);
  while((elem = nextElement(&iter)) != NULL){
    Route* tmpRte = (Route*)elem;

    if(searchIndex == index){
      string = otherDataListToJSON(tmpRte->otherData);
    }

    searchIndex++;
  }

  if(string != NULL){
    deleteGPXdoc(gpx);
    return string;
  }
  else{
    deleteGPXdoc(gpx);
    char* error = NULL;
    error = (char*)malloc(sizeof(char)*2);
    strcpy(error,"");
    return error;
  }
}

char* getTrackData(char* fileName, int index)
{
  GPXdoc* gpx = NULL;

  gpx = createGPXdoc(fileName);

  char* string;

  int searchIndex = 0;

  void* elem;
  ListIterator iter = createIterator(gpx->tracks);
  while((elem = nextElement(&iter)) != NULL){
    Track* tmpTrk = (Track*)elem;

    if(searchIndex == index){
      string = otherDataListToJSON(tmpTrk->otherData);
    }

    searchIndex++;
  }

  if(string != NULL){
    deleteGPXdoc(gpx);
    return string;
  }
  else{
    deleteGPXdoc(gpx);
    return "";
  }
}

int renameRoute(char* fileName, char newName[], int index){
  GPXdoc* gpx = NULL;

  gpx = createGPXdoc(fileName);

  int searchIndex = 0;

  void* elem;
  ListIterator iter = createIterator(gpx->routes);
  while((elem = nextElement(&iter)) != NULL){
    Route* tmpRte = (Route*)elem;

    if(searchIndex == index){
      updateRouteName(tmpRte, newName);
    }

    searchIndex++;
  }

  bool b = validateGPXDoc(gpx, "./parser/gpx.xsd");

  if(b == FALSE){
    deleteGPXdoc(gpx);
    return 0;
  }


    bool x = writeGPXdoc(gpx, fileName);
    if(x == TRUE){
      deleteGPXdoc(gpx);
      return 1;
    }
    else{
      deleteGPXdoc(gpx);
      return 0;
    }
}

int renameTrack(char* fileName, char newName[], int index){
  GPXdoc* gpx = NULL;

  gpx = createGPXdoc(fileName);

  int searchIndex = 0;

  void* elem;
  ListIterator iter = createIterator(gpx->tracks);
  while((elem = nextElement(&iter)) != NULL){
    Track* tmpTrk = (Track*)elem;

    if(searchIndex == index){
      updateTrackName(tmpTrk, newName);
    }

    searchIndex++;
  }

  bool b = validateGPXDoc(gpx, "./parser/gpx.xsd");

  if(b == FALSE){
    deleteGPXdoc(gpx);
    return 0;
  }

    bool x = writeGPXdoc(gpx, fileName);
    if(x == TRUE){
      deleteGPXdoc(gpx);
      return 1;
    }
    else{
      deleteGPXdoc(gpx);
      return 0;
    }
}

int createNewGPXFileFromGIU(char* json, char* fileName){

  GPXdoc* gpx = JSONtoGPX(json);

  if(gpx == NULL){
    return 0;
  }

  bool b = validateGPXDoc(gpx, "./parser/gpx.xsd");

  if(b == FALSE){
    deleteGPXdoc(gpx);
    return 0;
  }


  bool x = writeGPXdoc(gpx, fileName);
  if(x == TRUE){
    deleteGPXdoc(gpx);
    return 1;
  }
  else{
    deleteGPXdoc(gpx);
    return 0;
  }

}

int addRouteFromGUI(char* fileName, char* route){

    GPXdoc* gpx = NULL;

    gpx = createGPXdoc(fileName);

    if(gpx == NULL){
      return 0;
    }

    Route* rte = JSONtoRoute(route);

    if(rte == NULL){
      deleteGPXdoc(gpx);
      return 0;
    }

    addRoute(gpx, rte);

    bool x = writeGPXdoc(gpx, fileName);
    if(x == TRUE){
      deleteGPXdoc(gpx);
      return 1;
    }
    else{
      deleteGPXdoc(gpx);
      return 0;
    }
}

int addWaypointFromGUI(char* fileName, char* waypoint){

    GPXdoc* gpx = NULL;

    gpx = createGPXdoc(fileName);

    if(gpx == NULL){
      return 0;
    }

    Waypoint* wpt = JSONtoWaypoint(waypoint);

    if(wpt == NULL){
      deleteGPXdoc(gpx);
      return 0;
    }

    Route* rte = getFromBack(gpx->routes);

    addWaypoint(rte, wpt);

    bool x = writeGPXdoc(gpx, fileName);
    if(x == TRUE){
      deleteGPXdoc(gpx);
      return 1;
    }
    else{
      deleteGPXdoc(gpx);
      return 0;
    }
}

char* getPathBetweenRoutes(char* fileName, float sourceLat, float sourceLong, float destLat, float destLong){

    GPXdoc* gpx = NULL;

    gpx = createGPXdoc(fileName);

    if(gpx == NULL){
      return 0;
    }

    List* l = getRoutesBetween(gpx, sourceLat, sourceLong, destLat, destLong, 10);

    char * string = NULL;
    string = routeListToJSON(l);

    deleteGPXdoc(gpx);
    return string;
}

char* getPathBetweenTracks(char* fileName, float sourceLat, float sourceLong, float destLat, float destLong){

    GPXdoc* gpx = NULL;

    gpx = createGPXdoc(fileName);

    if(gpx == NULL){
      return 0;
    }

    List* l = getTracksBetween(gpx, sourceLat, sourceLong, destLat, destLong, 10);

    char * string = NULL;
    string = trackListToJSON(l);

    deleteGPXdoc(gpx);
    return string;
}

char* getRouteWpts (char* fileName, int routeIndex){

  GPXdoc* gpx = NULL;

  gpx = createGPXdoc(fileName);
  int currIndex = 0;
  char* string;

  void* elem;
  ListIterator iter = createIterator(gpx->routes);
  while((elem = nextElement(&iter)) != NULL){
    Route* rte = (Route*)elem;

    if(currIndex == routeIndex){
      string = WaypointListToJSON(rte->waypoints);
    }

    currIndex++;
  }

  if(string != NULL){
    deleteGPXdoc(gpx);
    return string;
  }
  else{
    deleteGPXdoc(gpx);
    char* error = NULL;
    error = (char*)malloc(sizeof(char)*2);
    strcpy(error,"");
    return error;
  }
}

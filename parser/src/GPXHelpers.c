#include <stdio.h>
#include <string.h>
#include "GPXParser.h"
#include "LinkedListAPI.h"
#include "GPXHelpers.h"

TrackSegment* trackSegmentConstructor()
{
  TrackSegment* tmpTrkSeg = malloc(sizeof(TrackSegment));
  tmpTrkSeg->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);

  return tmpTrkSeg;
}

void updateTrackName(Track* trk, char name [])
{
  trk->name = (char*)realloc(trk->name, strlen(name) + 1);
  strcpy(trk->name, (char*)name);
}

Track* trackConstructor()
{
  Track* tmpTrack = malloc(sizeof(Track));
  tmpTrack->name = malloc(sizeof((char*)""));
  strcpy(tmpTrack->name, (char*)"");
  tmpTrack->segments = initializeList(&trackSegmentToString, &deleteTrackSegment, &compareTrackSegments);
  tmpTrack->otherData = initializeList(&gpxDataToString, &deleteGpxData, &compareGpxData);

  return tmpTrack;
}
void updateRouteName(Route* rte, char name [])
{
  rte->name = (char*)realloc(rte->name, strlen(name) + 1);
  strcpy(rte->name, (char*)name);
}

Route* routeConstructor()
{
  Route* tmpRoute = malloc(sizeof(Route));
  tmpRoute->name = malloc(sizeof((char*)""));
  strcpy(tmpRoute->name, (char*)"");
  tmpRoute->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
  tmpRoute->otherData = initializeList(&gpxDataToString, &deleteGpxData, &compareGpxData);

  return tmpRoute;
}

GPXData* gpxDataConstructor(char name [], char content [])
{
  GPXData* tmpGPXData = malloc(sizeof(GPXData) + strlen(content) * sizeof(char) + 1);
  strcpy(tmpGPXData->name, name);
  strcpy(tmpGPXData->value, content);

  return tmpGPXData;
}

void updateWaypointName(Waypoint* wpt, char name [])
{
  wpt->name = (char*)realloc(wpt->name, strlen(name) + 1);
  strcpy(wpt->name, (char*)name);
}

Waypoint* waypointConstructor(double latitude, double longitude)
{
  Waypoint* wpt = malloc(sizeof(Waypoint));
  wpt->name = malloc(sizeof((char*)""));
  strcpy(wpt->name, (char*)"");
  wpt->longitude = longitude;

  wpt->latitude = latitude;
  wpt->otherData = initializeList(&gpxDataToString, &deleteGpxData, &compareGpxData);

  return wpt;
}

GPXdoc* GPXdocConstructor(char namespace[256], double version, char* creator)
{
  GPXdoc* gpx = malloc(sizeof(GPXdoc));

  strcpy(gpx->namespace, namespace);
  gpx->version = version;
  gpx->creator = malloc(strlen(creator) + 1);
  strcpy(gpx->creator, creator);
  gpx->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
  gpx->routes = initializeList(&routeToString, &deleteRoute, &compareRoutes);
  gpx->tracks = initializeList(&trackToString, &deleteTrack, &compareTracks);

  return gpx;
}

GPXdoc* build_gpx_doc(xmlNode * a_node, GPXdoc* gpx)
{
  xmlNode *cur_node = NULL;
  xmlNode *search_children = NULL;
  xmlNode *search_children2 = NULL;
  xmlNode *search_children3 = NULL;
  Waypoint* wpt = NULL;
  Route* rte = NULL;

  for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
      if (cur_node->type == XML_ELEMENT_NODE) {
          //printf("node type: Element, name: %s\n", cur_node->name);
      }

      /*if (strcmp((char*)cur_node->name,"wpt") == 0 || strcmp((char*)cur_node->name,"rte") == 0 || strcmp((char*)cur_node->name,"trk") == 0) {
          printf("NEW ELEMENT-------------------------------------------------\n");
      }*/

      //GPX doc
      if(strcmp((char*)cur_node->name, "gpx") == 0){
          char namespace[256];
          double version;
          char* tmpVersion  = NULL;
          char* creator = NULL;

          // Iterate through every attribute of the current node
          xmlAttr *attr;
          for (attr = cur_node->properties; attr != NULL; attr = attr->next)
          {
              xmlNode *value = attr->children;
              char *attrName = (char *)attr->name;
              char *cont = (char *)(value->content);
              //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
              if(strcmp((char*)attrName, "version") == 0){
                  tmpVersion = cont;
              }else if(strcmp((char*)attrName, "creator") == 0){
                  creator = cont;
              }/*else if(strcmp((char*)attrName, "schemaLocation") == 0){
                  strcpy(namespace, cont);
              }*/

          }
          //get namespace
          strcpy(namespace, (char*)cur_node->ns->href);
          //check if namespace, version and creator are invalid
          if(creator == NULL || strcmp(creator, "") == 0){
            return NULL;
          }
          if(namespace == NULL || strcmp(namespace, "") == 0){
            return NULL;
          }
          if(tmpVersion == NULL || strcmp(tmpVersion, "") == 0){
            return NULL;
          }

          //convert tmpVersion string to double
          version = atof(tmpVersion);
          //call GPXdoc constructor
          gpx = GPXdocConstructor(namespace, version, creator);
          //free(creator);
      }
      //Waypoints
      if(strcmp((char*)cur_node->name, "wpt") == 0){

        //char* name;
        double longitude;
        double latitude;

        // Iterate through every attribute of the current node
        xmlAttr *attr;
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);
            //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
            if(strcmp((char*)attrName, "lat") == 0){
                latitude = atof(cont);
            }else if(strcmp((char*)attrName, "lon") == 0){
                longitude = atof(cont);
            }
        }

        //cal waypoint constructor
        wpt = waypointConstructor(latitude, longitude);

        //search children of waypoint
          char content [256] = "";
          char name [256] = "";
          for(search_children = cur_node->children; search_children != NULL; search_children = search_children->next)
          {
            if (search_children->type == XML_ELEMENT_NODE) {
                //printf("%s\n", search_children->name);
                //printf("  content: %s\n", xmlNodeGetContent(search_children));
                if(strcmp((char*)search_children->name, "name") == 0){

                  xmlChar* xmlName = xmlNodeGetContent(search_children);
                  strcpy(content, (char*)xmlName);
                  updateWaypointName(wpt, content);
                  free(xmlName);
                }
                else{

                  xmlChar* xmlName = xmlNodeGetContent(search_children);
                  strcpy(content, (char*)xmlName);
                  strcpy(name, (char*)search_children->name);
                  free(xmlName);

                  GPXData* tmpGPXData = NULL;
                  tmpGPXData = gpxDataConstructor(name, content);
                  insertBack(wpt->otherData, tmpGPXData);

                }
            }

          }

        insertBack(gpx->waypoints, wpt);
      }

      //Routes
      if(strcmp((char*)cur_node->name, "rte") == 0){

        rte = routeConstructor();

        //Search route children
        char content [256] = "";
        char name [256] = "";
        for(search_children = cur_node->children; search_children != NULL; search_children = search_children->next)
        {
          if (search_children->type == XML_ELEMENT_NODE) {
              //printf("%s\n", search_children->name);
              //printf("  content: %s\n", xmlNodeGetContent(search_children));
              if(strcmp((char*)search_children->name, "name") == 0){

                xmlChar* xmlName = xmlNodeGetContent(search_children);
                strcpy(content, (char*)xmlName);
                updateRouteName(rte, content);
                free(xmlName);

              }else if(strcmp((char*)search_children->name, "rtept") == 0){

                  double longitude;
                  double latitude;

                  // Iterate through every attribute of the current node
                  xmlAttr *attr;
                  for (attr = search_children->properties; attr != NULL; attr = attr->next)
                  {
                      xmlNode *value = attr->children;
                      char *attrName = (char *)attr->name;
                      char *cont = (char *)(value->content);
                      //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                      if(strcmp((char*)attrName, "lat") == 0){
                          latitude = atof(cont);
                      }else if(strcmp((char*)attrName, "lon") == 0){
                          longitude = atof(cont);
                      }
                  }

                  //cal waypoint constructor
                  wpt = waypointConstructor(latitude, longitude);

                  //search children of waypoint
                    char content [256] = "";
                    char name [256] = "";
                    for(search_children2 = search_children->children; search_children2 != NULL; search_children2 = search_children2->next)
                    {
                      if (search_children2->type == XML_ELEMENT_NODE) {
                          //printf("%s\n", search_children->name);
                          //printf("  content: %s\n", xmlNodeGetContent(search_children));
                          if(strcmp((char*)search_children2->name, "name") == 0){

                            xmlChar* xmlName = xmlNodeGetContent(search_children2);
                            strcpy(content, (char*)xmlName);
                            updateWaypointName(wpt, content);
                            free(xmlName);
                          }
                          else{

                            xmlChar* xmlName = xmlNodeGetContent(search_children2);
                            strcpy(content, (char*)xmlName);
                            strcpy(name, (char*)search_children2->name);
                            free(xmlName);

                            GPXData* tmpGPXData = NULL;
                            tmpGPXData = gpxDataConstructor(name, content);
                            insertBack(wpt->otherData, tmpGPXData);

                          }
                      }
                    }

                      insertBack(rte->waypoints, wpt);
              }

              else{

                xmlChar* xmlName = xmlNodeGetContent(search_children);
                strcpy(content, (char*)xmlName);
                strcpy(name, (char*)search_children->name);
                free(xmlName);
                GPXData* tmpGPXData = NULL;
                tmpGPXData = gpxDataConstructor(name, content);
                insertBack(rte->otherData, tmpGPXData);
              }

            }

          }

          insertBack(gpx->routes, rte);
      }







      //Tracks
      if(strcmp((char*)cur_node->name, "trk") == 0){

        //cal track constructor
        Track* trk = trackConstructor();

        //search children of track
          char content [256] = "";
          char name [256] = "";
          for(search_children = cur_node->children; search_children != NULL; search_children = search_children->next)
          {
            if (search_children->type == XML_ELEMENT_NODE) {
                //printf("%s\n", search_children->name);
                //printf("  content: %s\n", xmlNodeGetContent(search_children));
                if(strcmp((char*)search_children->name, "name") == 0){

                  xmlChar* xmlName = xmlNodeGetContent(search_children);
                  strcpy(content, (char*)xmlName);
                  updateTrackName(trk, content);
                  free(xmlName);
                }
                else if(strcmp((char*)search_children->name, "trkseg") == 0){

                  TrackSegment* trkseg = trackSegmentConstructor();

                  for(search_children2 = search_children->children; search_children2 != NULL; search_children2 = search_children2->next)
                  {
                    if (search_children2->type == XML_ELEMENT_NODE) {

                        if(strcmp((char*)search_children2->name, "trkpt") == 0){

                          double longitude;
                          double latitude;

                          // Iterate through every attribute of the current node
                          xmlAttr *attr;
                          for (attr = search_children2->properties; attr != NULL; attr = attr->next)
                          {
                              xmlNode *value = attr->children;
                              char *attrName = (char *)attr->name;
                              char *cont = (char *)(value->content);
                              //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                              if(strcmp((char*)attrName, "lat") == 0){
                                  latitude = atof(cont);
                              }else if(strcmp((char*)attrName, "lon") == 0){
                                  longitude = atof(cont);
                              }
                          }

                          //cal waypoint constructor
                          wpt = waypointConstructor(latitude, longitude);

                          //search children of waypoint
                            char content [256] = "";
                            char name [256] = "";
                            for(search_children3 = search_children2->children; search_children3 != NULL; search_children3 = search_children3->next)
                            {
                              if (search_children3->type == XML_ELEMENT_NODE) {
                                  //printf("%s\n", search_children->name);
                                  //printf("  content: %s\n", xmlNodeGetContent(search_children));
                                  if(strcmp((char*)search_children3->name, "name") == 0){

                                    xmlChar* xmlName = xmlNodeGetContent(search_children3);
                                    strcpy(content, (char*)xmlName);
                                    updateWaypointName(wpt, content);
                                    free(xmlName);
                                  }
                                  else{

                                    xmlChar* xmlName = xmlNodeGetContent(search_children3);
                                    strcpy(content, (char*)xmlName);
                                    strcpy(name, (char*)search_children3->name);
                                    free(xmlName);

                                    GPXData* tmpGPXData = NULL;
                                    tmpGPXData = gpxDataConstructor(name, content);
                                    insertBack(wpt->otherData, tmpGPXData);

                                  }
                              }
                            }

                              insertBack(trkseg->waypoints, wpt);


                        }
                      }
                    }

                  insertBack(trk->segments, trkseg);
                }
                else{

                  xmlChar* xmlName = xmlNodeGetContent(search_children);
                  strcpy(content, (char*)xmlName);
                  strcpy(name, (char*)search_children->name);
                  free(xmlName);

                  GPXData* tmpGPXData = NULL;
                  tmpGPXData = gpxDataConstructor(name, content);
                  insertBack(trk->otherData, tmpGPXData);

                }
            }

          }

        insertBack(gpx->tracks, trk);
      }
      // Uncomment the code below if you want to see the content of every node.

      //if (cur_node->content != NULL ){
        //   printf("  content: %s\n", cur_node->content);
       //}


      //go to next node
      build_gpx_doc(cur_node->children, gpx);
  }

  return gpx;
}

int validateTree(xmlDoc* doc, char* gpxSchemaFile){

  int validity;
  xmlSchemaPtr schema = NULL;
  xmlSchemaParserCtxtPtr ctxt;

  xmlLineNumbersDefault(1);

  ctxt = xmlSchemaNewParserCtxt(gpxSchemaFile);



  schema = xmlSchemaParse(ctxt);

  if(schema == NULL){
    xmlSchemaFreeParserCtxt(ctxt);
    return 0;
  }

  xmlSchemaFreeParserCtxt(ctxt);

  xmlSchemaValidCtxtPtr validCtxt;
  validCtxt = xmlSchemaNewValidCtxt(schema);
  int ret;

  ret = xmlSchemaValidateDoc(validCtxt, doc);

  if(ret == 0){
    validity = 1;
  }
  else if (ret > 1){
    validity = 0;
  }
  else{
    validity = 0;
  }

  xmlSchemaFreeValidCtxt(validCtxt);

  if(schema != NULL){
    xmlSchemaFree(schema);
  }

  xmlSchemaCleanupTypes();
  //xmlCleanupParser();
  //xmlMemoryDump();

  return validity;
}

void addGPXDataToTree(List* otherData, xmlNode* node){
  if(otherData != NULL){
    char buff[256];
    char buff2[256];
    void* elemData;
    ListIterator iterData = createIterator(otherData);
    while((elemData = nextElement(&iterData)) != NULL){
      GPXData* tmpData = (GPXData*)elemData;

      strcpy(buff, tmpData->name);
      strcpy(buff2, tmpData->value);
      xmlNewChild(node, NULL, BAD_CAST buff, BAD_CAST buff2);
    }
  }
}

void addWptGPXDataToTree(List* otherData, xmlNode* node, char* name){
  if(otherData != NULL){
    char buff[256];
    char buff2[256];
    void* elemData;
    ListIterator iterData = createIterator(otherData);
    while((elemData = nextElement(&iterData)) != NULL){
      GPXData* tmpData = (GPXData*)elemData;
      if((strcmp(tmpData->name, "ele") == 0) || (strcmp(tmpData->name, "time") == 0) || (strcmp(tmpData->name, "magvar") == 0) || (strcmp(tmpData->name, "geoidheight") == 0))  {
        strcpy(buff, tmpData->name);
        strcpy(buff2, tmpData->value);
        xmlNewChild(node, NULL, BAD_CAST buff, BAD_CAST buff2);
      }
    }

    //add name if exists
    if(strcmp(name, "") != 0){
      strcpy(buff, name);
      xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST buff);
    }

    iterData = createIterator(otherData);
    while((elemData = nextElement(&iterData)) != NULL){
      GPXData* tmpData = (GPXData*)elemData;
      if((strcmp(tmpData->name, "ele") != 0) && (strcmp(tmpData->name, "time") != 0) && (strcmp(tmpData->name, "magvar") != 0) && (strcmp(tmpData->name, "geoidheight") != 0))  {
        strcpy(buff, tmpData->name);
        strcpy(buff2, tmpData->value);
        xmlNewChild(node, NULL, BAD_CAST buff, BAD_CAST buff2);
      }
    }
  }
}

void addWaypointsToTree(List* waypoints, xmlNode* node){

  if(waypoints != NULL){
    xmlNodePtr node1 = NULL;
    char buff[256];
    void* elem;
    ListIterator iter = createIterator(waypoints);
    while((elem = nextElement(&iter)) != NULL){
      Waypoint* tmpWpt = (Waypoint*)elem;
      //check parent type
      if(strcmp((char*)node->name, "gpx") == 0){
        node1 = xmlNewChild(node, NULL, BAD_CAST "wpt", NULL);
      }else if(strcmp((char*)node->name, "rte") == 0){
        node1 = xmlNewChild(node, NULL, BAD_CAST "rtept", NULL);
      }else{
        node1 = xmlNewChild(node, NULL, BAD_CAST "trkpt", NULL);
      }
      //add lat
      sprintf(buff, "%f", tmpWpt->latitude);
      xmlNewProp(node1, BAD_CAST "lat", BAD_CAST buff);
      //add long
      sprintf(buff, "%f", tmpWpt->longitude);
      xmlNewProp(node1, BAD_CAST "lon", BAD_CAST buff);

      //Get gpx Data
      addWptGPXDataToTree(tmpWpt->otherData, node1, tmpWpt->name);
    }
  }
}

xmlDoc* gpxToTree(GPXdoc * doc){

  xmlDocPtr newXML = NULL;
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;
  char buff[256];

  LIBXML_TEST_VERSION;

  //create new xml tree, node and set root node1

  newXML = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "gpx");
  xmlDocSetRootElement(newXML, root_node);

  //set namespace
  xmlNsPtr ns = xmlNewNs(root_node, BAD_CAST doc->namespace, NULL);
  xmlSetNs(root_node, ns);

  //add attributes for gpx document
  sprintf(buff, "%.1f", doc->version);
  xmlNewProp(root_node, BAD_CAST "version", BAD_CAST buff);
  strcpy(buff, doc->creator);
  xmlNewProp(root_node, BAD_CAST "creator", BAD_CAST buff);

  //add waypoints
  addWaypointsToTree(doc->waypoints, root_node);

  //add route
  if(doc->routes != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->routes);
    while((elem = nextElement(&iter)) != NULL){
      Route* tmpRte = (Route*)elem;
      node = xmlNewChild(root_node, NULL, BAD_CAST "rte", NULL);

      //add name if exists
      if(strcmp(tmpRte->name, "") != 0){
        strcpy(buff, tmpRte->name);
        xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST buff);
      }
      //Get gpx Data
      addGPXDataToTree(tmpRte->otherData, node);

      //Get routepoints
      addWaypointsToTree(tmpRte->waypoints, node);
    }
  }

  //add tracks
  if(doc->tracks != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->tracks);
    while((elem = nextElement(&iter)) != NULL){
      Track* tmpTrk = (Track*)elem;
      node = xmlNewChild(root_node, NULL, BAD_CAST "trk", NULL);

      //add name if exists
      if(strcmp(tmpTrk->name, "") != 0){
        strcpy(buff, tmpTrk->name);
        xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST buff);
      }
      //Get gpx Data
      addGPXDataToTree(tmpTrk->otherData, node);

      if(tmpTrk->segments != NULL){
        void* elem2;
        ListIterator iter2 = createIterator(tmpTrk->segments);
        while ((elem2 = nextElement(&iter2)) != NULL) {
          TrackSegment* tmpSeg = (TrackSegment*)elem2;

          node1 = xmlNewChild(node, NULL, BAD_CAST "trkseg", NULL);
          //Get trackpoints
          addWaypointsToTree(tmpSeg->waypoints, node1);
        }
      }
    }
  }

  return newXML;

}

int checkGPXDataConstraints(List* otherData){

  if(otherData != NULL){

    void* elemData;

    ListIterator iterData = createIterator(otherData);
    while((elemData = nextElement(&iterData)) != NULL){
      GPXData* tmpData = (GPXData*)elemData;

      if(strcmp(tmpData->name, "") == 0){
        return 0;
      }

      if(strcmp((char*)tmpData->value, "") == 0 || tmpData->value == NULL){
        return 0;
      }
    }
  }
  return 1;
}

int checkWaypointConstraints(List* waypoints){

  if(waypoints != NULL){
    void* elem;
    ListIterator iter = createIterator(waypoints);
    while((elem = nextElement(&iter)) != NULL){
      Waypoint* tmpWpt = (Waypoint*)elem;
      //name
      if(tmpWpt->name == NULL){
        return 0;
      }
      //lat
      /*if(tmpWpt->latitude == NULL){
        return 0;
      }*/
      if(tmpWpt->latitude > 90 || tmpWpt->latitude < -90){
        return 0;
      }
      //add long
      /*if(tmpWpt->longitude == NULL){
        return 0;
      }*/
      if(tmpWpt->longitude > 180 || tmpWpt->longitude < -180){
        return 0;
      }

      if(tmpWpt->otherData == NULL){
        return 0;
      }

      //int checkGPXData;
      //Get gpx Data
      int checkGPXData = checkGPXDataConstraints(tmpWpt->otherData);

      if(checkGPXData == 0){
        return 0;
      }

    }
  }
  return 1;
}

int checkGPXConstraints(GPXdoc* doc){

  //Check GPX struct
  if(strcmp(doc->namespace, "") == 0){
    return 0;
  }

  if(doc->version < 0){
    return 0;
  }

  if(strcmp(doc->creator, "") == 0 || doc->creator == NULL){
    return 0;
  }

  //add waypoints
  if(doc->waypoints == NULL){
    return 0;
  }

  int checkWaypoints;
  checkWaypoints = checkWaypointConstraints(doc->waypoints);

  if(checkWaypoints == 0){
    return 0;
  }

  //add route
  if(doc->routes != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->routes);
    while((elem = nextElement(&iter)) != NULL){
      Route* tmpRte = (Route*)elem;

      if(tmpRte->name == NULL){
        return 0;
      }

      if(tmpRte->waypoints == NULL){
        return 0;
      }

      int checkWaypoints = checkWaypointConstraints(tmpRte->waypoints);

      if(checkWaypoints == 0){
        return 0;
      }

      int checkGPXData = checkGPXDataConstraints(tmpRte->otherData);

      if(checkGPXData == 0){
        return 0;
      }
    }
  }else{
    return 0;
  }

  //add tracks
  if(doc->tracks != NULL){
    void* elem;
    ListIterator iter = createIterator(doc->tracks);
    while((elem = nextElement(&iter)) != NULL){
      Track* tmpTrk = (Track*)elem;

      if(tmpTrk->name == NULL){
        return 0;
      }

      //Get gpx Data
      int checkGPXData = checkGPXDataConstraints(tmpTrk->otherData);

      if(checkGPXData == 0){
        return 0;
      }

      if(tmpTrk->segments != NULL){
        void* elem2;
        ListIterator iter2 = createIterator(tmpTrk->segments);
        while ((elem2 = nextElement(&iter2)) != NULL) {
          TrackSegment* tmpSeg = (TrackSegment*)elem2;

          if(tmpSeg->waypoints == NULL){
            return 0;
          }

          int checkWaypoints = checkWaypointConstraints(tmpSeg->waypoints);

          if(checkWaypoints == 0){
            return 0;
          }
        }
      }
    }
  }else{
    return 0;
  }



  return 1;
}

void fakeDelete(void* data){

}

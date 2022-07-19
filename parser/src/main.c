#include <stdio.h>
#include <string.h>
#include "GPXParser.h"
#include "LinkedListAPI.h"

int main (void){

  GPXdoc* gpx = NULL;
  //Waypoint* wpt = NULL;
  /*int numWpts = 0;
  int numRoutes = 0;
  int numTracks = 0;
  int numSegs = 0;
  int numGPXData = 0;*/
  char* fileName = "simple.gpx";

  gpx = createGPXdoc(fileName);

  //test getters
  /*

  numWpts = getNumWaypoints(gpx);
  printf("Num Waypoints: %d\n", numWpts);

  numRoutes = getNumRoutes(gpx);
  printf("Num Routes: %d\n", numRoutes);

  numTracks = getNumTracks(gpx);
  printf("Num Tracks: %d\n", numTracks);

  numSegs = getNumSegments(gpx);
  printf("Num Segments: %d\n", numSegs);

  numGPXData = getNumGPXData(gpx);
  printf("Num GPXData: %d\n", numGPXData);*/

  /*Route* tmpRte = getRoute(gpx, "Some route");

  if(tmpRte != NULL){
    printf("Name: %s\n", tmpRte->name);
    strcpy(tmpRte->name, "new name");
  }*/
  //test to string
  char* string;
  string = GPXdocToString(gpx);

  if(string != NULL)
    printf("%s\n", string);
  free(string);

  deleteGPXdoc(gpx);

  //free(fileName);


  return 0;
}

// Put all onload AJAX calls here, and event listeners

var waypoints = new Array();
var validFiles = new Array();

$(document).ready(function() {
    // On page-load AJAX Example
    /*let stat = statusNoAlert();
    console.log(stat);
    if(stat != 1){
      $('#dbStuff').hide();
    }*/

    // Fill file log panel
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/getGPXFiles',   //The server endpoint we are connecting to

        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string,
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
          //  if(data.fileNames.length > 2){
                //$('#fileLogPanel').css("overflow-y", "scroll");
            //}

          if(data.gpxInfo.length == 0){
            $('#fileLogBody').append("No files found on server");
          }

          //remove pesky .DS_Store file
          /*let foundDSStore = 0;
          for(var i in data.fileNames){

            if(data.fileNames[i] != ".DS_Store"){
              foundDSStore = 1;
            }
          }
          if(foundDSStore == 1){
            data.fileNames.shift();
          }*/

          //console.log(data.fileNames);
           for(var i in data.fileNames){
              //  console.log(data.fileNames[i]);

                  //add data to filelog panel
                  $('#fileLogBody').append('<tr>'
                  +'<td>'+ '<a href ='+ './uploads/' +data.fileNames[i] +'>'+data.fileNames[i]+'</td>'
                  +'<td>' + data.gpxInfo[i].version+'</td>'
                  +'<td>' + data.gpxInfo[i].creator+'</td>'
                  +'<td>' + data.gpxInfo[i].numWaypoints+'</td>'
                  +'<td>' + data.gpxInfo[i].numRoutes+'</td>'
                  +'<td>' + data.gpxInfo[i].numTracks+'</td>'
                  );

                  //fill dropdown menu with filenames
                  let newFile = $("<option>");
                  newFile.text(data.fileNames[i]);
                  newFile.on("click", dropDownToTable);
                  $("#gpxmenu").append(newFile);

                  let newFile3 = $("<option>");
                  newFile3.text(data.fileNames[i]);
                  $("#pickFilemenu").append(newFile3);

                  let newFile2 = $("<option>");
                  newFile2.text(data.fileNames[i]);
                  newFile2.on("click", addToRoutesDropDown);
                  $("#pickFilemenu2").append(newFile2);

                  let newFile4 = $("<option>");
                  newFile4.text(data.fileNames[i]);
                  $("#pickFilemenu3").append(newFile4);

                  let newFile5 = $("<option>");
                  newFile5.text(data.fileNames[i]);
                  $("#pickFilemenu4").append(newFile5);

                  validFiles.push(String(data.fileNames[i]));


           }

           let status = "Successfuly added all existing and valid files to table";
           console.log(status);

        },
        fail: function(error) {
            // Non-200 return, do something with error

            console.log(error);
        }
    });

    //rename form*********************************************************************************************************
    $('#renameForm').submit(function(e){

        let myNewName = $('#renameBox').val();
        let myFile = $("#gpxmenu").find(":selected").text();
        let myIndex = $("#OtherDatamenu").find(":selected").attr('id');
        let myType = $("#OtherDatamenu").find(":selected").attr('class');

        //console.log(newName);
        //console.log(myFile);
        //console.log(index);


        $('#renameBox').empty();
        e.preventDefault();
        //location.reload();
        if(myNewName != "" || typeof myNewName === 'string'){
          if(typeof myIndex !== "undefined" && typeof myType !== "undefined"){

            $.ajax({
                type: 'get',            //Request type
                dataType: 'json',       //Data type - we will use JSON for almost everything
                url: '/rename',   //The server endpoint we are connecting to
                data: {
                  fileName: myFile,
                  newName: myNewName,
                  index: myIndex,
                  type: myType,

                },

                success: function (data) {
                    /*  Do something with returned object
                        Note that what we get is an object, not a string,
                        so we do not need to parse it on the server.
                        JavaScript really does handle JSONs seamlessly
                    */
                  //  if(data.fileNames.length > 2){
                        //$('#fileLogPanel').css("overflow-y", "scroll");
                    //}
                    let x = data.success;
                    let status;

                    if(x == 1){
                      alert("renamed successfuly");
                    }else{
                      alert("error");
                    }


                    location.reload();

                },
                fail: function(error) {
                    // Non-200 return, do something with error

                    console.log(error);
                }
            });


          }else{
            alert("Please ensure that a GPX file and component are selected!");
          }
        }else{
          alert("Invalid input!");
        }

    });

    $('#CreateGPXForm').submit(function(e){

        let myFileName = $('#fileNameBox').val();
        let myVersion = $('#versionBox').val();
        let myCreator = $('#creatorBox').val();

        if(myFileName.endsWith(".gpx") == 1 && myFileName != ".gpx"){

          if(!isNaN(parseFloat(myVersion))){

            if(myCreator != ""){

              $.ajax({
                  type: 'get',            //Request type
                  dataType: 'json',       //Data type - we will use JSON for almost everything
                  url: '/createGPX',   //The server endpoint we are connecting to
                  data: {
                    fileName: myFileName,
                    version: myVersion,
                    creator: myCreator,

                  },

                  success: function (data) {
                      /*  Do something with returned object
                          Note that what we get is an object, not a string,
                          so we do not need to parse it on the server.
                          JavaScript really does handle JSONs seamlessly
                      */
                    //  if(data.fileNames.length > 2){
                          //$('#fileLogPanel').css("overflow-y", "scroll");
                      //}

                      let x = data.success;
                      let y = data.writeSucess;


                      if(x == 1){
                        if(y == 1){
                          alert("GPX created successfuly");
                          location.reload();
                        }else{
                          alert("Schema validation error! Check input.");
                        }

                      }else{
                        alert("error");
                      }

                  },
                  fail: function(error) {
                      // Non-200 return, do something with error

                      console.log(error);
                  }
              });

            }else{
              alert("Must fill creator")
            }

          }else{
            alert("version must be double");
          }

        }else{
          alert("file name must end with .gpx");
        }


        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });

    $('#CreateWaypointForm').submit(function(e){

        let myLong = $('#longBox').val();
        let myLat = $('#latBox').val();

        if(!isNaN(parseFloat(myLong)) && !isNaN(parseFloat(myLat))){

          if(myLat >= -90 && myLat <= 90 && myLong >= -180 && myLong <= 180){

            let wpt = new Object();
            wpt.lat = myLat;
            wpt.lon = myLong;
            let wptStr = JSON.stringify(wpt);

            waypoints.push(wptStr);


          }else{
              alert("Latitude/longitude out of range");
          }
        }else{
          alert("Latitude and longitude must be doubles");
        }

        e.preventDefault();
    });

    $('#CreateRouteForm').submit(function(e){

        let myRouteName = $('#routeNameBox').val();
        let myFile = $("#gpxmenu").find(":selected").text();

        let route = new Object();
        route.name = myRouteName;
        let rteStr = JSON.stringify(route);

        e.preventDefault();

        if(myFile != "Choose GPX file"){



          $.ajax({
              type: 'get',            //Request type
              dataType: 'json',       //Data type - we will use JSON for almost everything
              url: '/addRoute',   //The server endpoint we are connecting to
              data: {
                fileName: myFile,
                route: rteStr,
                waypointArr: waypoints,
                length: waypoints.length,
              },
              success: function (data) {
                  /*  Do something with returned object
                      Note that what we get is an object, not a string,
                      so we do not need to parse it on the server.
                      JavaScript really does handle JSONs seamlessly
                  */
                  let x = data.success;



                  if(x == 1){

                    alert("Route added successfuly");
                    location.reload();

                  }else{
                    alert("error");
                  }




              },
              fail: function(error) {
                  // Non-200 return, do something with error
                  console.log(error);
              }
          });
        }else{
          alert("Select a gpx file");
        }
    });

    $('#FindPathForm').submit(function(e){

        let myStartLong = $('#startLongBox').val();
        let myStartLat = $('#startLatBox').val();
        let myEndLong = $('#endLongBox').val();
        let myEndLat = $('#endLatBox').val();

        e.preventDefault();

        if(!isNaN(parseFloat(myStartLong)) && !isNaN(parseFloat(myStartLat)) && !isNaN(parseFloat(myEndLong)) && !isNaN(parseFloat(myEndLat)) ){

          if(myStartLong >= -180 && myStartLong <= 180 && myStartLat >= -90 && myStartLat <= 90 && myEndLong >= -180 && myEndLong <= 180 && myEndLat >= -90 && myEndLat <= 90){

            $.ajax({
                type: 'get',            //Request type
                dataType: 'json',       //Data type - we will use JSON for almost everything
                url: '/findPath',   //The server endpoint we are connecting to
                data: {
                  startLat: myStartLat,
                  startLong: myStartLong,
                  endLat: myEndLat,
                  endLong: myEndLong,
                  files: validFiles,
                },
                success: function (data) {
                    /*  Do something with returned object
                        Note that what we get is an object, not a string,
                        so we do not need to parse it on the server.
                        JavaScript really does handle JSONs seamlessly
                    */
                    $("#displayPathsBody").empty();
                    if(data.routes.length == 0 && data.tracks.length == 0){
                      $('#displayPathsBody').append("No Components Found");
                    }

                    for(var i in data.routes){
                       //  console.log(data.fileNames[i]);

                           //add data to filelog panel
                           $('#displayPathsBody').append('<tr>'

                           +'<td>' + "Route "+ (i) +'</td>'
                           +'<td>' + data.routes[i].name+'</td>'
                           +'<td>' + data.routes[i].numPoints+'</td>'
                           +'<td>' + data.routes[i].len+'</td>'
                           +'<td>' + data.routes[i].loop+'</td>'

                           );

                    }
                    for(var i in data.tracks){
                       //  console.log(data.fileNames[i]);

                           //add data to filelog panel
                           $('#displayPathsBody').append('<tr>'

                           +'<td>' + "Track "+ (i) +'</td>'
                           +'<td>' + data.tracks[i].name+'</td>'
                           +'<td>' + data.tracks[i].points +'</td>'
                           +'<td>' + data.tracks[i].len+'</td>'
                           +'<td>' + data.tracks[i].loop+'</td>'

                           );

                    }




                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }
            });


          }else{
              alert("Latitude/longitude out of range");
          }
        }else{
          alert("Latitude and longitude must be doubles");
        }


    });

    $('#LoginForm').submit(function(e){

        let myUsername = $('#userNameBox').val();
        let myPassword = $('#passwordBox').val();
        let myDB = $('#dataBaseBox').val();



        let dbConf = new Object();
        dbConf.host = "dursley.socs.uoguelph.ca";
        dbConf.user = myUsername;
        dbConf.password = myPassword;
        dbConf.database = myUsername;


        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/dbLogin',   //The server endpoint we are connecting to
            data: {
              conf: dbConf,
            },
            success: function (data) {
                /*  Do something with returned object
                    Note that what we get is an object, not a string,
                    so we do not need to parse it on the server.
                    JavaScript really does handle JSONs seamlessly
                */
                if(data.success == 1){
                  //$("#dbStuff").show();
                  status();
                }



            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });



        e.preventDefault();
    });

    $('#populateDBForm').submit(function(e){


        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/dbStoreFiles',   //The server endpoint we are connecting to
            data: {
              files: validFiles,
            },
            success: function (data) {
                /*  Do something with returned object
                    Note that what we get is an object, not a string,
                    so we do not need to parse it on the server.
                    JavaScript really does handle JSONs seamlessly
                */
                status();



            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });



        e.preventDefault();
    });

    $('#clearDBForm').submit(function(e){


        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/dbClear',   //The server endpoint we are connecting to

            success: function (data) {
                /*  Do something with returned object
                    Note that what we get is an object, not a string,
                    so we do not need to parse it on the server.
                    JavaScript really does handle JSONs seamlessly
                */

                status();


            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });



        e.preventDefault();
    });

    $('#statusDBForm').submit(function(e){


        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/dbStatus',   //The server endpoint we are connecting to

            success: function (data) {
                /*  Do something with returned object
                    Note that what we get is an object, not a string,
                    so we do not need to parse it on the server.
                    JavaScript really does handle JSONs seamlessly
                */
                alert("Database has "+data.file+" files, "+data.route+" routes, and "+data.point+" points.");




            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });



        e.preventDefault();
    });



});

function dropDownToTable(){
  //console.log(this.value);
  //let currentFile = this.value;
  $("#GPXViewBody").empty();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/getGPXElements',   //The server endpoint we are connecting to
      data: {
        fileName: this.value,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
        //  if(data.fileNames.length > 2){
              //$('#fileLogPanel').css("overflow-y", "scroll");
          //}
          /*let x = $("#gpxmenu").find(":selected").text();
          console.log(x);*/
          //console.log(data.gpxElements.myRoutes);

          $("#OtherDatamenu").empty();
          $("#OtherDataBody").empty();
          let newComponent = $("<option>");
          newComponent.text("Choose GPX Component");
          $("#OtherDatamenu").append(newComponent);

          if(data.gpxElements.myRoutes.length == 0 && data.gpxElements.myTracks.length == 0){
            $('#GPXViewBody').append("No Components Found");
          }

          for(var i in data.gpxElements.myRoutes){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#GPXViewBody').append('<tr>'

                 +'<td>' + "Route "+ (i) +'</td>'
                 +'<td>' + data.gpxElements.myRoutes[i].name+'</td>'
                 +'<td>' + data.gpxElements.myRoutes[i].numPoints+'</td>'
                 +'<td>' + data.gpxElements.myRoutes[i].len+'</td>'
                 +'<td>' + data.gpxElements.myRoutes[i].loop+'</td>'

                 );

                 //fill dropdown menu with filenames
                 let newComponent = $("<option>");
                 newComponent.addClass("route");
                 newComponent.attr('id', i);
                 newComponent.text(data.gpxElements.myRoutes[i].name);
                 newComponent.on("click", getOtherData);
                 $("#OtherDatamenu").append(newComponent);


          }
          for(var i in data.gpxElements.myTracks){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#GPXViewBody').append('<tr>'

                 +'<td>' + "Track "+ (i) +'</td>'
                 +'<td>' + data.gpxElements.myTracks[i].name+'</td>'
                 +'<td>' + data.gpxElements.myTracks[i].points +'</td>'
                 +'<td>' + data.gpxElements.myTracks[i].len+'</td>'
                 +'<td>' + data.gpxElements.myTracks[i].loop+'</td>'

                 );

                 //fill dropdown menu with filenames
                 let newComponent = $("<option>");
                 newComponent.addClass("track");
                 newComponent.attr('id', i);
                 newComponent.text(data.gpxElements.myTracks[i].name);
                 newComponent.on("click", getOtherData);
                 $("#OtherDatamenu").append(newComponent);


          }



      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });

}

function getOtherData(){
  //console.log(this.id)
  //console.log(this.className);
  let myFile = $("#gpxmenu").find(":selected").text();
  //console.log(myFile);

  let myType = 0;
  if(this.className == 'route'){
    myType = 1;
  }
  else{
    myType = 2;
  }
  //console.log(myType);

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/getGPXData',   //The server endpoint we are connecting to
      data: {
        fileName: myFile,
        index: this.id,
        type: myType,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          $("#OtherDataBody").empty();
          if(data.gpxData.length == 0){
            $('#OtherDataBody').append("No GPX Data Found");
          }
          for(var i in data.gpxData){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#OtherDataBody').append('<tr>'

                 +'<td>' + data.gpxData[i].name+'</td>'
                 +'<td>' + data.gpxData[i].value +'</td>'

                 );

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });
}

//clear array of waypoints
function clearWpts(){
  while(waypoints.length){
    waypoints.pop();
  }
}

function displayWpts(){
  if(waypoints.length == 0){
    alert("No waypoints found");
  }else{
    alert(waypoints);
  }
}

function query1Name(){
  $("#Query1Body").empty();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query1Name',   //The server endpoint we are connecting to
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          for(var i in data.routes){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#Query1Body').append('<tr>'

                 +'<td>' + data.routes[i].route_name+'</td>'
                 +'<td>' + data.routes[i].route_len+'</td>'
                 );

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });
}

function query1Len(){
  $("#Query1Body").empty();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query1Len',   //The server endpoint we are connecting to
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          for(var i in data.routes){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#Query1Body').append('<tr>'

                 +'<td>' + data.routes[i].route_name+'</td>'
                 +'<td>' + data.routes[i].route_len+'</td>'
                 );

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });

}

function query2Name(){
  $("#Query2Body").empty();

  let myFile = $("#pickFilemenu").find(":selected").text();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query2Name',   //The server endpoint we are connecting to
      data: {
        fileName: myFile,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          for(var i in data.routes){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#Query2Body').append('<tr>'
                 +'<td>' + myFile+'</td>'
                 +'<td>' + data.routes[i].route_name+'</td>'
                 +'<td>' + data.routes[i].route_len+'</td>'
                 );

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });
}

function query2Len(){
  $("#Query2Body").empty();

  let myFile = $("#pickFilemenu").find(":selected").text();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query2Len',   //The server endpoint we are connecting to
      data: {
        fileName: myFile,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          for(var i in data.routes){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#Query2Body').append('<tr>'
                 +'<td>' + myFile+'</td>'
                 +'<td>' + data.routes[i].route_name+'</td>'
                 +'<td>' + data.routes[i].route_len+'</td>'
                 );

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });
}

function addToRoutesDropDown(){
  //console.log(this.value);
  //let currentFile = this.value;
  $("#pickRoutemenu").empty();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/getGPXElements',   //The server endpoint we are connecting to
      data: {
        fileName: this.value,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          let newComponent = $("<option>");
          newComponent.text("Choose Route");
          $("#pickRoutemenu").append(newComponent);

          for(var i in data.gpxElements.myRoutes){
             //  console.log(data.fileNames[i]);


                 //fill dropdown menu with filenames
                 let newComponent = $("<option>");
                 newComponent.addClass("route");
                 newComponent.attr('id', i);
                 newComponent.text(data.gpxElements.myRoutes[i].name);
                 newComponent.on("click", query3);
                 $("#pickRoutemenu").append(newComponent);


          }

      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });

}

function query3(){
  $("#Query3Body").empty();

  let myFile = $("#pickFilemenu2").find(":selected").text();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query3',   //The server endpoint we are connecting to
      data: {
        fileName: myFile,
        index: this.id,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          let nullCount = 0;
          for(var i in data.points){
             //  console.log(data.fileNames[i]);

             if(data.points[i].point_name === null){
               data.points[i].point_name = "Unnamed point " + nullCount;
               nullCount++;
             }
                 //add data to filelog panel
                 $('#Query3Body').append('<tr>'
                 +'<td>' + data.points[i].point_index+'</td>'
                 +'<td>' + data.points[i].point_name+'</td>'
                 +'<td>' + data.points[i].latitude+'</td>'
                 +'<td>' + data.points[i].longitude+'</td>'

               );
               //

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });
}

function getPointsForFileLen(){
  //query 4
  $("#Query4Body").empty();

  let myFile = $("#pickFilemenu3").find(":selected").text();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query4',   //The server endpoint we are connecting to
      data: {
        fileName: myFile,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly

          */
          let nullCount = 0;

          for(var i in data.points){
             //  console.log(data.fileNames[i]);

                if(data.points[i].route_name === null){
                  data.points[i].route_name = "Unnamed route " + nullCount;
                  nullCount++;
                }
                 //add data to filelog panel
                 $('#Query4Body').append('<tr>'
                  +'<td>' + myFile+'</td>'
                  +'<td>' + data.points[i].route_name+'</td>'
                  +'<td>' + data.points[i].route_len+'</td>'
                 +'<td>' + data.points[i].point_index+'</td>'

                 +'<td>' + data.points[i].latitude+'</td>'
                 +'<td>' + data.points[i].longitude+'</td>'

               );
               //

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });

}

function getPointsForFileName(){
  //query 4
  $("#Query4Body").empty();

  let myFile = $("#pickFilemenu3").find(":selected").text();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query4Name',   //The server endpoint we are connecting to
      data: {
        fileName: myFile,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly

          */
          let nullCount = 0;

          for(var i in data.points){
             //  console.log(data.fileNames[i]);

                if(data.points[i].route_name === null){
                  data.points[i].route_name = "Unnamed route " + nullCount;
                  nullCount++;
                }
                 //add data to filelog panel
                 $('#Query4Body').append('<tr>'
                  +'<td>' + myFile+'</td>'
                  +'<td>' + data.points[i].route_name+'</td>'
                  +'<td>' + data.points[i].route_len+'</td>'
                 +'<td>' + data.points[i].point_index+'</td>'

                 +'<td>' + data.points[i].latitude+'</td>'
                 +'<td>' + data.points[i].longitude+'</td>'

               );
               //

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });

}

function query5Len(){
  $("#Query5Body").empty();

  let myFile = $("#pickFilemenu4").find(":selected").text();
  let ls = $("#longOrShortmenu").find(":selected").text();
  let nVal = $('#nBox').val();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query5Len',   //The server endpoint we are connecting to
      data: {
        fileName: myFile,
        longorshort: ls,
        n: nVal,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          for(var i in data.routes){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#Query5Body').append('<tr>'
                 +'<td>' + myFile+'</td>'
                 +'<td>' + data.routes[i].route_name+'</td>'
                 +'<td>' + data.routes[i].route_len+'</td>'
                 );

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });
}

function query5Name(){
  $("#Query5Body").empty();

  let myFile = $("#pickFilemenu4").find(":selected").text();
  let ls = $("#longOrShortmenu").find(":selected").text();
  let nVal = $('#nBox').val();

  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query5Name',   //The server endpoint we are connecting to
      data: {
        fileName: myFile,
        longorshort: ls,
        n: nVal,
      },
      success: function (data) {
          /*  Do something with returned object
              Note that what we get is an object, not a string,
              so we do not need to parse it on the server.
              JavaScript really does handle JSONs seamlessly
          */
          for(var i in data.routes){
             //  console.log(data.fileNames[i]);

                 //add data to filelog panel
                 $('#Query5Body').append('<tr>'
                 +'<td>' + myFile+'</td>'
                 +'<td>' + data.routes[i].route_name+'</td>'
                 +'<td>' + data.routes[i].route_len+'</td>'
                 );

          }


      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });
}

function status(){
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/dbStatus',   //The server endpoint we are connecting to

        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string,
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            alert("Database has "+data.file+" files, "+data.route+" routes, and "+data.point+" points.");




        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });




}

function statusNoAlert(){

  var status1 = 0;
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/dbStatus',   //The server endpoint we are connecting to

        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string,
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */

             status1 = 1;




        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });
    
    return status1;
}
